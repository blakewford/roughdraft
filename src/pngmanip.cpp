/****************************************************************************
  Copyright (C) 2010 Blake W. Ford

  This file is part of the roughdraft project.

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  zlib style

************************************************************************/
#include "pngmanip.h"
#include "imgutils.h"
#include "rdecompress.h"
#include <stdio.h>
#include <stdlib.h>

//required by libpng to retrieve unknown chunk 
unsigned int compressed_rde_data_size = 0;
png_byte* compressed_rde_data = NULL;

int read_chunk_callback(png_struct* png_ptr, png_unknown_chunk* unknown)
{
	char* name = (char*)unknown->name;
	if(strcmp(name, "rDVs") == 0) {
		compressed_rde_data_size = unknown->size;
		compressed_rde_data = new png_byte[compressed_rde_data_size];
		for(size_t i = 0; i < compressed_rde_data_size; i++)
		{
			compressed_rde_data[i] = unknown->data[i];
		}
		return (1); /* success */
		//return (-1); /* chunk had an error */
	}

	else {
		return (0); /* did not recognize */
	}
}

//pngmanip impl
pngmanip::pngmanip(size_t img_width, size_t img_height, rdl** canvas, const std::vector<rde_object>& rde_vector)
:
imgmanip(img_width, img_height),
m_row_pointers(NULL),
m_rde_vector(rde_vector)
{

        m_row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * img_height);

	imgutils::rgb pixel;

	for(size_t i = 0; i < img_height; i++)
	{
	   png_byte* row = (png_byte*) malloc(sizeof(png_byte) * (img_width*3));
	   
	   int p = 0;
	   for(size_t j = 0; j < img_width; j++,p+=3)
	   {
		pixel = imgutils::getCanvasRGB(canvas, j, i);

		row[p]   = pixel.r;
		row[p+1] = pixel.g;
		row[p+2] = pixel.b;

	   }

	   m_row_pointers[img_height-i-1] = row;
	}

}

pngmanip::pngmanip(const char* input_file)
:
imgmanip(0,0)
{
        m_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	_init_png_members(input_file, "rb");

	int (*callback) (png_struct*, png_unknown_chunk*);
	callback = read_chunk_callback;

	png_set_read_user_chunk_fn(m_png_ptr, NULL, callback);

        png_read_info(m_png_ptr, m_info_ptr);

        m_img_width = png_get_image_width(m_png_ptr, m_info_ptr);
        m_img_height = png_get_image_height(m_png_ptr, m_info_ptr);

        m_row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * m_img_height);
        for (size_t y = 0; y < m_img_height; y++)
                m_row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(m_png_ptr, m_info_ptr));

        png_read_image(m_png_ptr, m_row_pointers);
        png_read_end(m_png_ptr, m_info_ptr);

        fclose(m_fp);

}

void pngmanip::write(const char* outputFile) 
{

	png_byte color_type = 2;
	png_byte bit_depth = 8;

	m_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	_init_png_members(outputFile, "w");

        png_set_IHDR(m_png_ptr, m_info_ptr, m_img_width, m_img_height,
                     bit_depth, color_type, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_unknown_chunkp roughdraft_info = new png_unknown_chunk[1];
	_embed_rde_vector(roughdraft_info);

        png_write_info(m_png_ptr, m_info_ptr);
        png_write_image(m_png_ptr, m_row_pointers);
	png_write_chunk(m_png_ptr, roughdraft_info[0].name, roughdraft_info[0].data, roughdraft_info[0].size);
        png_write_end(m_png_ptr, NULL);

	png_destroy_write_struct(&m_png_ptr, &m_info_ptr);
        fclose(m_fp);

}

void pngmanip::_init_png_members(const char* file, const char* mode)
{
        m_fp = fopen(file, mode);
        m_info_ptr = png_create_info_struct(m_png_ptr);
        png_init_io(m_png_ptr, m_fp);

}

void pngmanip::extract_rde_vector(std::vector<rde_object>& rde_vector)
{	

	m_version = compressed_rde_data[0];
	m_scale_antecedent = compressed_rde_data[1];
	m_scale_consequent = compressed_rde_data[2];

	unsigned int number_of_shapes = 0;
	png_byte* num_data = (png_byte*)&number_of_shapes;
	num_data[0] = compressed_rde_data[3];
	num_data[1] = compressed_rde_data[4];

	m_compressed_rde_data_size = compressed_rde_data_size-5;
	png_byte* rde_data = new png_byte[m_compressed_rde_data_size];

	for(size_t i = 5; i < compressed_rde_data_size; i++)
	{
		rde_data[i-5] = compressed_rde_data[i];
	}

        m_uncompressed_rde_data_size = _get_uncompressed_data_size(number_of_shapes);

        unsigned char* uncompressed_rde_data = new unsigned char [m_uncompressed_rde_data_size];
	rdecompress::rde_inflate(uncompressed_rde_data, rde_data, m_uncompressed_rde_data_size, m_compressed_rde_data_size);

	_uncompress_rde_vector(uncompressed_rde_data, number_of_shapes, rde_vector);

	delete uncompressed_rde_data;

}

pngmanip::~pngmanip()
{

	for (size_t k = 0; k < m_img_height; k++)
		free(m_row_pointers[k]);
	free(m_row_pointers);

	if(m_png_ptr != NULL && m_info_ptr != NULL)
	{
		png_destroy_read_struct(&m_png_ptr, &m_info_ptr, NULL);
	}

	if(compressed_rde_data != NULL)
	{
		delete compressed_rde_data;
	}

}

void pngmanip::_embed_rde_vector(png_unknown_chunkp roughdraft_info)
{

	//Number_of_Shapes has a standardized max
	unsigned short number_of_shapes = static_cast<unsigned short>(m_rde_vector.size());

        m_uncompressed_rde_data_size = _get_uncompressed_data_size(number_of_shapes);
        unsigned char* compressed_rde_data = _compress_rde_vector(m_rde_vector);

	m_compressed_rde_data_size+=5; //Add uncompressed data

	png_byte name[5] = "rDVs";

	roughdraft_info[0].name[0] = name[0];
	roughdraft_info[0].name[1] = name[1];
	roughdraft_info[0].name[2] = name[2];
	roughdraft_info[0].name[3] = name[3];
	roughdraft_info[0].name[4] = name[4];

	roughdraft_info[0].data = new png_byte[m_compressed_rde_data_size];
	roughdraft_info[0].size = m_compressed_rde_data_size;

	roughdraft_info[0].data[0] = m_version;
	roughdraft_info[0].data[1] = m_scale_antecedent;
	roughdraft_info[0].data[2] = m_scale_consequent;

	png_byte* data = (png_byte*)&number_of_shapes;
	roughdraft_info[0].data[3] = data[0];
	roughdraft_info[0].data[4] = data[1];

	for(size_t i = 5; i < m_compressed_rde_data_size; i++)
	{
		roughdraft_info[0].data[i] = compressed_rde_data[i-5];
	}

	if(compressed_rde_data != NULL)
	{
		delete compressed_rde_data;
	}

}
