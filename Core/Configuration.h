#pragma once

#include "File.h"

static const char PATH_SEPARATOR = '.';
static const unsigned char INDENT_WIDTH = 2;

class Configuration{
public:
	Configuration();
	~Configuration();

	// Load from file
	bool load(File& file);

	// Save to file
	const bool save(File& file);

	// Children of a node (parent.child1, parent.child2)
	const std::vector<std::string> children(const std::string& path, const bool& fullPath = true);

	// Node has a value
	const bool hasValue(const std::string& path);

	// Remove node (and all under it)
	const void remove(const std::string& path);

	// Unset a value to node
	const void unset(const std::string& path);

	// Set value to node
	void set(const std::string& path, const bool& value);
	void set(const std::string& path, const std::string& value);
	void set(const std::string& path, const float& value);
	void set(const std::string& path, const int& value);
	void set(const std::string& path, const std::vector<std::string>& value);
	void set(const std::string& path, const std::vector<float>& value);
	void set(const std::string& path, const std::vector<int>& value);

	// Get value from a node
	const bool boolValue(const std::string& path);
	const std::string stringValue(const std::string& path);
	const float floatValue(const std::string& path);
	const int intValue(const std::string& path);
	const std::vector<std::string> stringVector(const std::string& path);
	const std::vector<float> floatVector(const std::string& path);
	const std::vector<int> intVector(const std::string& path);
};