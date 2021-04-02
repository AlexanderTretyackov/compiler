#pragma once
#include <string>

#include <string>
#include <iostream>

// Для работы с умными указателями
#include <memory>

// Так же можно типизировать функцию-фабрику умных указателей:
template<typename T, typename... Params>
std::unique_ptr<T> CreatePtr(Params... parameters)
{
	return std::make_unique<T>(parameters...);
}

enum EVariantType
{
	Integer,
	String,
	Char,
	Real,
	Record,
	Boolean
};

using namespace std;

class CVariant
{
public:
	EVariantType type;
	CVariant(EVariantType variantType);
	virtual string ToString();
};
typedef std::unique_ptr<CVariant> CVariantPtr;

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

