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
#include "draft.h"
#include "draftutils.h"
#include "roughdraftdefines_private.h"
#include "imgmanip.h"
#include "bmpmanip24.h"
#include "bmpmanip256.h"
#include "pngmanip.h"
#include "imgconstants.h"
#include "imgutils.h"
#include <stdio.h>
#include <math.h>
#include <algorithm>

draft::draft(){}

draft::~draft(){}

draft* draft::open_draft(size_t img_width, size_t img_height)
{
	draft* roughdraft = new draft();

	roughdraft->m_img_width = img_width;
	roughdraft->m_img_height = img_height;

        roughdraft->m_canvas = new rdl*[img_width];
	for(size_t i = 0; i < img_width; i++)
	{
		roughdraft->m_canvas[i] = new rdl[img_height];	
	}

	for(size_t j = 0; j < img_width; j++)
	{
	   for(size_t k = 0; k < img_height; k++)
	   {
	   	roughdraft->m_canvas[j][k] = black;
	   }

	}

	return roughdraft;

}
        
draft* draft::open_draft(const char* input_file)
{
	draft* roughdraft = NULL;
	supported_file_type sample = unknownExtension;

	if(imgmanip::is_valid(input_file, sample)){
		if(sample == bitmap24)
		{
			bmpmanip24 vbmp(input_file);
			roughdraft = open_draft(vbmp.get_img_width(), vbmp.get_img_height());
			vbmp.extract_rde_vector(roughdraft->m_rde_vector);
		}
		else if(sample == bitmap256)
		{
			bmpmanip256 vbmp(input_file);
			roughdraft = open_draft(vbmp.get_img_width(), vbmp.get_img_height());
			vbmp.extract_rde_vector(roughdraft->m_rde_vector);
		}
		else if(sample == png)
		{
			pngmanip vbmp(input_file);
			roughdraft = open_draft(vbmp.get_img_width(), vbmp.get_img_height());
			vbmp.extract_rde_vector(roughdraft->m_rde_vector);
		}			
	}

	return roughdraft;
}

void draft::add_rde(
	const rds rds_shape,
	const rdl rdl_layer,
	const size_t weight,
	const size_t start_x,
	const size_t start_y, 
	const size_t end_x, 
	const size_t end_y,
	const size_t plane,
	const size_t plane_offset,
	const size_t depth
)
{
	rde_object element(rds_shape, rdl_layer, weight, start_x, start_y, end_x, end_y, depth);
	m_rde_vector.push_back(element);
}

void draft::add_dimension(const rde_object& element, const size_t dim_height)
{
	rde_object* dim_element;
	if(_check_bounds(element) && element.strct.shape != arc){
		size_t shape_offset = (dim_height*3)/4;
		if (element.strct.start[0] != element.strct.end[0]) {
			if(element.strct.shape == rectangle){
				dim_element = new rde_object(dimension, l_magenta, element.strct.weight, element.strct.start[0], element.strct.end[1] + shape_offset, element.strct.end[0], element.strct.end[1] + dim_height + shape_offset, dim_height);
			}
			else{
				dim_element = new rde_object(dimension, l_magenta, element.strct.weight, element.strct.start[0], element.strct.start[1] + shape_offset, element.strct.end[0], element.strct.end[1] + dim_height + shape_offset, dim_height);
			}
		}
		else {
			dim_element = new rde_object(dimension, l_magenta, element.strct.weight, element.strct.start[0] + shape_offset, element.strct.start[1], element.strct.end[0]+ shape_offset + dim_height, element.strct.end[1], dim_height);			
		}

		if(dim_element != NULL){
			m_rde_vector.push_back(*dim_element);
			delete dim_element;
		}
	}
};

void draft::set_background_color(const rdl& rdl_layer)
{
	for(size_t j = 0; j < m_img_width; j++)
	{
	   for(size_t k = 0; k < m_img_height; k++)
	   {
	   	m_canvas[j][k] = rdl_layer;
	   }

	}

}

void draft::remove_rde(const rde_object& remove_element)
{
	std::vector<rde_object>::iterator element = std::find(m_rde_vector.begin(), m_rde_vector.end(), remove_element);
	m_rde_vector.erase(element);
}

