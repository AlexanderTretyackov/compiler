#include "CType.h"

CType::CType(EVariantType type)
{
	this->type = type;
}

CRecordType::CRecordType(EVariantType type, string name, map<string, CType> identifiersMap) : CType(type)
{
	this->identifiersMap = identifiersMap;
	this->name = name;
}
