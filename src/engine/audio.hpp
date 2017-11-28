#include <string>

namespace Audio
{
	enum class SoundType
	{
		GEM,
		HURT,
		JUMP,
		HEAL,
		BOP,
		DEATH,
		PAUSE,
		SELECT,
		CONFIRM,
		CANCEL,
		DIAMOND,
		ITEM,
		JINGLE
	};

	void init();
	void close();

	void changeSong( const std::string& song_name, bool loop = true );
	void turnOffSong();
	void pauseSong();
	void resumeSong();
	void closeSong();
	void playSound( SoundType sound );
};