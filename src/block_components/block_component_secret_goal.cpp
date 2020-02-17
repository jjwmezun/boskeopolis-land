#include "block_component_secret_goal.hpp"
#include "event_system.hpp"

BlockComponentSecretGoal::BlockComponentSecretGoal() {};

BlockComponentSecretGoal::~BlockComponentSecretGoal() {};

void BlockComponentSecretGoal::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health, BlockSystem& blocks, SpriteSystem& sprites ) const
{
	events.secretGoal();
};
