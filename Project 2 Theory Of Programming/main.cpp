#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>


//	no i didnt use ai 
//  this should handle everything including errors quite well!
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
vector<string> outputTokens;
int globalcount = 0;

bool errorFlag = false;

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

void program() {
	cout << "program -> stmtlist" << endl;
	outputTokens.push_back("program -> stmtlist");
	if (!validID(globalcount))
		return;
	stmtlist();
	if (tokens.at(globalcount) == END) {
		cout << "End Of File $$" << endl;
		if (errorFlag)
		{
			cout << "Errors encountered during parsing" << endl;
			outputTokens.push_back("End Of File $$");
			outputTokens.push_back("Parsing Completed but encountered Errors");
			return;
		}
		else
		{
			cout << "Parsing Complete Successfully" << endl;
			outputTokens.push_back("End Of File $$");
			outputTokens.push_back("Parsing Complete Successfully");
			return;
		}
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
	outputTokens.push_back("stmtlist -> stmt stmtlist | ϵ");
	if (tokens.at(globalcount) == "read"){
		// this is an id so we just take it in and consume it.
		cout << "Consuming read: " << tokens.at(globalcount) << endl;
		outputTokens.push_back("Consuming read: " + tokens.at(globalcount));
		globalcount++;
		if (validID(globalcount))
		{
			cout << "Consuming " << tokens.at(globalcount) << endl;
			outputTokens.push_back("Consuming " + tokens.at(globalcount));
			globalcount++;
		}
		else
		{
			outputTokens.push_back("Error: Invalid ID at STMT");
			cout << "Invalid ID";
			errorFlag = true;
		}
	}
	else if (tokens.at(globalcount) == "write") {
		cout << "stmt -> write expr" << endl;
		outputTokens.push_back("stmt -> write expr");
		cout << "Consuming write: " << tokens.at(globalcount) << endl;
		outputTokens.push_back("Consuming write: " + tokens.at(globalcount));
		globalcount++;
		expr(); // id := expr | read id | write expr
	}
	else if (validID(globalcount)) // an id
	{
		cout << "Consuming " << tokens.at(globalcount) << endl;
		outputTokens.push_back("Consuming " + tokens.at(globalcount));
		globalcount++;
		if (tokens.at(globalcount) == opAssign)
		{
			cout << "Consuming " << tokens.at(globalcount) << endl;
			outputTokens.push_back("Consuming " + tokens.at(globalcount));
			globalcount++;
			expr();
		}

	}
	else if (!validID(globalcount)){
		cout << "Error: Invalid ID at STMT" << endl;
		outputTokens.push_back("Error: Invalid ID at STMT");
		errorFlag = true;
		return;
	}
}

void expr() {
	cout << "expr -> term termtail" << endl;
	outputTokens.push_back("expr -> term termtail");

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
	outputTokens.push_back("term -> factor fact tail");
	factor();
	facttail();
}
void termtail()
{
	if (tokens.size() != globalcount && ((tokens.at(globalcount) == opAdd) || tokens.at(globalcount) == opSub)) {
		cout << "termtail -> add op term term tail | ϵ" << endl;
		outputTokens.push_back("termtail -> add op term term tail | ϵ");
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
	outputTokens.push_back("factor -> ( expr ) | id | number");
	if (tokens.at(globalcount) == "(")
	{
		cout << "Consuming " << tokens.at(globalcount) << endl;
		outputTokens.push_back("Consuming " + tokens.at(globalcount));
		globalcount++;
		expr();

		if (tokens.at(globalcount) == ")")
		{
			cout << "Consuming " << tokens.at(globalcount) << endl;
			outputTokens.push_back("Consuming " + tokens.at(globalcount));
			globalcount++;
			//return;
		}
		else
		{
			cout << "Expecting )" << endl;
			outputTokens.push_back("Expecting )");
			errorFlag = true;
		}
	}
	else if (validID(globalcount))
	{
		cout << "Consuming " << tokens.at(globalcount) << endl;
		outputTokens.push_back("Consuming " + tokens.at(globalcount));
		globalcount++;
		//return;

	}
	else if (validNum(globalcount))
	{
		cout << "Consuming " << tokens.at(globalcount) << endl;
		outputTokens.push_back("Consuming " + tokens.at(globalcount));
		globalcount++;
		//return;
	}
	else
	{
		cout << "ERROR at factor"  << endl;
		outputTokens.push_back("ERROR at factor");
		errorFlag = true;
		//return;
	}

	// ( expr ) | id | number
}
void facttail()
{
	if (tokens.size() != globalcount && ((tokens.at(globalcount) == opMulti) || (tokens.at(globalcount) == opDiv))) {
		cout << "fact tail -> mult op factor fact tail | ϵ" << endl;
		outputTokens.push_back("fact tail -> mult op factor fact tail | ϵ");
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
		outputTokens.push_back("Consuming " + tokens.at(globalcount));
		globalcount++;
		return;
	}
	else
	{
		cout << "Error at addop" << endl;
		outputTokens.push_back("Error at addop");
		errorFlag = true;
	}
	// plus or minus
}
void multiop() {
	if (tokens.at(globalcount) == opMulti || tokens.at(globalcount) == opDiv)
	{
		cout << "Consuming " << tokens.at(globalcount) << endl;
		outputTokens.push_back("Consuming " + tokens.at(globalcount));
		globalcount++;
		return;

	}
	else {
		cout << "Error at multiop" << endl;
		outputTokens.push_back("Error at multiop");
		errorFlag = true;
	}
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
		ofstream output("output.txt");
		if (!output) {
			cout << "Error opening output file." << endl;
			return 1;
		}
		//output << "Output Tokens:" << endl;
		for (const string& token : outputTokens) {
			output << token << endl;
		}

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