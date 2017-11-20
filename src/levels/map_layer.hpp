#pragma once

class BlockSystem;
class Camera;
class EventSystem;
class Health;
class Map;
class Sprite;

class MapLayer
{
	public:
		MapLayer();
		virtual ~MapLayer();
		virtual void update( EventSystem& events, BlockSystem& blocks, const Camera& camera, Map& lvmap ) {};
		virtual void render( const Camera& camera ) = 0;
		virtual void interact( Sprite& sprite, Health& health ) {};
};