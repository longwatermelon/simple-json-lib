#pragma once
#include <string>

namespace jsonutils
{
	enum class TokenType
	{
		TOKEN_LBRACE,
		TOKEN_RBRACE,
		TOKEN_COLON,
		TOKEN_COMMA,
		TOKEN_STRING,
		TOKEN_INT,
		TOKEN_PERIOD,
		TOKEN_LBRACKET,
		TOKEN_RBRACKET,
		TOKEN_EOF
	};

	struct Token
	{
		TokenType type_{ TokenType::TOKEN_EOF };
		std::string value;

		Token() = default;

		Token(TokenType type, std::string value)
			: type_(std::move(type)), value(value)
		{
		}

	
	};
}