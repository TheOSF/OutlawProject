
#include <sstream>


std::ostream& operator << (std::ostream& out , Vector3 const& v)
{
     return out << "( " << v.x << ", " << v.y << ", " << v.z << " )";
}

std::wostream& operator << (std::wostream& out , Vector3 const& v)
{
     return out << L"( " << v.x << L", " << v.y << L", " << v.z << L" )";
}



