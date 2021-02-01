#pragma once
#include "token.h"
#include <memory>
#include <iostream>

namespace json_utils
{
	struct Lexer
	{
		char current_char{ ' ' };
		unsigned int index{ 0 };
		std::string contents;
		unsigned int line_num{ 0 };
		unsigned int start_idx{ 0 };
		unsigned int prev_idx{ 0 };

		Lexer() = default;
	
		Lexer(std::string contents)
			: contents(contents)
		{
			if (contents.empty())
			{
				std::stringstream err;
				err << "Cannot read file: file is either empty or does not exist.\n";
				throw std::runtime_error(err.str());
			}

			current_char = contents.at(index);
		}

		Token get_next_token()
		{
			while (index < contents.size() - 1)
			{
				prev_idx = index;
				if (isspace(current_char) && current_char != 10)
				{
					skip_whitespace();
				}

				if (isdigit(current_char))
				{
					return Token(TokenType::TOKEN_INT, collect_int());
				}

				switch (current_char)
				{
				case ',': return Token(TokenType::TOKEN_COMMA, collect_char_as_str()); break;
				case ':': return Token(TokenType::TOKEN_COLON, collect_char_as_str()); break;
				case '{': return Token(TokenType::TOKEN_LBRACE, collect_char_as_str()); break;
				case '}': return Token(TokenType::TOKEN_RBRACE, collect_char_as_str()); break;
				case '"': return Token(TokenType::TOKEN_STRING, collect_string()); break;
				case '.': return Token(TokenType::TOKEN_PERIOD, collect_char_as_str()); break;
				}

				advance();
				start_idx = index;
				++line_num;
			}

			return { TokenType::TOKEN_EOF, "" };
		}

	private:
		void advance()
		{
			if (index < contents.size() - 1)
			{
				++index;
				current_char = contents.at(index);
			}
		}

		void skip_whitespace()
		{
			while (isspace(current_char)) { advance(); }
		}

		std::string collect_string()
		{
			const auto current_index = index + 1;
			advance(); // skip quote
			while (current_char != '"' && index < contents.size() && current_char != 10) { advance(); }

			const auto string = contents.substr(current_index, index - current_index);
			advance(); // skip end quote
			return string;
		}

		std::string collect_int()
		{
			std::string integer;
			while (isdigit(current_char))
			{
				integer += current_char;
				advance();
			}

			return integer;
		}

		std::string collect_char_as_str()
		{
			const auto val = std::string(1, current_char);
			advance();
			return val;
		}
	};
}