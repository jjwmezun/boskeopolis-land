#include "mezun_exceptions.hpp"

namespace mezun
{
	MissingLevel::MissingLevel( int level_id )
	:
		std::runtime_error( "Level #" + Text::stringifyNum( level_id ) + " is missing its JSON file in the resources/levels directory.\nPlease redownload game." )
	{};

	BrokenLevelFile::BrokenLevelFile( int level_id )
	:
		std::runtime_error( "The JSON file for level #" + Text::stringifyNum( level_id ) + " in the resources/levels directory has become corrupted & isn't valid JSON & can't be loaded.\nPlease redownload game." )
	{};

	CantLoadLevelNames::CantLoadLevelNames()
	:
		std::runtime_error( "The resources/levels directory is missing or corrupted, & thus none o' the level names can be found.\nPlease redownload this game." )
	{};
}
