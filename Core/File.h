#pragma once

#include "FileHelper.h"

class File{
public:
	File(const std::string& path);

	File();

	const std::string path() const;

	const std::string name();

	const std::string nameNoExtension();

	const std::string extension();

	const long size();

	const bool exists();

	const bool hasParent();

	File parent();

	File child(const std::string& child);

	const bool isFile();

	const bool isDirectory();

	const std::vector<std::string> listFiles();

	const bool mkdir();

	const bool mkdirs();

	std::vector<std::string>* readTextFile();

	const bool writeTextFile(const std::vector<std::string>* content);

	bool operator==(const File& f){
		return path() == f.path();
	}
private:
	std::string path_;
};