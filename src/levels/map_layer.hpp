#pragma once

#include "unit.hpp"

class Camera;
class LevelState;
class Sprite;

class MapLayer
{
	public:
		static constexpr Unit::Layer DEFAULT_POSITION = Unit::Layer::BG_1;
		MapLayer( Unit::Layer layer_position = DEFAULT_POSITION );
		virtual ~MapLayer();
		virtual void update( LevelState& level_state );
		virtual void render( const Camera& camera ) = 0;
		virtual void interact( Sprite& sprite, LevelState& level_state );
		Unit::Layer layer_position_;
		int renderable_id_;
};
