#pragma once

class Camera;
class LevelState;
class Sprite;

class MapLayer
{
	public:
		constexpr MapLayer() {};
		virtual ~MapLayer();
		virtual void update( LevelState& level_state );
		virtual void render( const Camera& camera ) = 0;
		virtual void interact( Sprite& sprite, LevelState& level_state );
};
