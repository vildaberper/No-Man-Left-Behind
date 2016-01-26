#include "Configuration.h"

#include <vector>
#include <map>

using namespace std;

static const bool isDigits(string value){
	for (size_t i = 0; i < value.length(); i++){
		if (!isdigit(value.at(i))){
			return false;
		}
	}
	return true;
}

static const bool isInt(string value){
	size_t i = 0;

	if (value.length() == 0){
		return false;
	}
	if (value.at(0) == '-'){
		i = 1;
	}
	for (; i < value.length(); i++){
		if (!isdigit(value.at(i))){
			return false;
		}
	}
	return true;
}

static const bool isFloat(string value){
	size_t index = value.find_first_of('.');

	if (index == string::npos || index == 0 || index == value.length()){
		return false;
	}

	if (!isInt(value.substr(0, index)) || !isDigits(value.substr(index + 1))){
		return false;
	}
	return true;
}

static const enum Type{
	TYPEUNDEFINED,
	TYPESTRING,
	TYPEFLOAT,
	TYPEINT,
	TYPEVECTORSTRING,
	TYPEVECTORFLOAT,
	TYPEVECTORINT
};

class ConfigurationNode{
public:
	static const char SEPARATOR = ',';

	Type type = TYPEUNDEFINED;
	void* value = NULL;

	std::map<std::string, ConfigurationNode> nodes;

	ConfigurationNode(){
		
	}

	~ConfigurationNode(){
		if (type != TYPEUNDEFINED){
			delete value;
		}
	}

	void ConfigurationNode::unset(){
		if (type != TYPEUNDEFINED){
			type = TYPEUNDEFINED;
			delete value;
		}
	}

	void ConfigurationNode::set(const string& value){
		type = TYPESTRING;
		ConfigurationNode::value = new string(value);
	}

	void ConfigurationNode::set(const float& value){
		type = TYPEFLOAT;
		ConfigurationNode::value = new float(value);
	}

	void ConfigurationNode::set(const int& value){
		type = TYPEINT;
		ConfigurationNode::value = new int(value);
	}

	void ConfigurationNode::set(const vector<string> &value){
		type = TYPEVECTORSTRING;
		ConfigurationNode::value = new vector<string>(value);
	}

	void ConfigurationNode::set(const vector<float> &value){
		type = TYPEVECTORFLOAT;
		ConfigurationNode::value = new vector<float>(value);
	}

	void ConfigurationNode::set(const vector<int> &value){
		type = TYPEVECTORINT;
		ConfigurationNode::value = new vector<int>(value);
	}

	const string ConfigurationNode::stringValue(){
		if (type != TYPESTRING){
			return "";
		}
		return *static_cast<string*>(value);
	}

	const float ConfigurationNode::floatValue(){
		if (type != TYPEFLOAT){
			return 0.0f;
		}
		return *static_cast<float*>(value);
	}

	const int ConfigurationNode::intValue(){
		if (type != TYPEINT){
			return 0;
		}
		return *static_cast<int*>(value);
	}

	const vector<string> ConfigurationNode::stringVector(){
		if (type != TYPEVECTORSTRING){
			return vector<string>();
		}
		return *static_cast<vector<string>*>(value);
	}

	const vector<float> ConfigurationNode::floatVector(){
		if (type != TYPEVECTORFLOAT){
			return vector<float>();
		}
		return *static_cast<vector<float>*>(value);
	}

	const vector<int> ConfigurationNode::intVector(){
		if (type != TYPEVECTORINT){
			return vector<int>();
		}
		return *static_cast<vector<int>*>(value);
	}

	const bool hasValue(){
		return type != TYPEUNDEFINED;
	}

