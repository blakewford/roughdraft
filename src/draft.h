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
#ifndef draft_h
#define draft_h
#include "roughdraftdefines.h"
#include "roughdraftdefines_private.h"
#include <vector>

class draft{

public:
	virtual ~draft();

        static draft* open_draft(size_t img_width, size_t img_height);
        static draft* open_draft(const char* input_file);

	void add_rde(
		const rds rds_shape,
		const rdl rdl_layer,
		const size_t rde_weight,
		const size_t start_x,
		const size_t start_y,
		const size_t end_x,
		const size_t end_y,
		//For 3D Drafts -- Not Currently Supported
		const size_t plane = 0,
		const size_t plane_offset = 0,
		const size_t rde_depth = 0
	);
	void add_dimension(const rde_object& element, const size_t dim_height);
	void set_background_color(const rdl& rdl_layer);
	void remove_rde(const rde_object& remove_element);

	void compile(const char* output_file, supported_file_type type);

	size_t get_img_width();
	size_t get_img_height();

	size_t get_rde_data_size();
	void get_rde_elements(rde_object* rde_elements, size_t size);

	static void close_draft(draft* draft);
	
private:
	draft();
	draft(const draft&);

	void _draw_rds_line(const rde_object& element);
	void _draw_rds_rectangle(const rde_object& element);
	void _draw_rds_swatch(const rde_object& element);
	void _draw_rds_circle(const rde_object& element);
	void _draw_circle(const size_t pixel_start_x, const size_t pixel_start_y, 
					  const size_t pixel_end_x, const size_t pixel_end_y, const size_t pixel_weight, const rdl layer);
	void _draw_rds_arc(const rde_object& arc1, const rde_object& arc2);
	void _draw_arc(const size_t center_x, const size_t center_y, const size_t arc1_end_x, 
				   const size_t arc1_end_y, const size_t arc2_end_x, const size_t arc2_end_y,
				   const size_t pixel_weight, const rdl layer);
	void _draw_rds_dimension(const rde_object& element);

	void _draw_line(size_t weight, const size_t x1, const size_t y1, const size_t x2, const size_t y2, const rdl layer);
	void _draw_line_weight(size_t weight, const size_t x1, const size_t y1, const size_t x2, const size_t y2, const rdl layer);
	int  _arc_y_equation(const size_t input_x, const size_t center_x, const size_t radius);
	bool _check_bounds(const rde_object& element);

	size_t m_img_width;
	size_t m_img_height;
	std::vector<rde_object> m_rde_vector;
	rdl** m_canvas;
	unsigned char *m_cairo_data;
	
};

#endif
