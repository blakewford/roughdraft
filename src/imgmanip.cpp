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
#include "imgmanip.h"
#include "bmpconstants.h"
#include "rdecompress.h"
#include "imgutils.h"
#include "png.h"
#include <stdio.h>
#include <stdlib.h>

imgmanip::imgmanip(size_t img_width, size_t img_height)
:
m_img_width(img_width),
m_img_height(img_height),
m_version(1),
m_scale_antecedent(1),
m_scale_consequent(1),
m_compressed_rde_data_size(0)
{
	

}

bool imgmanip::is_valid(const char* input_file, supported_file_type& type)
{
	bool valid = false;
	
	FILE* file_ptr = NULL;
	unsigned char* memblock = NULL;
	
	file_ptr = fopen(input_file, "rb");

	if (file_ptr != NULL)
	{
		
		fseek(file_ptr, 0, SEEK_END); 
		size_t size_in_bytes = ftell(file_ptr);
		rewind(file_ptr);
		
		memblock = new unsigned char[size_in_bytes];
		fread(memblock, 1, size_in_bytes, file_ptr);
	}

	if(memblock[BSpace] == 'B')
	{
		if(memblock[MSpace] == 'M')
		{
			valid = true;
		}
	}

	if(valid)
	{
		size_t pixel_depth = 0;
		imgutils::read_data_into_size_t(memblock, pixel_depth, ENDDEPTH, 2);

		if(pixel_depth == 24)
		{
			type = bitmap24;
		}	
		else
		{
			type = bitmap256;
		}

		delete memblock;
	}
	else
	{
		rewind(file_ptr);
	        char header[8];    // 8 is the maximum size that can be checked
	
		png_byte color_type = 2;
		png_byte bit_depth = 8;

	       	fread(header, 1, 8, file_ptr);

		png_structp png_ptr;
		png_infop info_ptr;

        	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

        	info_ptr = png_create_info_struct(png_ptr);

        	png_init_io(png_ptr, file_ptr);
        	png_set_sig_bytes(png_ptr, 8);

        	png_read_info(png_ptr, info_ptr);

        	if((color_type == png_get_color_type(png_ptr, info_ptr)) && (bit_depth == png_get_bit_depth(png_ptr, info_ptr)))
		{
			valid = true;
			type = png;
		}
	}

	if(file_ptr)
		fclose(file_ptr);

	return valid;	
}

size_t imgmanip::_get_uncompressed_data_size(unsigned short number_of_shapes)
{

	return number_of_shapes*(sizeof(rds)+sizeof(rdl)+sizeof(unsigned char)+(sizeof(unsigned int)*6)+sizeof(rdp));

}

void imgmanip::_insert_data_into_uncompressed_data_ptr(size_t& data_offset, unsigned char* uncompressed_data, size_t size_of_data_type, void* data)
{

	unsigned char* byte_data = NULL;
	byte_data = (unsigned char*)data;

	for(size_t i = 0; i < size_of_data_type; i++)
	{
		uncompressed_data[data_offset] = byte_data[i];
		data_offset++;
	}


}

void* imgmanip::_extract_data_from_uncompressed_data_ptr(size_t& data_offset, unsigned char* uncompressed_data, size_t size_of_data_type)
{

	unsigned char* byte_data = new unsigned char[size_of_data_type];
	for(size_t i = 0; i < size_of_data_type; i++)
	{
		byte_data[i] = uncompressed_data[data_offset];
		data_offset++;
	}

	return (void*)byte_data;

}

