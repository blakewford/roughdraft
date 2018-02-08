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
#ifndef roughdraft_h
#define roughdraft_h
#include "roughdraftdefines.h"

#define _2D 0, 0, 0

#ifdef __cplusplus 
extern "C" {
#endif

	void open_draft(size_t img_width, size_t img_height);
	void recover_draft(const char* input_file);

	void add_rde(
		const rds rds_shape, const rdl rdl_layer,
		const size_t rde_weight, const size_t start_x,
		const size_t start_y, const size_t end_x, const size_t end_y,
		//RFU
		const size_t plane,
		const size_t plane_offset,
		const size_t rde_depth
	);

	void add_dimension(const rde element, const size_t dim_height);
	float get_rde_dimension(const rde element, const rdu unit);
	
	void set_background_color(const rdl rdl_layer);
	void remove_rde(const rde remove_element);
	void compile(const char* output_file, supported_file_type type);
	size_t get_img_width();
	size_t get_img_height();
	size_t get_rde_data_size();
	void get_rde_elements(rde* rde_elements, size_t size);

	void close_draft();

#ifdef __cplusplus 
}
#endif 

#endif
