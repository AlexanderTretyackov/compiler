#include "CType.h"

CType::CType(EVariantType type)
{
	this->type = type;
}

CRecordType::CRecordType(EVariantType type, map<string, CType> identifiersMap) : CType(type)
{
	this->identifiersMap = identifiersMap;
}
