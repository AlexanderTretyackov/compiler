#include "CSyntaxAnalyzer.h"
#include "CLexicalAnalyzer.h"
#include "CException.h"

CSyntaxAnalyzer::CSyntaxAnalyzer(string fileName)
{
	lexicalAnalyzer = new CLexicalAnalyzer(fileName);
}

void CSyntaxAnalyzer::Accept(CToken* targetToken)
{
	CTokenPtr targetTokenPtr = CTokenPtr(targetToken);
	if (targetToken == nullptr ||
		currentTokenPtr->type != targetToken->type ||
		currentTokenPtr->type == Operator && currentTokenPtr->_operator != targetToken->_operator)
	{
		throw SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(), "Expected other type token");
	}
	NextToken();
}

void CSyntaxAnalyzer::NextToken()
{
	//ссчитываем новый токен
	auto nextToken = lexicalAnalyzer->GetNextToken();
	//очищаем память из под текущего токена, записываем туда новый
	currentTokenPtr.reset(nextToken);
}