#pragma once

#include <fstream>
#include "main.hpp"
#include "wmessage_state.hpp"
#include "mezun_json.hpp"
#include <memory>

namespace mezun
{
	void loadJSON
	(
		const std::string& file_path,
		void ( *f )( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& ),
		std::u32string file_load_error_message,
		std::u32string json_load_error_message,
		void ( *file_load_error_function )(),
		void ( *json_load_error_function )()
	)
	{
		std::ifstream ifs( file_path );
		if ( ifs.is_open() )
		{
			rapidjson::IStreamWrapper ifs_wrap( ifs );
			rapidjson::Document document;
			document.ParseStream( ifs_wrap );
			if ( document.IsObject() )
			{
				const auto document_object = document.GetObject();
				f( document_object );
			}
			else
			{
				if ( json_load_error_message != U"" )
				{
					Main::pushState( WMessageState::generateErrorMessage( json_load_error_message, WMessageState::Type::POP, nullptr ) );
					if ( json_load_error_function != nullptr )
					{
						json_load_error_function();
					}
				}
			}
			ifs.close();
		}
		else
		{
			if ( file_load_error_message != U"" )
			{
				Main::pushState( WMessageState::generateErrorMessage( file_load_error_message, WMessageState::Type::POP, nullptr ) );
				if ( file_load_error_function != nullptr )
				{
					file_load_error_function();
				}
			}
		}
	};

	int JSONTryInt( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& object, const char* name, int fallback )
	{
		return ( object.HasMember( name ) && object[ name ].IsInt() ) ? object[ name ].GetInt() : fallback;
	};

	int JSONTryBool( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& object, const char* name, bool fallback )
	{
		return ( object.HasMember( name ) && object[ name ].IsBool() ) ? object[ name ].GetBool() : fallback;
	};
}
