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

#include "parser.h"
#include "roughdraft.h"
#include <stdio.h>
#include <stdlib.h>

const char* JSON = "{\"name\":\"position0\",\"parameters\":{\"w\":0,\"h\":0,\"element\":\"line\",\"layer\":\"black\",\"weight\":1,\"x1\":1,\"y1\":1,\"x2\":95,\"y2\":95}}\n";
int main(int argc, char *argv[])
{
  set_command c;
	parse(JSON, &c);
  printf("%s %s %s %ld\n", c.get_name(), c.get_shape(), c.get_layer(), c.get_weight());
  
	open_draft(96, 96);
	add_rde(line, black, 3, 1, 1, 95, 95, _2D);
	add_rde(circle, black, 9, 48, 48, 56, 48, _2D);

	set_background_color(white);
	compile("./test.png", png);
	
	close_draft();

	return 0;
}
