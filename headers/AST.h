#pragma once
#include <string>
#include <memory>
#include <vector>
#include <any>


namespace json_utils
{
	enum class AstType
	{
		AST_KEY,
		AST_VALUE,
		AST_STRING,
		AST_INT,
		AST_FLOAT,
		AST_VECTOR,
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

		// float
		float float_value{ 0.0f };

		// vector
		std::vector<std::any> vector_value;
	};
}