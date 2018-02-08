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
#include "bmpmanip24.h"
#include "imgutils.h"
#include "bmpconstants.h"
#include "imgconstants.h"

bmpmanip24::bmpmanip24(size_t img_width, size_t img_height, rdl** canvas, const std::vector<rde_object>& rde_vector)
:
bmpmanip(24, HEADERSIZE, img_width, img_height, rde_vector, 3, 2, 54, 0/*No raw data info, yet*/)
{
	set_bmp_embedding_bytes_available(get_bmp_raw_data_size()/((BYTESIZE/2)*3));

	imgutils::rgb pixel;

	size_t color_index = 0;

	const size_t pixel_offset = get_bmp_pixel_offset();
	const size_t padding_bytes_per_line = get_bmp_padding_bytes_per_line();

	for(size_t i = 0; i < img_height; i++)
	{
	   for(size_t j = 0; j < img_width; j++)
	   {
		pixel = imgutils::getCanvasRGB(canvas, j, i);
		
		_memblock[pixel_offset+color_index]   = pixel.b;
		_memblock[pixel_offset+color_index+1] = pixel.g;
		_memblock[pixel_offset+color_index+2] = pixel.r;

		color_index+=3;	
	   }

	   for(size_t pad = 0; pad < padding_bytes_per_line; pad++)
	   {
		_memblock[pixel_offset+color_index+pad] = PADDING;
	   }

	   color_index+=padding_bytes_per_line;

	}

	_embed_rde_vector();

}

bmpmanip24::bmpmanip24(const char* input_file)
:
bmpmanip(input_file, 3, 2, 54, (BYTESIZE/2)*_get_uncompressed_data_size(1)*3)
{

}

bmpmanip24::~bmpmanip24()
{
	
}

void* bmpmanip24::_extract_rde_data(size_t& memblock_offset, size_t size_of_data_type)
{
	unsigned char* byte_data = new unsigned char[size_of_data_type];

	for(size_t i = 0; i < size_of_data_type; i++)
	{
		byte_data[i] = 0;
	}

	size_t shift_counter = 1;
	byte_data[0] = _memblock[memblock_offset] & 0x3;
	for(size_t i = 1; i < 4*size_of_data_type; i++)
	{
		size_t index = i/4;
		size_t value = (_memblock[memblock_offset+(3*i)] & 0x3) << (2*shift_counter);
		byte_data[index]+=value;
		shift_counter++;
		if(shift_counter == 4)
		{
			shift_counter = 0;
		}
	}

	memblock_offset+=4*size_of_data_type*3;

	return (void*)byte_data;

}
