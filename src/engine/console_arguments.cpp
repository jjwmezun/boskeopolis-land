#include "console_arguments.hpp"
#include <cstdlib>
#include <iostream>
#include "mezun_helpers.hpp"

ConsoleArguments::ConsoleArguments( int argc, char** argv )
:
	windowed_ ( false ),
	noaudio_ ( false ),
	nosave_ ( false ),
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
		if ( mezun::areCStringsEqual( arg, "-m" ) )
		{
			const bool is_last_arg = i == argc - 1;
			if ( is_last_arg )
			{
				std::cout<<"Invalid magnification value for arguments.";
			}
			else
			{
				// Continue to next arg.
				i++;
				const char* magnification_arg = argv[ i ];
				if ( !mezun::isCStringOnlyNumeric( magnification_arg ) )
				{
					std::cout<<"Invalid magnification value for arguments.";
				}
				else
				{
					const long int magnification_num = strtol( magnification_arg, nullptr, 10 );
					if ( magnification_num <= 0 || magnification_num >= 127 )
					{
						std::cout<<"Invalid magnification value for arguments.";
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

int ConsoleArguments::magnification() const
{
	return magnification_;
};
