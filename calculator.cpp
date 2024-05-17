#include<iostream>
#include<string>
#include<vector>
#include<cctype>
#include<stdexcept>
#include<map>
#include<algorithm>
#include<stack>
#include<unordered_set>
class calculator {
public:
	double output = 0.0;
	calculator() {};
	inline calculator(std::string a) :expression(a) {};
	inline ~calculator() {};
	void evaulate() {
		if (!isdigit(expression[0]) || !isdigit(expression[expression.size() - 1])) {
			throw std::runtime_error("Error:The first character combined with the last character cannot be an operator");
			return;
		}for (char i : expression) {
			if (find(pass_str.begin(), pass_str.end(), i) == pass_str.end()) {
				throw std::runtime_error("Error:Illegal characters.");
			}
		}
		std::stack<char> op_stack;
		op_stack.push('\0');
		std::stack<double> value;
		std::string Postfix = infixToPostfix(expression);
		int a1 = 0, a2 = 0;
		for (char i : Postfix) {
			if (i == ' ') {
				continue;
			}
			else if (isdigit(i)) {
				value.push(i - '0');
			}
			else {
				a1 = value.top(); value.pop();
				a2 = value.top(); value.pop();
				output = apply_op(a2, i, a1);
				try {
					value.push(apply_op(a2, i, a1));
				}
				catch (std::runtime_error err) {
					throw std::runtime_error(err.what());
					return;
				}
			}
		}
	}
private:
	std::string expression;
	inline bool isOperator(char a) { return find(pass.begin(), pass.end(), a) != pass.end(); }
	inline double apply_op(double a, char op, double b) {
		switch (op) {
		case '+':
			return a + b;
		case '-':
			return a - b;
		case '*':
			return a * b;
		case '/':
			switch ((int)b) {
			case 0:
				throw std::runtime_error("Error: Divide by 0.");
			default:
				return a / b;
			}
		default:
			throw std::runtime_error("Error:Illegal characters.");
		}
	}
	std::unordered_set<char> pass_str{ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '+', '-', '*', '/', '(', ')'};
	std::unordered_set<char> pass = { '+', '-', '*', '/' };
	inline std::string infixToPostfix(const std::string& infix) {
		std::map<char, int> op_level;
		for_each(pass.begin(), pass.end(), [&](const char& op_str) {op_level[op_str] = op_str == '+' || op_str == '-' ? 1 : 2; });
		std::stack<char> operatorsStack;
		std::string postfix;
		for (char ch : infix) {
			if (std::isdigit(ch)) {
				postfix += ch;
			}
			else if (ch == '(') {
				operatorsStack.push(ch);
			}
			else if (ch == ')') {
				while (!operatorsStack.empty() && operatorsStack.top() != '(') {
					postfix += operatorsStack.top();
					operatorsStack.pop();
				}
				operatorsStack.pop(); // Pop '(' from the stack  
			}
			else if (isOperator(ch)) {
				while (!operatorsStack.empty() && op_level[operatorsStack.top()] >= op_level[ch]) {
					postfix += operatorsStack.top();
					operatorsStack.pop();
				}
				operatorsStack.push(ch);
			}
		}
		while (!operatorsStack.empty()) {
			postfix += operatorsStack.top();
			operatorsStack.pop();
		}
		return postfix;
	}
};
int main() {
	std::cout << "welcome to calculator 1.0!please enter an experssion or enter '0' to quit." << std::endl;
	std::string a;
	while (true) {
		std::cout << "enter an experssion:";
		std::cin >> a;
		if (a == "0") {
			return 0;
		}
		std::unique_ptr<calculator> c = std::make_unique<calculator>(a);
		try {
			c->evaulate();
			std::cout << "answer: " << c->output << std::endl;
		}
		catch (std::runtime_error err) {
			std::cout << err.what() << "please try again." << std::endl;
		}
		catch (std::invalid_argument err) {
			std::cout << err.what() << "please try again." << std::endl;
		}
	}
}