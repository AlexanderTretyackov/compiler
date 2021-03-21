#pragma once
#include "CVariant.h"
#include <map>

class CType {
public:
	EVariantType type; 
	CType(EVariantType type);
};
typedef std::unique_ptr<CType> CTypePtr;

class CRecordType : CType {
public:
	CRecordType(EVariantType type, map<string, CType> identifiersMap);
	map<string, CType> identifiersMap;
};

