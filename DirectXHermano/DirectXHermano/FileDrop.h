#pragma once
#include <queue>

enum FILE_TYPE
{
	INVALID,
	FBX,
	OBJ,
	PNG,
	JPG
};

class File
{
public:
	File(char* path);
	~File();

	const char* GetPath() const;
	FILE_TYPE GetType() const;

private:
	char* path = nullptr;
	FILE_TYPE type = INVALID;
};

class DropManager
{
	friend class Window;

public:
	DropManager() = default;
	DropManager(const DropManager&) = delete;

	bool AnyDroppedFiles() const;
	File* GetFile();
	void RemoveFile();

private:
	std::queue<File*> dropped_files;

	void AddDroppedFile(File*);
};