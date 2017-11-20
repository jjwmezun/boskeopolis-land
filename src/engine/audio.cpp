#include "audio.hpp"
#include "main.hpp"
#include <SDL2/SDL_mixer.h>

namespace Audio
{
	const std::string RELATIVE_DIR = "music";
	const std::string EXT = "mp3";
	std::string current_song_ = "";
	Mix_Music* music_ = nullptr;

	void init()
	{
		if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
		{
			printf( "SDL_mixer couldn't initialize. SDL_mixer Error: %s\n", Mix_GetError() );
			exit( -1 );
		}
	};

	void changeSong( const std::string& song_name )
	{
		if ( song_name != current_song_ )
		{
			closeSong();

			if ( song_name != "" )
			{
				const std::string song_file_path = Main::resourcePath() + RELATIVE_DIR + Main::pathDivider() + song_name + "." + EXT;
				const char* song_file_path_raw = song_file_path.c_str();
				music_ = Mix_LoadMUS( song_file_path_raw );
				if ( music_ == nullptr )
				{
					printf( "Failed to load music. SDL_mixer Error: %s\n", Mix_GetError() );
				}
				else
				{
					Mix_PlayMusic( music_, -1 );
					current_song_ = song_name;
				}
			}
		}
	};

	void turnOffSong()
	{
		Mix_HaltMusic();
	};
	
	void pauseSong()
	{
		Mix_PauseMusic();
	};

	void resumeSong()
	{
		if( Mix_PlayingMusic() == 0 )
		{
			Mix_PlayMusic( music_, -1 );
		}
		else
		{
			Mix_ResumeMusic();
		}
	};

	void closeSong()
	{
		if ( music_ != nullptr )
		{
			Mix_HaltMusic();
			Mix_FreeMusic( music_ );
			music_ = nullptr;
		}
		current_song_ = "";
	};

	void close()
	{
		closeSong();
		Mix_Quit();
	};
};