#include "../headers/jsonlib.h"

int main()
{
	// example code
	std::string path = "jsontest.json";

	std::map<std::string, float> dict = json::load<float>(&path);
	for (const auto& pair : dict)
	{
		std::cout << pair.first << " | " << pair.second << "\n";
	}

	std::map<std::string, float> new_dict{
		{"test", 1.0f},
		{"your mom", 2.5f}
	};

	json::dump(&path, &new_dict);

	return 0;
}
