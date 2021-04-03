#include "CGenerator.h"
#include <string>
#include <stdio.h>
using namespace std;

const string stringsForTypes[4] =
{
	"bool",
	"int32",
	"float32",
	"char"
};

void CGenerator::WriteDefaultInfo()
{
	if (isGenerationCancelled)
		return;
	outputIlFile << ".assembly CGenerator {}" << endl <<
		".method static void main()" << endl <<
		"{" << endl <<
		".entrypoint" << endl <<
		".maxstack 10" << endl;
}

void CGenerator::WriteOperation(EOperator operation)
{
	if (isGenerationCancelled)
		return;
	switch (operation)
	{
		case star: outputIlFile << "mul" << endl; break;
		case compiler::plus :  outputIlFile << "add" << endl; break;
		case compiler::minus:  outputIlFile << "sub" << endl; break;
		case compiler::_div:  outputIlFile << "div" << endl; break;
		case compiler::_mod:  outputIlFile << "rem" << endl; break;

		case compiler::later:  outputIlFile << "clt" << endl; break; // <
		case compiler::greater:  outputIlFile << "cgt" << endl; break; // >
		case compiler::laterequal:  outputIlFile << "cgt" << endl << "ldc.i4.0" << endl << "ceq" << endl; break; // <=
		case compiler::greaterequal:  outputIlFile << "clt" << endl << "ldc.i4.0" << endl << "ceq" << endl; break; // >=
		case compiler::latergreater:  outputIlFile << "ceq" << endl << "ldc.i4.0" << endl << "ceq" << endl; break; // <>
			
		//default:
			//exit(0);
	}
}

void CGenerator::CancelGeneration()
{
	isGenerationCancelled = true;
}

void CGenerator::WriteMarkIfStart()
{
	if (isGenerationCancelled)
		return;
	//создаем новую метку для перехода через блок then
	auto mark = "_IF" + to_string(countMarks++);
	//добавляем в стек меток новую метку
	marksIf.push(mark);
	outputIlFile << "brfalse " << mark << endl;
}

void CGenerator::WriteMarkElseStart()
{
	if (isGenerationCancelled)
		return;
	//создаем новую метку на конeц оператора if
	auto mark = "_IF" + to_string(countMarks++);
	//пишем команду безусловного перехода на метку конца оператора if
	outputIlFile << "br " << mark << endl;
	//ставим метку начала ветки else
	outputIlFile << marksIf.top() << " : ";
	//убираем из стека метку начала ветки else
	marksIf.pop();
	//добавляем в стек новую метку для конца оператора if
	marksIf.push(mark);
}

void CGenerator::WriteMarkIfEnd()
{
	if (isGenerationCancelled)
		return;
	//ставим метку конца оператора if
	outputIlFile << marksIf.top() << " : ";
	//убираем из стека конца оператора if
	marksIf.pop();
}

void CGenerator::WriteMarkWhileStart()
{
	if (isGenerationCancelled)
		return;
	//создаем новую метку начала оператора while
	auto mark = "_WHILE" + to_string(countMarks++);
	outputIlFile << mark << " : " << endl;
	marksWhileStart.push(mark);
}

void CGenerator::WriteMarkWhileBodyStart()
{
	if (isGenerationCancelled)
		return;
	auto mark = "_WHILE" + to_string(countMarks++);
	outputIlFile << "brfalse " << mark << endl;
	marksWhileEnd.push(mark);
}

void CGenerator::WriteMarkWhileEnd()
{
	if (isGenerationCancelled)
		return;
	auto markWhileStart = marksWhileStart.top();
	//делаем безусловный переход на начало цикла while (на проверку)
	outputIlFile << "br " << markWhileStart << endl;
	marksWhileStart.pop();

	auto markWhileEnd = marksWhileEnd.top();
	//пишем метку конца while
	outputIlFile << markWhileEnd << " : " << endl;
	marksWhileEnd.pop();
}

void CGenerator::WriteAssign(string variableName)
{
	if (isGenerationCancelled)
		return;
	outputIlFile << "stloc " << variableName << endl;
}

void CGenerator::FinishWrite()
{
	if (isGenerationCancelled)
		return;
	outputIlFile << "ldloc x" << endl <<
		"box int32" << endl <<
		"call void[mscorlib]System.Console::WriteLine(object)" << endl <<
		"ret" << endl <<
		"}";
}

void CGenerator::WriteConst(EVariantType vairantType, string variantString)
{
	if (isGenerationCancelled)
		return;
	switch (vairantType)
	{
		case EVariantType::Integer : outputIlFile << "ldc.i4 "; break;
		case EVariantType::Real: outputIlFile << "ldc.r4 "; break;
		//default:
			//exit(0);
	}
	outputIlFile << variantString << endl;
}

void CGenerator::WriteVariableValueToStack(string variableName)
{
	if (isGenerationCancelled)
		return;
	outputIlFile << "ldloc " << variableName << endl;
	
}

CGenerator::CGenerator(string outputIlFileName) {
	if (isGenerationCancelled)
		return;
	outputIlFile.open(outputIlFileName, ios::out);
	WriteDefaultInfo();
}

void CGenerator::DefineVariable(EType type, string variableName)
{
	if (isGenerationCancelled)
		return;
	outputIlFile << ".locals init(" << stringsForTypes[(int)type] << " " << variableName << ")\n";
}