unsigned char* imgmanip::_compress_rde_vector(const std::vector<rde_object>& rde_vector)
{
        unsigned char* uncompressed_rde_data = new unsigned char [m_uncompressed_rde_data_size];
        unsigned char* compressed_rde_data = new unsigned char [m_uncompressed_rde_data_size];

	size_t compression_offset = 0;
	for(std::vector<rde_object>::const_iterator i = rde_vector.begin(); i != rde_vector.end(); i++)
	{
		_insert_data_into_uncompressed_data_ptr(compression_offset, uncompressed_rde_data, sizeof(rds), (void*)&i->strct.shape);
		_insert_data_into_uncompressed_data_ptr(compression_offset, uncompressed_rde_data, sizeof(rdl), (void*)&i->strct.layer);
		_insert_data_into_uncompressed_data_ptr(compression_offset, uncompressed_rde_data, sizeof(unsigned char), (void*)&i->strct.weight);
		_insert_data_into_uncompressed_data_ptr(compression_offset, uncompressed_rde_data, sizeof(unsigned int), (void*)&i->strct.start[0]);
		_insert_data_into_uncompressed_data_ptr(compression_offset, uncompressed_rde_data, sizeof(unsigned int), (void*)&i->strct.start[1]);
		_insert_data_into_uncompressed_data_ptr(compression_offset, uncompressed_rde_data, sizeof(unsigned int), (void*)&i->strct.end[0]);
		_insert_data_into_uncompressed_data_ptr(compression_offset, uncompressed_rde_data, sizeof(unsigned int), (void*)&i->strct.end[1]);	
		_insert_data_into_uncompressed_data_ptr(compression_offset, uncompressed_rde_data, sizeof(rdp), (void*)&i->strct.plane);
		_insert_data_into_uncompressed_data_ptr(compression_offset, uncompressed_rde_data, sizeof(unsigned int), (void*)&i->strct.plane_offset);
		_insert_data_into_uncompressed_data_ptr(compression_offset, uncompressed_rde_data, sizeof(unsigned int), (void*)&i->strct.depth);
	}

	rdecompress::rde_deflate(uncompressed_rde_data, compressed_rde_data, m_uncompressed_rde_data_size, m_compressed_rde_data_size);

	return compressed_rde_data;

}

void imgmanip::_uncompress_rde_vector(unsigned char* uncompressed_rde_data, unsigned int number_of_shapes, std::vector<rde_object>& rde_vector)
{
	void* data = NULL;
	size_t decompression_offset = 0;
	for(size_t i = 0; i < number_of_shapes; i++)
	{
		data = _extract_data_from_uncompressed_data_ptr(decompression_offset, uncompressed_rde_data, sizeof(rds));
		rds shape = *(rds*)data;
		delete (rds*)data;
		data = _extract_data_from_uncompressed_data_ptr(decompression_offset, uncompressed_rde_data, sizeof(rdl));
		rdl layer = *(rdl*)data;
		delete (rdl*)data;
		data = _extract_data_from_uncompressed_data_ptr(decompression_offset, uncompressed_rde_data, sizeof(unsigned char));
		unsigned char weight = *(unsigned char*)data;
		delete (unsigned char*)data;
		data = _extract_data_from_uncompressed_data_ptr(decompression_offset, uncompressed_rde_data, sizeof(unsigned int));
		unsigned int start_x = *(unsigned int*)data;
		delete (unsigned int*)data;
		data = _extract_data_from_uncompressed_data_ptr(decompression_offset, uncompressed_rde_data, sizeof(unsigned int));
		unsigned int start_y = *(unsigned int*)data;
		delete (unsigned int*)data;
		data = _extract_data_from_uncompressed_data_ptr(decompression_offset, uncompressed_rde_data, sizeof(unsigned int));
		unsigned int end_x = *(unsigned int*)data;
		delete (unsigned int*)data;
		data = _extract_data_from_uncompressed_data_ptr(decompression_offset, uncompressed_rde_data, sizeof(unsigned int));
		unsigned int end_y = *(unsigned int*)data;
		delete (unsigned int*)data;
		data = _extract_data_from_uncompressed_data_ptr(decompression_offset, uncompressed_rde_data, sizeof(rdp));
		rdp plane = *(rdp*)data;
		delete (rdp*)data;
		data = _extract_data_from_uncompressed_data_ptr(decompression_offset, uncompressed_rde_data, sizeof(unsigned int));
		unsigned int plane_offset = *(unsigned int*)data;
		delete (unsigned int*)data;
		data = _extract_data_from_uncompressed_data_ptr(decompression_offset, uncompressed_rde_data, sizeof(unsigned int));
		unsigned int depth = *(unsigned int*)data;
		delete (unsigned int*)data;

		if((plane != unknownPlane || plane_offset != 0) || (depth != 0 && shape != dimension))
		{
			printf("Error, 3D data in 2D draft!\n");
			return;
		}

		rde_vector.push_back(rde_object(shape, layer, weight, start_x, start_y, end_x, end_y, depth));	

	}

}