	const string toString(){
		switch (type){
		case TYPESTRING:
			return '"' + stringValue() + '"';
		case TYPEFLOAT:
			return to_string(floatValue());
		case TYPEINT:
			return to_string(intValue());
		case TYPEVECTORSTRING:
		{
							 stringstream stream;
							 vector<string> vector = stringVector();

							 stream << '{';
							 for (size_t i = 0; i < vector.size(); i++){
								 if (i > 0){
									 stream << SEPARATOR;
								 }
								 stream << '"' << vector[i] << '"';
							 }
							 stream << '}';
							 return stream.str();
		}
		case TYPEVECTORFLOAT:
		{
							stringstream stream;
							vector<float> vector = floatVector();

							stream << '{';
							for (size_t i = 0; i < vector.size(); i++){
								if (i > 0){
									stream << SEPARATOR;
								}
								stream << to_string(vector[i]);
							}
							stream << '}';
							return stream.str();
		}
		case TYPEVECTORINT:
		{
						  stringstream stream;
						  vector<int> vector = intVector();

						  stream << '{';
						  for (size_t i = 0; i < vector.size(); i++){
							  if (i > 0){
								  stream << SEPARATOR;
							  }
							  stream << to_string(vector[i]);
						  }
						  stream << '}';
						  return stream.str();
		}
		}
		return "";
	}

	const bool parse(const string& value){
		if (value.at(0) == '"' && value.at(value.length() - 1) == '"'){ // TYPESTRING
			set(value.substr(1, value.length() - 2));
		}
		else if (isInt(value)){ // TYPEINT
			set(stoi(value));
		}
		else if (isFloat(value)){ // TYPEFLOAT
			set(stof(value));
		}
		else if (value.at(0) == '{' && value.at(value.length() - 1) == '}'){
			string val = value.substr(1, value.length() - 2);
			string::size_type index;

			if (value.at(1) == '"' && value.at(value.length() - 2) == '"'){ // TYPEVECTORSTRING
				vector<string> vector;

				while ((index = val.find_first_of(',')) != string::npos){
					vector.push_back(val.substr(1, index - 2));
					val = val.substr(index + 1);
				}
				vector.push_back(val.substr(1, val.length() - 2));
				set(vector);
			}
			else{
				string test = val;
				index = val.find_first_of(',');
				if (index != string::npos)
					test = val.substr(0, index);
				if (isInt(test)){ // TYPEVECTORINT
					vector<int> vector;

					while ((index = val.find_first_of(',')) != string::npos){
						vector.push_back(stoi(val.substr(0, index)));
						val = val.substr(index + 1);
					}
					vector.push_back(stoi(val));
					set(vector);
				}
				else if (isFloat(test)){ // TYPEVECTORFLOAT
					vector<float> vector;

					while ((index = val.find_first_of(',')) != string::npos){
						vector.push_back(stof(val.substr(0, index)));
						val = val.substr(index + 1);
					}
					vector.push_back(stof(val));
					set(vector);
				}
				else{
					return false;
				}
			}
		}
		else{
			return false;
		}
		return true;
	}

	const vector<string> children(){
		vector<string> children;

		for (auto const &node : nodes){
			children.push_back(node.first);
		}
		return children;
	}

	const bool containsNode(const string& path){
		size_t index;

		if ((index = path.find_first_of(Configuration::PATH_SEPARATOR)) != string::npos){
			string sub = path.substr(0, index);

			if (nodes.count(sub) > 0){
				return nodes[sub].containsNode(path.substr(index + 1));
			}
			else{
				return false;
			}
		}
		return nodes.count(path) > 0;
	}

	ConfigurationNode& node(const string& path){
		size_t index;
		if ((index = path.find_first_of(Configuration::PATH_SEPARATOR)) != string::npos){
			string sub = path.substr(0, index);

			if (nodes.count(sub) == 0){
				nodes[sub] = ConfigurationNode();
			}
			return nodes[sub].node(path.substr(index + 1));
		}
		if (nodes.count(path) == 0){
			nodes[path] = ConfigurationNode();
		}
		return nodes[path];
	}

	const void remove(const string& path){
		size_t index;

		if ((index = path.find_first_of(Configuration::PATH_SEPARATOR)) != string::npos){
			string sub = path.substr(0, index);

			if (nodes.count(sub) == 0){
				return;
			}
			return nodes[sub].remove(path.substr(index + 1));
		}
		nodes.erase(path);
	}

	vector<string> contents(string ind){
		vector<string> lines;

		for (string n : children()){
			lines.push_back(ind + n + ":" + (node(n).hasValue() ? (" " + node(n).toString()) : ""));

			vector<string> next = node(n).contents(ind + "  ");

			lines.insert(lines.end(), next.begin(), next.end());
		}
		return lines;
	}
};

