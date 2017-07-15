#pragma once

class BlockSystem;
class Camera;
class EventSystem;
class Graphics;
class Sprite;

class MapLayer
{
	public:
		MapLayer();
		virtual ~MapLayer();
		virtual void update( EventSystem& events, BlockSystem& blocks, const Camera& camera ) {};
		virtual void render( Graphics& graphics, Camera& camera ) const = 0;
		virtual void interact( Sprite& sprite ) {};
};