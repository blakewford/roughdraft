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
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	open_draft(1280, 800);
	add_rde(rectangle, red, 1, 160, 75, 188, 85, _2D);
	add_rde(line, blue, 1, 175, 100, 188, 85, _2D);
	add_rde(circle, cyan, 2, 160, 100, 172, 92, _2D);
	add_rde(arc, brown, 2, 160, 100, 172, 108, _2D);
	add_rde(arc, brown, 2, 160, 100, 174, 100, _2D);
	compile("./testpng.png", png);
	close_draft();

	recover_draft("./testpng.png");

	size_t size = get_rde_data_size();
	rde* elements = (rde*)malloc(sizeof(rde)*size);
	get_rde_elements(elements, size);
	size_t i = 0;
	for(i; i < size; i++)
	{
		add_dimension(elements[i],3);
	}

	set_background_color(white);

	compile("./test24.bmp", bitmap24);
	compile("./test256.bmp", bitmap256);
	compile("./testpng.png", png);
	
	close_draft();

	return 0;
}
