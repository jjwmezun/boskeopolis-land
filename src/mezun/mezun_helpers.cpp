#include <cctype>
#include "mezun_helpers.hpp"

namespace mezun
{
	std::string addLine( const std::string& string, int indent_level )
	{
		std::string text = "";
		for ( int i = 0; i < indent_level; ++i )
		{
			text += "\t";
		}
		return text + string + "\n";
	}

	bool isCStringOnlyNumeric( const char* c_string )
	{
		while ( c_string != nullptr )
		{
			if ( !isdigit( *c_string ) )
			{
				return false;
			}
			++c_string;
		}
		return true;
	};
}
