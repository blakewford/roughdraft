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
#include "bmpmanip256.h"
#include "bmpconstants.h"
#include "imgconstants.h"

#define rds_definition_bytes RDL_LAYERS*4

bmpmanip256::bmpmanip256(size_t img_width, size_t img_height, rdl** canvas, const std::vector<rde_object>& rde_vector)
:
bmpmanip(8, 1078, img_width, img_height, rde_vector, 1, 8, 54+(RDL_LAYERS*4), 1024 - (RDL_LAYERS*4))
{		

	size_t color_index = 0;
	unsigned char pixel = 0x00;

	const size_t pixel_offset = get_bmp_pixel_offset();
	const size_t padding_bytes_per_line = get_bmp_padding_bytes_per_line();

	for(size_t i = 0; i < img_height; i++)
	{
	   for(size_t j = 0; j < img_width; j++)
	   {
		switch(canvas[j][i]){
		case white:
			pixel = 0x00;
			break;
		case black:
			pixel = 0x01;
			break;
		case red:
			pixel = 0x02;
			break;
		case green:
			pixel = 0x03;
			break;
		case blue:
			pixel = 0x04;
			break;
		case brown:
			pixel = 0x05;
			break;
		case yellow:
			pixel = 0x06; 
			break;
		case cyan:
			pixel = 0x07; 
			break;
		case magenta:
			pixel = 0x08; 
			break;
		case gray:
			pixel = 0x09; 
			break;
		case l_blue:
			pixel = 0x0A; 
			break;
		case l_green:
			pixel = 0x0B;
			break;
		case l_cyan:
			pixel = 0x0C; 
			break;
		case l_red:
			pixel = 0x0D; 
			break;
		case l_magenta:
			pixel = 0x0E; 
			break;
		case l_gray:
			pixel = 0x0F;
			break;
		case unknownLayer:
		default:
			pixel = 0x00;
			break;

		}

		
		_memblock[pixel_offset+color_index] = pixel;
		color_index++;

	   }

	   for(size_t pad = 0; pad < padding_bytes_per_line; pad++)
	   {
		_memblock[pixel_offset+color_index+pad] = PADDING;
	   }

	   color_index+=padding_bytes_per_line;

	}

	unsigned char dictionary[rds_definition_bytes] = 
                                       {0xFF, 0xFF, 0xFF, 0x00,
				        0x00, 0x00, 0x00, 0x00,
				        0x00, 0x00, 0xFF, 0x00,
                                        0x00, 0xFF, 0x00, 0x00,
					0xFF, 0x00, 0x00, 0x00,
					0x00, 0x4C, 0x98, 0x00,
					0x00, 0xFF, 0xFF, 0x00, 
					0xFF, 0xFF, 0x00, 0x00,
					0xEA, 0x00, 0xEA, 0x00,
					0x80, 0x80, 0x80, 0x00, 
					0xEE, 0xE8, 0xA4, 0x00,
					0x00, 0xFF, 0x4D, 0x00,
					0xDD, 0xDD, 0x00, 0x00,
					0xE7, 0xC6, 0x21, 0x00,
					0xFF, 0x00, 0xFF, 0x00,
					0xCF, 0xC0, 0xC0, 0x00};

	for(size_t i = 0; i < rds_definition_bytes; i++)
	{
		_memblock[HEADERSIZE+i] = dictionary[i];
	}

	for(size_t i = HEADERSIZE+rds_definition_bytes; i < pixel_offset; i++)
	{
		_memblock[i] = 0xFF;
	}

	_embed_rde_vector();
	

}

bmpmanip256::bmpmanip256(const char* input_file)
:
bmpmanip(input_file, 1, 8, 54+(RDL_LAYERS*4), 1024 - (RDL_LAYERS*4))
{

}

bmpmanip256::~bmpmanip256()
{
	
}

void* bmpmanip256::_extract_rde_data(size_t& memblock_offset, size_t size_of_data_type)
{
	unsigned char* byte_data = new unsigned char[size_of_data_type];

	for(size_t i = 0; i < size_of_data_type; i++)
	{
		byte_data[i] = _memblock[memblock_offset+i];
	}

	memblock_offset+=size_of_data_type;

	return (void*)byte_data;

}

