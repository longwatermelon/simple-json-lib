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
    class Detail // helper functions that users shouldn't use
    {
    private:
        static std::string read_file(const std::string& filepath)
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
        static void fill_map(std::map<std::string, T>* map, jsonutils::Parser* parser)
        {
            for (int i = 0; i < parser->keys.size(); ++i)
            {
                std::string key = parser->keys[i]->value->string_value;
                T value;

                if constexpr (std::is_same_v<T, std::string>) value = parser->values[i]->value->string_value;
                else if constexpr (std::is_same_v<T, int>) value = parser->values[i]->value->int_value;
                else if constexpr (std::is_same_v<T, float>) value = parser->values[i]->value->float_value;
                else if constexpr (std::is_same_v<T, std::vector<std::string>> || std::is_same_v<T, std::vector<int>> || std::is_same_v<T, std::vector<float>>)
                {
                    for (int j = 0; j < parser->values[i]->value->vector_value.size(); ++j)
                    {
                        if constexpr (std::is_same_v<T, std::vector<int>>) value.push_back(std::any_cast<int>(parser->values[i]->value->vector_value[j]));
                        else if constexpr (std::is_same_v<T, std::vector<float>>) value.push_back(std::any_cast<float>(parser->values[i]->value->vector_value[j]));
                        else if constexpr (std::is_same_v<T, std::vector<std::string>>) value.push_back(std::any_cast<std::string>(parser->values[i]->value->vector_value[j]));
                    }
                }

                (*map)[key] = value;
            }
        }


        static void turn_into_str(std::string& str)
        {
            str.insert(str.begin(), 1, '"');
            str.insert(str.end(), 1, '"');
        }


        template <typename T>
        static bool is_string(T type) { return false; }

        static bool is_string(std::string type) { return true; }

    
        template <typename T>
        friend std::map<std::string, T> load(const std::string& fp);

        template <typename T>
        friend void load(const std::string& fp, std::map<std::string, T>* map);

        template <typename T>
        friend void dump(const std::string& fp, std::map<std::string, T>* map);
    };


    template <typename T>
    std::map<std::string, T> load(const std::string& fp)
    {
        std::string contents = Detail::read_file(fp);

        jsonutils::Parser parser{ contents };
        try { parser.parse(); } // keys and values vectors are filled now
        catch (const std::runtime_error& ex) { std::cout << ex.what() << "\n"; }

        std::map<std::string, T> dict;
        Detail::fill_map(&dict, &parser);

        return dict;
    }
    

    template <typename T>
    void load(const std::string& fp, std::map<std::string, T>* map)
    {
        std::string contents = Detail::read_file(fp);

        jsonutils::Parser parser{ contents };
        try { parser.parse(); }
        catch (const std::runtime_error& ex) { std::cout << ex.what() << "\n"; }

        Detail::fill_map(map, &parser);
    }


    template <typename T>
    void dump(const std::string& fp, std::map<std::string, T>* map)
    {
        // clear out existing data to write in new dictionary
        std::ofstream ofs;
        ofs.open(fp, std::ofstream::out | std::ofstream::trunc);
        ofs.close();

        std::ofstream file(fp);
        std::string final_string = "{\n\t";
        
        for (auto& pair : *map)
        {
            std::string key = pair.first;
            Detail::turn_into_str(key);

            std::stringstream val;
            
            if constexpr (std::is_same_v<T, std::string>) { val << '"' << pair.second << '"'; }
            else if constexpr (std::is_same_v<T, std::vector<std::string>> || std::is_same_v<T, std::vector<int>> || std::is_same_v<T, std::vector<float>>)
            {
                val << "[\n\t\t";
                for (int i = 0; i < pair.second.size(); ++i)
                {
                    if (Detail::is_string(pair.second[i])) val << '"' << pair.second[i] << "\",\n\t\t";
                    else val << pair.second[i] << ",\n\t\t";
                }

                std::string temp = val.str();

                // clear stringstream
                val.str("");
                val.clear();

                temp.replace(temp.end() - 4, temp.end(), "\n");
                
                val << temp << "\t]";
            }
            else if constexpr (std::is_same_v<T, int>) { val << pair.second; }
            else if constexpr (std::is_same_v<T, float>) { val << pair.second; }

            std::stringstream ss;
            ss << key << ": " << val.str() << ",\n\t";

            final_string += ss.str();
        }

        final_string.replace(final_string.end() - 3, final_string.end(), "\n");
        final_string += "}";

        file << final_string;
        file.close();
    }
}