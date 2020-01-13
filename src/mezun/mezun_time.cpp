#include <chrono>
#include <ctime>
#include "mezun_time.hpp"

namespace mezun
{
    std::string getCurrentYear()
    {
        // Courtesy o’ sehe @ https://stackoverflow.com/questions/8343676/how-to-get-current-date-and-time
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t( now );
        struct tm *parts = std::localtime( &now_c );
        return std::to_string( 1900 + parts->tm_year );
    };
}