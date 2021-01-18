#include "mezun_exceptions.hpp"
#include "mezun_helpers.hpp"

namespace mezun
{
	Exception::Exception( std::u32string message )
	:
		std::runtime_error( mezun::string32ToString8( message ) ),
		message_ ( message )
	{};

	const std::u32string& Exception::getMessage() const
	{
		return message_;
	};

	MissingLevel::MissingLevel( const std::string& lvname )
	:
		std::runtime_error( "Level “" + lvname + "” is missing its JSON file in the assets/levels directory.\nPlease redownload game." )
	{};

	CorruptedLevel::CorruptedLevel( const std::string& lvname )
	:
		std::runtime_error( "Level “" + lvname + "” JSON file has been corrupted in the resources/levels directory.\nPlease redownload game." )
	{};

	BrokenLevelFile::BrokenLevelFile( const std::string& lvname )
	:
		std::runtime_error( "The JSON file for level “" + lvname + "” in the resources/levels directory has become corrupted & isn't valid JSON & can't be loaded.\nPlease redownload game." )
	{};

	CantLoadLevelNames::CantLoadLevelNames()
	:
		std::runtime_error( "The resources/levels directory is missing or corrupted, & thus none o' the level names can be found.\nPlease redownload this game." )
	{};
}
