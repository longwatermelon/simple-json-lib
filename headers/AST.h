#pragma once
#include <string>
#include <memory>
#include <vector>

enum class AstType
{
	AST_KEY,
	AST_VALUE,
	AST_STRING,
	AST_INT,
	AST_NOOP
};

struct AST
{
	AST(AstType type)
	{}

	AstType type{ AstType::AST_NOOP };
	std::shared_ptr<AST> value;

	// string
	std::string string_value;

	// int
	int int_value{ 0 };
};