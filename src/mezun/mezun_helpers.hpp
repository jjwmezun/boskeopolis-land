#pragma once

#include <dirent.h>
#include "mezun_math.hpp"
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
	std::u32string intToChar32String( int n );
	std::u32string intToChar32StringWithPadding( int n, int padding );
	std::string string32ToString8( const std::u32string source );
	std::string stringReplace( const std::string& in, std::string from, std::string to );
	std::u32string removeEndingZeroFrom32String( const std::u32string& string );
	std::u32string merge32Strings( const std::u32string& first, const std::u32string& second );
	bool stringStartsWith( const std::string& haystack, const std::string& needle );
	bool stringEndsWith( const std::string& haystack, const std::string& needle );
	
	template<typename T>
	T stringReplace( T target, const T& from, const T& to )
	{
		const size_t start_position = target.find( from );
		if ( start_position == T::npos )
		{
			return target;
		}
		target.replace( start_position, from.length(), to );
		return target;
	};

	std::u32string stringReplace( std::u32string target, const std::u32string& from, const std::u32string& to );

	template<typename T>
	T implode( const std::vector<T>& list, const T& replace )
	{
		T out;
		if ( !list.empty() )
		{
			out = list[ 0 ];
			for ( int i = 1; i < list.size(); ++i )
			{
				out += replace + list[ i ];
			}
		}
		return out;
	};

	template<typename T, typename U, typename V>
	const V& findInMap( const T& list, const U& name, const V& fallback )
	{
		const auto& search = list.find( name );
		return ( search == list.end() )
			? fallback
			: search->second;
	};
};
