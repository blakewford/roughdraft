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
#ifndef bmpmanip_h
#define bmpmanip_h
#include "roughdraftdefines.h"
#include "roughdraftdefines_private.h"
#include "imgmanip.h"
#include <vector>

class bmpmanip: public imgmanip {
	
public:
	virtual ~bmpmanip();
	virtual void write(const char* outputFile);
	virtual void extract_rde_vector(std::vector<rde_object>& rde_vector);	

protected:
	bmpmanip(size_t depth, size_t pixel_offset, size_t img_width, size_t img_height, const std::vector<rde_object>& rde_vector, size_t embedding_offset, size_t embedding_bits, size_t embed_start, size_t embedding_bytes_availiable);
	bmpmanip(const char* input_file, size_t embedding_offset, size_t embedding_bits, size_t embed_start, size_t embedding_bytes_availiable);
	unsigned char* _memblock;
	std::vector<rde_object> m_rde_vector;

	const size_t get_bmp_bytes_per_line() const {return _bytes_per_line;};
	const size_t get_bmp_pixel_offset()const {return _pixel_offset;};
	const size_t get_bmp_pixel_depth()const {return _pixel_depth;};
	const size_t get_bmp_pixel_bytes_per_line()const {return _pixel_bytes_per_line;};
	const size_t get_bmp_padding_bytes_per_line()const {return _padding_bytes_per_line;};
        const size_t get_bmp_raw_data_size()const {return _raw_data_size;};
	const size_t get_bmp_size_in_bytes() const{return _size_in_bytes;};

	void set_bmp_embedding_bytes_available(size_t bytes){_embedding_bytes_availiable = bytes;};

	void _embed_rde_vector();
	void _embed_rde_data(size_t& memblock_offset, size_t size_of_data_type, void* data, size_t data_size);
	void _extract_rde_vector();
	virtual void* _extract_rde_data(size_t& memblock_offset, size_t size_of_data_type) = 0;

private:
	//deny generation
	bmpmanip(const bmpmanip&);
	bmpmanip();

private:
        //methods
	void _initialize(const char* input_file);
	void _generateHeaderInfo();
	void _parseHeaderInfo();
	void _initialize_members();
	void _write_data_to_bmp(size_t data, size_t value_offset, size_t number_of_bytes);

private:
	size_t _bytes_per_line;
	size_t _pixel_offset;
	size_t _pixel_depth;
	size_t _pixel_bytes_per_line;
	size_t _padding_bytes_per_line;
        size_t _raw_data_size;
	size_t _size_in_bytes;

	size_t _embedding_start_byte;
	size_t _embedding_offset;
	size_t _chunks_per_byte;
	size_t _embedding_bits_per_byte;
	size_t _embedding_bytes_availiable;
	size_t _embedding_mask;

};

#endif
