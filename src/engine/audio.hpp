#include <string>

namespace Audio
{
	void init();
	void close();

	void changeSong( const std::string& song_name );
	void turnOffSong();
	void pauseSong();
	void resumeSong();
	void closeSong();
};