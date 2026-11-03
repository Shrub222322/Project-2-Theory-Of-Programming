#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

string END = "$$";

string opMulti = "*";
string opAdd = "+";
string opSub = "-";
string opDiv = "/";

string opAssign = ":=";

string currentToken;
string oneLookahead;

vector<string> tokens;
int globalcount = 0;

void expr();
void stmtlist();
void stmt();
bool validID(int x);
void term();
void termtail();
void factor();
void facttail();
bool validNum(int x);
void addop();
void multiop();
string peekahead();

void program() {
	cout << "program -> stmtlist" << endl;
	if (!validID(globalcount))
		return;
	stmtlist();
	if (tokens.at(globalcount) == END) {
		cout << "End Of File $$" << endl;
		return;
	}
	

}
void stmtlist() {
	//cout << "stmtlist → stmt stmtlist | ϵ" << endl;
	stmt();
	if (validID(globalcount) || validNum(globalcount))
		stmtlist(); // or epsilon and validID above should handle this maybe
}
void stmt() {
	cout << "stmtlist -> stmt stmtlist | ϵ" << endl;
	if (tokens.at(globalcount) == "read"){
		// this is an id so we just take it in and consume it.
		cout << "Consuming id: " << tokens.at(globalcount) << endl;
		globalcount++;
		if (validID(globalcount))
		{
			cout << "Consuming " << tokens.at(globalcount) << endl;
			globalcount++;
		}
		else
		{
			cout << "Invalid ID";
		}
	}
	else if (tokens.at(globalcount) == "write") {
		cout << "stmt -> write expr" << endl;
		cout << "Consuming write: " << tokens.at(globalcount) << endl;
		globalcount++;
		expr(); // id := expr | read id | write expr
	}
	else if (validID(globalcount)) // an id
	{
		cout << "Consuming " << tokens.at(globalcount) << endl;
		globalcount++;
		if (tokens.at(globalcount) == opAssign)
		{
			cout << "Consuming " << tokens.at(globalcount) << endl;
			globalcount++;
			expr();
		}

	}
	else if (!validID(globalcount)){
		cout << "Error: Invalid ID at STMT" << endl;
		return;
	}
}

void expr() {
	cout << "expr -> term termtail" << endl;
	
	term();

	termtail();

	/*
		expr → term term tail
		term tail → add op term term tail | ϵ
		term → factor fact tail
		fact tail → mult op factor fact tail | ϵ
		factor → ( expr ) | id | number
		
	*/
	
	
}
void term() {
	cout << "term -> factor fact tail" << endl;
	factor();
	facttail();
}
void termtail()
{
	if (tokens.size() != globalcount && ((tokens.at(globalcount) == opAdd) || tokens.at(globalcount) == opSub)) {
		cout << "termtail -> add op term term tail | ϵ" << endl;
		addop();
		term();
		if (tokens.size() != globalcount && ((tokens.at(globalcount) == opAdd) || tokens.at(globalcount) == opSub))
			termtail();
		else
			return;
	}
	else
		return;
	//add op term term tail or nothing
}
void factor() {
	cout << "factor -> ( expr ) | id | number" << endl;
	if (tokens.at(globalcount) == "(")
	{
		cout << "Consuming " << tokens.at(globalcount) << endl;
		globalcount++;
		expr();

		if (tokens.at(globalcount) == ")")
		{
			cout << "Consuming " << tokens.at(globalcount) << endl;
			globalcount++;
			//return;
		}
		else
		{
			cout << "Expecting )" << endl;
		}
	}
	else if (validID(globalcount))
	{
		cout << "Consuming " << tokens.at(globalcount) << endl;
		globalcount++;
		//return;

	}
	else if (validNum(globalcount))
	{
		cout << "Consuming " << tokens.at(globalcount) << endl;
		globalcount++;
		//return;
	}
	else
	{
		cout << "ERROR at factor"  << endl;
		//return;
	}

	// ( expr ) | id | number
}
void facttail()
{
	if (tokens.size() != globalcount && ((tokens.at(globalcount) == opMulti) || (tokens.at(globalcount) == opDiv))) {
		cout << "fact tail -> mult op factor fact tail | ϵ" << endl;
		multiop();
		factor();
		if (tokens.size() != globalcount && ((tokens.at(globalcount) == opMulti) || (tokens.at(globalcount) == opDiv)))
			facttail();
		else
			return;
	}
	else
		return;
	// mult op factor fact tail | ϵ

}
void addop() {
	if (tokens.at(globalcount) == opAdd || tokens.at(globalcount) == opSub)
	{
		cout << "Consuming " << tokens.at(globalcount) << endl;
		globalcount++;
		return;
	}
	else
		cout << "Error at addop" << endl;
	// plus or minus
}
void multiop() {
	if (tokens.at(globalcount) == opMulti || tokens.at(globalcount) == opDiv)
	{
		cout << "Consuming " << tokens.at(globalcount) << endl;
		globalcount++;
		return;

	}
	else
		cout << "Error at multiop" << endl;
	// times or divide
}



bool validID(int x)
{
	string id = tokens.at(x);
	if (id.length() == 0) {
		return false;
	}
	if (!isalpha(id[0])) {
		return false;
	}

	return true;
}

bool validNum(int x)
{
	bool valid = true;
	string value = tokens.at(x);

	for (int i = 0; i < value.length(); i++)
	{
		if (isdigit(value[i]) || value[i] == '.')
		{
			valid = true;
		}
		else {
			valid = false;
			break;
		}

	}
	return valid;
	

}

int main() {
	
	ifstream inputFile("input.txt");
	if (!inputFile) {
		cout << "Error opening input file." << endl;
		return 1;
	}

	while (inputFile >> currentToken) {
		tokens.push_back(currentToken);
	}

	//while (tokens.size() != globalcount && (validID(globalcount)))
	//{
		program();
	//}
	//for (const string& token : tokens) {
	//	cout << token << endl;
	//}


	

	

}	

/*
program → stmtlist $$
stmtlist → stmt stmtlist | ϵ
stmt → id := expr | read id | write expr
expr → term termtail
termtail → add op term termtail | ϵ
term → factor facttail
facttail → mult op factor facttail | ϵ
factor → ( expr ) | id | number
add op → + | -
mult op → * | /

*/