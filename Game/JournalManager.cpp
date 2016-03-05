#include "JournalManager.h"

JournalManager::JournalManager(){
	loadFromDir(gc::journalDir);

	undefined = new Journal();
	undefined->lines.push_back("MISSING JOURNAL");
	undefined->injured.push_back("civil1");
}

JournalManager::~JournalManager(){
	for(auto &ent : journals){
		delete ent.second;
		ent.second = NULL;
	}
	journals.clear();
	delete undefined;
}

Journal* JournalManager::getJournal(std::string name){
	if(journals.count(name) != 0){
		return journals[name];
	}
	return undefined;
}

void JournalManager::loadFromDir(File& file){
	for(File f : file.listFiles()){
		if(f.isDirectory()){
			loadFromDir(f);
			continue;
		}

		if(!f.isFile() || f.extension() != "txt"){
			continue;
		}

		Journal* j = new Journal();

		std::vector<std::string>* lines = f.readTextFile();

		std::string r = (*lines)[0];
		size_t s;
		while((s = r.find_first_of(',')) != std::string::npos){
			j->requirements.push_back(parseResource(r.substr(0, s)));
			r = r.substr(s + 1);
		}
		j->requirements.push_back(parseResource(r));

		r = (*lines)[1];
		while((s = r.find_first_of(',')) != std::string::npos){
			j->injured.push_back(r.substr(0, s));
			r = r.substr(s + 1);
		}
		j->injured.push_back(r);

		for(size_t i = 2; i < lines->size(); i++){
			j->lines.push_back((*lines)[i]);
		}

		journals[f.nameNoExtension()] = j;

		delete lines;
	}
}