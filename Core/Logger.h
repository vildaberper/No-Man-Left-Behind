#pragma once

static const enum LogLevel{
	INFO,
	DEBUG,
	WARNING,
	FATAL
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
		}
		out += ' ' + message + '\n';
		printf(out.c_str());
	}

	static const void log(const std::string& message){
		log(message, INFO);
	}
}