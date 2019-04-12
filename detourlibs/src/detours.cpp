/*--------------------------------------------------------------------*/
/* GS5.dll : Ghost Recon Class Library
/*
/* detours.cpp : detour functions
/*
/* Copyright 1995-2001, Microsoft Corporation
/*
/* This program is free software; you can redistribute it and/or
/* modify it under the terms of the GNU General Public License
/* as published by the Free Software Foundation; either version 2
/* of the License, or (at your option) any later version.
/*
/* This program is distributed in the hope that it will be useful,
/* but WITHOUT ANY WARRANTY; without even the implied warranty of
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/* GNU General Public License for more details.
/*
/* You should have received a copy of the GNU General Public License
/* along with this program; if not, write to the Free Software
/* Foundation, Inc., 51 Franklin Street, Fifth Floor,
/* Boston, MA  02110-1301, USA.
/*--------------------------------------------------------------------*/

#include "stdafx.h"

/**************************************************/
/*												  */
/*		Name: GetFileDirectory					  */
/*												  */
/*		Purpose: Gets the directory of the file	  */
/*												  */
/*												  */
/**************************************************/
char *GetDirectoryFile(char *dlldir, char *filename)
{
	static char path[320];
	strcpy(path, dlldir);
	strcat(path, filename);

	return path;
}

/**************************************************/
/*												  */
/*		Name: DetourFunc						  */
/*												  */
/*		Purpose: Hooks an API					  */
/*												  */
/*												  */
/**************************************************/
void *DetourFunc(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len+5);
	DWORD dwback;

	VirtualProtect(src, len, PAGE_READWRITE, &dwback);

	memcpy(jmp, src, len);	jmp += len;
	
	jmp[0] = 0xE9;
	*(DWORD*)(jmp+1) = (DWORD)(src+len - jmp) - 5;

	src[0] = 0xE9;
	*(DWORD*)(src+1) = (DWORD)(dst - src) - 5;

	VirtualProtect(src, len, dwback, &dwback);

	return (jmp-len);
}

/**************************************************/
/*												  */
/*		Name: RetourFunc						  */
/*												  */
/*		Purpose: Unhooks and API				  */
/*												  */
/*												  */
/**************************************************/
bool RetourFunc(BYTE *src, BYTE *restore, const int len)
{
	DWORD dwback;
		
	if(!VirtualProtect(src, len, PAGE_READWRITE, &dwback))	{ return false; }
	if(!memcpy(src, restore, len))							{ return false; }

	restore[0] = 0xE9;
	*(DWORD*)(restore+1) = (DWORD)(src - restore) - 5;

	if(!VirtualProtect(src, len, dwback, &dwback))			{ return false; }
	
	return true;
}
