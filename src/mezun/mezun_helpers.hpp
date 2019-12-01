#pragma once

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

	std::string addLine( const std::string& string, int indent_level = 0 );

	template<typename T>
	T randomListItem( const std::vector<T>& list )
	{
		const int r = mezun::randInt( list.size() - 1, 0 );
		return list.at( r );
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

	inline bool areCStringsEqual( const char* one, const char* two )
	{
		return strcmp( one, two ) == 0;
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

	bool isCStringOnlyNumeric( const char* c_string );
	char32_t copyCharToChar32( const char* source );
	std::u32string charToChar32String( const char* source );
};
