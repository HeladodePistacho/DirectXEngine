#include "FileDrop.h"

//----------------------FILE------------------------
File::File(char* path) : path(path)
{
	std::string file_path = path;

	std::string file_extension = file_path.substr(file_path.find_last_of('.'));

	if (file_extension == ".fbx" || file_extension == ".FBX")
		type = FILE_TYPE::FBX;

	if (file_extension == ".obj" || file_extension == ".OBJ")
		type = FILE_TYPE::OBJ;

	if (file_extension == ".png" || file_extension == ".PNG")
		type = FILE_TYPE::PNG;
}

File::~File()
{
	if(!path)
		delete[] path;
}

const char * File::GetPath() const
{
	return path;
}

FILE_TYPE File::GetType() const
{
	return type;
}


//----------------------DROP MANAGER-----------------

bool DropManager::AnyDroppedFiles() const
{
	return !dropped_files.empty();
}

File* DropManager::GetFile()
{
	if (!dropped_files.empty())
	{
		return dropped_files.front();
	}
}

void DropManager::RemoveFile()
{
	if (!dropped_files.empty())
	{
		delete dropped_files.front();
		dropped_files.pop();
	}
}

void DropManager::AddDroppedFile(File* new_file)
{
	if (new_file)
		dropped_files.push(new_file);
}
