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
#include "bmpmanip.h"
#include "imgconstants.h"
#include "bmpconstants.h"
#include "rdecompress.h"
#include "imgutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

bmpmanip::bmpmanip(size_t depth, size_t pixel_offset, size_t img_width, size_t img_height, const std::vector<rde_object>& rde_vector, size_t embedding_offset, size_t embedding_bits, size_t embed_start, size_t embedding_bytes_availiable)
:
imgmanip(img_width, img_height),
m_rde_vector(rde_vector),
_pixel_offset(pixel_offset),
_pixel_depth(depth),
_embedding_start_byte(embed_start),
_embedding_offset(embedding_offset),
_embedding_bits_per_byte(embedding_bits),
_embedding_bytes_availiable(embedding_bytes_availiable)
{
	_initialize_members();
	_memblock = new unsigned char[_size_in_bytes];
	_generateHeaderInfo();		

}

bmpmanip::bmpmanip(const char* input_file, size_t embedding_offset, size_t embedding_bits, size_t embed_start, size_t embedding_bytes_availiable)
:
imgmanip(0, 0),
_embedding_start_byte(embed_start),
_embedding_offset(embedding_offset),
_embedding_bits_per_byte(embedding_bits),
_embedding_bytes_availiable(embedding_bytes_availiable)
{
	_initialize(input_file);
	_parseHeaderInfo();	
	_initialize_members();	

}

bmpmanip::~bmpmanip()
{
	if(_memblock != NULL)
	{
		delete[] _memblock;
	}
}

void bmpmanip::write(const char* output_file)
{
	
	FILE* file_ptr = NULL;
	
	file_ptr = fopen(output_file, "w");
	if (file_ptr != NULL)
	{
		fwrite(_memblock, 1, _size_in_bytes, file_ptr);
		fclose(file_ptr);
	}
	else
	{
		return;
	}
}

void bmpmanip::extract_rde_vector(std::vector<rde_object>& rde_vector)
{
	_extract_rde_vector();
	for(size_t i = 0; i < m_rde_vector.size(); i++)
	{
		rde_vector.push_back(m_rde_vector[i]);
	}

}

void bmpmanip::_initialize(const char* input_file)
{
	FILE* file_ptr = NULL;
	
	file_ptr = fopen(input_file, "rb");
	if (file_ptr != NULL)
	{
		
		fseek(file_ptr, 0, SEEK_END); 
		_size_in_bytes = ftell(file_ptr);
		rewind(file_ptr);
		
		_memblock = new unsigned char [_size_in_bytes];
		fread(_memblock, 1, _size_in_bytes, file_ptr);
		fclose(file_ptr);
	}
	
}

void bmpmanip::_embed_rde_vector()
{
	size_t offset = _embedding_start_byte;

	_embed_rde_data(offset, sizeof(unsigned char), (void*)&m_version, 1);
	_embed_rde_data(offset, sizeof(unsigned char), (void*)&m_scale_antecedent, 1);
	_embed_rde_data(offset, sizeof(unsigned char), (void*)&m_scale_consequent, 1);

	//Number_of_Shapes has a standardized max
	unsigned short number_of_shapes = static_cast<unsigned short>(m_rde_vector.size());
	_embed_rde_data(offset, sizeof(unsigned short), (void*)&number_of_shapes, 1);

        m_uncompressed_rde_data_size = _get_uncompressed_data_size(number_of_shapes);
        unsigned char* compressed_rde_data = _compress_rde_vector(m_rde_vector);

	if(m_compressed_rde_data_size < _embedding_bytes_availiable)
	{
		_embed_rde_data(offset, sizeof(unsigned int), (void*)&m_compressed_rde_data_size, 1);
		_embed_rde_data(offset, sizeof(unsigned char), (void*)compressed_rde_data, m_compressed_rde_data_size);
	}
	else
	{
		printf("Error, there is not enough space to embed the data! Byte Needed: %u Bytes Available %zu\n", m_compressed_rde_data_size, _embedding_bytes_availiable);
		return;
	}

	if(compressed_rde_data != NULL)
	{
		delete compressed_rde_data;
	}

}

void bmpmanip::_embed_rde_data(size_t& memblock_offset, size_t size_of_data_type, void* data, size_t data_size)
{	
	//Little Endian
	unsigned char* byte_data = (unsigned char*)data;
	size_t offset = 0;

	for(size_t i = 0; i < data_size*size_of_data_type; i++)
	{
		unsigned char byte = byte_data[i];	
		for(size_t j = 0; j < _chunks_per_byte*_embedding_offset; j+=_embedding_offset)
		{
			unsigned char temp = byte & _embedding_mask;
			offset = memblock_offset+(i*_chunks_per_byte*_embedding_offset)+j;

			_memblock[offset] = _memblock[offset] & (0xFF-_embedding_mask);
			_memblock[offset] = _memblock[offset] | temp;
			byte = byte >> _embedding_bits_per_byte;

		}

	}

	memblock_offset = offset+_embedding_offset;
}

