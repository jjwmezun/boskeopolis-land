#include "game.h"
#include <SDL2/SDL.h>

int main( int argc, char* argv[] )
{
    std::vector<std::string> args;

    for ( int i = 0; i < argc; ++i )
    {
        args.push_back( argv[ i ] );
    }

    Game game( args );

    // If game starts without running, there was an error.
    if ( !game.running() )
    {
		game.destruct();
        return 1;
    }

    while ( game.running() )
    {
        game.execute();
    }

	game.destruct();
    return 0;
};
