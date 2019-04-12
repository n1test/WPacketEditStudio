/* 

convert.h
Copyright (C) 2006  Chris Devereaux
www.source-magik.com

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

#include "stdafx.h"

#define __CONVERT__powerof(x,y) (unsigned int)pow((double)x, (int)y)

int axtoi(char *s)
{
	int ret = 0;
	unsigned int i, z, y, inhex;

	inhex = 0;

	for(i=(unsigned int)strlen(s)-1, z=0;i>=0;--i,++z)
	{
		y = __CONVERT__powerof(16, i);
		if(s[z] >= '0' && s[z] <= '9')
			ret += y * (s[z] - 0x30);
		else if((s[z] >= 'a' && s[z] <= 'f')
			    || (s[z] >= 'A' && s[z] <= 'F'))
			if(inhex == 0)
				ret += (s[z] - 0x57) * y;
			else
				ret += (s[z] - 0x57);
		if(y == 1)
			break;

		++inhex;

		if(inhex >= 1)
			inhex = 0;
	}

	return ret;
}

int abtoi(char *s)
{
	int ret = 0;
	unsigned int i, y, z;

	for(i=(unsigned int)strlen(s)-1, z=0;i>=0;--i, ++z)
	{
		y = __CONVERT__powerof(2, i);
		if(s[z] == '1')
			ret += y;
		if(y==1)
			break;
	}

	return ret;
}

const char *itoa(int i)
{
	static char p[35];
	int first, cur, num, y, z, tmp;

	if(i<0)
		tmp = -i;
	else if(i > 0)
		tmp = i;
	else
	{
		p[0] = '0';
		p[1] = '\0';
		return (const char *)&p;
	}

	for(y=1000000000,cur=tmp,z=0,first=0;y>=1;y=y/10)
	{
		num = cur / y;
		if((num)||(first))
		{
			if(!first)
				if(i<0)
				{
					p[z] = '-';
					++z;
				}
			p[z] = num + 0x30;
			p[z+1] = 0;
			++z;
			first = 1;
		}
		cur = cur - (y*num);
	}

	return (const char *)&p;

}

const char *btoa(unsigned char i)
{
	int s;
	double x;
	static char p[3];
	char *hex = "0123456789ABCDEF";

	x = (double)i;
	s = (int)floor(x / 16);
	p[0] = hex[s];
	p[1] = hex[i - s*16];
	p[2] = '\0';

	return (const char *)p;
}

unsigned char cto16(char c)
{
	if(c < 0x40)
		return c - 0x30;
	else
		return (c > 0x60) ? c - 0x57 : c - 0x37;
}

unsigned char cto8(char c)
{
	return c - 0x30;
}

int axtoa(char *str, unsigned int len, unsigned char *buffer)
{
	unsigned int i, y;

	for(i = 0, y = 0 ; i < len ; i += 2, ++y)
		buffer[y] = cto16(str[i])*16 + cto16(str[i+1]);

	return y;
}

const char *ioctal4(short i)
{
	char hex[] = "0123456789ABCDEF";
	char *ptr, *h;
	static char oct[5];

	h = (char *)btoa('\x00');

	ptr = (char *)&i;

	btoa(ptr[1]);
	oct[0] = h[0];
	oct[1] = h[1];

	btoa(ptr[0]);
	oct[2] = h[0];
	oct[3] = h[1];
	oct[4] = '\0';

	return (const char *)oct;
}

const char *ioctal8(int i)
{
	char hex[] = "0123456789ABCDEF";
	char *ptr, *h;
	static char oct[9];

	h = (char *)btoa('\x00');

	ptr = (char *)&i;

	btoa(ptr[3]);
	oct[0] = h[0];
	oct[1] = h[1];

	btoa(ptr[2]);
	oct[2] = h[0];
	oct[3] = h[1];

	btoa(ptr[1]);
	oct[4] = h[0];
	oct[5] = h[1];

	btoa(ptr[0]);
	oct[6] = h[0];
	oct[7] = h[1];
	oct[8] = '\0';

	return (const char *)oct;
}


#ifndef PESTUDIO_DLL

void full_dump(char *octal, char *hex, char *ascii, unsigned char *dat, unsigned int sz, int lim)
{
	char *ptr;
	int c16;
	int size = sz;
	unsigned int i, y, z, c, real_lim;
	char *hexdat;

	real_lim = (lim) ? lim : 0xFFFFFFFF;

	ptr = octal;
	c16 = 0;
	while(c16 < size)
	{
		if(real_lim <= 0x40)
		{
			strcpy_s(ptr, STATIC_BUFFER_SIZE, "    ");
			strcat_s(ptr, STATIC_BUFFER_SIZE, ioctal4((short)c16));
			strcat_s(ptr, STATIC_BUFFER_SIZE, "\n");
			ptr += 9;
		}
		else
		{
			strcpy_s(ptr, STATIC_BUFFER_SIZE, "    ");
			strcat_s(ptr, STATIC_BUFFER_SIZE, ioctal8((short)c16));
			strcat_s(ptr, STATIC_BUFFER_SIZE, "\n");
			ptr += 13;
		}

		c16 += 16;

		if((unsigned int)c16 > real_lim)
			break;
	}

	hexdat = (char *)btoa('\x00');
	c16 = 1;
	c = 0;
	for(i = 0, y = 0, z = 0; i < sz; ++i)
	{
		btoa(dat[i]);
		hex[y+0] = hexdat[0];
		hex[y+1] = hexdat[1];
		hex[y+2] = ' ';
		y+= 3;

		ascii[z] = ((dat[i] < ' ') || (dat[i] >= 0x7f)) ? '.' : dat[i];
		++z;

		if(c16 == 16)
		{
			c16 = 0;
			hex[y] = '\n';
			ascii[z] = '\n';
			++y;
			++z;
			c+=16;
		}

		if(c > real_lim)
			break;

		++c16;
	}

	hex[y] = '\0';
	ascii[z] = '\0';
}

#endif

void small_dump(char *hex, unsigned char *dat, unsigned int sz)
{
	int c16;
	int size = sz;
	unsigned int i, y, c;
	char *hexdat;

	hexdat = (char *)btoa('\x00');
	c16 = 1;
	c = 0;
	for(i = 0, y = 0; i < sz; ++i)
	{
		btoa(dat[i]);
		hex[y+0] = hexdat[0];
		hex[y+1] = hexdat[1];
		hex[y+2] = ' ';
		y+= 3;

		if(c16 == 16)
		{
			c16 = 0;
			hex[y] = '<';
			hex[y+1] = 'b';
			hex[y+2] = 'r';
			hex[y+3] = '>';
			y+=4;
			c+=16;
		}

		++c16;
	}

	hex[y] = '\0';
}