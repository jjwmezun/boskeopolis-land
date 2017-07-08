#include "block_component.h"

BlockComponent::BlockComponent
(
	Type type
)
:
	type_ ( type )
{};

BlockComponent::~BlockComponent() {};

void BlockComponent::update( EventSystem& events, BlockType& type ) {};

BlockComponent::Type BlockComponent::type() const
{
	return type_;
};