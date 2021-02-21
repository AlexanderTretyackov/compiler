#pragma once
#include <string>

enum EVariantType
{
	Integer,
	String,
	Char,
	Real,
};

using namespace std;

class CVariant
{
public:
	EVariantType type;
	CVariant(EVariantType variantType);
	virtual string ToString();
};

class CIntegerVariant : public CVariant
{
public:
	int value;
	CIntegerVariant(int value);
	virtual string ToString() override;
};

class CStringVariant : public CVariant
{
public:
	string value;
	CStringVariant(string value);
	virtual string ToString() override;
};

class CCharVariant : public CVariant
{
public:
	char value;
	CCharVariant(char value);
	virtual string ToString() override;
};

class CRealVariant : public CVariant
{
public:
	double value;
	CRealVariant(double value);
	virtual string ToString() override;
};

