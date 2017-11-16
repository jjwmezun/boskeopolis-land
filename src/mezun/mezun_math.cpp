#include "mezun_math.hpp"

namespace mezun
{
	bool nextFrame( int frame, int interval, int duration )
	{
		for ( int i = 0; i < duration; ++i )
		{
			if ( frame % interval == i )
			{
				return true;
			}
		}
		return false;
	};
}