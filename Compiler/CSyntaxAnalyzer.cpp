#include "CSyntaxAnalyzer.h"
#include "CLexicalAnalyzer.h"
#include "CException.h"
#include <list>
#include <cstdlib>
using namespace types;

template<class T> inline
std::list<T> operator+(const std::list<T>& first, const std::list<T>& second)
{
	std::list<T> newList(first.cbegin(), first.cend());
	newList.insert(newList.end(), second.cbegin(), second.cend());
	return newList;
}

CSyntaxAnalyzer::CSyntaxAnalyzer(string fileName)
{
	lexicalAnalyzer = new CLexicalAnalyzer(fileName);
}

void CSyntaxAnalyzer::Analyze()
{
	NextToken();
	Program();
}

/// <summary>
/// программа
/// </summary>
void CSyntaxAnalyzer::Program()
{
	try
	{
		Accept(new CToken(Operator, _program));
		Name();
	}
	catch (CompilerException)
	{
		SkipToOperators({ semicolon, _type, _var, _begin });
	}
	try {
		Accept(new CToken(Operator, semicolon));// ;
	}
	catch (SyntaxException)
	{
		SkipToOperators({ _type, _var, _begin });
	}
	Block();
}

/// <summary>
/// блок
/// </summary>
void CSyntaxAnalyzer::Block()
{
	if (currentTokenPtr->type == Operator && currentTokenPtr->_operator == _type)
		BlockTypes();
	if(currentTokenPtr->type == Operator && currentTokenPtr->_operator == _var)
		BlockVariables();
	if (currentTokenPtr->type == Operator && currentTokenPtr->_operator == _begin)
		BlockOperators();
}

void CSyntaxAnalyzer::BlockTypes()
{
	Accept(new CToken(Operator, _type));
	
	while (currentTokenPtr->type == Identifier)
	{
		DefinitionType({ semicolon, _var, _begin });
		Accept(new CToken(Operator, semicolon));//;
	}
	
	//if (SkipToOperators(followersBlockTypes));
	//{
	//	switch (currentTokenPtr->_operator)
	//	{
	//		case _var: BlockVariables();
	//			if (currentTokenPtr->type == Operator && currentTokenPtr->_operator == _begin)
	//				BlockOperators();
	//			return;
	//		case _begin: BlockOperators(); return;
	//	}
	//	return;
	//}
}

void CSyntaxAnalyzer::DefinitionType(list<EOperator> followers)
{
	string ident;
	CType* type;
	try {
		ident = Name();
		Accept(new CToken(Operator, compiler::equal));//=
		type = Type(followers);
		//если тип с таким именем уже есть, то генерируем исключение
		if (mapTypes.count(ident) != 0)
			PrintExceptionMessage(Semantic,
				lexicalAnalyzer->GetNumberLineStartToken(), lexicalAnalyzer->GetNumberCharStartToken(),
				"Type with that name already exist");
		else
			mapTypes[ident] = type;
	}
	catch (CompilerException)
	{
		SkipToOperators(followers);
	}
}

void CSyntaxAnalyzer::BlockVariables()
{
	Accept(new CToken(Operator, _var));
	
	while (currentTokenPtr->type == Identifier)
	{
		DefinitionVariables({ semicolon, _begin });
		try {
			Accept(new CToken(Operator, semicolon));//;
		}
		catch (CompilerException)
		{
			SkipToOperators({ _begin });
		}
	}
}

