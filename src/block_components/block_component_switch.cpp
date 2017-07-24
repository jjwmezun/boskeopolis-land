#include "animated_graphics.hpp"
#include "block_component_switch.hpp"
#include "block_type.hpp"
#include "event_system.hpp"

BlockComponentSwitch::BlockComponentSwitch
(
	BlockComponent* off_component,
	BlockComponent* on_component,
	std::vector<SpriteGraphics*> off_graphics,
	std::vector<SpriteGraphics*> on_graphics
)
:
	off_component_ ( std::move( off_component ) ),
	on_component_  ( std::move( on_component ) )
{
	/*
	for ( int i = 0; i < off_Render::size(); ++i )
	{
		off_graphics_.push_back( std::unique_ptr<SpriteGraphics> ( off_Render::at( i ) ) );
	}

	for ( int j = 0; j < on_Render::size(); ++j )
	{
		on_graphics_.push_back( std::unique_ptr<SpriteGraphics> ( on_Render::at( j ) ) );
	}*/
};

BlockComponentSwitch::~BlockComponentSwitch() {};

void BlockComponentSwitch::interact( Collision& collision, Sprite& sprite, Block& block, BlockType& type, Level& level, EventSystem& events, Camera& camera, Health& health )
{
	if ( events.switchOn() )
	{
		if ( on_component_ )
		{
			on_component_->interact( collision, sprite, block, type, level, events, camera, health );
		}
	}
	else
	{
		if ( off_component_ )
		{
			off_component_->interact( collision, sprite, block, type, level, events, camera, health );
		}
	}
};

void BlockComponentSwitch::update( EventSystem& events, BlockType& type )
{
	if ( events.switchOn() )
	{
		if ( on_component_ )
		{
			on_component_->update( events, type );
		}
	}
	else
	{
		if ( off_component_ )
		{
			off_component_->update( events, type );
		}
	}

	if ( events.switchChanged() || first_update_ )
	{
		if ( events.switchOn() )
		{
			changeGraphics( type, on_graphics_ );
		}
		else
		{
			changeGraphics( type, off_graphics_ );
		}

		first_update_ = false;

	}
};

void BlockComponentSwitch::changeGraphics( BlockType& type, std::vector<std::shared_ptr<SpriteGraphics>>& graphics )
{/*
	for ( int i = 0; i < Render::size() && i < BlockType::NUM_O_MINI_BLOCKS; ++i )
	{
		if ( Render::at( i ) )
		{
			type.graphics_.at( i ) = Render::at( i );
		}
		else
		{
			type.graphics_.at( i ) = nullptr;
		}
	}*/
};
