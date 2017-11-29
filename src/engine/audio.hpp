#include <string>

namespace Audio
{
	enum class SoundType
	{
		GEM = 0,
		HURT = 1,
		JUMP = 2,
		HEAL = 3,
		BOP = 4,
		DEATH = 5,
		PAUSE = 6,
		SELECT = 7,
		CONFIRM = 8,
		CANCEL = 9,
		DIAMOND = 10,
		ITEM = 11,
		JINGLE = 12,
		BOUNCE = 13
	};

	void init();
	void close();

	void changeSong( const std::string& song_name, bool loop = true, int quietness = 4 );
	void turnOffSong();
	void pauseSong();
	void resumeSong();
	void closeSong();
	void playSound( SoundType sound );
};