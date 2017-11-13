#include "direction.hpp"

namespace Direction
{
	std::string rotationString( Rotation r )
	{
		switch ( r )
		{
			case( Rotation::__NULL ):     return "NULL";
			case( Rotation::UP ):         return "UP";
			case( Rotation::UP_RIGHT ):   return "UP-RIGHT";
			case( Rotation::RIGHT ):      return "RIGHT";
			case( Rotation::DOWN_RIGHT ): return "DOWN-RIGHT";
			case( Rotation::DOWN ):       return "DOWN";
			case( Rotation::DOWN_LEFT ):  return "DOWN-LEFT";
			case( Rotation::LEFT ):       return "LEFT";
			case( Rotation::UP_LEFT ):    return "UP-LEFT";
		}
	};
};