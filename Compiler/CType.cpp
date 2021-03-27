#include "CType.h"

namespace types
{
	CType::CType(EType type)
	{
		this->type = type;
	}

	CRecordType::CRecordType(EType type, string name, map<string, CType*> identifiersMap) : CType(type)
	{
		this->identifiersMap = identifiersMap;
		this->name = name;
	}
}
