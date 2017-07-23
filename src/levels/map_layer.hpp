#pragma once

class BlockSystem;
class Camera;
class EventSystem;

class Sprite;

class MapLayer
{
	public:
		MapLayer();
		virtual ~MapLayer();
		virtual void update( EventSystem& events, BlockSystem& blocks, const Camera& camera ) {};
		virtual void render( Camera& camera ) const = 0;
		virtual void interact( Sprite& sprite ) {};
};