
// DIRECTIONS
//===================================

#ifndef DIRECTION_H
#define DIRECTION_H

#include "mezun_math.h"


// CLASS
//===================================

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
            __NULL,
            UP,
            RIGHT,
            DOWN,
            LEFT
        };
		static constexpr int SIMPLE_SIZE = (int)Simple::LEFT + 1;

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
                    return Type::HORIZONTAL;
                break;

                case ( Simple::UP ):
                case ( Simple::DOWN ):
                    return Type::VERTICAL;
                break;

                default:
                    return Type::__NULL;
                break;
            }
        };

        inline Horizontal randomHorizontal( bool allow_null = true )
        {
            if ( allow_null )
            {
                return (Horizontal)MezunMath::randInt( 2, 0 );
            }
            else
            {
                return (Horizontal)MezunMath::randInt( 2, 1 );
            }
        };

        inline Simple randomSimple( bool allow_null = true )
        {
            if ( allow_null )
            {
                return ( Simple )MezunMath::randInt( 4, 0 );
            }
            else
            {
                return ( Simple )MezunMath::randInt( 4, 1 );
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

    }

#endif // DIRECTION_H