void draft::compile(const char* output_file, supported_file_type type)
{

	for(std::vector<rde_object>::iterator element = m_rde_vector.begin(); element != m_rde_vector.end(); element++)
	{
		switch(element->strct.shape)
		{
			case line:
				_draw_rds_line(*element);
				break;
			case rectangle:
				_draw_rds_rectangle(*element);
				break;
			case swatch:
				_draw_rds_swatch(*element);
				break;
			case circle:
				_draw_rds_circle(*element);
				break;
			case dimension:
				_draw_rds_dimension(*element);
				break;
			case arc:
				_draw_rds_arc(*element, *(element++));
				break;
			case unknownShape:
			default:
				break;
		}

	}

   	imgmanip* img = NULL;

	if(type == bitmap24){
		img = (imgmanip*) new bmpmanip24(m_img_width, m_img_height, m_canvas, m_rde_vector);
	}
	else if(type == bitmap256){
		img = (imgmanip*) new bmpmanip256(m_img_width, m_img_height, m_canvas, m_rde_vector);
	}
	else if(type == png){
		img = (imgmanip*) new pngmanip(m_img_width, m_img_height, m_canvas, m_rde_vector);
	}

	if(img != NULL){
		img->write(output_file);
	}
}

size_t draft::get_img_width()
{
	return m_img_width;
}

size_t draft::get_img_height()
{
	return m_img_height;
}

size_t draft::get_rde_data_size()
{
	return m_rde_vector.size();
}

void draft::get_rde_elements(rde_object* rde_elements, size_t size)
{
	for(size_t i = 0; i < size; i++){
		rde_elements[i] = m_rde_vector[i];
	}
}

void draft::close_draft(draft* draft)
{
	delete draft;
}

void draft::_draw_rds_line(const rde_object& element)
{
	size_t startX = (size_t)(element.strct.start[0]/MM_PER_PIXEL);
	size_t startY = (size_t)(element.strct.start[1]/MM_PER_PIXEL);
	size_t endX = (size_t)(element.strct.end[0]/MM_PER_PIXEL);
	size_t endY = (size_t)(element.strct.end[1]/MM_PER_PIXEL);
	if(_check_bounds(element)){
		_draw_line(element.strct.weight, startX, startY, endX, endY, element.strct.layer);
	}

};

void draft::_draw_rds_rectangle(const rde_object& element)
{
	size_t startX = (size_t)(element.strct.start[0]/MM_PER_PIXEL);
	size_t startY = (size_t)(element.strct.start[1]/MM_PER_PIXEL);
	size_t endX = (size_t)(element.strct.end[0]/MM_PER_PIXEL);
	size_t endY = (size_t)(element.strct.end[1]/MM_PER_PIXEL);
	if(_check_bounds(element)){
		_draw_line(element.strct.weight, startX, startY, endX, startY, element.strct.layer);
		_draw_line(element.strct.weight, endX, startY, endX, endY, element.strct.layer);
		_draw_line(element.strct.weight, startX, endY, endX, endY, element.strct.layer);
		_draw_line(element.strct.weight, startX, startY, startX, endY, element.strct.layer);
	}
};

void draft::_draw_rds_swatch(const rde_object& element)
{
	size_t startX = (size_t)(element.strct.start[0]/MM_PER_PIXEL);
	size_t startY = (size_t)(element.strct.start[1]/MM_PER_PIXEL);
	size_t endX = (size_t)(element.strct.end[0]/MM_PER_PIXEL);
	size_t endY = (size_t)(element.strct.end[1]/MM_PER_PIXEL);
	if(_check_bounds(element)){
		size_t lowest_y = startY;
		size_t highest_y = endY;

		draftutils::find_lowest(lowest_y, highest_y);
		for(size_t i = lowest_y; i < highest_y; i++){
			size_t y = (size_t)(i/MM_PER_PIXEL);
			_draw_line(1, startX, y, endX, y, element.strct.layer);
		}
	}
};

void draft::_draw_rds_circle(const rde_object& element)
{
	size_t startX = (size_t)(element.strct.start[0]/MM_PER_PIXEL);
	size_t startY = (size_t)(element.strct.start[1]/MM_PER_PIXEL);
	size_t endX = (size_t)(element.strct.end[0]/MM_PER_PIXEL);
	size_t endY = (size_t)(element.strct.end[1]/MM_PER_PIXEL);
	
	_draw_circle(startX, startY, endX, endY, element.strct.weight, element.strct.layer);
};

