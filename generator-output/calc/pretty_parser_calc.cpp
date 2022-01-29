
#include "pretty_parser_calc.h"

format_parser_t::format_parser_t(std::string input)
: lex(std::move(input)) {}

std::tuple<int> format_parser_t::parse_e() {
	int res_x;
	switch (lex.get_token()) {
		case (LBR):
		case (DIGIT):
		{
			auto [t0_x] = parse_t();
			int left = t0_x;  // usr-code
			auto [e_1_x] = parse_e_(left);
			res_x = e_1_x;  // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: e");
	}
}

std::tuple<int> format_parser_t::parse_e_(int arg) {
	int res_x;
	switch (lex.get_token()) {
		case (PLUS):
		{
			if (lex.get_token() != PLUS)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: e_");
			lex.next_token();
			auto [t1_x] = parse_t();
			int val = t1_x;  // usr-code
			auto [plus_op2_x] = parse_plus_op(arg, val);
			int cur = plus_op2_x;  // usr-code
			auto [e_3_x] = parse_e_(cur);
			res_x = e_3_x;  // usr-code
			return {res_x};
		}
		case (MINUS):
		{
			if (lex.get_token() != MINUS)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: e_");
			lex.next_token();
			auto [t1_x] = parse_t();
			int val = t1_x;  // usr-code
			auto [minus_op2_x] = parse_minus_op(arg, val);
			int cur = minus_op2_x;  // usr-code
			auto [e_3_x] = parse_e_(cur);
			res_x = e_3_x;  // usr-code
			return {res_x};
		}
		case (END):
		case (RBR):
		{
			res_x = arg;  // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: e_");
	}
}

std::tuple<int> format_parser_t::parse_t() {
	int res_x;
	switch (lex.get_token()) {
		case (LBR):
		case (DIGIT):
		{
			auto [w0_x] = parse_w();
			int left = w0_x;  // usr-code
			auto [t_1_x] = parse_t_(left);
			res_x = t_1_x;  // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: t");
	}
}

std::tuple<int> format_parser_t::parse_t_(int arg) {
	int res_x;
	switch (lex.get_token()) {
		case (MUL):
		{
			if (lex.get_token() != MUL)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: t_");
			lex.next_token();
			auto [w1_x] = parse_w();
			int val = w1_x;  // usr-code
			auto [mul_op2_x] = parse_mul_op(arg, val);
			int cur = mul_op2_x;  // usr-code
			auto [t_3_x] = parse_t_(cur);
			res_x = t_3_x;  // usr-code
			return {res_x};
		}
		case (DIV):
		{
			if (lex.get_token() != DIV)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: t_");
			lex.next_token();
			auto [w1_x] = parse_w();
			int val = w1_x;  // usr-code
			auto [div_op2_x] = parse_div_op(arg, val);
			int cur = div_op2_x;  // usr-code
			auto [t_3_x] = parse_t_(cur);
			res_x = t_3_x;  // usr-code
			return {res_x};
		}
		case (MINUS):
		case (RBR):
		case (PLUS):
		case (END):
		{
			res_x = arg;  // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: t_");
	}
}

std::tuple<int> format_parser_t::parse_w() {
	int res_x;
	switch (lex.get_token()) {
		case (LBR):
		case (DIGIT):
		{
			auto [fact0_x] = parse_fact();
			auto [w_1_x] = parse_w_();
			auto [comb_op2_x] = parse_comb_op(fact0_x, w_1_x);
			res_x = comb_op2_x;  // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: w");
	}
}

std::tuple<int> format_parser_t::parse_w_() {
	int res_x;
	switch (lex.get_token()) {
		case (DOT_COMMA):
		{
			if (lex.get_token() != DOT_COMMA)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: w_");
			lex.next_token();
			auto [fact1_x] = parse_fact();
			auto [w_2_x] = parse_w_();
			auto [comb_op3_x] = parse_comb_op(fact1_x, w_2_x);
			res_x = comb_op3_x;  // usr-code
			return {res_x};
		}
		case (END):
		case (PLUS):
		case (MINUS):
		case (RBR):
		case (MUL):
		case (DIV):
		{
			res_x = 1;  // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: w_");
	}
}

std::tuple<int> format_parser_t::parse_fact() {
	int res_x;
	switch (lex.get_token()) {
		case (LBR):
		case (DIGIT):
		{
			auto [f0_x] = parse_f();
			int val = f0_x;  // usr-code
			auto [fact_1_x] = parse_fact_(val);
			res_x = fact_1_x;  // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: fact");
	}
}

std::tuple<int> format_parser_t::parse_fact_(int arg) {
	int res_x;
	switch (lex.get_token()) {
		case (RUN_FACT):
		{
			if (lex.get_token() != RUN_FACT)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: fact_");
			lex.next_token();
			auto [fact_op1_x] = parse_fact_op(arg);
			int val = fact_op1_x;  // usr-code
			auto [fact_2_x] = parse_fact_(val);
			res_x = fact_2_x;  // usr-code
			return {res_x};
		}
		case (DOT_COMMA):
		case (DIV):
		case (MUL):
		case (MINUS):
		case (RBR):
		case (PLUS):
		case (END):
		{
			res_x = arg;  // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: fact_");
	}
}

std::tuple<int> format_parser_t::parse_f() {
	int res_x;
	switch (lex.get_token()) {
		case (LBR):
		{
			if (lex.get_token() != LBR)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: f");
			lex.next_token();
			auto [e1_x] = parse_e();
			if (lex.get_token() != RBR)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: f");
			res_x = e1_x;  // usr-code
			lex.next_token();
			return {res_x};
		}
		case (DIGIT):
		{
			if (lex.get_token() != DIGIT)
				throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: f");
			res_x = std::atoi(lex.get_value().c_str());  // usr-code
			lex.next_token();
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: f");
	}
}

std::tuple<int> format_parser_t::parse_fact_op(int arg) {
	int res_x;
	switch (lex.get_token()) {
		case (DOT_COMMA):
		case (DIV):
		case (MUL):
		case (MINUS):
		case (RBR):
		case (PLUS):
		case (END):
		case (RUN_FACT):
		{
			int result_ = 1;
        for (int i_ = 2; i_ <= arg; i_++) {
            result_ *= i_;
        }
        res_x = result_;
     // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: fact_op");
	}
}

std::tuple<int> format_parser_t::parse_comb_op(int n,int k) {
	int res_x;
	switch (lex.get_token()) {
		case (DIV):
		case (MUL):
		case (MINUS):
		case (RBR):
		case (PLUS):
		case (END):
		{
			auto [fact_op0_x] = parse_fact_op(n);
			auto [fact_op1_x] = parse_fact_op(k);
			int diff = n - k;  // usr-code
			auto [fact_op2_x] = parse_fact_op(diff);
			if (n == 0) {
            res_x = 0;
        } else {
            res_x = fact_op0_x / (fact_op1_x * fact_op2_x);
        }
     // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: comb_op");
	}
}

std::tuple<int> format_parser_t::parse_plus_op(int a,int b) {
	int res_x;
	switch (lex.get_token()) {
		case (END):
		case (PLUS):
		case (MINUS):
		case (RBR):
		{
			res_x = a + b;  // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: plus_op");
	}
}

std::tuple<int> format_parser_t::parse_minus_op(int a,int b) {
	int res_x;
	switch (lex.get_token()) {
		case (END):
		case (PLUS):
		case (MINUS):
		case (RBR):
		{
			res_x = a - b;  // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: minus_op");
	}
}

std::tuple<int> format_parser_t::parse_mul_op(int a,int b) {
	int res_x;
	switch (lex.get_token()) {
		case (MINUS):
		case (RBR):
		case (PLUS):
		case (END):
		case (MUL):
		case (DIV):
		{
			res_x = a * b;  // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: mul_op");
	}
}

std::tuple<int> format_parser_t::parse_div_op(int a,int b) {
	int res_x;
	switch (lex.get_token()) {
		case (MINUS):
		case (RBR):
		case (PLUS):
		case (END):
		case (MUL):
		case (DIV):
		{
			if (b != 0) {
            res_x = a / b;
        } else {
            throw std::logic_error("Divide by zero");
        }
     // usr-code
			return {res_x};
		}
		default:
			throw std::logic_error("Unexpected token: " + lex.get_value() + "\nWhen processing nterm: div_op");
	}
}


