#pragma once
#include <string>

enum class TokenType
{
	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_COLON,
	TOKEN_COMMA,
	TOKEN_STRING,
	TOKEN_INT,
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