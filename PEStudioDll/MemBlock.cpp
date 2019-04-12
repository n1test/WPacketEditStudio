/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  MemBlock.cpp

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com) - CPacketList
	(chris@sourcemagik.com)
  
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
*/

//simple dynamic buffer


#include "stdafx.h"

MemBlock::MemBlock()
{
	alloc_size = 0;
	memory_ptr = NULL;
	readonly = false;
}

MemBlock::MemBlock(unsigned char *buff, size_t sz)
{
	readonly = false;
	mem_init(sz);
	memcpy(memory_ptr, buff, sz);
}

MemBlock::~MemBlock()
{
	if(memory_ptr && !readonly)
		delete [] memory_ptr;
}

void MemBlock::mem_init(size_t sz)
{
	alloc_size = sz;
	memory_ptr = new unsigned char [alloc_size];
}

bool MemBlock::allocate(size_t sz)
{
	unsigned char *copy;
	size_t chunk_sz;

	if(readonly)
		return false;

	chunk_sz = alloc_size;

	if(memory_ptr)
	{
		copy = new unsigned char [chunk_sz];
		memcpy(copy, memory_ptr, chunk_sz);

		delete memory_ptr;

		mem_init(alloc_size + sz);
		memcpy(memory_ptr, copy, chunk_sz);

		delete copy;
	}
	else
		mem_init(sz);

	return true;
}

bool MemBlock::reallocate(size_t sz)
{
	if(readonly)
		return false;

	if(memory_ptr)
	{
		delete memory_ptr;
		memory_ptr = NULL;
	}

	allocate(sz);

	return true;
}

bool MemBlock::mem_set(unsigned char *buff, size_t sz)
{
	if(readonly)
		return false;

	reallocate(sz);
	memcpy(memory_ptr, buff, sz);
	return true;
}

bool MemBlock::mem_attach(unsigned char *buff, size_t sz)
{
	if(readonly)
		return false;

	unsigned char *insert_ptr;
	size_t chunk_sz;

	chunk_sz = alloc_size;

	if(memory_ptr)
	{
		allocate(sz);
		insert_ptr = memory_ptr + chunk_sz;
		memcpy(insert_ptr, buff, sz);
	}
	else
		mem_set(buff, sz);

	return true;
}

size_t MemBlock::get_size()
{
	return alloc_size;
}

unsigned char *MemBlock::get_ptr()
{
	return memory_ptr;
}

unsigned char MemBlock::get_char(size_t index)
{
	if(index >= alloc_size)
		return '\0';

	return memory_ptr[index];
}

bool MemBlock::copy_ptr(MemBlock &object)
{
	memory_ptr = object.get_ptr();
	alloc_size = object.get_size();
	readonly = true;
	return readonly;
}

bool MemBlock::copy_mem(MemBlock &object)
{
	return mem_set(object.get_ptr(), object.get_size());
}
