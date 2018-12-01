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

#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <string>

#include "parser.h"
#include "roughdraft.h"

const char* JSON = "{\"name\":\"position0\",\"parameters\":{\"w\":96,\"h\":96,\"element\":\"line\",\"layer\":\"black\",\"weight\":1,\"x1\":1,\"y1\":1,\"x2\":95,\"y2\":95}}\n";

int main(int argc, char *argv[])
{

  std::ifstream sample("sample.json");

  set_command c;
  std::string line;
  std::getline(sample, line);
  std::istringstream iss(line);
  parse(line.c_str(), &c);
  open_draft(c.get_width(), c.get_height());
  while(std::getline(sample, line))
  {
      std::istringstream iss(line);
      parse(line.c_str(), &c);
      add_rde(c.get_shape(), c.get_layer(), c.get_weight(), c.get_x1(), c.get_y1(), c.get_x2(), c.get_y2(), _2D);
  }

	set_background_color(green);
	compile("./test.png", png);
	
	close_draft();

	return 0;
}
