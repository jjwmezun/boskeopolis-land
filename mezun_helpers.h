
#ifndef MEZUN_HELPERS_H
#define MEZUN_HELPERS_H

#include "mezun_math.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace MezunHelpers
{
	inline std::string emptyString()
	{
		return "";
	};
	
	template<typename T>
	T randomListItem( const std::vector<T>& list )
	{
		const int r = MezunMath::randInt( list.size() - 1, 0 );
		
		try
		{
			return list.at( r );
		}
		catch ( std::out_of_range )
		{
			std::cout<<"SIZE: "<<list.size()<<std::endl;
			std::cout<<"R: "<<r<<std::endl;
		}
	};
	
	inline bool areStringsEqual( const std::string& one, const std::string& two )
	{
		return ( one.compare( two ) == 0 );
	}
	
	inline bool isStringEmpty( const std::string& text )
	{
		return areStringsEqual( text, "" );
	}
};

#endif // MEZUN_HELPERS_H
