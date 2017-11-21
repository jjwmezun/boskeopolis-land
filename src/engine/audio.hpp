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
		CANCEL
	};

	void init();
	void close();

	void changeSong( const std::string& song_name );
	void turnOffSong();
	void pauseSong();
	void resumeSong();
	void closeSong();
	void playSound( SoundType sound );
};