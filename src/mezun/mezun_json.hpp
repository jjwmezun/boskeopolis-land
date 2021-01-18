#pragma once

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <string>

namespace mezun
{
	void loadJSON
	(
		const std::string& file_path,
		void ( *f )( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& ),
		std::u32string file_load_error_message = U"",
		std::u32string json_load_error_message = U"",
		void ( *file_load_error_function )() = nullptr,
		void ( *json_load_error_function )() = nullptr
	);
	int JSONTryInt( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& object, const char* name, int fallback );
	int JSONTryBool( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& object, const char* name, bool fallback );
}
