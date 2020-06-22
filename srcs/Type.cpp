#include "Type.hpp"

Type::Type(eType type/*, std::string name*/) :/* name(name),*/ _type(type)
{

}

Type::~Type() {}


eType Type::GetType() const { return _type; }
