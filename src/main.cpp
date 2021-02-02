#include "../headers/jsonlib.h"

int main()
{
	// example code
	// this will probably throw an error since jsontest.json is always changing
	std::string path = "jsontest.json";


	// load
	std::map<std::string, float> dict = json::load<float>(path);
	for (const auto& pair : dict)
	{
		std::cout << pair.first << " | " << pair.second << "\n";
	}
	

	// dump
	std::map<std::string, float> new_dict{
		{"test", 1.0f},
		{"your mom", 2.5f}
	};

	json::dump(path, new_dict);


	// use of overloaded load function if you dont like angular brackets
	std::map<std::string, float> overload_example;
	json::load(path, overload_example);

	for (const auto& pair : overload_example)
	{
		std::cout << pair.first << " | " << pair.second << "\n";
	}

	std::map<std::string, std::vector<std::string>> map = {
		{"key 1", {"a", "b"}},
		{"key 2", {"a", "b2"}}
	};

	json::dump(path, map);


	// using vectors
	std::map<std::string, std::vector<int>> vector_example{
		{"test", {1, 2, 3}}
	};

	json::dump(path, vector_example);

	return 0;
}