ConfigurationNode root;

Configuration::Configuration(){

}

Configuration::~Configuration(){

}

int h(string& s){
	int i = 0;
	while (s.length() > 0 && s.at(0) == ' '){
		s = s.substr(1);
		i++;
	}
	return i / Configuration::INDENT_WIDTH;
}

bool parse(ConfigurationNode& root, vector<string> content, unsigned int curLine, string curPath, int indent){
	if (curLine >= content.size())
		return true;

	string line = content[curLine];
	int curIndent = h(line);

	if (line.length() == 0 || line.at(0) == '#')
		return parse(root, content, curLine + 1, curPath, indent);

	string::size_type colIndex = line.find_first_of(":");
	string node = line.substr(0, colIndex);
	string val;

	if (indent < curIndent){
		curPath += (curPath.length() > 0 ? "." : "") + node;
	}
	else{
		for (int s = 0; s <= indent - curIndent; s++){
			string::size_type index = curPath.find_last_of('.');

			if (index != string::npos){
				curPath = curPath.substr(0, index);
			}
			else{
				curPath = "";
				break;
			}
		}
		curPath += (curPath.length() > 0 ? "." : "") + node;
	}
	if (line.length() > colIndex + 2){
		val = line.substr(colIndex + 2);
		h(val);
		if (val.length() == 0 || val.at(0) == '#'){
			return parse(root, content, curLine + 1, curPath, curIndent);
		}
		if (!root.node(curPath).parse(val)){
			return false;
		}
	}
	return parse(root, content, curLine + 1, curPath, curIndent);
}

bool Configuration::load(File& file){
	if (!parse(root, file.readTextFile(), 0, "", -1)){
		logger::fatal("Failed to parse file: " + file.path());
		return false;
	}
	return true;
}

const bool Configuration::save(File& file){
	return file.writeTextFile(root.contents(""));
}

const vector<string> Configuration::children(const string& path){
	vector<string> children = root.children();

	if (root.containsNode(path)){
		children = root.node(path).children();
	}
	if (path.length() > 0){
		for (unsigned int i = 0; i < children.size(); i++){
			children[i] = path + PATH_SEPARATOR + children[i];
		}
	}
	return children;
}

const bool Configuration::hasValue(const string& path){
	return root.containsNode(path) && root.node(path).hasValue();
}

const void Configuration::remove(const std::string& path){
	if (root.containsNode(path)){
		root.remove(path);
	}
}

const void Configuration::unset(const std::string& path){
	if (root.containsNode(path)){
		root.node(path).unset();
	}
}

void Configuration::set(const string& path, const string& value){
	root.node(path).set(value);
}
void Configuration::set(const string& path, const float& value){
	root.node(path).set(value);
}
void Configuration::set(const string& path, const int& value){
	root.node(path).set(value);
}
void Configuration::set(const string& path, const vector<string> &value){
	root.node(path).set(value);
}
void Configuration::set(const string& path, const vector<float> &value){
	root.node(path).set(value);
}
void Configuration::set(const string& path, const vector<int> &value){
	root.node(path).set(value);
}

const string Configuration::stringValue(const std::string& path){
	if (root.containsNode(path)){
		return root.node(path).stringValue();
	}
	return "";
}
const float Configuration::floatValue(const string& path){
	if (root.containsNode(path)){
		return root.node(path).floatValue();
	}
	return 0.0f;
}
const int Configuration::intValue(const string& path){
	if (root.containsNode(path)){
		return root.node(path).intValue();
	}
	return 0;
}
const vector<string> Configuration::stringVector(const string& path){
	if (root.containsNode(path)){
		return root.node(path).stringVector();
	}
	return vector<string>();
}
const vector<float> Configuration::floatVector(const string& path){
	if (root.containsNode(path)){
		return root.node(path).floatVector();
	}
	return vector<float>();
}
const vector<int> Configuration::intVector(const string& path){
	if (root.containsNode(path)){
		return root.node(path).intVector();
	}
	return vector<int>();
}