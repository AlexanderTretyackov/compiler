#pragma once
#include <string>

enum EVariantType
{
	Integer,
	String,
	Char,
	Float,
};

using namespace std;

class CVariant
{
	EVariantType type;
	CVariant(EVariantType variantType);
	virtual string ToString();
};

class CIntegerVariant : CVariant
{
	CIntegerVariant(int value);
	virtual string ToString() override;
};

