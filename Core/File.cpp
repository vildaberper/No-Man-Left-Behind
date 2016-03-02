#include "File.h"

File::File(const std::string& path){
	path_ = path;
	if (path_[path_.length() - 1] == file::FILE_SEPARATOR)
		path_ = path_.substr(0, path_.length() - 1);
}

File::File(){
	path_ = file::workingDir();
}

const std::string File::path() const{
	return path_;
}

const std::string File::name(){
	return path().substr(parent().path().length() + 1);
}

const std::string File::nameNoExtension(){
	std::string name = File::name();
	int last = name.find_last_of('.');

	return name.substr(0, last == std::string::npos ? name.length() : last);
}

const std::string File::extension(){
	std::string name = File::name();
	int last = name.find_last_of('.');

	return last == std::string::npos ? "" : name.substr(last + 1);
}

const long File::size(){
	return file::size(path());
}

const bool File::exists(){
	return file::exists(path());
}

const bool File::hasParent(){
	return file::hasParent(path());
}

File File::parent(){
	return File(file::parent(path()));
}

File File::child(const std::string& child){
	return File(file::child(path(), child));
}

const bool File::isFile(){
	return file::isFile(path());
}

const bool File::isDirectory(){
	return file::isDirectory(path());
}

const std::vector<std::string> File::listFiles(){
	return file::listFiles(path());
}

const bool File::mkdir(){
	return file::mkdir(path());
}

const bool File::mkdirs(){
	return file::mkdirs(path());
}

std::vector<std::string>* File::readTextFile(){
	return file::readTextFile(path());
}

const bool File::writeTextFile(const std::vector<std::string>* content){
	return parent().mkdirs() && file::writeTextFile(path(), content);
}