#include "console_arguments.hpp"
#include <cstdlib>
#include <cstdio>
#include "mezun_helpers.hpp"

ConsoleArguments::ConsoleArguments( int argc, char** argv )
:
	windowed_ ( false ),
	noaudio_ ( false ),
	nosave_ ( false ),
	noharm_ ( false ),
	changepalette_ ( false ),
	magnification_ ( -1 )
{
	for ( int i = 0; i < argc; ++i )
	{
		const char* arg = argv[ i ];
		if ( mezun::areCStringsEqual( arg, "--windowed" ) )
		{
			windowed_ = true;
		}
		if ( mezun::areCStringsEqual( arg, "--noaudio" ) )
		{
			noaudio_ = true;
		}
		if ( mezun::areCStringsEqual( arg, "--nosave" ) )
		{
			nosave_ = true;
		}
		if ( mezun::areCStringsEqual( arg, "--noharm" ) )
		{
			noharm_ = true;
		}
		if ( mezun::areCStringsEqual( arg, "--changepal" ) )
		{
			changepalette_ = true;
		}
		if ( mezun::areCStringsEqual( arg, "-m" ) )
		{
			const bool is_last_arg = i == argc - 1;
			if ( is_last_arg )
			{
				printf( "Magnification argument needs a value.\n" );
			}
			else
			{
				// Continue to next arg.
				i++;
				const char* magnification_arg = argv[ i ];
				if ( !mezun::isCStringOnlyNumeric( magnification_arg ) )
				{
					printf( "Invalid magnification value for arguments: %s\n", magnification_arg );
				}
				else
				{
					const long int magnification_num = strtol( magnification_arg, nullptr, 10 );
					if ( magnification_num <= 0 || magnification_num >= 127 )
					{
						printf( "Invalid magnification value for arguments: %s\n", magnification_arg );
					}
					else
					{
						magnification_ = ( int )( magnification_num );
					}
				}
			}
		}
	}
};

bool ConsoleArguments::windowed() const
{
	return windowed_;
};

bool ConsoleArguments::noaudio() const
{
	return noaudio_;
};

bool ConsoleArguments::nosave() const
{
	return nosave_;
};

bool ConsoleArguments::noharm() const
{
	return noharm_;
};

bool ConsoleArguments::changepalette() const
{
	return changepalette_;
};

int ConsoleArguments::magnification() const
{
	return magnification_;
};
