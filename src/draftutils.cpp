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
#include "draftutils.h"
#include <stdio.h>
#include <cmath>
namespace draftutils {

	bool find_lowest(size_t& lowest_value, size_t& highest_value)
	{
		size_t temp_val = lowest_value;
		bool switched = false;
		if(lowest_value > highest_value)
		{
			lowest_value = highest_value;
			highest_value = temp_val;
			switched = true;
		}
		
		return switched;
		
	}
	
	float get_length(size_t x1, size_t y1, size_t x2, size_t y2)
	{		
		find_lowest(y1, y2);
		float ydiff = y2 - y1;
		find_lowest(x1, x2);		
		float xdiff = x2 - x1;
		
		return sqrt((xdiff*xdiff)+(ydiff*ydiff));
	}
}
