#include "Logger.h"

namespace logger{
	sf::Clock clock;

	std::vector<LogEntry> history;
	size_t historyCount = 22;

	void log(const std::string& message, const LogLevel& level){
		std::string out;

		switch(level){
		case INFO:
			out += "[INFO]";
			break;
		case DEBUG:
			out += "[DEBUG]";
			break;
		case WARNING:
			out += "[WARNING]";
			break;
		case FATAL:
			out += "[FATAL]";
			break;
		case TIMING:
			out += "[TIMING]";
			break;
		}
		out += ' ' + message + '\n';
		printf(out.c_str());

		while(history.size() > historyCount){
			history.erase(history.begin());
		}
		history.push_back(LogEntry(out));
	}

	void info(const std::string& message){
		log(message, INFO);
	}

	void debug(const std::string& message){
		log(message, DEBUG);
	}

	void warning(const std::string& message){
		log(message, WARNING);
	}

	void fatal(const std::string& message){
		log(message, FATAL);
	}

	void timing(const std::string& message){
		log(message, TIMING);
	}
}