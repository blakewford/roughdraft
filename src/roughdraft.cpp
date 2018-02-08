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
#include "roughdraft.h"
#include "imgconstants.h"
#include "draft.h"
#include "draftutils.h"
#include "stdio.h"
#include "stdlib.h"
#include <cmath>

draft* singleton_draft = NULL;

void open_draft(size_t img_width, size_t img_height)
{
	singleton_draft = draft::open_draft(img_width, img_height);
}

void recover_draft(const char* input_file)
{
	singleton_draft = draft::open_draft(input_file);
}

void add_rde(
	const rds rds_shape, const rdl rdl_layer,
	const size_t rde_weight, const size_t start_x,
	const size_t start_y, const size_t end_x, const size_t end_y,
	const size_t plane, const size_t plane_offset, const size_t rde_depth
)
{
	singleton_draft->add_rde(rds_shape, rdl_layer, rde_weight, start_x, start_y, end_x, end_y, 0, 0, rde_depth);


}

void add_dimension(const rde element, const size_t dim_height)
{
	rde_object rde(element.shape,element.layer,element.weight,element.start[0],element.start[1],element.end[0],element.end[1],element.depth);
	singleton_draft->add_dimension(rde, dim_height);

}

float get_rde_dimension(const rde element, const rdu unit)
{
	return draftutils::get_length(element.start[0], element.start[1], element.end[0], element.end[1]);
	
}

void set_background_color(const rdl rdl_layer)
{
	singleton_draft->set_background_color(rdl_layer);

}

void remove_rde(const rde remove_element)
{
	rde_object rde(remove_element.shape,remove_element.layer,remove_element.weight,remove_element.start[0],remove_element.start[1],remove_element.end[0],remove_element.end[1], remove_element.depth);
	singleton_draft->remove_rde(rde);

}

void compile(const char* output_file, supported_file_type type)
{
	singleton_draft->compile(output_file, type);


}

size_t get_img_width()
{

	return singleton_draft->get_img_width();
}

size_t get_img_height()
{

	return singleton_draft->get_img_height();
}

size_t get_rde_data_size()
{

	return singleton_draft->get_rde_data_size();
}

void get_rde_elements(rde* rde_elements, size_t size)
{
	rde_object* elements = (rde_object*) malloc(sizeof(rde_object)*size);
	singleton_draft->get_rde_elements(elements, size);
	for(size_t i = 0; i < size; i++)
	{
		rde_elements[i] = elements[i].get_rde();
	}

}

void close_draft()
{
	draft::close_draft(singleton_draft);

}
