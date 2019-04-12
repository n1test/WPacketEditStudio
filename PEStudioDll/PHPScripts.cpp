/*
  Packet Edit Studio Copyright (C) 2008 Chris Smith

  PHPScripts.cpp

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

// The precompiled header file, all includes are included here

#include "stdafx.h"

#pragma warning ( disable : 4800 )

/*************************************************************/
/*			PHP Code Output									 */
/*************************************************************/

void phplog(char *path, char *script, int i, char *in, char *out, int in_sz, int out_sz, char *output)
{
	std::ofstream f;
	static char tmp[18000];

	if(i == 1)
		DeleteFile(path);

	f.open(path, std::ios::app);

	if(!f.good())
		return;

	if(i == 1)
	{
		f << "<html>\n"
		  << "\t<head>\n"
		  <<		"\t\t<!--Packet Edit Studio PHP Output File-->\n"
		  <<		"\t\t<title>blank</title>\n"
		  << "\t</head>\n"
		  << "\n"
		  << "\t<body>\n";
	}
	else if(!output)
	{
		f << "\n"
		  << "\t</body>\n"
		  << "\n"
		  << "</html>\n";
		return;
	}

	f << "\n"
	  << 		"\t\t<br>\n"
	  << "\n"
	  <<		"\t\t<table cellpadding=\"0\" cellspacing=\"0\" width=\"100%\" style=\"border:2px solid black;\"><tr><td>\n"
	  <<		"\t\t<table bgcolor=\"#AAAAAA\" width=\"100%\">\n"
	  <<			"\t\t\t<tr>\n"
	  <<				"\t\t\t\t<td width=\"10%\"><b>" << script << "</b></td>\n"
	  <<				"\t\t\t\t<td><b>" << i << "</b></td>\n"
	  <<			"\t\t\t</tr>\n"
	  <<		"\t\t</table>\n"
	  <<		"\t\t<table bgcolor=\"#CCCCCC\" width=\"100%\">\n"
	  <<			"\t\t\t<tr>\n"
	  <<				"\t\t\t\t<td><b>Results:</b><br><a name=\"" << i << "\"></a>\n"
	  <<					"\t\t\t\t\t<table style=\"border:2px solid gray;\" bgcolor=\"#EEEEEE\" width=\"85%\" align=\"center\" cellpadding=\"3\">\n"
	  <<						"\t\t\t\t\t\t<tr>\n"
	  <<							"\t\t\t\t\t\t\t<td><code>" << output << "</code></td>\n"
	  <<						"\t\t\t\t\t\t</tr>\n"
	  <<					"\t\t\t\t\t</table>\n"
	  <<				"\t\t\t\t</td>\n"
	  <<			"\t\t\t</tr>\n"
	  <<		"\t\t</table>\n"
	  <<		"\t\t<table bgcolor=\"#AAAAAA\" width=\"100%\">\n"
	  <<			"\t\t\t<tr>\n"
	  <<				"\t\t\t\t<td width=\"50%\"><b>Input Data</b> - " << in_sz  << " byte(s)</td>\n"
	  <<				"\t\t\t\t<td width=\"50%\"><b>Output Data</b> - "   << out_sz << " byte(s)</td>\n"
	  <<			"\t\t\t</tr>\n"
	  <<		"\t\t</table>\n"
	  <<		"\t\t<table bgcolor=\"#CCCCCC\" width=\"100%\">\n"
	  <<			"\t\t\t<tr>\n"
	  <<				"\t\t\t\t<td>\n"
	  <<					"\t\t\t\t\t<table style=\"border:2px solid gray;\" bgcolor=\"#EEEEEE\" width=\"100%\" align=\"center\" cellpadding=\"3\">\n"
	  <<						"\t\t\t\t\t\t<tr>\n"
	  <<							"\t\t\t\t\t\t\t<td valign=\"top\" width=\"50%\" style=\"border:2px solid gray;\"><code>";
	
	small_dump(tmp, (unsigned char *)in, in_sz);
	f <<							tmp << "</code></td>\n"
	  <<							"\t\t\t\t\t\t\t<td valign=\"top\" width=\"50%\" style=\"border:2px solid gray;\"><code>";

	small_dump(tmp, (unsigned char *)out, out_sz);
	f <<							tmp << "</code></td>\n"
	  <<						"\t\t\t\t\t\t</tr>\n"
	  <<					"\t\t\t\t\t</table>\n"
	  <<				"\t\t\t\t</td>\n"
	  <<			"\t\t\t</tr>\n"
	  <<		"\t\t</table>\n"
	  <<		"\t\t</table>\n";

	f.close();
}

