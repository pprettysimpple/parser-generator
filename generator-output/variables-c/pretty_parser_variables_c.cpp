
#include "pretty_parser_variables_c.h"

format_parser_t::format_parser_t(std::string input)
: lex(std::move(input)) {}

std::tuple<node_t> format_parser_t::parse_declare() {
	node_t res_node;
	switch (lex.get_token()) {
		case (ID):
		{
			if (lex.get_token() != ID)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: declare");
			res_node.set_name("declare"); res_node.add_child(node_t(lex.get_value() + " ")); // usr-code
			lex.next_token();
			auto [names1_node] = parse_names();
			res_node.add_child(names1_node); // usr-code
			if (lex.get_token() != DOT_COMMA)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: declare");
			res_node.add_child(node_t(";")); // usr-code
			lex.next_token();
			return {res_node};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: declare");
	}
}

std::tuple<node_t> format_parser_t::parse_names() {
	node_t res_node;
	switch (lex.get_token()) {
		case (STAR):
		case (ID):
		{
			auto [name0_node] = parse_name();
			res_node.set_name("names"); res_node.add_child(name0_node); // usr-code
			auto [names_continue1_node] = parse_names_continue();
			res_node.add_child(names_continue1_node); // usr-code
			return {res_node};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: names");
	}
}

std::tuple<node_t> format_parser_t::parse_names_continue() {
	node_t res_node;
	switch (lex.get_token()) {
		case (COMMA):
		{
			if (lex.get_token() != COMMA)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: names_continue");
			res_node.set_name("names_continue"); res_node.add_child(node_t(lex.get_value())); // usr-code
			lex.next_token();
			auto [name1_node] = parse_name();
			res_node.add_child(name1_node); // usr-code
			auto [names_continue2_node] = parse_names_continue();
			res_node.add_child(names_continue2_node); // usr-code
			return {res_node};
		}
		case (DOT_COMMA):
		{
			return {res_node};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: names_continue");
	}
}

std::tuple<node_t> format_parser_t::parse_name() {
	node_t res_node;
	switch (lex.get_token()) {
		case (STAR):
		{
			if (lex.get_token() != STAR)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: name");
			res_node.set_name("name"); res_node.add_child(node_t(lex.get_value())); // usr-code
			lex.next_token();
			auto [name1_node] = parse_name();
			res_node.add_child(name1_node); // usr-code
			return {res_node};
		}
		case (ID):
		{
			if (lex.get_token() != ID)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: name");
			res_node.set_name("name"); res_node.add_child(node_t(lex.get_value())); // usr-code
			lex.next_token();
			auto [maybe_brackets1_node] = parse_maybe_brackets();
			res_node.add_child(maybe_brackets1_node); // usr-code
			return {res_node};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: name");
	}
}

std::tuple<node_t> format_parser_t::parse_maybe_brackets() {
	node_t res_node;
	switch (lex.get_token()) {
		case (LBR):
		{
			if (lex.get_token() != LBR)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: maybe_brackets");
			res_node.set_name("maybe_brackets"); res_node.add_child(node_t(lex.get_value())); // usr-code
			lex.next_token();
			if (lex.get_token() != NUM)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: maybe_brackets");
			res_node.add_child(node_t(lex.get_value())); // usr-code
			lex.next_token();
			if (lex.get_token() != RBR)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: maybe_brackets");
			res_node.add_child(node_t(lex.get_value())); // usr-code
			lex.next_token();
			return {res_node};
		}
		case (COMMA):
		case (DOT_COMMA):
		{
			return {res_node};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: maybe_brackets");
	}
}


