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
}
