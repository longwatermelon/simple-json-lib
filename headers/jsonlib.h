#pragma once
#include "parser.h"
#include <iostream>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <string>

namespace json
{
    class detail // helper functions that users shouldn't use
    {
    private:
        std::string read_file(std::string filepath)
        {
            std::ifstream infile;
            infile.open(filepath);

            std::stringstream contents;
            std::string line;
            while (std::getline(infile, line))
            {
                contents << line << "\n";
            }

            infile.close();

            return contents.str();
        }

    
        template <typename T>
        friend std::map<std::string, T> load(std::string* filepath);
    };


    template <typename T>
    std::map<std::string, T> load(std::string* filepath)
    {
        detail detail{};
        std::string contents = detail.read_file(*filepath);

        json_utils::Parser parser{ contents };
        try { parser.parse(); } // keys and values vectors are filled now
        catch (const std::runtime_error& ex) { std::cout << ex.what() << "\n"; }

        std::map<std::string, T> dict;

        for (int i = 0; i < parser.keys.size(); ++i)
        {
            std::string key = parser.keys[i]->value->string_value;
            T value;
        
            if constexpr (std::is_same_v<T, std::string>) value = parser.values[i]->value->string_value;
            else if constexpr (std::is_same_v<T, int>) value = parser.values[i]->value->int_value;
            else if constexpr (std::is_same_v<T, float>) value = parser.values[i]->value->float_value;
        
        
            dict[key] = value;
        }


        return dict;
    }


    template <typename T>
    void dump(std::string* filepath, std::map<std::string, T>* dict)
    {
        // clear out existing data to write in new dictionary
        std::ofstream ofs;
        ofs.open(*filepath, std::ofstream::out | std::ofstream::trunc);
        ofs.close();

        std::ofstream file(*filepath);
        std::string final_string = "{\n\t";
    
        for (auto& pair : *dict)
        {
            std::string key, val;
            key = pair.first;
            key.insert(key.begin(), 1, '"');
            key.insert(key.end(), 1, '"');

            if (std::is_same_v<T, std::string>) { val = pair.second; val.insert(val.begin(), 1, '"'); val.insert(val.end(), 1, '"'); }
            else if (std::is_same_v<T, int>) { std::stringstream ss; ss << pair.second; val = ss.str(); }
            else if (std::is_same_v<T, float>) { std::stringstream ss; ss << pair.second; val = ss.str(); }

            std::string addition = key + ": " + val + ",\n\t";
            final_string += addition;
        }

        final_string.replace(final_string.end() - 3, final_string.end(), "\n");
        final_string += "}";

        file << final_string;
        file.close();
    }
}