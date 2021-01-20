#pragma once
#include "parser.h"
#include <iostream>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

namespace json
{
    class detail
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

        
        template <typename T, typename U>
        friend std::map<T, U> load_json(std::string filepath);
    };
    

    template <typename T, typename U>
    std::map<T, U> load_json(std::string filepath)
    {
        detail detail{};
        std::string contents = detail.read_file(filepath);
        /*Lexer lexer{ contents };
        Token token{ TokenType::TOKEN_EOF, "" };
        token = lexer.get_next_token();
        std::cout << "token type: " << int(token.type_) << " || token value: " << token.value << std::endl;
        while (token.type_ != TokenType::TOKEN_EOF)
        {
            token = lexer.get_next_token();
            std::cout << "token type: " << int(token.type_) << " || token value: " << token.value << std::endl;
        }

        std::cout << contents;*/

        Parser parser{ contents };
        parser.parse(); // keys and values vectors are filled now

        std::map<T, U> dict;

        for (int i = 0; i < parser.keys.size(); ++i)
        {
            T key;
            U value;
            if (std::is_same<T, std::string>::value) 
            { 
                key = parser.keys[i]->value->string_value; 
            }
            else if (std::is_same<T, int>::value) 
            {
                key = parser.keys[i]->value->int_value; 
            }

            if (std::is_same<U, std::string>::value)
            { 
                value = parser.values[i]->value->string_value; 
            }
            else if (std::is_same<U, int>::value) 
            { 
                value = parser.values[i]->value->int_value; 
            }
        
            
            dict[key] = value;
        }


        return dict;
    }
}