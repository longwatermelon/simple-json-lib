#include "../headers/jsonlib.h"

int main()
{
	// example code
	std::string path = "C:\\Users\\qianh\\OneDrive\\Documents\\code\\cpp\\json-lib\\jsontest.json";
	std::map<std::string, std::string> dict = json::load<std::string, std::string>(path);
	dict["pee"] = "modified value";
	json::dump<std::string, std::string>(path, dict);

	std::map<std::string, int> new_dict{
		{"5", 3},
		{"2", 1}
	};

	json::dump<std::string, int>(path, new_dict);

	return 0;
}