char *char2php(char c)
{
	int sixteens, ones;
	static char str[5];
	char *hex = "0123456789abcdef";

	sixteens = (int)floor((double)(c / 16));
	ones = (c - (sixteens * 16));

	str[0] = '\\';
	str[1] = 'x';
	str[2] = hex[sixteens];
	str[3] = hex[ones];
	str[4] = '\0';

	return str;
}

void buff2php(char *to, char *buff, int sz)
{
	int i;

	to[0] = '\0';

	for(i = 0 ; i < sz; ++i)
		strcat_s(to, buffersize, char2php(buff[i]));
}


void php2buff(char *buff)
{
	int i, y, sz;

	sz = (int) strlen(buff);

	for(i = 0, y = 0 ; i < sz ; ++i)
	{

		/* ESCAPE CHARACTER PHP PARSE */

		if(buff[i] == '\\')
		{

			switch(buff[i+1])
			{	
				/* SINGLE CHAR PHP PARSE */

				case 'n':
					buff[y] = '\x0a';
					++i;
					break;
				case 'r':
					buff[y] = '\x0d';
					++i;
					break;
				case 't':
					buff[y] = '\x09';
					++i;
					break;
				case 'b':
					buff[y] = '\x0b';
					++i;
					break;
				case 'f':
					buff[y] = '\x0c';
					++i;
					break;
				case '\\':
					buff[y] = '\\';
					++i;
					break;
				case '$':
					buff[y] = '$';
					++i;
					break;
				case '\"':
					buff[y] = '\"';
					++i;
					break;

				/* BASE 16 PHP PARSE */
				case 'X':
				case 'x':

		            //   i   i+1    i+2    i+3
					//   \   x       0     END
					if(buff[i+3] == '\\' || buff[i+3] == 0x00)
					{
						buff[y] = (char)cto16(buff[i+1]);
						i += 2;
					}

		            //      i    i+1       i+2      i+3    i+4
					//      \     x         0       0      END
					else if(buff[i+4] == '\\' || buff[i+4] == 0x00)
					{
						buff[y] = (char)(cto16(buff[i+2]) * 16) + (char)cto16(buff[i+3]);
						i+=3;
					}

	    	        //      i    i+1       i+2
					//      \     x        END
					else
						buff[y] = buff[i];

					break;

				/* BASE 8 PHP PARSE */
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:

					//    i    i+1    i+2
					//    \     0     END
					if(!isdigit(buff[i+2]))
					{
						buff[y] = cto8(buff[i+1]);
						++i;
					}

					//    i    i+1    i+2    i+3
					//    \     0     0      END
					else if(!isdigit(buff[i+3]))
					{
						buff[y] = (char)(cto8(buff[i+1]) * 8) + (char)cto8(buff[i+2]);
						i += 2;
					}

					//    i    i+1    i+2    i+3    i+4
					//    \     0     0      0      END
					else
					{
						buff[y] = (char)(cto8(buff[i+1]) * 64) + (char)(cto8(buff[i+2]) * 8) + (char)cto8(buff[i+3]);
						i += 3;
					}

					break;

				/* INVALID ESCAPE */
				default:
					buff[y] = buff[i];
					++i;
					break;
			}

			++y;
		}

		/* ASCII STRING PHP PARSE */

		else
		{
			buff[y] = buff[i];
			++y;
		}
	}
}

void phpstart(char *buffer, size_t sz)
{
	strcpy_s(buffer, sz, "<?php\n");
}

void phpstop(char *buffer, size_t sz)
{
	strcat_s(buffer, sz, "?>\n");
}

void phpinclude(char *buffer, size_t sz, char *path, char *file=NULL)
{
	strcat_s(buffer, sz, "\tinclude('");
	strcat_s(buffer, sz, path);
	if(file)
	{
		strcat_s(buffer, sz, "\\");
		strcat_s(buffer, sz, file);
	}
	strcat_s(buffer, sz, "');\n");
}

void phpintvar(char *buffer, size_t sz, char *name, int val)
{
	strcat_s(buffer, sz, "\t$");
	strcat_s(buffer, sz, name);
	strcat_s(buffer, sz, " = ");
	strcat_s(buffer, sz, itoa(val));
	strcat_s(buffer, sz, ";\n");
}

void phpstrvar(char *buffer, size_t sz, char *name, char *val)
{
	strcat_s(buffer, sz, "\t$");
	strcat_s(buffer, sz, name);
	strcat_s(buffer, sz, " = \"");
	strcat_s(buffer, sz, val);
	strcat_s(buffer, sz, "\";\n");
}

/*************************************************************/
/*			PHPScripts										 */
/*************************************************************/

