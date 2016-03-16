#include "JournalManager.h"

JournalManager::JournalManager(){
	loadFromDir(gc::journalDir);

	undefined = new Journal();
	undefined->lines.push_back("MISSING JOURNAL");
	undefined->injured.push_back("civil1");
}

JournalManager::~JournalManager(){
	for(auto &ent : journals){
		for(auto &ent0 : ent.second){
			delete ent0.second;
			ent0.second = NULL;
		}
	}
	journals.clear();
	delete undefined;
}

Journal* JournalManager::getJournal(std::string name){
	std::string::size_type index = name.find_first_of('.');

	if(journals.count(name.substr(0, index)) != 0 && journals[name.substr(0, index)].count(name.substr(index + 1)) != 0){
		return journals[name.substr(0, index)][name.substr(index + 1)];
	}
	return undefined;
}

std::vector<std::string> JournalManager::getJournals(std::string folder){
	std::vector<std::string> js;

	if(journals.count(folder) != 0){
		for(auto &ent : journals[folder]){
			js.push_back(folder + "." + ent.first);
		}
	}
	return js;
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

		size_t s;
		std::string r = (*lines)[0];
		if(r.size() > 0){
			while((s = r.find_first_of(',')) != std::string::npos){
				j->requirements.push_back(parseResource(r.substr(0, s)));
				r = r.substr(s + 1);
			}
			j->requirements.push_back(parseResource(r));
		}

		r = (*lines)[1];
		if(r.size() > 0){
			while((s = r.find_first_of(',')) != std::string::npos){
				j->injured.push_back(r.substr(0, s));
				r = r.substr(s + 1);
			}
			j->injured.push_back(r);
		}

		r = (*lines)[2];
		if(r.length() > 0){
			j->deathTimer = sf::seconds(float(std::stoi(r)));
		}
		else{
			switch(j->requirements.size()){
			case 2:
				j->deathTimer = sf::seconds(gc::criticalTimer * 2.0f);
				break;
			case 3:
				j->deathTimer = sf::seconds(gc::criticalTimer);
				break;
			}
		}

		for(size_t i = 3; i < lines->size(); i++){
			j->lines.push_back((*lines)[i]);
		}

		journals[file.nameNoExtension()][f.nameNoExtension()] = j;

		delete lines;
	}
}