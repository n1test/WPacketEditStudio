/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  convert.h

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

// A class for storing and formatting the packets returned by the dll's
// sniffing mechanism

// (I also go (have gone) by the name Chris Devereaux

/* 

convert.h
Copyright (C) 2006  Chris Devereaux
www.sourcemagik.com

/*

	These functions are some that I have found useful that are not
	included with the standard library (most of the time).  They demonstrate
	basic computer math and base conversion.

--------------------------------------------------------------------------

    axtoi can be used to convert a string of acsii values from base 16 (hex)
	to base a base 10 integer (decimal).
	
	Eg. char *s = "2AFF"

	int i = axtoi(s) will convert the string s to 32 bit decimal format.  i
	will now equal 11,007.
    
    s - string of hex to convert

	(Non-Hex chars will be treated as 0's)
    
    returns the 32 bit decimal number
    
--------------------------------------------------------------------------

    abtoi can be used to convert a string of binary values from base 2 (binary)
	to base a base 10 integer (decimal).
	
	Eg. char *s = "01100001"

	int i = abtoi(s) will convert the string s to 32 bit decimal format.  i
	will now equal 97.
    
    s - string to convert

	(Non-Binary chars will be treated as 0's)
    
    returns the 32 bit decimal number

--------------------------------------------------------------------------

	I find this function very useful as it is a commonly used one that is not
	included with the standard library.

    itoa can be used to convert a 32 bit decimal number to string representation.
	
	Eg. int i = 1234;

	char *s = itoa(i) will convert the integer i into a string.
    
    i - integer to convert

	(No need to free the return value it is a static buffer)
    
    returns a pointer to the static buffer
    
--------------------------------------------------------------------------
    
    axtoui(s)					- convert to unsigned int
	abtoui(s)					- convert to unsigned int
    
*/

#include <math.h>

#define axtoui(s) (unsigned int)axtoi(s)
#define abtoui(s) (unsigned int)abtoi(s)

int axtoi(char *s);
int abtoi(char *s);
const char *itoa(int i);
const char *btoa(unsigned char i);
unsigned char cto16(char c);
unsigned char cto8(char c);
int axtoa(char *str, unsigned int len, unsigned char *buffer);
void full_dump(char *octal, char *hex, char *ascii, unsigned char *dat, unsigned int sz, int lim=0);
void small_dump(char *hex, unsigned char *dat, unsigned int sz);
