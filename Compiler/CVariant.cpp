#include "CVariant.h"

#pragma region CVariant
CVariant::CVariant(EVariantType variantType)
{
	type = variantType;
}

string CVariant::ToString() {
	return "base";
}
#pragma endregion

#pragma region CIntegerVariant
CIntegerVariant::CIntegerVariant(int _value) : CVariant(Integer)
{
	value = _value;
}

string CIntegerVariant::ToString()
{
	return to_string(value);
}
#pragma endregion

#pragma region CStringVariant
CStringVariant::CStringVariant(string _value) : CVariant(String)
{
	value = _value;
}

string CStringVariant::ToString()
{
	return value;
}
#pragma endregion

#pragma region CCharVariant
CCharVariant::CCharVariant(char _value) : CVariant(Char)
{
	value = _value;
}

string CCharVariant::ToString()
{
	return string(1, value);
}
#pragma endregion

#pragma region CRealVariant
CRealVariant::CRealVariant(double _value) : CVariant(Real)
{
	value = _value;
}

string CRealVariant::ToString()
{
	return to_string(value);
}
#pragma endregion
