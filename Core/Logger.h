#pragma once

#include <string>
#include <vector>

#include "SFMLI.h"

const enum LogLevel{
	INFO,
	DEBUG,
	WARNING,
	FATAL,
	TIMING
};

namespace logger{
	extern sf::Clock clock;

	class LogEntry{
	public:
		LogEntry(std::string message){
			LogEntry::message = message;
			time = logger::clock.getElapsedTime();
		}
		~LogEntry(){

		}

		float fadeValue(const sf::Time& showTime, const sf::Time& fadeTime) const{
			if(time + showTime > logger::clock.getElapsedTime()){
				return 1.0f;
			}
			float f = 1.0f - (logger::clock.getElapsedTime() - time - showTime).asSeconds() / fadeTime.asSeconds();
			return f < 0.0f ? 0.0f : f;
		}

		std::string message;
		sf::Time time;
	};

	extern std::vector<LogEntry> history;
	extern size_t historyCount;

	void log(const std::string& message, const LogLevel& level);

	void info(const std::string& message);

	void debug(const std::string& message);

	void warning(const std::string& message);

	void fatal(const std::string& message);

	void timing(const std::string& message);
}