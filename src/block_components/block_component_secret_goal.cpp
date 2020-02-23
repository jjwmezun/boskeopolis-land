#include "block_component_secret_goal.hpp"
#include "event_system.hpp"
#include "level_state.hpp"

BlockComponentSecretGoal::BlockComponentSecretGoal() {};

BlockComponentSecretGoal::~BlockComponentSecretGoal() {};

void BlockComponentSecretGoal::interact( const Collision& collision, Sprite& sprite, Block& block, BlockType& type, LevelState& level_state ) const
{
	level_state.events().secretGoal();
};
