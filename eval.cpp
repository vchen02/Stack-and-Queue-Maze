#include<stack>
#include<string>
#include<iostream>
#include<cassert>

using namespace std;

void infToPosf(const string&, string&, bool&);
bool calcPosf(bool*, string&);

int evaluate(const string& infix, bool values[], string& postfix, bool& result)
{
	bool syntax;
	infToPosf(infix, postfix, syntax);
	if (syntax == true)				//valid boolean syntax
	{
		result	= calcPosf(values, postfix);
		return !syntax;				//returns 0
	}
	else							//invalid boolean syntax
		return 1;					//malformed infix
}

void infToPosf(const string& infx, string& posfx, bool& syntax)
{	
	posfx	=	"";					//initialize null string
	char lastChar = '@';			//special '@' to check for beginning infix syntax errors
	syntax	= true;
	
	stack<char> charStack;
	
	if (infx.length() == 0)			//empty infix string
		syntax	= false;
	
	for (int ch_i = 0; ch_i < infx.length();ch_i++)
	{
		switch(infx[ch_i])
		{
			case ' ': break;
			case '(': 
				charStack.push(infx.at(ch_i));	
				if ( ch_i+1 == infx.length() || infx[ch_i+1] == '&' || infx[ch_i+1] ==  '|' || infx[ch_i+1] ==  ')')
					syntax	= false;
				lastChar		= infx.at(ch_i);
				break;
			case ')': 
			{	
				if (ch_i+1 != infx.length() && ( isdigit(infx[ch_i+1]) || infx[ch_i+1] == '!' || lastChar =='@' || infx[ch_i+1] == '(' ) )
					syntax	= false;
				if (ch_i+1 == infx.length() && lastChar == '!' || lastChar == '&' || lastChar == '|' )
					syntax	= false;
				
				if (!charStack.empty())
				{
					while (charStack.top() != '(')
					{	
						posfx	+= charStack.top();		//put everything between the parenthesis into postfix string
						charStack.pop();
						if (charStack.empty())
							break;
					}
					if (!charStack.empty())
					{
						charStack.pop();			//pop the '(' parenethesis
						lastChar		= infx[ch_i];
						break;
					}
				}
			}
			case '!':
			case '&':
			case '|':
				if (infx[ch_i] != '!' && lastChar == '@' )
					syntax	= false;
				if ( ch_i+1 == infx.length() ||  lastChar == '(' || lastChar == '!' || infx[ch_i+1] == '&' || infx[ch_i+1] == '|' || infx[ch_i+1] == ')' )
					syntax	= false;
				while (!charStack.empty() && charStack.top() != '(')
				{
					//check for	operator precedence
					if ( infx[ch_i] != charStack.top() && ( (infx[ch_i] == '!' || infx.at(ch_i) == '&') && (charStack.top() == '&' || charStack.top() == '|') ) )
						break;
					posfx	+= charStack.top();
					charStack.pop();
				}
				charStack.push(infx[ch_i]);
				lastChar		= infx[ch_i];
				break;
				
			default: 
				if(isdigit(infx.at(ch_i)))
					posfx	+= infx.at(ch_i);
				if ( ch_i+1 != infx.length() && ( infx[ch_i+1] == '!' || infx[ch_i+1] == '(' || isdigit(infx[ch_i+1]) ) )
					syntax	= false;
				if(isdigit(lastChar))
					syntax	= false;
				lastChar		= infx[ch_i];
				break;
		}
	}
	while (!charStack.empty())
	{
		if (charStack.top() == '(')			//an open parenthesis is not popped off the stack
			syntax	= false;
		posfx	+= charStack.top();
		charStack.pop();
	}
}

