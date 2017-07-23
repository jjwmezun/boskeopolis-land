#ifndef BLOCK_COMPONENT_SWITCH_H
#define BLOCK_COMPONENT_SWITCH_H

#include "block_component.hpp"
#include <memory>
#include "sprite_graphics.hpp"
#include <vector>

class BlockComponentSwitch : public BlockComponent
{
	public:
		BlockComponentSwitch
		(
			BlockComponent* off_component,
			BlockComponent* on_component,
			std::vector<SpriteGraphics*> off_graphics,
			std::vector<SpriteGraphics*> on_graphics
		);
		~BlockComponentSwitch();
		void interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera ) override;
		void update( EventSystem& events, BlockType& type ) override;


	private:
		std::unique_ptr<BlockComponent> off_component_;
		std::unique_ptr<BlockComponent> on_component_;
		std::vector<std::shared_ptr<SpriteGraphics>> on_graphics_;
		std::vector<std::shared_ptr<SpriteGraphics>> off_graphics_;
		bool first_update_ = true;

		void changeGraphics( BlockType& type, std::vector<std::shared_ptr<SpriteGraphics>>& graphics );
};

#endif // BLOCK_COMPONENT_SWITCH_H

