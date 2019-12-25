#include "direction.hpp"

namespace Direction
{
	std::string simpleString( Simple dir )
	{
		switch ( dir )
		{
			case( Simple::__NULL ):     return "NULL";
			case( Simple::UP ):         return "UP";
			case( Simple::RIGHT ):      return "RIGHT";
			case( Simple::DOWN ):       return "DOWN";
			case( Simple::LEFT ):       return "LEFT";
		}
	};

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

	std::string horizontalString( Horizontal dir )
	{
		switch ( dir )
		{
			case ( Horizontal::__NULL ): return "NULL";
			case ( Horizontal::LEFT ):   return "LEFT";
			case ( Horizontal::RIGHT ):  return "RIGHT";
		}
	};
};
