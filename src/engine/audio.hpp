#include <string>
#include <vector>

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
		BOUNCE = 13,
		CHEST_OPEN = 14,
		CHEST_LOCK = 15,
		LIGHTNING = 16,
		SWIM = 17,
		SEWER_HOLE = 18,
		UNLOCK = 19,
		BUMP = 20,
		SWITCH = 21,
		GHOST = 22
	};

	void init( bool noaudio );
	void close();

	void changeSong( const std::string& song_name, bool loop = true, int quietness = 4 );
	void turnOffSong();
	void pauseSong();
	void resumeSong();
	void closeSong();
	void playSound( SoundType sound );
	void fadeMusic( int milliseconds );
};
