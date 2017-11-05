#pragma once

class Object;
class Collision;
class Sprite;

#include "unit.hpp"

class SpriteMovement
{
	public:
		enum class Type
		{
			GROUNDED,
			FLOATING,
			FLUTTERING,
			SWIMMING
		};

		SpriteMovement( Type type = Type::FLOATING );
		Type type() const;
		bool hasType( Type type ) const;

		void moveLeft( Sprite& sprite ) const;
		void moveRight( Sprite& sprite ) const;
		virtual void moveUp( Sprite& sprite ) const;
		virtual void moveDown( Sprite& sprite ) const;
		virtual void jump( Sprite& sprite ) const;
		virtual void bounce( Sprite& sprite, int amount ) const;
		virtual void position( Sprite& sprite ) const;
		void collideStopXLeft( Sprite& sprite, int overlap ) const;
		void collideStopXRight( Sprite& sprite, int overlap ) const;
		virtual void collideStopYBottom( Sprite& sprite, int overlap ) const;
		virtual void collideStopYTop( Sprite& sprite, int overlap ) const;
		virtual void collideStopAny( Sprite& sprite, const Collision& collision ) const;
		virtual const Collision testCollision( const Sprite& me, const Object& them ) const;

	protected:
		const Type type_;

	private:
		static constexpr int SMOOTH_MOVEMENT_PADDING = Unit::PixelsToSubPixels( 4 );
};