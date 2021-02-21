//#include "CLexicalAnalyzer.h"
#include <list>
#include "CToken.h"
#include "CLexicalAnalyzer.h"
#include <fstream>
#include <iostream>
#include <map>

using namespace std;


//классы исключений для лексического, синтаксического
//при считывании чисел считывать строку и конвертить в число
//идентификаторы в токен складывать как строку
//ловить исключения во внешнем коде

int main() {
	auto lexicalAnalyzer = CLexicalAnalyzer("pascal.txt");
	CToken* token;
	
	while ((token = lexicalAnalyzer.GetNextToken()) != nullptr)
	{
		CTokenPtr c(token);
		string str = c->ToString();
		cout << str << endl;
	}
	//char c;
	//while((c = lexicalAnalyzer.GetNextChar()) != EOF)
	//{
	//	cout << "\'" << c << "\'" << endl;
	//}
}