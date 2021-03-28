#pragma once
#include "CVariant.h"
#include <map>

namespace types
{
	enum class EType 
	{
		Boolean,
		Integer,
		Real,
		Char,
		Record
	};

	class CType {
	public:
		EType type;
		CType(EType type);
	};
	typedef std::unique_ptr<CType> CTypePtr;

	class CRecordType : public CType {
	public:
		CRecordType(EType type, string name, map<string, CType*> identifiersMap);
		string name;
		map<string, CType*> identifiersMap;
	};
}

