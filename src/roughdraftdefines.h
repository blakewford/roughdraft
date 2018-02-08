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
#ifndef roughdraftdefines_h
#define roughdraftdefines_h
#include <stddef.h>

typedef enum _rds{
	line         =  0,
	rectangle    =  1,
	circle       =  2,
	swatch       =  3,
	dimension    =  4,
	arc			 =  5,
	unknownShape = ~0
}rds;

typedef enum _rdl{
        black = 250,
        green = 3,
        red = 1,
        brown = 15,
        yellow = 2,
        cyan = 4,
        magenta = 6,
        gray = 8,
        blue = 5,
        l_blue = 163,
        l_green = 121,
        l_cyan = 131,
        l_red = 23,
        l_magenta = 221,
        l_gray = 252,
        white = 7,
        bylayer = 256,
        byblock = 0,
        unknownLayer = ~0 
}rdl;

typedef enum _rdp{
	front        =  0,
	back         =  1,
	left_p       =  2,
	right_p      =  3,
	top          =  4,
	bottom       =  5,
	unknownPlane = ~0 
}rdp;

typedef enum _supported_file_type{
	bitmap24         =  0,
	bitmap256        =  1,
	png              =  2,
	unknownExtension = ~0
}supported_file_type;

typedef enum _rdu{
	millimeters      =  0,
	inches           =  1,
	unknownUnit      = ~0
}rdu;

typedef struct _rde{
	rds shape;
	rdl layer;
	unsigned char weight;
	unsigned int start[2];
	unsigned int end[2];

	rdp plane;
	unsigned int plane_offset;
	unsigned int depth;
}rde;

#endif
