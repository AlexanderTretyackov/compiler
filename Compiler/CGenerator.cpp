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
	outputIlFile << ".assembly CGenerator {}" << endl <<
		".method static void main()" << endl <<
		"{" << endl <<
		".entrypoint" << endl <<
		".maxstack 1" << endl;
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
		//default:
			//exit(0);
	}
}

void CGenerator::CancelGeneration()
{
	isGenerationCancelled = true;
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
	outputIlFile << "ret" << endl <<
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
	outputIlFile.open(outputIlFileName, ios::out);
	WriteDefaultInfo();
}

void CGenerator::DefineVariable(EType type, string variableName)
{
	if (isGenerationCancelled)
		return;
	outputIlFile << ".locals init(" << stringsForTypes[(int)type] << " " << variableName << ")\n";
}
