#include "event_system.hpp"
#include "level_state.hpp"
#include "map_layer_image_switch.hpp"

MapLayerImageSwitch::MapLayerImageSwitch
(
	std::string&& texture,
	int width,
	int height,
	int offset_x,
	int offset_y,
	double scroll_speed_x,
	double scroll_speed_y,
	int num_o_frames,
	int repeat_x,
	int repeat_y,
	int move_speed_x,
	int move_speed_y,
	int animation_speed,
	bool flip,
	Uint8 alpha,
	SDL_BlendMode blend_mode,
	Unit::Layer layer_position
)
:
	MapLayerImage
	(
		std::move( texture ),
		width,
		height,
		offset_x,
		offset_y,
		scroll_speed_x,
		scroll_speed_y,
		num_o_frames,
		repeat_x,
		repeat_y,
		move_speed_x,
		move_speed_y,
		animation_speed,
		flip,
		0,
		blend_mode,
		layer_position
	),
	on_alpha_ ( alpha )
{};

MapLayerImageSwitch::~MapLayerImageSwitch() {};

void MapLayerImageSwitch::update( LevelState& level_state )
{
	if ( level_state.events().isSwitchOn() )
	{
		if ( alpha_ < on_alpha_ )
		{
			alpha_ += 4;
		}

		if ( alpha_ > on_alpha_ )
		{
			alpha_ = on_alpha_;
		}
	}
	else
	{
		if ( alpha_ > 0 )
		{
			alpha_ -= 4;
		}

		if ( alpha_ < 0 )
		{
			alpha_ = 0;
		}
	}
};
