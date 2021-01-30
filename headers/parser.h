#pragma once
#include "lexer.h"
#include "AST.h"
#include <memory>
#include <map>
#include <string>
#include <sstream>

namespace json_utils
{
struct Parser
{
	Lexer lexer;
	Token current_token;
	Token prev_token;
	std::vector<std::shared_ptr<AST>> keys;
	std::vector<std::shared_ptr<AST>> values;

	Parser(std::string contents)
	{
		lexer = Lexer(contents);
		current_token = lexer.get_next_token();
		prev_token = current_token;
	}

	void eat(TokenType type)
	{
		if (current_token.type_ == type)
		{
			prev_token = current_token;
			current_token = lexer.get_next_token();
		}
		else
		{
			std::string expected;
			std::map<TokenType, std::string> token_repr{
				{TokenType::TOKEN_COLON, ":"},
				{TokenType::TOKEN_COMMA, ","},
				{TokenType::TOKEN_INT, "int"},
				{TokenType::TOKEN_LBRACE, "{"},
				{TokenType::TOKEN_RBRACE, "}"},
				{TokenType::TOKEN_STRING, "string"},
				{TokenType::TOKEN_PERIOD, "."}
			};

			std::stringstream err;
			err << "Unexpected token '" << lexer.contents.at(lexer.prev_idx) << "' at line " << lexer.line_num
				<< "\nExpected: " << token_repr[type] << "\n";
			std::cout << err.str();
			exit(1);
		}
	}


	void parse()
	{
		parse_statements();
	}


	std::shared_ptr<AST> parse_key()
	{
		if (current_token.type_ != TokenType::TOKEN_LBRACE) eat(TokenType::TOKEN_COMMA);
		else eat(TokenType::TOKEN_LBRACE);

		const auto ast_value = parse_expr();
		const auto key = std::make_shared<AST>(AstType::AST_KEY);
		key->type = AstType::AST_KEY;
		key->value = ast_value;

		return key;
	}


	std::shared_ptr<AST> parse_value()
	{
		eat(TokenType::TOKEN_COLON);

		const auto ast_value = parse_expr();
		const auto value = std::make_shared<AST>(AstType::AST_VALUE);
		value->type = AstType::AST_VALUE;
		value->value = ast_value;

		return value;
	}


	std::shared_ptr<AST> parse_int()
	{
		const auto ast_int = std::make_shared<AST>(AstType::AST_INT);
		ast_int->type = AstType::AST_INT;
		std::istringstream(current_token.value) >> ast_int->int_value;
		eat(TokenType::TOKEN_INT);

		// checking for float
		if (current_token.type_ == TokenType::TOKEN_PERIOD)
		{
			return parse_float(ast_int->int_value);
		}

		return ast_int;
	}


	std::shared_ptr<AST> parse_float(int int_val)
	{
		eat(TokenType::TOKEN_PERIOD);

		float float_val = static_cast<float>(int_val);
		int after_decimal = 0;
		std::istringstream(current_token.value) >> after_decimal;
		std::stringstream final_float;
		final_float << int_val << '.' << after_decimal;
		std::istringstream(final_float.str()) >> float_val;

		eat(TokenType::TOKEN_INT);

		const auto ast_float = std::make_shared<AST>(AstType::AST_FLOAT);
		ast_float->type = AstType::AST_FLOAT;
		ast_float->float_value = float_val;
		return ast_float;
	}

	
	std::shared_ptr<AST> parse_string()
	{
		const auto ast_str = std::make_shared<AST>(AstType::AST_STRING);
		ast_str->type = AstType::AST_STRING;
		ast_str->string_value = current_token.value;
		eat(TokenType::TOKEN_STRING);
		return ast_str;
	}


	std::shared_ptr<AST> parse_expr()
	{
		switch (current_token.type_)
		{
		case TokenType::TOKEN_LBRACE: return parse_key();   // beginning of json file
		case TokenType::TOKEN_COLON: return parse_value();  // after a colon comes a value (key: value)
		case TokenType::TOKEN_COMMA: return parse_key();    // after a comma comes a key (key: value, key2: value2)
		case TokenType::TOKEN_INT: return parse_int();
		case TokenType::TOKEN_STRING: return parse_string();
		}

		return nullptr;
	}


	void parse_statements()
	{
		auto expr = parse_expr();
		assign_item_to_vector(expr);

		while (lexer.index < lexer.contents.size())
		{
			expr = parse_expr();

			if (expr != nullptr) assign_item_to_vector(expr);
			else break; // end of file if expr is nullptr
		}
	}


	void assign_item_to_vector(std::shared_ptr<AST> expr)
	{
		switch (expr->type)
		{
		case AstType::AST_KEY: keys.emplace_back(expr); break;
		case AstType::AST_VALUE: values.emplace_back(expr); break;
		}
	}
};
}