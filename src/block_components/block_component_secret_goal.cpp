#include "block_component_secret_goal.hpp"
#include "event_system.hpp"

BlockComponentSecretGoal::BlockComponentSecretGoal() {};

BlockComponentSecretGoal::~BlockComponentSecretGoal() {};

void BlockComponentSecretGoal::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	events.secretGoal();
};
