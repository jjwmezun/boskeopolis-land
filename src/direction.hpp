#pragma once

#include "mezun_math.hpp"
#include <string>

namespace Direction
{
	enum class Type
	{
		__NULL,
		HORIZONTAL,
		VERTICAL,
		SIMPLE
	};

	enum class Horizontal
	{
		__NULL,
		LEFT,
		RIGHT
	};

	enum class Vertical
	{
		__NULL,
		UP,
		DOWN
	};

	enum class Simple
	{
		__NULL = 0,
		UP     = 1,
		RIGHT  = 2,
		DOWN   = 3,
		LEFT   = 4
	};
	static constexpr int SIMPLE_SIZE = ( int )Simple::LEFT + 1;
	std::string simpleString( Simple dir );

	enum class Rotation
	{
		__NULL,
		UP,
		UP_RIGHT,
		RIGHT,
		DOWN_RIGHT,
		DOWN,
		DOWN_LEFT,
		LEFT,
		UP_LEFT
	};
	std::string rotationString( Rotation r );

	enum class Clockwise
	{
		__NULL,
		CLOCKWISE,
		COUNTERCLOCKWISE
	};

	enum class Justify
	{
		AUTO,
		LEFT,
		RIGHT,
		CENTER,
		JUSTIFY
	};


	constexpr Simple horizontalToSimple( Horizontal d )
	{
		switch ( d )
		{
			case ( Horizontal::LEFT ):
				return Simple::LEFT;
			break;
			case ( Horizontal::RIGHT ):
				return Simple::RIGHT;
			break;
			default:
				return Simple::__NULL;
		}
	};

	constexpr Simple verticalToSimple( Vertical d )
	{
		switch ( d )
		{
			case ( Vertical::UP ):
				return Simple::UP;
			break;
			case ( Vertical::DOWN ):
				return Simple::DOWN;
			break;
			default:
				return Simple::__NULL;
		}
	};

	constexpr Horizontal simpleToHorizontal( Simple d )
	{
		switch ( d )
		{
			case ( Simple::LEFT ):
				return Horizontal::LEFT;
			break;
			case ( Simple::RIGHT ):
				return Horizontal::RIGHT;
			break;
			default:
				return Horizontal::__NULL;
		}
	};

	constexpr Vertical simpleToVertical( Simple d )
	{
		switch ( d )
		{
			case ( Simple::UP ):
				return Vertical::UP;
			break;
			case ( Simple::DOWN ):
				return Vertical::DOWN;
			break;
			default:
				return Vertical::__NULL;
		}
	};

	constexpr Horizontal switchHorizontal( Horizontal d )
	{
		switch ( d )
		{
			case ( Horizontal::LEFT ):
				return Horizontal::RIGHT;
			break;
			case ( Horizontal::RIGHT ):
				return Horizontal::LEFT;
			break;
			default:
				return Horizontal::__NULL;
		}
	};

	constexpr Vertical switchVertical( Vertical d )
	{
		switch ( d )
		{
			case ( Vertical::UP ):
				return Vertical::DOWN;
			break;
			case ( Vertical::DOWN ):
				return Vertical::UP;
			break;
			default:
				return Vertical::__NULL;
		}
	};

	constexpr Type SimpleIsOfWhatType( Simple d )
	{
		switch ( d )
		{
			case ( Simple::LEFT ):
			case ( Simple::RIGHT ):
			{
				return Type::HORIZONTAL;
			}
			break;

			case ( Simple::UP ):
			case ( Simple::DOWN ):
			{
				return Type::VERTICAL;
			}
			break;

			default:
			{
				return Type::__NULL;
			}
			break;
		}
	};

	constexpr Simple rotateSimpleClockwise( Simple d )
	{
		switch ( d )
		{
			case ( Simple::UP ):
			{
				return Simple::RIGHT;
			}
			break;
			case ( Simple::RIGHT ):
			{
				return Simple::DOWN;
			}
			break;
			case ( Simple::DOWN ):
			{
				return Simple::LEFT;
			}
			break;
			case ( Simple::LEFT ):
			{
				return Simple::UP;
			}
			break;
			default:
			{
				return Simple::__NULL;
			}
			break;
		}
	};

	inline Horizontal randomHorizontal( bool allow_null = true )
	{
		if ( allow_null )
		{
			return static_cast<Horizontal> ( mezun::randInt( 2, 0 ) );
		}
		else
		{
			return static_cast<Horizontal> ( mezun::randInt( 2, 1 ) );
		}
	};

	inline Simple randomSimple( bool allow_null = true )
	{
		if ( allow_null )
		{
			return static_cast<Simple> ( mezun::randInt( 4, 0 ) );
		}
		else
		{
			return static_cast<Simple> ( mezun::randInt( 4, 1 ) );
		}
	};

	constexpr Simple oppositeSimple( Simple d )
	{
		switch( d )
		{
			case ( Simple::UP ):
				return Simple::DOWN;
			break;
			case ( Simple::DOWN ):
				return Simple::UP;
			break;
			case ( Simple::LEFT ):
				return Simple::RIGHT;
			break;
			case ( Simple::RIGHT ):
				return Simple::LEFT;
			break;
		}
	};

	constexpr Rotation horizontalAndVerticalToRotation( Horizontal h, Vertical v )
	{
		switch ( h )
		{
			case ( Horizontal::LEFT ):
				switch ( v )
				{
					case ( Vertical::UP ):
						return Rotation::UP_LEFT;
					break;
					case ( Vertical::DOWN ):
						return Rotation::DOWN_LEFT;
					break;
					case ( Vertical::__NULL ):
						return Rotation::LEFT;
					break;
				}
			break;
			case ( Horizontal::RIGHT ):
				switch ( v )
				{
					case ( Vertical::UP ):
						return Rotation::UP_RIGHT;
					break;
					case ( Vertical::DOWN ):
						return Rotation::DOWN_RIGHT;
					break;
					case ( Vertical::__NULL ):
						return Rotation::RIGHT;
					break;
				}
			break;
			case ( Horizontal::__NULL ):
				switch ( v )
				{
					case ( Vertical::UP ):
						return Rotation::UP;
					break;
					case ( Vertical::DOWN ):
						return Rotation::DOWN;
					break;
					case ( Vertical::__NULL ):
						return Rotation::__NULL;
					break;
				}
			break;
		}
	};
}