PHPScripts::PHPScripts()
{
}

PHPScripts::~PHPScripts()
{
}

void PHPScripts::Empty()
{
	PHPScript *ptr, *ptr_next;
	char log_name[MAX_PATH];

	strcpy_s(log_name, MAX_PATH, base_log_path);
	strcat_s(log_name, MAX_PATH, "\\output-");
	strcat_s(log_name, MAX_PATH, itoa(HookInterface.GetHookHandle()));
	strcat_s(log_name, MAX_PATH, ".html");

	phplog(log_name, NULL, NULL, NULL, 0, 0, 0, NULL);

	ptr = base;

	while(ptr)
	{
		ptr_next = ptr->next;
		ptr_next = ptr->next;

		delete ptr;

		ptr = ptr_next;
	}

	
}

//load xml
void PHPScripts::ReloadLoadList()
{
	TiXmlDocument list_file;
	TiXmlNode *current_node, *script_node;
	TiXmlElement *elmt_node;
	TiXmlAttribute *elmt_attr;
	PHPScript *ptr;

	Empty();

	base = NULL;
	
	if(list_file.LoadFile(list_path))
	{
		current_node = list_file.FirstChildElement();
		while(current_node)
		{
			if(!strcmp(current_node->Value(), "PEStudio"))
			{
				current_node = current_node->FirstChild();
				while(current_node)
				{
					if(!strcmp(current_node->Value(), "Script"))
					{
						ptr = AllocateItemSpace();

						elmt_node = (TiXmlElement *)current_node;
						elmt_attr = (TiXmlAttribute *)elmt_node->FirstAttribute();
						ptr->loaded = (bool)atoi(elmt_attr->Value());

						script_node = current_node->FirstChild();

						while(script_node)
						{
							if(!strcmp(script_node->Value(), "name"))
							{
								strcpy_s(ptr->name, 255, script_node->FirstChild()->Value());

								if(log)
								{
									strcpy_s(ptr->log_path, MAX_PATH, Process::dlldir);
									strcat_s(ptr->log_path, MAX_PATH, "\\logs\\php\\");
									strcat_s(ptr->log_path, MAX_PATH, Process::name);
									CreateDirectory(ptr->log_path, NULL);

									strcpy_s(base_log_path, MAX_PATH, ptr->log_path);

									strcat_s(ptr->log_path, MAX_PATH, "\\");
									strcat_s(ptr->log_path, MAX_PATH, ptr->name);
									strcat_s(ptr->log_path, MAX_PATH, "-");
									strcat_s(ptr->log_path, MAX_PATH, itoa((int)time(NULL)));
									CreateDirectory(ptr->log_path, NULL);
								}

								ptr->log_index = 0;
							}
							/*
							else if(!strcmp(script_node->Value(), "author"))
							{
								author_loaded = true;
								strcpy_s(ptr->author, 255, script_node->FirstChild()->Value());
							}
							else if(!strcmp(script_node->Value(), "version"))
							{
								version_loaded = true;
								strcpy_s(ptr->version, 255, script_node->FirstChild()->Value());
							}*/

							if(!strcmp(script_node->Value(), "index"))
								ptr->index = atoi(script_node->FirstChild()->Value());

							else if(!strcmp(script_node->Value(), "path"))
								strcpy_s(ptr->path, 255, script_node->FirstChild()->Value());

							script_node = script_node->NextSibling();
						}
					}

					current_node = current_node->NextSibling();
				}

				break;
			}
			current_node = list_file.NextSiblingElement();
		}
	}
}

//new list member
PHPScript *PHPScripts::AllocateItemSpace()
{
	PHPScript *ptr;

	if(!base)
	{
		base = new PHPScript;
		ptr = base;
	}
	else
	{
		ptr = base;
		while(ptr)
		{
			if(!ptr->next)
			{
				ptr->next = new PHPScript;
				ptr = ptr->next;
				break;
			}
			ptr = ptr->next;
		}
	}

	ptr->next = NULL;

	return ptr;
}