bool calcPosf(bool val[], string& posfx)
{
	bool result, operand2, operand1;
	stack<bool> numStack;					//empty operand stack
	for (int ch_k = 0; ch_k < posfx.length(); ch_k++)
	{
		if (isdigit(posfx.at(ch_k)))		//if character is a number
		{
			result	= val[posfx.at(ch_k) - '0'];
			numStack.push(result);
		}
		else								//character is an operator
		{
			operand2	= numStack.top();	//copy data of top stack
			numStack.pop();
			if (posfx.at(ch_k) == '!')
				result	= !operand2;
			
			else
			{
				operand1	= numStack.top();	//copy data of 2nd top stack
				numStack.pop();
				
				switch (posfx.at(ch_k))			
				{
					case '&': result	= operand1 && operand2; break;
					case '|': result	= operand1 || operand2; break;
				}
			}
			numStack.push(result);
		}
	}
	result	= numStack.top();
	numStack.pop();
	return result;
}

int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	
	string pf;
	bool answer;
	
	assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  answer);
	
	assert(evaluate("8|", ba, pf, answer) == 1 && answer);
	assert(evaluate("4 5", ba, pf, answer) == 1 && answer);
	assert(evaluate("01", ba, pf, answer) == 1 && answer);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1 && answer);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0 &&  pf == "43!03&&|"  &&  !answer);
	assert(evaluate("", ba, pf, answer) == 1 && !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0  &&  pf == "9"  &&  !answer);
	
	assert(evaluate("(9!!!)  ", ba, pf, answer)==1 && pf =="9!!!" && !answer);
	assert(evaluate("&7|9", ba, pf, answer) == 1 && !answer);
	/*	assert(evaluate("4 & 5|!6", ba, pf, answer) == 0 && pf == "45&6!|" && !answer);
	 assert(evaluate("!3 | !1 | 5 |2", ba, pf, answer) == 0 && pf=="3!1!|5|2|" && answer);
	 assert(evaluate(")", ba, pf, answer) == 1 && answer);
	 //	assert(evaluate("(", ba, pf, answer) == 1 && pf == "(" && answer);
	 //	assert(evaluate("(&9|!7)", ba, pf, answer) == 1 && answer);
	 /*	
	 ba[2] = false;
	 ba[9] = true;
	 assert(evaluate("((9))", ba, pf, answer) == 0  &&  pf == "9"  &&  answer);
	 assert(evaluate("2| 3", ba, pf, answer) == 0  &&  pf == "23|" &&  !answer);
	 assert(evaluate("&", ba, pf, answer) == 1 && !answer);
	 assert(evaluate("3&!(4)", ba, pf, answer) == 0 && !answer);
	 assert(evaluate("!| (4 & 5)", ba, pf, answer) == 1);
	 /*	assert(evaluate("!)", ba, pf, answer) == 1);
	 assert(evaluate("!|", ba, pf, answer) == 1);
	 assert(evaluate("!&", ba, pf, answer) == 1);
	 assert(evaluate("&)", ba, pf, answer) == 1);
	 assert(evaluate("&|", ba, pf, answer) == 1);
	 assert(evaluate("&&", ba, pf, answer) == 1);
	 assert(evaluate("|)", ba, pf, answer) == 1);
	 assert(evaluate("||", ba, pf, answer) == 1);
	 assert(evaluate("|&", ba, pf, answer) == 1);
	 
	 assert(evaluate("()", ba, pf, answer) == 1);
	 assert(evaluate("(|", ba, pf, answer) == 1);
	 assert(evaluate("(&", ba, pf, answer) == 1);
	 assert(evaluate("99", ba, pf, answer) == 1);
	 
	 assert(evaluate("00(", ba, pf, answer) == 1);
	 assert(evaluate("00|", ba, pf, answer) == 1);
	 assert(evaluate("00&", ba, pf, answer) == 1);
	 assert(evaluate("00!!)", ba, pf, answer) == 1);
	 
	 assert(evaluate("&", ba, pf, answer) == 1);
	 assert(evaluate(")", ba, pf, answer) == 1);
	 assert(evaluate("|)", ba, pf, answer) == 1);
	 */	
	cout << "Passed all tests" << endl;
	
	return 0;
}

