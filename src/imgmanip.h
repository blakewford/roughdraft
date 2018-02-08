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
#ifndef imgmanip_h
#define imgmanip_h
#include "roughdraftdefines.h"
#include "roughdraftdefines_private.h"
#include <vector>

class imgmanip{
	
public:
	imgmanip(size_t img_width, size_t img_height);
	virtual ~imgmanip(){};

	static bool is_valid(const char* inputFile, supported_file_type& type);
	virtual const size_t get_img_width() const{return m_img_width;};
	virtual const size_t get_img_height()const {return m_img_height;};
	virtual void write(const char* outputFile) = 0;
	virtual void extract_rde_vector(std::vector<rde_object>& rde_vector) = 0;

protected: //methods
	//Temp
	void* _extract_data_from_uncompressed_data_ptr(size_t& data_offset, unsigned char* uncompressed_data, size_t size_of_data_type);

	size_t _get_uncompressed_data_size(unsigned short number_of_shapes);
	unsigned char* _compress_rde_vector(const std::vector<rde_object>& rde_vector);
	void _uncompress_rde_vector(unsigned char* uncompressed_rde_data, unsigned int number_of_shapes, std::vector<rde_object>& rde_vector);

protected: //data
	size_t m_img_width;
	size_t m_img_height;

	unsigned char m_version;
	unsigned char m_scale_antecedent;
	unsigned char m_scale_consequent;

        unsigned int m_uncompressed_rde_data_size;
        unsigned int m_compressed_rde_data_size;

private:
	void _insert_data_into_uncompressed_data_ptr(size_t& data_offset, unsigned char* uncompressed_data, size_t size_of_data_type, void* data);

	//deny generation
	imgmanip(const imgmanip&);
	imgmanip();

};

#endif
