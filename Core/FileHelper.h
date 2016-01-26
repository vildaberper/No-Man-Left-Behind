#pragma once

#include <sys/stat.h>
#include <Windows.h>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "dirent.h"

#include "Logger.h"

namespace file{
	static const char FILE_SEPARATOR = '\\';

	// Lists all files in a directory
	static const std::vector<std::string> listFiles(const std::string& path){
		DIR *dir;
		struct dirent *ent;
		std::vector<std::string> files;

		if ((dir = opendir(path.data())) != NULL){
			while ((ent = readdir(dir)) != NULL){
				if (ent->d_name[0] == '.' && ent->d_name[1] == 0){
					continue;
				}
				if (ent->d_name[0] == '.' && ent->d_name[1] == '.' && ent->d_name[2] == 0){
					continue;
				}
				files.push_back(path + FILE_SEPARATOR + ent->d_name);
			}
			closedir(dir);
			delete ent;
		}
		return files;
	}

	static const bool isFile(const std::string& path) {
		struct stat s;

		stat(path.data(), &s);

		return (s.st_mode & S_IFREG) != 0;
	}

	static const bool isDirectory(const std::string& path) {
		struct stat s;

		stat(path.data(), &s);

		return (s.st_mode & S_IFDIR) != 0;
	}

	// File or directory size in bytes
	static const long size(const std::string& path){
		long size_ = 0;

		if (isDirectory(path)){
			for (std::string child : listFiles(path)){
				size_ += size(child);
			}
			return size_;
		}

		std::ifstream file(path, std::ios_base::binary);
		file.seekg(0, std::ios_base::end);
		size_ = long(file.tellg());
		file.close();

		return size_;
	}

	// File has parent (C:\parent = true, C:\ = false)
	static const bool hasParent(const std::string& path){
		return path.find_first_of(FILE_SEPARATOR) != path.find_last_of(FILE_SEPARATOR);
	}

	// Parent of path (C:\parent\path)
	static const std::string parent(const std::string& path){
		return path.substr(0, path.find_last_of(FILE_SEPARATOR));
	}

	// Child of path (C:\path\child)
	static const std::string child(const std::string& path, const std::string& child){
		return path + FILE_SEPARATOR + child;
	}

	// Current working directory
	static const std::string workingDir(){
		char dir[MAX_PATH];

		return parent(std::string(dir, GetModuleFileName(NULL, dir, MAX_PATH)));
	}

	// Check if file exists
	static const bool exists(const std::string& filename){
		if (isDirectory(filename)){
			return true;
		}

		std::ifstream file(filename);

		bool good = file.good();

		file.close();

		return good;
	}

	// Create directory
	static const bool mkdir(const std::string& path){
		return _mkdir(path.data()) == 0;
	}

	// Create directory and all parent directories
	static const bool mkdirs(const std::string path){
		if (path.length() < 4){
			return false;
		}
		if (isDirectory(path)){
			return true;
		}

		return mkdirs(parent(path)) && mkdir(path);
	}

	// Returns a vector<string> with lines read from file
	static const std::vector<std::string> readTextFile(const std::string& filename){
		std::vector<std::string> content;

		std::string line;
		std::ifstream file(filename);

		if (file.is_open()){
			while (std::getline(file, line)){
				content.push_back(line);
			}
			file.close();
		}
		else{
			logger::warning("Could not read file: " + filename);
		}

		return content;
	}

	// Write a file with lines vector<string>
	static const bool writeTextFile(const std::string& filename, const std::vector<std::string>& content){
		std::stringstream stream;

		for (std::string line : content){
			stream << line << '\n';
		}

		std::ofstream file(filename);
		if (file.is_open()){
			file << stream.str();
			file.close();
		}
		else{
			logger::warning("Could not write file: " + filename);
			return false;
		}

		return true;
	}
}