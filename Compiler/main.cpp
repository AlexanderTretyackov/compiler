//#include "CLexicalAnalyzer.h"
#include <list>
#include "CToken.h"
#include "CLexicalAnalyzer.h"
#include "CException.h"
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
	CToken* token = nullptr;
	do
	{
		CTokenPtr t;
		try {
			
			token = lexicalAnalyzer.GetNextToken();
			t.reset(token);		
			string str = t->ToString();
			cout << str << endl;
			if (token->type == Eof)
			{
				return 0;
			}
		}
		catch (LexicalException& e)
		{
			cout << e.ToString() << '\n';
			auto targetToken = new CToken(Operator, semicolon);
			//если при пропуске дошли до конца файла
			if (!lexicalAnalyzer.SkipToToken(targetToken))
				return 0;
		}
	} while(true);
	//char c;
	//while((c = lexicalAnalyzer.GetNextChar()) != EOF)
	//{
	//	cout << "\'" << c << "\'" << endl;
	//}
}