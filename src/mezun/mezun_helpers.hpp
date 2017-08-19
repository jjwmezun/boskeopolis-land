#pragma once
#include <dirent.h>
#include "mezun_math.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <unordered_map>

#define lengthOfCArray( list ) sizeof( list ) / sizeof( list[ 0 ] )

namespace mezun
{
	template<typename T>
	inline bool inVector( const std::vector<T>& haystack, T needle )
	{
		return std::find( haystack.begin(), haystack.end(), needle ) != haystack.end();
	};

	// For multidimensional arrays only.
	// To make this work, pass haystack[ 0 ][ 0 ].
	// Note that if any string ends prematurely & they've matched so far, then it counts as a match, e'en if other string continues differently.
	// Example: "flavor" == "flavortown".
	inline int findIndexOfCStringInCArray( const char* needle, const char* haystack, int haystack_size, int name_limit )
	{
		// Loop o'er strings.
		for ( int name_index = 0; name_index < haystack_size; ++name_index )
		{
			bool match = true;
			// Loop o'er letters.
			for ( int letter_index = 0; letter_index < name_limit; ++letter_index )
			{
				// If reached terminater char, stop loop prematurely.
				if ( needle[ letter_index ] == '\0' || haystack[ ( name_index * name_limit ) + letter_index ] == '\0' )
				{
					break;
				}
				else if ( needle[ letter_index ] != haystack[ ( name_index * name_limit ) + letter_index ] )
				{
					match = false;
					break; // Confirmed mismatch; don't bother with rest o' loop.
				}
			}
			if ( match ) // No letter mismatches through whole strings; must be total match.
			{
				return name_index;
			}
		}
		// Default
		return 0;
	};

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
	V findInMap( const std::unordered_map<K, V>& container, const K& item )
	{
		const auto& search = container.find( item );
		assert( search != container.end() );
		return search->second;
	};
	
	template<typename K, typename V>
	bool notInMap( const std::unordered_map<K, V>& container, const K& item )
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