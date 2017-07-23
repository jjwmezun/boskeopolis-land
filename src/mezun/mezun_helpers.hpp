
#ifndef MEZUN_HELPERS_H
#define MEZUN_HELPERS_H

#include <dirent.h>
#include "mezun_math.hpp"
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace mezun
{
	inline std::string emptyString()
	{
		return "";
	};
	
	template<typename T>
	T randomListItem( const std::vector<T>& list )
	{
		const int r = mezun::randInt( list.size() - 1, 0 );
		
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
	
	template<typename K, typename V>
	bool notInMap( const std::map<K, V>& container, const K& item )
	{
		return container.find( item ) == container.end();
	};
	
	inline bool areStringsEqual( const std::string& one, const std::string& two )
	{
		return ( one.compare( two ) == 0 );
	}
	
	inline bool isStringEmpty( const std::string& text )
	{
		return areStringsEqual( text, "" );
	}
	
	inline bool checkDirectory( const std::string& path )
	{
		const auto cpath = path.c_str();
	
		bool test = false;
	
		DIR* dir = opendir( cpath );

		if ( dir )
		{
			test = true;
			closedir( dir );
		}
		
		return test;
	};
};

#endif // MEZUN_HELPERS_H