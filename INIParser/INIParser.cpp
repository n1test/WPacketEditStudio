#include "stdafx.h"

#undef strcpy
#undef strcat

#define strcpy(x,y) strcpy_s(x,strlen(y)+1,y)
#define strcat(x,y) strcat_s(x, strlen(y)+strlen(x)+1, y)

INIParser::INIParser()
{
	this->file_index = 0;
	this->dir[0] = '\0';
	memset(this->buffers, 0, sizeof(this->buffers));
}

INIParser::INIParser(LPSTR *table)
{
	INIParser::INIParser();
	this->LoadFiles(table);
}

INIParser::~INIParser()
{
	int i, y;

	for(i = 0; i < this->file_index; ++i)
		delete [] this->files[i];

	for(i = 0; i < this->file_index ; ++i)
	{
		y = 0;
		while(this->buffers[i][y])
		{
			delete [] this->buffers[i][y];
			++y;
		}
	}
}

//loads a set of ini config files
bool INIParser::LoadFiles(LPSTR *table)
{
	int x, y, z;
	std::ifstream file;
	char tmp[MAX_LINE];

	// load the file name buffer
	int i = this->file_index;
	while(table[i])
	{
		if(!strlen(table[i]))
			break;

		// Make sure the line is not too long
		if(strlen(table[i]) < MAX_LINE)
		{
			this->files[this->file_index] = new char [MAX_LINE];
			strcpy(this->files[this->file_index], table[i]);
			++this->file_index;
		}
		++i;
	}

	//load the settings for each file
	for(i = 0; i < this->file_index ; ++i)
	{
		file.open(this->BuildPath(this->files[i]), std::ios::in);

		if(!file.good())
			return false;

		y = 0;
		x = 0;
		while(!file.eof())
		{
			this->buffers[i][y] = new char[MAX_LINE];
			file.getline(tmp, MAX_LINE);
			for(x = 0, z = 0; x <= (int)strlen(tmp) ; ++x)
				if(this->IsGoodChar(tmp[x]))
				{
					this->buffers[i][y][z] = tmp[x];
					
					//prevent buffer overflow
					if(z == (MAX_LINE-1))
						break;

					++z;
				}
				this->buffers[i][y][z] = '\0';
			++y;
		}

		file.close();
	}

	return true;

}

//string variable access
char *INIParser::strGetVar(const char *file, const char *name)
{
	int i, y, len;
	char tmp[MAX_LINE];

	strcpy(tmp, name);
	strcat(tmp, "=");
	len = (int)strlen(tmp);

	for(i = 0; i < this->file_index ; ++i)
	{
		if(!strcmp(this->files[i], file))
		{
			y = 0;
			while(this->buffers[i][y])
			{
				if(!strncmp(this->buffers[i][y], tmp, len))
					return &this->buffers[i][y][len];
				++y;
			}
			break;
		}
	}

	return "\0";
}

//int variable access
int INIParser::iGetVar(const char *file, const char *name)
{
	return atoi(this->strGetVar(file, name));
}

//bool variable access
bool INIParser::boolGetVar(const char *file, const char *name)
{
	if(!atoi(this->strGetVar(file, name)))
		return false;
	return true;
}

//long variable access
long INIParser::lGetVar(const char *file, const char *name)
{
	return atol(this->strGetVar(file, name));
}

//isprint
bool INIParser::IsGoodChar(char c)
{
	if(isprint(c))
		return true;
	return false;
}

//edits or adds a config value
bool INIParser::WriteValue(const char *file, const char *var, const char *val)
{
	int i, y, len;
	std::ofstream file_s;
	char tmp[MAX_LINE], tmp2[MAX_LINE];

	strcpy(tmp, var);
	strcat(tmp, "=");
	len = (int)strlen(tmp);

	strcpy(tmp2, tmp);

	bool added = false;

	// find the file
	for(i = 0; i < this->file_index ; ++i)
	{

		// file found
		if(!strcmp(this->files[i], file))
		{
			file_s.open(this->BuildPath(this->files[i]), std::ios::out);

			y = 0;
			while(this->buffers[i][y])
			{
				//try to update an existing member
				if(!strncmp(this->buffers[i][y], tmp, len))
				{
					strcat(tmp2, val);
					strcat(tmp2, "\n");
					strcpy(this->buffers[i][y], tmp2);
					added = true;
				}

				file_s.write(this->buffers[i][y], (std::streamsize)strlen(this->buffers[i][y]));
				++y;
			}

			file_s.close();

			//if the settings was not written then add it to the file
			if(!added)
			{
				if(this->AddVar(i, var, val))
					return this->WriteValue(file, var, val);
				else
					return false;
			}

			return true;
		}
	}

	return false;
}

//adds a variable to the config file
bool INIParser::AddVar(int f, const char *var, const char *val)
{
	int i;
	i = 0;

	if(this->files[f])
	{
		while(this->buffers[f][i])
			++i;
		buffers[f][i] = new char[MAX_LINE];
		strcpy(this->buffers[f][i], var);
		strcat(this->buffers[f][i], "=");
		strcat(this->buffers[f][i], val);
		this->buffers[f][i+1] = 0;
		return true;
	}

	return false;
}

//sets the path to look for a file in
void INIParser::SetWorkingDirectory(const char * Directory)
{
	DWORD dwAttr, dwError;
	
	dwAttr = GetFileAttributes(Directory);
	dwError = GetLastError();

	if(dwError == ERROR_PATH_NOT_FOUND || dwError == ERROR_FILE_NOT_FOUND)
		CreateDirectory(Directory, NULL);

	strcpy(this->dir, Directory);
}

//builds the full path to the file supplied
const char *INIParser::BuildPath(const char *file)
{
	static char path[MAX_PATH];
	
	if(this->dir[0])
	{
		strcpy(path, this->dir);
		strcat(path, "\\");
		strcat(path, file);
	}
	else strcpy(path, file);

	return path;
}