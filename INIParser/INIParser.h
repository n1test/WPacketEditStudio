/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  INIParser.h

  This file is part of the Packet Edit Studio Project.
  version: ALPHA

  *Distributed under the terms of the zlib/libpng public license.*
  
  Author: 

	Chris Smith (sourcemagik.com) - INIParser
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

// Class for handling configuration loading and writing, you can write to multiple
// ini files with the same class

	#pragma once

	//max size of a setting line
	#define MAX_LINE MAX_PATH

	/*************************************************************/
	/*			INIParser										 */
	/*															 */
	/*	An ini file parser										 */
	/*************************************************************/
	class INIParser 
	{
		public:

			/////////////////////////////
			//  Constructor / Destructor
			/////////////////////////////

				INIParser();

				INIParser(LPSTR *table);

				virtual ~INIParser();

			/////////////////////////////
			//  Settings APIs
			/////////////////////////////

				//set the directory it will look in for ini files
				void SetWorkingDirectory(const char *dir);

				//list of files to load
				bool LoadFiles(char **table);

				//load a file
				bool WriteValue(const char *file, const char *var, const char *val);

			/////////////////////////////
			//  Value Querying
			/////////////////////////////

				//string query
				char *strGetVar(const char *file, const char *name);

				//boolean query
				bool boolGetVar(const char *file, const char *name);

				//integer query
				int iGetVar(const char *file, const char *name);

				//long integer query
				long lGetVar(const char *file, const char *name);

			/////////////////////////////
			//  Internal Management
			/////////////////////////////
		private:

				//add variable to the file
				bool AddVar(int f, const char *var, const char *val);

				//returns true if the char is printable
				bool IsGoodChar(char c);

				//build full path to the file
				const char *BuildPath(const char *file);

			/////////////////////////////
			//  Private Variables
			/////////////////////////////

				char *buffers[MAX_PATH][1000];				//buffer for holding values
				char *files[10];							//buffer for holding file names
				char dir[MAX_PATH];							//working directory
				int file_index;								//current slot in "dir"
	};