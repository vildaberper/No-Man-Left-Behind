#pragma once

#include <map>
#include <string>
#include <vector>

#include "File.h"
#include "Journal.h"
#include "GameConstants.h"

class JournalManager{
public:
	JournalManager();
	~JournalManager();

	Journal* getJournal(std::string name);

	std::vector<std::string> getJournals(std::string folder);
private:
	void loadFromDir(File& file);
	std::map<std::string, std::map<std::string, Journal*>> journals;

	Journal* undefined;
};