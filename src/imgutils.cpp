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
#include "imgutils.h"
#include <stdio.h>
namespace imgutils {

	void read_data_into_size_t(unsigned char* memblock, size_t& data, size_t far_offset, size_t number_of_bytes) {
		size_t temp = 0;

		data = memblock[far_offset] << 8*(number_of_bytes-1);
	
		size_t j = 0;
		for(size_t i = number_of_bytes-1; i != 0; --i, ++j)
		{
			temp = memblock[far_offset-j] << 8*i;
			data = data | temp;
		}

		data = data | memblock[far_offset-j];
	}

	rgb getCanvasRGB(rdl** canvas, size_t x, size_t y) {

		rgb pixel;

		switch(canvas[x][y]){
		case white:
			pixel.r = 0xFF; pixel.g = 0xFF; pixel.b=0xFF;
			break;
		case black:
			pixel.r = 0x00; pixel.g = 0x00; pixel.b=0x00;
			break;
		case red:
			pixel.r = 0xFF; pixel.g = 0x00; pixel.b=0x00;
			break;
		case green:
			pixel.r = 0x00; pixel.g = 0xFF; pixel.b=0x00;
			break;
		case blue:
			pixel.r = 0x00; pixel.g = 0x00; pixel.b=0xFF;
			break;
		case brown:
			pixel.r = 0x98; pixel.g = 0x4C; pixel.b=0x00;
			break;
		case yellow:
			pixel.r = 0xFF; pixel.g = 0xFF; pixel.b=0x00;
			break;
		case cyan:
			pixel.r = 0x00; pixel.g = 0xFF; pixel.b=0xFF;
			break;
		case magenta:
			pixel.r = 0xEA; pixel.g = 0x00; pixel.b=0xEA;
			break;
		case gray:
			pixel.r = 0x80; pixel.g = 0x80; pixel.b=0x80;
			break;
		case l_blue:
			pixel.r = 0xA4; pixel.g = 0xE8; pixel.b=0xEE;
			break;
		case l_green:
			pixel.r = 0x4D; pixel.g = 0xFF; pixel.b=0x00;
			break;
		case l_cyan:
			pixel.r = 0x00; pixel.g = 0xDD; pixel.b=0xDD;
			break;
		case l_red:
			pixel.r = 0x21; pixel.g = 0xC6; pixel.b=0xE7;
			break;
		case l_magenta:
			pixel.r = 0xFF; pixel.g = 0x00; pixel.b=0xFF;
			break;
		case l_gray:
			pixel.r = 0xC0; pixel.g = 0xC0; pixel.b=0xCF;
			break;
		case unknownLayer:
		default:
			pixel.r = 0xFF; pixel.g = 0xFF; pixel.b=0xFF;
			break;

		}

		return pixel;
	}

	void setCanvasRGB(rdl** canvas, size_t x, size_t y, rgb pixel) {

		rdl layer = unknownLayer;

		if(pixel.r == 0xFF && pixel.g == 0xFF && pixel.b == 0xFF)
			layer = white;
		else if(pixel.r == 0x00 && pixel.g == 0x00 && pixel.b == 0x00)
			layer = black;
		else if(pixel.r == 0xFF && pixel.g == 0x00 && pixel.b == 0x00)
			layer = red;
		else if(pixel.r == 0x00 && pixel.g == 0xFF && pixel.b == 0x00)
			layer = green;
		else if(pixel.r == 0x00 && pixel.g == 0x00 && pixel.b == 0xFF)
			layer = blue;
		else if(pixel.r == 0x98 && pixel.g == 0x4C && pixel.b == 0x00)
			layer = brown;
		else if(pixel.r == 0xFF && pixel.g == 0xFF && pixel.b == 0x00)
			layer = yellow;
		else if(pixel.r == 0x00 && pixel.g == 0xFF && pixel.b == 0xFF)
			layer = cyan;
		else if(pixel.r == 0xEA && pixel.g == 0x00 && pixel.b == 0xEA)
			layer = magenta;
		else if(pixel.r == 0x80 && pixel.g == 0x80 && pixel.b == 0x80)
			layer = gray;
		else if(pixel.r == 0xA4 && pixel.g == 0xE8 && pixel.b == 0xEE)
			layer = l_blue;
		else if(pixel.r == 0x4D && pixel.g == 0xFF && pixel.b == 0x00)
			layer = l_green;
		else if(pixel.r == 0x00 && pixel.g == 0xDD && pixel.b == 0xDD)
			layer = l_cyan;
		else if(pixel.r == 0x21 && pixel.g == 0xC6 && pixel.b == 0xE7)
			layer = l_red;
		else if(pixel.r == 0xFF && pixel.g == 0x00 && pixel.b == 0xFF)
			layer = l_magenta;
		else if(pixel.r == 0xC0 && pixel.g == 0xC0 && pixel.b == 0xCF)
			layer = l_gray;
		else
			layer = white;

		canvas[x][y] = layer;
	}

	unsigned char* cairotize_canvas_data(rdl** canvas, size_t width, size_t height, size_t& data_stride){

		data_stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, width);
		size_t data_size = data_stride*height;
		unsigned char *data = new unsigned char[data_size];

		rgb pixel;
		size_t color_index = data_size-1;
		for(size_t i = 0; i < height; i++)
		{
	   		for(int j = width-1; j >=0; j--, color_index-=4)
	   		{
				pixel = imgutils::getCanvasRGB(canvas, j, i);
		
				data[color_index] = 0x00;
				data[color_index-1] = pixel.r;
				data[color_index-2] = pixel.g;
				data[color_index-3] = pixel.b;
				
	   		}

		}

		return data;
	}

	void uncairotize_canvas_data(unsigned char* data, size_t width, size_t height, rdl** canvas){

		int data_stride = cairo_format_stride_for_width(CAIRO_FORMAT_RGB24, width);

		rgb pixel;
		int color_index = (data_stride*height)-1;
		for(size_t i = 0; i < height; i++)
		{
	   		for(int j = width-1; j >=0; j--, color_index-=4)
	   		{
				data[color_index] = 0x00;
				pixel.r = data[color_index-1];
				pixel.g = data[color_index-2];
				pixel.b = data[color_index-3];
				imgutils::setCanvasRGB(canvas, j, i, pixel);
				
	   		}

		}

	}

}