void draft::_draw_rds_arc(const rde_object& arc1, const rde_object& arc2)
{
	size_t centerX = (size_t)(arc1.strct.start[0]/MM_PER_PIXEL);
	size_t centerY = (size_t)(arc1.strct.start[1]/MM_PER_PIXEL);
	size_t arc1endX = (size_t)(arc1.strct.end[0]/MM_PER_PIXEL);
	size_t arc1endY = (size_t)(arc1.strct.end[1]/MM_PER_PIXEL);
	
	size_t arc2endX = (size_t)(arc2.strct.end[0]/MM_PER_PIXEL);
	size_t arc2endY = (size_t)(arc2.strct.end[1]/MM_PER_PIXEL);
	
	_draw_arc(centerX, centerY, arc1endX, arc1endY, arc2endX, arc2endY, arc1.strct.weight, arc1.strct.layer);
};

void draft::_draw_circle(const size_t pixel_start_x, const size_t pixel_start_y, const size_t pixel_end_x, 
						 const size_t pixel_end_y, const size_t pixel_weight, const rdl layer)
{
	int radius = abs(draftutils::get_length(pixel_start_x, pixel_start_y, pixel_end_x, pixel_end_y));
	size_t iteration = 0;
	for(unsigned int i = pixel_start_x - radius; i <= pixel_start_x; i++, iteration++){
		int offset_y = _arc_y_equation(i, pixel_start_x, radius);
		
		if(
		   i < m_img_width &&
		   pixel_start_y+offset_y < m_img_height &&
		   pixel_start_y-offset_y < m_img_height &&	
		   pixel_start_x+radius-iteration < m_img_width		
		   ){
			m_canvas[i][pixel_start_y+offset_y] = layer;
			m_canvas[i][pixel_start_y-offset_y] = layer;
			
			m_canvas[pixel_start_x+radius-iteration][pixel_start_y+offset_y] = layer;
			m_canvas[pixel_start_x+radius-iteration][pixel_start_y-offset_y] = layer;
		}
		else{
			printf("Value out of bounds!\n");
		}
	}
	
	unsigned char weight = pixel_weight;
	if(weight % 2 == 0)
	{
		//weight is even
		weight++;
		
	} 
	size_t half_of_weight = weight/2;
	for(size_t i = 1; i <= half_of_weight; i++)
	{
		_draw_circle(pixel_start_x, pixel_start_y, (pixel_start_x+radius)+i, pixel_start_y, 1, layer);	
		_draw_circle(pixel_start_x, pixel_start_y, (pixel_start_x+radius)-i, pixel_start_y, 1, layer);	
	}
};

void draft::_draw_arc(const size_t center_x, const size_t center_y, const size_t arc1_end_x, 
						 const size_t arc1_end_y, const size_t arc2_end_x, const size_t arc2_end_y,
							const size_t pixel_weight, const rdl layer)
{
	int radius = abs(draftutils::get_length(center_x, center_y, arc1_end_x, arc1_end_y));
	
	size_t left_most_x = arc2_end_x;
	size_t right_most_x = arc1_end_x;
	draftutils::find_lowest(left_most_x, right_most_x);
	if ((left_most_x) <= (right_most_x-radius)){
		left_most_x = right_most_x - radius;
		left_most_x++;
	}
	for(unsigned int i = left_most_x-1; i <= right_most_x; i++){
		int offset_y = _arc_y_equation(i, center_x, radius);
		//IsBelowCenter
		if(arc1_end_y <= center_y && arc2_end_y <= center_y)
			offset_y = -offset_y;

		size_t y = center_y + offset_y;
		
		if(i < m_img_width){
				m_canvas[i+1][y] = layer;
		}
		else{
			printf("Value out of bounds!\n");
		}
	}
	
	unsigned char weight = pixel_weight;
	if(weight % 2 == 0)
	{
		//weight is even
		weight++;
		
	} 
	size_t half_of_weight = weight/2;
	for(size_t i = 1; i <= half_of_weight; i++)
	{
		_draw_arc(center_x, center_y+i, arc1_end_x, arc1_end_y+i, arc2_end_x, arc2_end_y+i, 1, layer);
		_draw_arc(center_x, center_y-i, arc1_end_x, arc1_end_y-i, arc2_end_x, arc2_end_y-i, 1, layer);
	}
	
};

int draft::_arc_y_equation(const size_t input_x, const size_t center_x, const size_t radius)
{
	//sqrt(100-(x-9)^2))+6 = y is a circle centered at (9,6) with a radius of 10
	return (int)sqrt((pow(radius, 2)) - pow(abs(input_x-center_x),2));	
	
}

