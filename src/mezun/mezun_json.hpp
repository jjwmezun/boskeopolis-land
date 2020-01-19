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
		const char* file_load_error_message = nullptr,
		const char* json_load_error_message = nullptr,
		void ( *file_load_error_function )() = nullptr,
		void ( *json_load_error_function )() = nullptr
	);

	const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > > getJSONData
	(
		const std::string& file_path,
		const char* file_load_error_message = nullptr,
		const char* json_load_error_message = nullptr,
		void ( *file_load_error_function )() = nullptr,
		void ( *json_load_error_function )() = nullptr
	);
}
