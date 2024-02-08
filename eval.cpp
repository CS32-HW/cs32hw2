#include <stack>
#include <string>
#include <ctype.h>
#include <iostream>
#include "Map.h"
using namespace std;

// returns true if precendence of ch <= precedence of top
bool lessPrecedence(char ch, char top);
// returns precendence of operator as an integer
int precedence(char ch);
// returns true if ch is an operator
bool isOperator(char ch);

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
  // Evaluates an integer arithmetic expression
  //   If infix is a syntactically valid infix integer expression whose
  //   only operands are single lower case letters (whether or not they
  //   appear in the values map), then postfix is set to the postfix
  //   form of the expression; if not, postfix may or may not be
  //   changed, result is unchanged, and the function returns 1.  If
  //   infix is syntactically valid but contains at least one lower
  //   case letter operand that does not appear in the values map, then
  //   result is unchanged and the function returns 2.  If infix is
  //   syntactically valid and all its lower case operand letters
  //   appear in the values map, then if evaluating the expression
  //   (using for each letter in the expression the value in the map
  //   that corresponds to it) attempts to divide by zero, then result
  //   is unchanged and the function returns 3; otherwise, result is
  //   set to the value of the expression and the function returns 0.

	if (infix.length() == 0)
		return 1;

	int operands = 0;
	int operators = 0;
	string processed;
	for (int i = 0; i < infix.length(); i++) {
		if (isOperator(infix[i]))
			operators++;
		else if (islower(infix[i]))
			operands++;

		// get rid of spaces
		if (infix[i] != ' ')
			processed += infix[i];
	}

	if (operands != operators + 1)
		return 1;

	stack<bool> paren;
	for (int i = 0; i < processed.length() - 1; i++) {
		char ch = processed[i];
		char next = processed[i+1];
		if (islower(next) && ch == ')')
			return 1;
		if (isOperator(next) && ch == '(')
			return 1;
	}
	for (int i = 0; i < processed.length(); i++) {
		char ch = processed[i];
		if (ch == '(') {
			paren.push(true);
		} else if (ch == ')') {
			if (paren.empty())
				return 1;
			else
				paren.pop();
		}
	}
	if (!paren.empty())
		return 1;

	postfix = "";
	stack<char> op;
	for (int i = 0; i < processed.length(); i++) {
		char ch = processed[i];
		// is operand
		if (ch >= 'a' && ch <= 'z') {
			postfix += ch;
		} else if (ch == '(') {
			op.push(ch);
		} else if (ch == ')') {
			while (op.top() != '(') {
				postfix += op.top();
				op.pop();
			}
			op.pop(); // remove '('
		} else if (isOperator(ch)) {
			while (!op.empty() && op.top() != '(' && 
					lessPrecedence(ch, op.top())) {
				postfix += op.top();
				op.pop();
			}
			op.push(ch);
		} else if (ch == ' ') {
			;
		} else {
			return 1;
		}
	}

	while (!op.empty()) {
		postfix += op.top();
		op.pop();
	}

	stack<int> opstack;

	for (int i = 0; i < postfix.length(); i++) {
		char ch = postfix[i];
		if (ch >= 'a' && ch <= 'z') {
			int op1;
			if (!values.get(ch, op1))
				return 2;
			opstack.push(op1);
		} else {
			int op2 = opstack.top();
			opstack.pop();
			int op1 = opstack.top();
			opstack.pop();
			switch (ch) {
			case '+':
				opstack.push(op1 + op2);
				break;
			case '-':
				opstack.push(op1 - op2);
				break;
			case '*':
				opstack.push(op1 * op2);
				break;
			case '/':
				if (op2 == 0)
					return 3;
				opstack.push(op1 / op2);
				break;
			}
		}
	}

	if (!opstack.empty())
		result = opstack.top();
	return 0;
}

bool lessPrecedence(char ch, char top)
{
	return precedence(ch) <= precedence(top);	
}

int precedence(char ch)
{
	switch (ch) {
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	}
	return 0;
}

bool isOperator(char ch)
{
	return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

int main()
{
	Map m;
	m.insert('a', 1);
	m.insert('b', 2);
	m.insert('c', 2);

	string postfix;
	int result;
	evaluate("a + b / c", m, postfix, result);
	cerr << postfix << " " << result << endl;

	return 0;
}