void draft::_draw_rds_dimension(const rde_object& element)
{
	float length = 0;
	bool isNotVertical = element.strct.start[0] != (element.strct.end[0] - element.strct.depth);
	size_t startX = (size_t)(element.strct.start[0]/MM_PER_PIXEL);
	size_t startY = (size_t)(element.strct.start[1]/MM_PER_PIXEL);
	size_t endX = (size_t)(element.strct.end[0]/MM_PER_PIXEL);
	size_t endY = (size_t)(element.strct.end[1]/MM_PER_PIXEL);
	size_t depth = (element.strct.depth/MM_PER_PIXEL);
	
	size_t offsetStartX = startX+depth;
	size_t offsetStartY = startY+depth;
	size_t offsetEndX = endX-depth;
	size_t offsetEndY = endY-depth;
	
	if(_check_bounds(element)){
		if (isNotVertical) {
			_draw_line(element.strct.weight, startX, startY, startX, offsetStartY, element.strct.layer);
			_draw_line(element.strct.weight, endX, endY, endX, offsetEndY, element.strct.layer);
			_draw_line(element.strct.weight, startX, offsetStartY, endX, endY, element.strct.layer);
			length = draftutils::get_length(startX, offsetStartY, endX, endY)*MM_PER_PIXEL;
		}
		else {
			_draw_line(element.strct.weight, startX, startY, offsetStartX, startY, element.strct.layer);
			_draw_line(element.strct.weight, endX, endY, offsetEndX, endY, element.strct.layer);
			_draw_line(element.strct.weight, offsetStartX, startY, endX, endY, element.strct.layer);
			length = draftutils::get_length(offsetStartX, startY, endX, endY)*MM_PER_PIXEL;
		}

		size_t data_stride = 0;
		m_cairo_data = imgutils::cairotize_canvas_data(m_canvas, m_img_width, m_img_height, data_stride);

		cairo_surface_t *surface = 
			cairo_image_surface_create_for_data(m_cairo_data, CAIRO_FORMAT_RGB24, m_img_width, m_img_height, data_stride);

		cairo_t *cr =
			cairo_create (surface);
		
		cairo_font_options_t *fo = cairo_font_options_create();
		cairo_font_options_set_antialias (fo, CAIRO_ANTIALIAS_NONE);
		cairo_select_font_face (cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

		char* string = new char[10];
		sprintf(string, "%.0f mm", length);
		// length*IN_PER_MM For inches

		cairo_set_font_size (cr, 16.0);
		cairo_set_source_rgb (cr, 1, 0, 1);
		cairo_set_font_options(cr, fo);

		size_t midpoint_x = (size_t)(((element.strct.start[0]+element.strct.end[0])/MM_PER_PIXEL)/2);
		size_t midpoint_y = (size_t)(((element.strct.start[1]+element.strct.end[1])/MM_PER_PIXEL)/2);

		size_t lowest_y = startY;
		size_t highest_y = endY;

		int dimension_text_height = 20;
		if(draftutils::find_lowest(lowest_y, highest_y))
		{
			dimension_text_height/=2;
		}
		
		if (isNotVertical) {
			cairo_move_to (cr, midpoint_x, m_img_height-midpoint_y-dimension_text_height);
		}
		else {
			cairo_move_to (cr, midpoint_x+10, m_img_height-midpoint_y);
		}

		cairo_show_text (cr, string);
		cairo_destroy (cr);

		imgutils::uncairotize_canvas_data(m_cairo_data, m_img_width, m_img_height, m_canvas);

		delete m_cairo_data;
		cairo_font_options_destroy(fo);
		cairo_surface_destroy (surface);
	}
};

void draft::_draw_line(size_t weight, const size_t x1, const size_t y1, const size_t x2, const size_t y2, const rdl layer)
{	
	//Always draw from left to right
	size_t left_most_x = x1;
	size_t left_most_y = y1;
	size_t right_most_x = x2;
	size_t right_most_y = y2;
	
	size_t lowest_y = y1;
	size_t highest_y = y2;
	
	float slope = 1;
	
	bool switched = draftutils::find_lowest(left_most_x, right_most_x);
	if(switched)
	{
		left_most_y = y2;
		right_most_y = y1;
	}
	
	bool slopeNeg = draftutils::find_lowest(lowest_y, highest_y);
	if(slopeNeg^switched)
	{
		slope = -slope;
	}

	float ydiff = highest_y - lowest_y;
	float xdiff = right_most_x - left_most_x;

	
	if(xdiff > 1 && ydiff > 1)
	{
		slope = slope*ydiff/xdiff;
		
		long int b = (long int)(slope*left_most_x - left_most_y); 		
		
		b = labs(b);
		
		for(size_t i = left_most_x; i < right_most_x; i++){
			size_t y = 0;
			if(slopeNeg^switched)
				y = (size_t)(slope*i + b);
			else 
				y = (size_t)(slope*i - b);
			
			m_canvas[i][y] = layer;
			
			if(abs(slope) != 1){
				if(slope > 1){
					for(size_t j = y+1; j <  (slope*+(i+1)) + b; j++){
						m_canvas[i][j] = layer;
					}
				}
				else{
					for(size_t j = y-1; j >  (slope*+(i+1)) + b; j--){
						m_canvas[i][j] = layer;
					}
				}
				
			}
		}
		
	}
	
	else if(xdiff < 1)
	{
		for(size_t i = lowest_y; i <= highest_y; i++)
		{
			m_canvas[left_most_x][i] = layer;
		}
	}
	else
	{
		size_t lowest_x = x1;
		size_t highest_x = x2;
		
		draftutils::find_lowest(lowest_x, highest_x);
		
		for(size_t i = lowest_x; i <= highest_x; i++)
		{
			m_canvas[i][left_most_y] = layer;
		}
	}
	
	_draw_line_weight(weight, x1, y1, x2, y2, layer);
	
}

void draft::_draw_line_weight(size_t weight, const size_t x1, const size_t y1, const size_t x2, const size_t y2, const rdl layer)
{	
	if(weight % 2 == 0)
	{
		//weight is even
		weight++;
		
	} 
	
	size_t half_of_weight = weight/2;
	if(x2 != x1)
	{
		for(size_t i = 1; i <= half_of_weight; i++)
		{
			_draw_line(1, x1, y1+i, x2, y2+i, layer);	
		}
		
		
		for(size_t i = 1; i <= half_of_weight; i++)
		{
			_draw_line(1, x1, y1-i, x2, y2-i, layer);	
		}
	}
	else
	{
		for(size_t i = 1; i <= half_of_weight; i++)
		{
			_draw_line(1, x1+i, y1, x2+i, y2, layer);	
		}
		for(size_t i = 1; i <= half_of_weight; i++)
		{
			_draw_line(1, x1-i, y1, x2-i, y2, layer);	
		}
	}
}

bool draft::_check_bounds(const rde_object& element)
{
	if(
		element.strct.start[0]/MM_PER_PIXEL < m_img_width &&
		element.strct.start[1]/MM_PER_PIXEL < m_img_height &&
		element.strct.end[0]/MM_PER_PIXEL < m_img_width &&
		element.strct.end[1]/MM_PER_PIXEL < m_img_height
	){
		return true;
	}
	else{
		printf("Value out of bounds! %zu %zu\n", m_img_width, m_img_height);
	}

	return false;

}

rde_object::rde_object(const rds rds_shape, const rdl rdl_layer, const size_t shape_weight, const size_t start_x, const size_t start_y, const size_t end_x, const size_t end_y, size_t const depth)
{
	strct.shape = rds_shape;
	strct.layer = rdl_layer;
	strct.weight = shape_weight;;
	strct.start[0] = start_x;
	strct.start[1] = start_y;
	strct.end[0] = end_x;
	strct.end[1] = end_y;

	strct.plane = unknownPlane;
	strct.plane_offset = 0;
	strct.depth = depth;	

}

bool rde_object::operator==(const rde_object& rhs)
{
	if
	(
		strct.shape == rhs.strct.shape &&
		strct.layer == rhs.strct.layer &&
		strct.weight == rhs.strct.weight &&
		strct.start[0] == rhs.strct.start[0] &&
		strct.start[1] == rhs.strct.start[1] &&
		strct.end[0] == rhs.strct.end[0] &&
		strct.end[1] == rhs.strct.end[1] &&

		strct.plane == rhs.strct.plane &&
		strct.plane_offset == rhs.strct.plane_offset &&
		strct.depth == rhs.strct.depth
	){
		return true;
	}
	else{
		return false;
	}
}

rde rde_object::get_rde()
{
	return strct;
}