//set paths
void PHPScripts::SetPath()
{
	HMODULE handle;
	char dll_path[MAX_PATH], current_path[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, current_path);

	strcpy_s(list_path, MAX_PATH, Process::dlldir);
	strcpy_s(dll_path, MAX_PATH, Process::dlldir);

	strcat_s(list_path, MAX_PATH, "\\phpengine\\scripts.xml");
	
	strcat_s(dll_path, MAX_PATH, "\\phpengine\\");
	strcat_s(dll_path, MAX_PATH, pConfigParser->strGetVar("pestudio.ini", "phpver"));
	SetCurrentDirectory(dll_path);

	log = pConfigParser->boolGetVar("pestudio.ini", "logphp");

	handle = LoadLibrary("php4app.dll");
	
	if( handle == NULL )
            return ;

	// import
	lpfnRegisterVariable = (RegisterVariable) GetProcAddress(handle, "RegisterVariable");
	lpfnExecutePHP = (ExecutePHP) GetProcAddress(handle, "ExecutePHP");
	lpfnExecuteCode = (ExecuteCode)GetProcAddress(handle, "ExecuteCode");
	lpfnInitRequest = (InitRequest)GetProcAddress(handle, "InitRequest");
	lpfnDoneRequest = (DoneRequest)GetProcAddress(handle, "DoneRequest");
	lpfnGetResultText = (GetResultText)GetProcAddress(handle, "GetResultText");
	lpfnGetVariableSize = (GetVariableSize)GetProcAddress(handle, "GetVariableSize");
	lpfnGetVariable = (GetVariable)GetProcAddress(handle, "GetVariable");
	lpfnSaveToFile = (SaveToFile)GetProcAddress(handle, "SaveToFile");
	lpfnGetResultBufferSize = (GetResultBufferSize)GetProcAddress(handle, "GetResultBufferSize");

	base = NULL;

	if(lpfnRegisterVariable == NULL ||
            lpfnExecutePHP  == NULL ||
			lpfnExecuteCode == NULL )
         {
            FreeLibrary( handle ) ;
            return;
         }

	SetCurrentDirectory(current_path);
}

// here is where the magic happens
void PHPScripts::ApplyScript(char *buff, int sz, MemBlock &MemObject, unsigned int iAPI)
{
	char *php_packet;
	static MemBlock send_data;
	int request_id, send_sz, out_size;
	PHPScript *ptr;
	char log_name[MAX_PATH];
	char code[6000];
	char *output;

	if(CaptureEngine::LoadPHP)
	{
		send_data.mem_set((unsigned char *)buff, sz);
		send_sz = sz;

		php_packet = new char [buffersize];

		ptr = base;

		while(ptr)
		{
			++ptr->log_index;
			if(ptr->log_index == INT_MAX)
				ptr->log_index = 1;

			buff2php(php_packet, (char *)send_data.get_ptr(), send_sz);

			request_id = lpfnInitRequest();

			//register variables we need to access later
			lpfnRegisterVariable(request_id, "bytedata", "0");
			lpfnRegisterVariable(request_id, "bytelength", "0");

			phpstart(code, 6000);
				phpintvar(code, 6000, "bytelength", send_sz);
				phpintvar(code, 6000, "api", iAPI);
				phpstrvar(code, 6000, "bytedata", php_packet);
				phpinclude(code, 6000, Process::dlldir, "includes\\script_header.php");
				phpinclude(code, 6000, ptr->path);
				phpinclude(code, 6000, Process::dlldir, "includes\\script_footer.php");
			phpstop(code, 6000);

			lpfnExecuteCode(request_id, code);

			lpfnGetVariable(request_id, "bytelength", php_packet, buffersize);

			send_sz = atoi(php_packet);

			lpfnGetVariable(request_id, "bytedata", php_packet, buffersize);
			//MessageBox(NULL, php_packet, "test", MB_OK);
			//php2buff(php_packet);

			send_data.mem_set((unsigned char *)php_packet, send_sz);

			out_size = lpfnGetResultBufferSize(request_id);
			output = new char [out_size];
			lpfnGetResultText(request_id, output, out_size);

			strcpy_s(log_name, MAX_PATH, base_log_path);
			strcat_s(log_name, MAX_PATH, "\\output-");
			strcat_s(log_name, MAX_PATH, itoa(HookInterface.GetHookHandle()));
			strcat_s(log_name, MAX_PATH, ".html");

			phplog(log_name, ptr->name, ptr->log_index, buff, php_packet, sz, send_sz, output);
			delete [] output;

			if(log)
			{
				strcpy_s(log_name, MAX_PATH, ptr->log_path);
				strcat_s(log_name, MAX_PATH, "\\");
				strcat_s(log_name, MAX_PATH, ptr->name);
				strcat_s(log_name, MAX_PATH, "-");
				strcat_s(log_name, MAX_PATH, itoa(ptr->log_index));
				strcat_s(log_name, MAX_PATH, ".log");
				lpfnSaveToFile(request_id, log_name);
			}

			lpfnDoneRequest(request_id);

			ptr = ptr->next;
		}

		delete [] php_packet;

		MemObject.copy_mem(send_data);
	}
	else
		MemObject.mem_set((unsigned char *)buff, (size_t)sz);
}

#pragma warning ( default : 4800 )