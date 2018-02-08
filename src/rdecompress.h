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
#ifndef rdecompress_h
#define rdecompress_h

class rdecompress{

public:
	virtual ~rdecompress();
	static int 
	rde_deflate(unsigned char* uncompressed_data, unsigned char* compressed_data, unsigned int uncompressed_data_size, unsigned int& compressed_data_size);
	static int
	rde_inflate(unsigned char* uncompressed_data, unsigned char* compressed_data, unsigned int uncompressed_data_size, unsigned int compressed_data_size);

private:
	/* report a zlib or i/o error */
	static void _zerr(int ret);

};

#endif