void CSyntaxAnalyzer::DefinitionVariables(list<EOperator> followers)
{
	//список нужен для временного хранения идентификаторов, пока не дойдем до типа объявленных переменных
	list<string> listNewVariablesIdentifiers;
	//идентификатор новой переменной
	auto newVariableIdentifier = Name();
	//если переменная с таким идентификатором уже содержится в области видимости, то пишем ошибку
	if (mapIdentifiers.count(newVariableIdentifier) != 0)
		PrintExceptionMessage(Semantic,
			lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Identifier already exist");
	else
	{
		//записываем в временный список новый идентификатор
		listNewVariablesIdentifiers.push_back(newVariableIdentifier);
		//записываем в таблицу идентификаторов новый идентификатор пока без типа
		mapIdentifiers[newVariableIdentifier] = nullptr;
	}

	while(currentTokenPtr->type == Operator && currentTokenPtr->_operator == comma)//,
	{
		NextToken();
		newVariableIdentifier = Name();
		if (mapIdentifiers.count(newVariableIdentifier) != 0)
			PrintExceptionMessage(Semantic,
				lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Identifier already exist");
		else
		{
			//записываем в временный список новый идентификатор
			listNewVariablesIdentifiers.push_back(newVariableIdentifier);
			//записываем в таблицу идентификаторов новые идентификаторы пока без типа
			mapIdentifiers[newVariableIdentifier] = nullptr;
		}
	}
	try {
		Accept(new CToken(Operator, colon));//:
		auto typeVariables = Type(followers);////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//указываем тип у добавленных идентификаторов
		for (auto iterator = listNewVariablesIdentifiers.begin(); iterator != listNewVariablesIdentifiers.end(); iterator++)
		{
			newVariableIdentifier = *iterator;
			mapIdentifiers[newVariableIdentifier] = typeVariables;
		}
	}
	catch (CompilerException)
	{
		SkipToOperators(followers);
	}

}

CType* CSyntaxAnalyzer::Type(list<EOperator> followers)
{
	CType* type = nullptr;
	try {
		if (currentTokenPtr->type == Operator
			&& currentTokenPtr->_operator == _record)
			type = CombinedType(followers);
		else
			type = SimpleType();
	}
	catch (CompilerException)
	{
		SkipToOperators(followers);
	}
	return type;
}

CType* CSyntaxAnalyzer::SimpleType()
{
	if (currentTokenPtr->type == Identifier)
	{
		auto identifier = currentTokenPtr->identifier;
		NextToken();
		//если тип есть, то возвращаем его, иначе кидаем исключение
		if (mapTypes.count(identifier) != 0)
		{
			return mapTypes[identifier];
		}
		PrintExceptionMessage(Semantic,
			lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Type not defined");
		throw CompilerException();
	}
	PrintExceptionMessage(Syntax,
		lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
		"Expected type identifier");
	throw CompilerException();
}

CType* CSyntaxAnalyzer::CombinedType(list<EOperator> followers)
{
	Accept(new CToken(Operator, _record));
	//если тип без полей, то сразу возвращаем
	if (currentTokenPtr->type == Operator
		&& currentTokenPtr->_operator == _end)
	{
		NextToken();
		map<string, CType*> mapFields;
		return new CRecordType(EType::Record, "", mapFields);
	}
	list<EOperator> additionalFollowers = {_end};
	auto mapFields = ListFields(followers + additionalFollowers);
	Accept(new CToken(Operator, _end));
	return new CRecordType(EType::Record, "", mapFields);
}

map<string, CType*> CSyntaxAnalyzer::ListFields(list<EOperator> followers)
{
	list<EOperator> addFollowers = { semicolon };
	map<string, CType*> mapFields;
	try {

		SectionRecord(mapFields, followers);
	}
	catch (CompilerException)
	{
		
		SkipToOperators(followers + addFollowers);
	}
	while (currentTokenPtr->type == Operator
		&& currentTokenPtr->_operator == semicolon) //;
	{
		NextToken();
		SectionRecord(mapFields, followers + addFollowers);
	}
	return mapFields;
}

void CSyntaxAnalyzer::SectionRecord(map<string, CType*>& mapIdentifiersRecord, list<EOperator> followers)
{
	if (currentTokenPtr->type == Operator
		&& currentTokenPtr->_operator == _end)
		return;
	//список нужен для временного хранения полей, пока не дойдем до типа полей записи
	list<string> listFieldsRecord;

	if (currentTokenPtr->type == Identifier)
	{
		auto nameFiledRecord = NameField();
		//если такой идентификатор не объявлен
		if (mapIdentifiersRecord.count(nameFiledRecord) == 0)
		{
			//записываем в таблицу идентификаторов новый идентификатор пока без типа
			mapIdentifiersRecord[nameFiledRecord] = nullptr;
			//записываем в временный список новый идентификатор поля записи
			listFieldsRecord.push_back(nameFiledRecord);
		}
		else
			PrintExceptionMessage(Semantic,
				lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Identifier with same name already defined");
	}

	while (currentTokenPtr->type == Operator
		&& currentTokenPtr->_operator == comma)
	{
		NextToken();
		auto nameFiledRecord = NameField();
		//если такой идентификатор не объявлен
		if (mapIdentifiersRecord.count(nameFiledRecord) == 0)
		{
			//записываем в таблицу идентификаторов новый идентификатор пока без типа
			mapIdentifiersRecord[nameFiledRecord] = nullptr;
			//записываем в временный список новый идентификатор поля записи
			listFieldsRecord.push_back(nameFiledRecord);
		}
		else
			PrintExceptionMessage(Semantic,
				lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Identifier with same name already defined");
	}

	Accept(new CToken(Operator, colon)); //:

	auto typeFields = Type(followers);

	//указываем тип у добавленных идентификаторов
	for (auto iterator = listFieldsRecord.begin(); iterator != listFieldsRecord.end(); iterator++)
	{
		auto nameFieldRecord = *iterator;
		mapIdentifiersRecord[nameFieldRecord] = typeFields;
	}
}

string CSyntaxAnalyzer::NameField()
{
	auto identifier = currentTokenPtr->identifier;

	NextToken();
		
	return identifier;
}

void CSyntaxAnalyzer::BlockOperators()
{
	CompountOperator({_eof});
}

void CSyntaxAnalyzer::IfOperator(list<EOperator> followers)
{
	Accept(new CToken(Operator, _if));
	list<EOperator> additionalFollowers = { _then, _else };
	auto typeExpression = Expression(followers + additionalFollowers);
	if(typeExpression != typeBoolean && typeExpression != nullptr)
		PrintExceptionMessage(Semantic,
			lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Expected boolean expression");
	Accept(new CToken(Operator, _then));
	list<EOperator> addFollowers = { _else };
	_Operator(followers + addFollowers);
	if (currentTokenPtr->type == Operator && currentTokenPtr->_operator == _else)
	{
		NextToken();
		_Operator(followers);
	}
}

void CSyntaxAnalyzer::CaseOperator(list<EOperator> followers)
{
	Accept(new CToken(Operator, _case));
	list<EOperator> additionalFollowers = { _of };
	auto typeCaseExpression = Expression(followers + additionalFollowers);
	//case поддерживает только типы integer и char
	if (typeCaseExpression != typeInteger && typeCaseExpression != typeChar 
		&& typeCaseExpression != nullptr)
		PrintExceptionMessage(Semantic,
			lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Expected integer or char type expression");
	Accept(new CToken(Operator, _of));

	//если текущий токен константа integer или char, то разбираем как элемент списка вариантов
	if (currentTokenPtr->type == Value &&
		(currentTokenPtr->variantPtr->type == Integer ||
			currentTokenPtr->variantPtr->type == Char))
	{
		list<EOperator> addFollowers = { semicolon, _end };
		auto typeCaseListItem = CaseListItem(followers + addFollowers);
		//если тип выражения case не совпадает с типом элемента списка вариантов
		if (typeCaseExpression != nullptr && typeCaseListItem != nullptr &&
			typeCaseExpression != typeCaseListItem)
			PrintExceptionMessage(Semantic, lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Type of case expression doesn't match type label");
	}

	while(currentTokenPtr->type == Operator && 
		currentTokenPtr->_operator == semicolon)
	{
		NextToken();
		//если текущий токен константа integer или char, то разбираем как элемент списка вариантов
		if (currentTokenPtr->type == Value &&
			(currentTokenPtr->variantPtr->type == Integer ||
				currentTokenPtr->variantPtr->type == Char))
		{
			list<EOperator> addFollowers = { _end };
			auto typeCaseListItem = CaseListItem(followers + addFollowers);
			//если тип выражения case не совпадает с типом элемента списка вариантов
			if (typeCaseExpression != nullptr && typeCaseListItem != nullptr &&
				typeCaseExpression != typeCaseListItem)
				PrintExceptionMessage(Semantic, lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
					"Type of case expression doesn't match type label");
		}
	}
	try 
	{
		Accept(new CToken(Operator, _end));
	}
	catch (CompilerException)
	{
		SkipToOperators(followers);
	}
}

void CSyntaxAnalyzer::WithOperator()
{
	Accept(new CToken(Operator, _with));
	if (currentTokenPtr->type != Identifier)
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Expected record variable");

	//если такой идентификатор не объявлен, генерируем исключение
	if (mapIdentifiers.count(currentTokenPtr->identifier) == 0)
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Record variable not defined");

	auto identifierRecordType = mapIdentifiers[currentTokenPtr->identifier];
	//если идентификатор не record, генерируем исключение
	if(identifierRecordType->type != EType::Record)
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Expected record variable");
}

void CSyntaxAnalyzer::WhileOperator(list<EOperator> followers)
{
	Accept(new CToken(Operator, _while));
	list<EOperator> additionalFollowers = { _do };
	auto typeExpression = Expression(followers + additionalFollowers);

	if (typeExpression != typeBoolean && typeExpression != nullptr)
		PrintExceptionMessage(Semantic,
			lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Expected boolean expression");
	Accept(new CToken(Operator, _do));
	_Operator(followers);
}

void CSyntaxAnalyzer::CompountOperator(list<EOperator> followers)
{
	try {
		Accept(new CToken(Operator, _begin));
		list<EOperator> addFollowers = { semicolon, _end };
		_Operator(followers + addFollowers);
		while (currentTokenPtr->type == Operator && currentTokenPtr->_operator == semicolon)
		{
			NextToken();
			if (currentTokenPtr->type == Operator
				&& currentTokenPtr->_operator == _end)
				break;
			_Operator(followers);
		}
		Accept(new CToken(Operator, _end));
	}
	catch (SyntaxException ex)
	{
		SkipToOperators(followers);
	}
}

void CSyntaxAnalyzer::_Operator(list<EOperator> followers)
{
	UnlabeledOperator(followers);
}

void CSyntaxAnalyzer::UnlabeledOperator(list<EOperator> followers)
{
	if (currentTokenPtr->type == Identifier)
		SimpleOperator(followers);
	else
		ComplexOperator(followers);
}

void CSyntaxAnalyzer::SimpleOperator(list<EOperator> followers)
{
	AssignOperator(followers);
}

void CSyntaxAnalyzer::ComplexOperator(list<EOperator> followers)
{
	if (currentTokenPtr->type == Operator)
	{
		switch (currentTokenPtr->_operator)
		{
			case _begin: CompountOperator(followers); return;
			case _if: IfOperator(followers); return;
			case _case: CaseOperator(followers); return;
			case _with: WithOperator(); return;
			case _while: WhileOperator(followers); return;
			default:
					PrintExceptionMessage(Syntax, 
					lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
					"Expected operator");
					SkipToOperators(followers);
		}
	}
}

void CSyntaxAnalyzer::AssignOperator(list<EOperator> followers)
{
	try {
		list<EOperator> addFollowers = { assign };
		auto typeVariable = Variable(followers + addFollowers);
		Accept(new CToken(Operator, assign));//:=
		auto typeExpression = Expression(followers);
		if ( typeVariable != nullptr && typeExpression != nullptr &&
			typeVariable != typeExpression)
		{
			PrintExceptionMessage(Semantic, lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Types variable and expression doesn't match");
		}
	}
	catch (CompilerException)
	{
		SkipToOperators(followers);
	}
}

CType* CSyntaxAnalyzer::Variable(list<EOperator> followers)
{
	if (currentTokenPtr->type != Identifier)
	{
		PrintExceptionMessage(Syntax,
			lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Expected variable");
		throw CompilerException();
	}
	//если в таблице идентификаторов нет рассматриваемой переменной, то кидаем исключение
	if (mapIdentifiers.count(currentTokenPtr->identifier) == 0)
	{
		PrintExceptionMessage(Semantic, 
			lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Variable not defined");
		return nullptr;
	}

	try {
		if (mapIdentifiers[currentTokenPtr->identifier] == nullptr)
		{
			auto identifier = currentTokenPtr->identifier;
			NextToken();
			return nullptr;
		}
		//если тип переменной - запись
		if (mapIdentifiers[currentTokenPtr->identifier]->type == EType::Record)
			return VariableComponent();
		auto identifier = currentTokenPtr->identifier;
		NextToken();
		//возвращаем тип переменной
		return mapIdentifiers[identifier];
	}
	catch (CompilerException)
	{
		SkipToOperators(followers);
		return nullptr;
	}
}

CType* CSyntaxAnalyzer::VariableComponent()
{
	return FieldDesignation();
}

CType* CSyntaxAnalyzer::FieldDesignation()
{
	auto identifierRecord = currentTokenPtr->identifier;
	NextToken();
	Accept(new CToken(Operator, point));
	//если вместо поля идет не идентификатор
	if (currentTokenPtr->type != Identifier)
	{
		PrintExceptionMessage(Syntax, lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Expected record field");
		throw CompilerException();
	}
	//получаем тип записи
	auto recordType = (CRecordType*)mapIdentifiers[identifierRecord];
	auto identifierField = currentTokenPtr->identifier;
	//если у типа записи нет поля с нужным именем, генерируем исключение
	if (recordType->identifiersMap.count(identifierField) == 0)
	{
		PrintExceptionMessage(Semantic, 
			lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Record field not defined");
		throw CompilerException();
	}

	auto fieldType = recordType->identifiersMap[identifierField];
	NextToken();
	return fieldType;
}

CType* CSyntaxAnalyzer::Expression(list<EOperator> followers)
{
	list<EOperator> addFollowers =
	{ compiler::later , compiler::greater ,
		compiler::greaterequal, compiler::latergreater };

	auto typeExpression = SimpleExpression(followers + addFollowers);
	//если операция отношения
	if (currentTokenPtr->type == Operator &&
		(currentTokenPtr->_operator == later || //<
			currentTokenPtr->_operator == compiler::greater || //>
			currentTokenPtr->_operator == compiler::laterequal || //<=
			currentTokenPtr->_operator == compiler::greaterequal || //>=
			currentTokenPtr->_operator == compiler::latergreater)) //<>
	{
		NextToken();
		auto typeExpressionRight = SimpleExpression(followers);
		if (typeExpression != nullptr && typeExpressionRight != nullptr &&
			typeExpression != typeExpressionRight)
		{
			PrintExceptionMessage(Semantic,
				lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Expected another expression type");
			return nullptr;
		}
		//возвращаем логический тип для выражения, т.к. был оператор сравнения
		return typeBoolean;
	}
	return typeExpression;
}

CType* CSyntaxAnalyzer::SimpleExpression(list<EOperator> followers)
{
	list<EOperator> addFollowers = { compiler::plus ,  compiler::minus,  compiler::_or };
	//сохраняем тип слагаемого
	auto typeSummand = Summand(followers + addFollowers);
	while(currentTokenPtr->type == Operator && 
			(currentTokenPtr->_operator == compiler::plus || //+
			currentTokenPtr->_operator == compiler::minus || //-
			currentTokenPtr->_operator == compiler::_or)) //or
	{
		NextToken();
		auto typeSummandRight = Summand(followers);
		//если типы слагаемых не совпадают, то кидаем исключение
		if (typeSummand != nullptr && typeSummandRight != nullptr &&
			typeSummand != typeSummandRight)
		{
			PrintExceptionMessage(Semantic,
				lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Expected another type");
			return nullptr;
		}
	}
	return typeSummand;
}

CType* CSyntaxAnalyzer::Summand(list<EOperator> followers)
{
	list<EOperator> addFollowers = { compiler::star , compiler::slash , 
		compiler::_div ,compiler::_mod , compiler::_and };
	try {
		//сохраняем тип множителя
		auto typeMultiplier = Multiplier(followers + addFollowers);

		while (currentTokenPtr->type == Operator &&
			(currentTokenPtr->_operator == compiler::star || //*
				currentTokenPtr->_operator == compiler::slash || ///
				currentTokenPtr->_operator == compiler::_div ||
				currentTokenPtr->_operator == compiler::_mod ||
				currentTokenPtr->_operator == compiler::_and))
		{
			NextToken();
			auto typeMultiplierRight = Multiplier(followers);
			//если типы множителей не совпадают, то кидаем исключение
			if (typeMultiplier != nullptr && typeMultiplierRight != nullptr &&
				typeMultiplier != typeMultiplierRight)
			{
				PrintExceptionMessage(Semantic,
					lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
					"Expected another type");
				return nullptr;
			}
		}
		return typeMultiplier;
	}
	catch (CompilerException)
	{
		SkipToOperators(followers);
		return nullptr;
	}
}

CType* CSyntaxAnalyzer::Multiplier(list<EOperator> followers)
{
	if (currentTokenPtr->type == Operator)
	{
		if (currentTokenPtr->_operator == leftpar)//(
		{
			NextToken();
			list<EOperator> addFollowers = { rightpar };
			auto typeExpression = Expression(addFollowers);
			Accept(new CToken(Operator, rightpar));//)
			return typeExpression;
		}
		if (currentTokenPtr->_operator == _not)
		{
			NextToken();
			auto typeMultiplier = Expression(followers);
			if (typeMultiplier != nullptr &&
				typeMultiplier != typeBoolean)
			{
				PrintExceptionMessage(Semantic,
					lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
					"Expected boolean type multiplier");
				return nullptr;
			}
			return typeMultiplier;
		}
	}

	if (currentTokenPtr->type == Value)
	{
		if (currentTokenPtr->variantPtr->type == Char)
		{
			NextToken();
			return typeChar;
		}
		return NumberWithoutSign();
	}

	if (currentTokenPtr->type == Identifier)
		return Variable(followers);

	PrintExceptionMessage(Syntax,
		lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
		"Expected multiplier");
	throw CompilerException();
}

string CSyntaxAnalyzer::Name()
{

	if (currentTokenPtr->type == Identifier)
	{
		auto identifier = currentTokenPtr->identifier;
		NextToken();
		return identifier;
		////если такой идентификатор не объявлен
		//if (mapIdentifiers.count(identifier) == 0)
		//{
		//	NextToken();
		//	return identifier;
		//}
		//PrintExceptionMessage(Semantic,
		//	lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
		//	"Identifier with same name already defined");
		//throw CompilerException();
	}
	PrintExceptionMessage(Syntax,
		lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
		"Expected identifier");
	throw CompilerException();
}

void CSyntaxAnalyzer::FileName()
{
	Name();
}

CType* CSyntaxAnalyzer::Constant(list<EOperator> followers)
{
	try {
		if (currentTokenPtr->type == Value)
		{
			auto typeVariant = currentTokenPtr->variantPtr->type;
			NextToken();
			switch (typeVariant)
			{
			case Char: return typeChar;
			case Integer: return typeInteger;
			case Real: return typeReal;
			}

			if (currentTokenPtr->type == Operator &&
				(currentTokenPtr->_operator == compiler::plus ||
					currentTokenPtr->_operator == compiler::minus))
			{
				NextToken();
				return NumberWithoutSign();
			}

			PrintExceptionMessage(Syntax,
				lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Expected constant");
			throw CompilerException();
		}
	}
	catch (CompilerException)
	{
		SkipToOperators(followers);
	}
}

CType* CSyntaxAnalyzer::NumberWithoutSign()
{
	if (currentTokenPtr->type == Value)
	{
		auto variantType = currentTokenPtr->variantPtr->type;
		NextToken();
		if (variantType == Real)
			return typeReal;
		if (variantType == Integer)
			return typeInteger;
	}
	PrintExceptionMessage(Syntax,
		lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(), 
		"Expected NumberWithoutSign");
	throw CompilerException();
}

CType* CSyntaxAnalyzer::CaseListItem(list<EOperator> followers)
{
	list<EOperator> addFollowers = { colon };
	//список меток варианта
	auto typeLabels = CaseListLabels(followers + addFollowers);
	Accept(new CToken(Operator, colon));
	if (currentTokenPtr->type == Value)
	{
		if (currentTokenPtr->variantPtr->type != Integer &&
			currentTokenPtr->variantPtr->type != Char)
		{
			PrintExceptionMessage(Semantic,
				lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Expected different type constant");
		}
			
		return typeLabels;
	}
	_Operator(followers);
	return typeLabels;
}

CType* CSyntaxAnalyzer::CaseListLabels(list<EOperator> followers)
{
	list<EOperator> addFollowers = { comma };
	list<EOperator> followers1 = followers + addFollowers;
	auto typeCaseLabel = CaseLabel(followers1);
	while (currentTokenPtr->type == Operator &&
		currentTokenPtr->_operator == comma)
	{
		NextToken();
		auto typeCaseLabelNext = CaseLabel(followers1);
		if (typeCaseLabel != nullptr && typeCaseLabelNext != nullptr &&
			typeCaseLabel != typeCaseLabelNext)
			PrintExceptionMessage(Semantic,
				lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"case labels have different types");
	}
	return typeCaseLabel;
}

CType* CSyntaxAnalyzer::CaseLabel(list<EOperator> followers)
{
	return Constant(followers);
}

void CSyntaxAnalyzer::Accept(CToken* targetToken) throw (CompilerException)
{
	CTokenPtr targetTokenPtr = CTokenPtr(targetToken);
	if (targetTokenPtr == nullptr ||
		currentTokenPtr->type != targetTokenPtr->type ||
		currentTokenPtr->type == Operator && currentTokenPtr->_operator != targetTokenPtr->_operator)
	{
		PrintExceptionMessage(Syntax,
			lexicalAnalyzer->GetNumberLineStartToken(), lexicalAnalyzer->GetNumberCharStartToken(),
			"Expected token " + targetToken->ToString());
		throw CompilerException();
	}
	NextToken();
}

void CSyntaxAnalyzer::NextToken()
{
	CToken* nextToken = nullptr;
	while (nextToken == nullptr)
	{
		try {
			//ссчитываем новый токен
			nextToken = lexicalAnalyzer->GetNextToken();
			cout << nextToken->ToString() << endl;
			//очищаем память из под текущего токена, записываем туда новый
			currentTokenPtr.reset(nextToken);
		}
		catch (LexicalException ex) {

			cout << ex.ToString() << endl;
		}
	}
}

bool CSyntaxAnalyzer::IsBelong(EOperator findingOperator, list<EOperator> operators)
{
	for (auto iterator = operators.begin(); iterator != operators.end(); iterator++)
		if (*iterator == findingOperator)
			return true;
	return false;
}

bool CSyntaxAnalyzer::SkipToOperators(list<EOperator> operators, list<EOperator> followerOperators)
{
	bool inOperators = IsBelong(currentTokenPtr->_operator, operators);
	bool inFollowerOperators = IsBelong(currentTokenPtr->_operator, followerOperators);
	while (!inOperators && !inFollowerOperators)
	{
		try {
			NextToken();
			inOperators = IsBelong(currentTokenPtr->_operator, operators);
			inFollowerOperators = IsBelong(currentTokenPtr->_operator, followerOperators);
			//если дошли до конца файла
			if (currentTokenPtr->type == Eof)
				exit(0);
		}
		catch(LexicalException ex)
		{

		}
	}
	return inOperators;
}

bool CSyntaxAnalyzer::SkipToOperators(list<EOperator> operators)
{
	while (!IsBelong(currentTokenPtr->_operator, operators))
	{
		try {
			NextToken();	
			//если дошли до конца файла
			if (currentTokenPtr->type == Eof)
				exit(0);
		}
		catch (LexicalException ex)
		{

		}
	}
	return true;
}

void CSyntaxAnalyzer::PrintExceptionMessage(ExceptionType excType, int line, int liter, string exceptionMessage)
{
	cout << StrExceptionsTypes[excType] + ": position: " +
		to_string(line) + "," + to_string(liter) +
		".Description: " + exceptionMessage << endl;
}
