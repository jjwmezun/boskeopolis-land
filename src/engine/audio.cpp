#include <iostream>
#include "audio.hpp"
#include "main.hpp"
#include <SDL2/SDL_mixer.h>

namespace Audio
{
	static constexpr int NUM_O_SOUNDS = 13;
	const std::string RELATIVE_DIR = "music";
	const std::string EXT = "mp3";
	std::string current_song_ = "";
	Mix_Music* music_ = nullptr;
	Mix_Chunk* sounds_[ NUM_O_SOUNDS ];

	Mix_Chunk* loadSound( const std::string& sound_name );

	void init()
	{
		if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
		{
			printf( "SDL_mixer couldn't initialize. SDL_mixer Error: %s\n", Mix_GetError() );
			exit( -1 );
		}
		Mix_VolumeMusic( MIX_MAX_VOLUME / 3 );
		Mix_Volume( -1, MIX_MAX_VOLUME / 2 );
		sounds_[ ( int )( SoundType::GEM ) ] = loadSound( "gem.wav" );
		sounds_[ ( int )( SoundType::HURT ) ] = loadSound( "hurt.wav" );
		sounds_[ ( int )( SoundType::HEAL ) ] = loadSound( "heal.wav" );
		sounds_[ ( int )( SoundType::JUMP ) ] = loadSound( "jump.wav" );
		sounds_[ ( int )( SoundType::BOP ) ] = loadSound( "bop.wav" );
		sounds_[ ( int )( SoundType::DEATH ) ] = loadSound( "death.wav" );
		sounds_[ ( int )( SoundType::PAUSE ) ] = loadSound( "pause.wav" );
		sounds_[ ( int )( SoundType::SELECT ) ] = loadSound( "select.wav" );
		sounds_[ ( int )( SoundType::CONFIRM ) ] = loadSound( "confirm.wav" );
		sounds_[ ( int )( SoundType::CANCEL ) ] = loadSound( "cancel.wav" );
		sounds_[ ( int )( SoundType::DIAMOND ) ] = loadSound( "diamond.wav" );
		sounds_[ ( int )( SoundType::ITEM ) ] = loadSound( "item.wav" );
		sounds_[ ( int )( SoundType::JINGLE ) ] = loadSound( "jingle.wav" );
	};

	void changeSong( const std::string& song_name, bool loop )
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
					Mix_PlayMusic( music_, ( loop ) ? -1 : 1 );
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
		for ( int i = 0; i < NUM_O_SOUNDS; ++i )
		{
			Mix_FreeChunk( sounds_[ i ] );
		}
		closeSong();
		Mix_Quit();
	};

	Mix_Chunk* loadSound( const std::string& sound_name )
	{
		const std::string file_path = Main::resourcePath() + RELATIVE_DIR + Main::pathDivider() + sound_name;
		const char* file_path_raw = file_path.c_str();

		Mix_Chunk* store = Mix_LoadWAV( file_path_raw );
		if( store == nullptr )
		{
			printf( std::string( "Failed to load sound \"" + sound_name + "\". SDL_mixer Error: %s\n" ).c_str(), Mix_GetError() );
			printf( "Full path: %s\n", file_path_raw );
		}
		return store;
	};
	
	void playSound( SoundType sound )
	{
		Mix_PlayChannel( -1, sounds_[ ( int )( sound ) ], 0 );
	};
};