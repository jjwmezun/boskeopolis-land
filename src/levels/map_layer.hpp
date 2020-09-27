#pragma once

class Camera;
class LevelState;
class Sprite;

class MapLayer
{
	public:
		static constexpr int DEFAULT_POSITION = 2;
		MapLayer( int layer_position = DEFAULT_POSITION );
		virtual ~MapLayer();
		virtual void update( LevelState& level_state );
		virtual void render( const Camera& camera ) = 0;
		virtual void interact( Sprite& sprite, LevelState& level_state );
		int layer_position_;
		int renderable_id_;
};
