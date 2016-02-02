#pragma once

#include <string>

static const enum LogLevel{
	INFO,
	DEBUG,
	WARNING,
	FATAL,
	TIMING
};

namespace logger{
	static const void log(const std::string& message, const LogLevel& level){
		std::string out;

		switch (level){
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
	}

	static const void info(const std::string& message){
		log(message, INFO);
	}

	static const void debug(const std::string& message){
		log(message, DEBUG);
	}

	static const void warning(const std::string& message){
		log(message, WARNING);
	}

	static const void fatal(const std::string& message){
		log(message, FATAL);
	}

	static const void timing(const std::string& message){
		log(message, TIMING);
	}
}