void bmpmanip::_extract_rde_vector()
{

	size_t offset = _embedding_start_byte;
	void* data = NULL;

	data = _extract_rde_data(offset, sizeof(unsigned char));
	unsigned char version = *(unsigned char*)data;
	delete (unsigned char*)data; 

	if(version != 1)
	{
		return;
	}	

	data = _extract_rde_data(offset, sizeof(unsigned char));
	unsigned char scale_antecedent = *(unsigned char*)data;
	delete (unsigned char*)data;	

	if(scale_antecedent != 1)
	{
		return;
	}

	data = _extract_rde_data(offset, sizeof(unsigned char));
	unsigned char scale_consequent = *(unsigned char*)data;
	delete (unsigned char*)data;	

	if(scale_consequent != 1)
	{
		return;
	}

	data = _extract_rde_data(offset, sizeof(unsigned short));
	unsigned short number_of_shapes = *(unsigned short*)data;	
	delete (unsigned short*)data;

	data = _extract_rde_data(offset, sizeof(unsigned int));
	m_compressed_rde_data_size = *(unsigned int*)data;
	delete (unsigned int*)data;

	m_uncompressed_rde_data_size = _get_uncompressed_data_size(number_of_shapes);

        unsigned char* uncompressed_rde_data = new unsigned char [m_uncompressed_rde_data_size];
        unsigned char* compressed_rde_data = NULL;

	data = _extract_rde_data(offset, (sizeof(unsigned char)*m_uncompressed_rde_data_size));
	compressed_rde_data = (unsigned char*)data;

	rdecompress::rde_inflate(uncompressed_rde_data, 
				 compressed_rde_data, m_uncompressed_rde_data_size, m_compressed_rde_data_size);

	_uncompress_rde_vector(uncompressed_rde_data, number_of_shapes, m_rde_vector);

	delete uncompressed_rde_data;
}

void bmpmanip::_parseHeaderInfo()
{
	//Get Size of BMP file
	imgutils::read_data_into_size_t(_memblock, _size_in_bytes, ENDBMPSZ, 4);	
	
	//Skip Unused, app specific Space 
	//_memblock[6] - _memblock[9]

	//Pixel Offset
	imgutils::read_data_into_size_t(_memblock, _pixel_offset, ENDPXLOFFSET, 4);
	
	//Bytes of Header left is constant 40
	
	//Width in Pixels
	imgutils::read_data_into_size_t(_memblock, m_img_width, ENDWIDTH, 4);
	
	//Height in Pixels
	imgutils::read_data_into_size_t(_memblock, m_img_height, ENDHEIGHT, 4);
	
	//Planes Used is constant 1
	
	//Bits per pixel
	imgutils::read_data_into_size_t(_memblock, _pixel_depth, ENDDEPTH, 2);
	
	//Compression is constant 0
	
	//BMP Raw Data (After Header)
	imgutils::read_data_into_size_t(_memblock, _raw_data_size, ENDRAWDATASZ, 4);
	
	//Horizontal and Vertical Resolution are constant 2835
	
	//Colors in Palette and All Colors Important are constant 0
}

void bmpmanip::_initialize_members()
{

	_pixel_bytes_per_line = (_pixel_depth*m_img_width)/BYTESIZE;
	_padding_bytes_per_line = 0;

	if(_pixel_bytes_per_line%HEXDIGITSIZE != 0)
        {
		_padding_bytes_per_line = HEXDIGITSIZE-(_pixel_bytes_per_line%HEXDIGITSIZE);
 	}

	_bytes_per_line = _pixel_bytes_per_line+_padding_bytes_per_line;
	_size_in_bytes = _pixel_offset + (_bytes_per_line * labs(m_img_height));
	_raw_data_size = _size_in_bytes - _pixel_offset;

	_chunks_per_byte = (int)BYTESIZE/_embedding_bits_per_byte;
	_embedding_mask = 0;
	for(size_t i = 0; i < _embedding_bits_per_byte; i++)
	{
		_embedding_mask+=(int)pow(2,i);	
	}
	
}

void bmpmanip::_generateHeaderInfo()
{	
	_memblock[BSpace] = 'B';
        _memblock[MSpace] = 'M';
	
	_write_data_to_bmp(_size_in_bytes, BEGINBMPSZ, 4);
	//Write 0x00 to app specific Space 
	_write_data_to_bmp(0, BEGINAPPSPACE, 4);	
	_write_data_to_bmp(_pixel_offset, BEGINPXLOFFSET, 4);
	_write_data_to_bmp(40, BEGINHDRREM, 4);
	_write_data_to_bmp(m_img_width, BEGINWIDTH, 4);
	_write_data_to_bmp(m_img_height, BEGINHEIGHT, 4);
	_write_data_to_bmp(1, BEGINPLANES, 2);
	_write_data_to_bmp(_pixel_depth, BEGINDEPTH, 2);
	_write_data_to_bmp(0, BEGINBI_RGB, 4);
	_write_data_to_bmp(_raw_data_size, BEGINRAWDATASZ, 4);
	_write_data_to_bmp(2835, BEGINHORRES, 4);
	_write_data_to_bmp(2835, BEGINVERRES, 4);
	_write_data_to_bmp(0, BEGINPALCOLORS, 4);
	_write_data_to_bmp(0, BEGINIMPCOLORS, 4);
	
}

void bmpmanip::_write_data_to_bmp(size_t data, size_t value_offset, size_t number_of_bytes)
{
	for(size_t i = 0; i < number_of_bytes; i++)
	{
		_memblock[value_offset+i]  = (data >> 8*i); 
	}
}
