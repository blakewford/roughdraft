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
#ifndef pngmanip_h
#define pngmanip_h
#include "roughdraftdefines.h"
#include "roughdraftdefines_private.h"
#include "imgmanip.h"
#include <png.h>
#include <vector>

class pngmanip: public imgmanip {
	
public:
	pngmanip(size_t img_width, size_t img_height, rdl** canvas, const std::vector<rde_object>& rde_vector);
	pngmanip(const char* input_file);

	virtual void write(const char* outputFile);
	virtual void extract_rde_vector(std::vector<rde_object>& rde_vector);
	virtual ~pngmanip();

private:
	//deny generation
	pngmanip(const pngmanip&);
	pngmanip();

private:
	//Lots of bmpmanip overlap
	void _embed_rde_vector(png_unknown_chunkp);
	void _init_png_members(const char* file, const char* mode);

	png_bytep * m_row_pointers;
	std::vector<rde_object> m_rde_vector;

        FILE *m_fp;
	png_structp m_png_ptr;
	png_infop m_info_ptr;

};

#endif
