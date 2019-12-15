#include "config.hpp"
#include <fstream>
#include "main.hpp"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

namespace Config
{
    std::string getConfigPath();

    rapidjson::Document readData()
    {
		rapidjson::Document document;
		std::ifstream ifs( getConfigPath() );
		if ( ifs.is_open() )
		{
			rapidjson::IStreamWrapper ifs_wrap( ifs );
			document.ParseStream( ifs_wrap );
			ifs.close();
		}
		else
		{
			document.SetObject();
		}
		return document;
    };

    void saveData( const rapidjson::Document& document )
    {
		std::ofstream ofs( getConfigPath() );
		rapidjson::StringBuffer buffer;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer( buffer );
		document.Accept( writer );
		ofs << buffer.GetString();
		ofs.close();
    };

	std::string getLanguage()
	{
		rapidjson::Document document = readData();
		if ( document.HasMember( "language" ) && document[ "language" ].IsString() )
		{
			return document[ "language" ].GetString();
		}
		return "";
	};

    std::string getConfigPath()
    {
        return Main::resourcePath() + "config.json";
    }
}