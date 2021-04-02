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

void CGenerator::FinishWrite()
{
	outputIlFile << "ret" << endl <<
		"}";
}

CGenerator::CGenerator(string outputIlFileName) {
	outputIlFile.open(outputIlFileName, ios::out);
	WriteDefaultInfo();
}

void CGenerator::DefineVariable(EType type, string variableName)
{
	outputIlFile << ".locals init(" << stringsForTypes[(int)type] << variableName << ")\n";
}
