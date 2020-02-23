#include "block_component.hpp"

BlockComponent::BlockComponent
(
	Type type
)
:
	type_ ( type )
{};

BlockComponent::~BlockComponent() {};

BlockComponent::Type BlockComponent::type() const
{
	return type_;
};

void BlockComponent::init( Block& block, LevelState& level_state ) const {};
