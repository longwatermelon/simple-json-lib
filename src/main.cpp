#include "../include/jsonlib.h"

int main()
{
	std::map<std::string, std::string> dict = json::load_json<std::string, std::string>("../jsontest.json");
	for (auto const& [key, val] : dict)
	{
		std::cout << key << ':' << val << "\n";
	}

	return 0;
}
