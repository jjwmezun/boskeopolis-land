#pragma once

#include <fstream>
#include <iostream>
#include "main.hpp"
#include "message_state.hpp"
#include "mezun_json.hpp"
#include <memory>

namespace mezun
{
	void loadJSON
	(
		const std::string& file_path,
		void ( *f )( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& ),
		const char* file_load_error_message,
		const char* json_load_error_message,
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
				if ( json_load_error_message != nullptr )
				{
					std::cout<<json_load_error_message<<std::endl;
					Main::pushState( std::unique_ptr<MessageState> ( MessageState::errorMessage( json_load_error_message ) ) );
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
			if ( file_load_error_message != nullptr )
			{
				std::cout<<file_load_error_message<<std::endl;
				Main::pushState( std::unique_ptr<MessageState> ( MessageState::errorMessage( file_load_error_message ) ) );
				if ( file_load_error_function != nullptr )
				{
					file_load_error_function();
				}
			}
		}
	};
}
