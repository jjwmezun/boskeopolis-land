#include "block_component_add_time.hpp"
#include "block_component_bouncy.hpp"
#include "block_component_change_block.hpp"
#include "block_component_change_direction.hpp"
#include "block_component_change_palette.hpp"
#include "block_component_climb_down.hpp"
#include "block_component_climbable.hpp"
#include "block_component_conveyor.hpp"
#include "block_component_current.hpp"
#include "block_component_diamond.hpp"
#include "block_component_door.hpp"
#include "block_component_force_leftward.hpp"
#include "block_component_force_rightward.hpp"
#include "block_component_force_upward.hpp"
#include "block_component_full_heal.hpp"
#include "block_component_goal.hpp"
#include "block_component_heal.hpp"
#include "block_component_hero_to_shooter.hpp"
#include "block_component_hole.hpp"
#include "block_component_honey.hpp"
#include "block_component_hurt.hpp"
#include "block_component_key.hpp"
#include "block_component_kill.hpp"
#include "block_component_kill_animated.hpp"
#include "block_component_layer.hpp"
#include "block_component_lock_box.hpp"
#include "block_component_lose.hpp"
#include "block_component_mcguffin.hpp"
#include "block_component_message.hpp"
#include "block_component_money.hpp"
#include "block_component_push_down.hpp"
#include "block_component_secret_goal.hpp"
#include "block_component_set_gravity.hpp"
#include "block_component_sewer_door.hpp"
#include "block_component_shmup_bullet.hpp"
#include "block_component_single_use.hpp"
#include "block_component_sink.hpp"
#include "block_component_slope.hpp"
#include "block_component_very_steep.hpp"
#include "block_component_sludge_water.hpp"
#include "block_component_solid.hpp"
#include "block_component_spark_wall.hpp"
#include "block_component_sticky.hpp"
#include "block_component_suit.hpp"
#include "block_component_swimmable.hpp"
#include "block_component_switch_off.hpp"
#include "block_component_switch_on.hpp"
#include "block_component_switch_sound.hpp"
#include "block_component_warp.hpp"
#include "block_condition_bull.hpp"
#include "block_condition_collide_any.hpp"
#include "block_condition_collide_bottom.hpp"
#include "block_condition_collide_left.hpp"
#include "block_condition_collide_not_bottom.hpp"
#include "block_condition_collide_right.hpp"
#include "block_condition_collide_specific.hpp"
#include "block_condition_collide_top.hpp"
#include "block_condition_collide_center.hpp"
#include "block_condition_enemy.hpp"
#include "block_condition_fade_cloud.hpp"
#include "block_condition_fade_out.hpp"
#include "block_condition_hero.hpp"
#include "block_condition_key.hpp"
#include "block_condition_layer.hpp"
#include "block_condition_not_ducking.hpp"
#include "block_condition_not_full_health.hpp"
#include "block_condition_not_hero.hpp"
#include "block_condition_not_on_ladder.hpp"
#include "block_condition_rival.hpp"
#include "block_condition_switch.hpp"
#include "block_condition_switch_off.hpp"
#include "block_condition_switch_on.hpp"
#include <fstream>
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "mezun_exceptions.hpp"
#include "rapidjson/istreamwrapper.h"
#include "switch_graphics.hpp"
#include "tileset.hpp"

Tileset::Tileset( std::string tileset )
:
	name_ ( tileset ),
	universal_block_types_ ( std::move( makeBlockTypes( "universal" ) ) ),
	block_types_ ( std::move( makeBlockTypes( tileset ) ) )
{};

void Tileset::update( EventSystem& events )
{
	for ( auto& u : universal_block_types_ )
	{
		if ( u ) u->update( events );
	}

	for ( auto& t : block_types_ )
	{
		if ( t ) t->update( events );
	}
};

BlockType* Tileset::blockType( int type ) const
{
	if ( type <= EMPTY_BLOCK )
	{
		return nullptr;
	}
	else if ( type < UNIVERSAL_TILESET_SIZE )
	{
		return universal_block_types_.at( type ).get();
	}
	else
	{
		return block_types_.at( type - UNIVERSAL_TILESET_SIZE ).get();
	}
};


std::vector<std::unique_ptr<BlockType>> Tileset::makeBlockTypes( const std::string& tileset ) const
{
	const std::string path = Main::resourcePath() + "tilesets" + Main::pathDivider() + tileset + Main::pathDivider();

	if ( !mezun::checkDirectory( path ) )
	{
		throw mezun::CantLoadTileset( tileset );
	}

	std::vector<std::unique_ptr<BlockType>> types;

	for ( int i = 0; i < TILESET_LIMIT; ++i )
	{
		const std::string file_path = path + Text::formatNumDigitPadding( i, 3 ) + ".json";

		std::ifstream ifs( file_path );

		if ( ifs.is_open() )
		{
			rapidjson::IStreamWrapper ifs_wrap( ifs );
			rapidjson::Document block;
			block.ParseStream( ifs_wrap );

			if ( block.IsObject() )
			{
				types.emplace_back( std::move( makeType( block, tileset ) ) );
			}
			else
			{
				types.emplace_back( nullptr );
			}

			ifs.close();
		}
		else
		{
			types.emplace_back( nullptr );
		}
	}

	return types;
};

std::unique_ptr<BlockType> Tileset::makeType( const rapidjson::Document& block, const std::string& tileset ) const
{

	/* GRAPHICS
	=================================================================*/

		std::unique_ptr<SpriteGraphics> graphics = nullptr;
		const std::string texture = "tilesets" + Main::pathDivider() + tileset + ".png";

		if ( block.HasMember( "graphics" ) && block[ "graphics" ].IsObject() )
		{
			auto g = block[ "graphics" ].GetObject();

			if ( g.HasMember( "type" ) && g[ "type" ].IsString() && strcmp( g[ "type" ].GetString(), "switch" ) == 0 )
			{
				std::vector<std::unique_ptr<SpriteGraphics>> gfx_list;

				if ( g.HasMember( "frames" ) && g[ "frames" ].IsArray() )
				{
					for ( auto& frame : g[ "frames" ].GetArray() )
					{
						if ( frame.IsObject() )
						{
							gfx_list.emplace_back( getGraphics( frame.GetObject(), texture ) );	
						}
					}
					graphics = std::make_unique<SwitchGraphics> ( std::move( gfx_list ) );
				}
				else if
				(
					g.HasMember( "off" ) && g[ "off" ].IsObject() &&
					g.HasMember( "on" )  && g[ "on" ].IsObject()
				)
				{				
					gfx_list.emplace_back( getGraphics( g[ "off" ].GetObject(), texture ) );
					gfx_list.emplace_back( getGraphics( g[ "on" ].GetObject(), texture ) );
					graphics = std::make_unique<SwitchGraphics> ( std::move( gfx_list ) );
				}
				else
				{
					graphics = nullptr;
				}
			}
			else
			{
				graphics = getGraphics( g, texture );
			}
		}


	/* BEHAVIOR
	=================================================================*/

		std::vector<std::unique_ptr<BlockComponent>> components;
		std::vector<std::vector<std::unique_ptr<BlockCondition>>> conditions;

		if ( block.HasMember( "behavior" ) && block[ "behavior" ].IsArray() )
		{
			int i = 0;
			for ( auto& c : block[ "behavior" ].GetArray() )
			{
				if
				(
					c.HasMember( "component" ) &&
					c[ "component" ].IsObject() &&
					c[ "component" ].HasMember( "type" ) &&
					c[ "component" ][ "type" ].IsString()
				)
				{

					auto comp_obj = c[ "component" ].GetObject();
					auto comp_type = comp_obj[ "type" ].GetString();

					if ( mezun::areStringsEqual( comp_type, "solid" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSolid> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "money" ) )
					{
						if ( comp_obj.HasMember( "amount" ) && comp_obj[ "amount" ].IsInt() )
						{
							components.emplace_back( std::make_unique<BlockComponentMoney> ( comp_obj[ "amount" ].GetInt() ) );
						}
						else
						{
							components.emplace_back( std::make_unique<BlockComponentMoney> () );
						}
					}
					else if ( mezun::areStringsEqual( comp_type, "diamond" ) )
					{
						if
						(
							comp_obj.HasMember( "replacement" ) &&
							comp_obj[ "replacement" ].IsInt()
						)
						{
							components.emplace_back( std::make_unique<BlockComponentDiamond> ( comp_obj[ "replacement" ].GetInt() ) );
						}
						else
						{
							components.emplace_back( std::make_unique<BlockComponentDiamond> () );
						}
					}
					else if ( mezun::areStringsEqual( comp_type, "suit" ) )
					{
						if
						(
							comp_obj.HasMember( "suit" ) &&
							comp_obj[ "suit" ].IsString()
						)
						{
							const std::string suit = comp_obj[ "suit" ].GetString();
							if ( suit == "club" )
							{
								components.emplace_back( std::make_unique<BlockComponentSuit> ( CardSuit::CLUB ) );
							}
							else if ( suit == "diamond" )
							{
								components.emplace_back( std::make_unique<BlockComponentSuit> ( CardSuit::DIAMOND ) );
							}
							else if ( suit == "heart" )
							{
								components.emplace_back( std::make_unique<BlockComponentSuit> ( CardSuit::HEART ) );
							}
							else if ( suit == "spade" )
							{
								components.emplace_back( std::make_unique<BlockComponentSuit> ( CardSuit::SPADE ) );
							}
						}
					}
					else if ( mezun::areStringsEqual( comp_type, "mcguffin" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentMcGuffin> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "single" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSingleUse> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "goal" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentGoal> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "climb" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentClimbable> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "heal" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentHeal> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "hurt" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentHurt> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "full_heal" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentFullHeal> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "swim" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSwimmable> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "warp" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentWarp> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "key" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentKey> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "lose" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentLose> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_low_left" ) )
					{
						components.emplace_back( generateBlockComponentMediumSlopeLeftLow() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_high_left" ) )
					{
						components.emplace_back( generateBlockComponentMediumSlopeLeftHigh() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_low_right" ) )
					{
						components.emplace_back( generateBlockComponentMediumSlopeRightLow() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_high_right" ) )
					{
						components.emplace_back( generateBlockComponentMediumSlopeRightHigh() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_steep_left" ) )
					{
						components.emplace_back( generateBlockComponentSteepSlopeLeft() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_steep_right" ) )
					{
						components.emplace_back( generateBlockComponentSteepSlopeRight() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_shallow_left_lowest" ) )
					{
						components.emplace_back( generateBlockComponentShallowSlopeLeftLowest() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_shallow_left_lower" ) )
					{
						components.emplace_back( generateBlockComponentShallowSlopeLeftLower() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_shallow_left_higher" ) )
					{
						components.emplace_back( generateBlockComponentShallowSlopeLeftHigher() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_shallow_left_highest" ) )
					{
						components.emplace_back( generateBlockComponentShallowSlopeLeftHighest() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_shallow_right_lowest" ) )
					{
						components.emplace_back( generateBlockComponentShallowSlopeRightLowest() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_shallow_right_lower" ) )
					{
						components.emplace_back( generateBlockComponentShallowSlopeRightLower() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_shallow_right_higher" ) )
					{
						components.emplace_back( generateBlockComponentShallowSlopeRightHigher() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_shallow_right_highest" ) )
					{
						components.emplace_back( generateBlockComponentShallowSlopeRightHighest() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_very_steep_left_lower" ) )
					{
						components.emplace_back( new BlockComponentVerySteep( Direction::Horizontal::LEFT, 2, 1, 32, 20 ) );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_very_steep_left_higher" ) )
					{
						components.emplace_back( new BlockComponentVerySteep( Direction::Horizontal::LEFT, 2, 2, 32, 20 ) );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_very_steep_right_lower" ) )
					{
						components.emplace_back( new BlockComponentVerySteep( Direction::Horizontal::RIGHT, 2, 1, 32, 20 ) );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_very_steep_right_higher" ) )
					{
						components.emplace_back( new BlockComponentVerySteep( Direction::Horizontal::RIGHT, 2, 2, 32, 20 ) );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_medium_left_high" ) )
					{
						components.emplace_back( generateBlockComponentCeilingMediumSlopeLeftHigh() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_steep_left" ) )
					{
						components.emplace_back( generateBlockComponentCeilingSteepSlopeLeft() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_steep_right" ) )
					{
						components.emplace_back( generateBlockComponentCeilingSteepSlopeRight() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_medium_left_low" ) )
					{
						components.emplace_back( generateBlockComponentCeilingMediumSlopeLeftLow() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_medium_right_high" ) )
					{
						components.emplace_back( generateBlockComponentCeilingMediumSlopeRightHigh() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_medium_right_low" ) )
					{
						components.emplace_back( generateBlockComponentCeilingMediumSlopeRightLow() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_shallow_left_highest" ) )
					{
						components.emplace_back( generateBlockComponentCeilingShallowSlopeLeftHighest() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_shallow_left_higher" ) )
					{
						components.emplace_back( generateBlockComponentCeilingShallowSlopeLeftHigher() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_shallow_left_lower" ) )
					{
						components.emplace_back( generateBlockComponentCeilingShallowSlopeLeftLower() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_shallow_left_lowest" ) )
					{
						components.emplace_back( generateBlockComponentCeilingShallowSlopeLeftLowest() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_shallow_right_highest" ) )
					{
						components.emplace_back( generateBlockComponentCeilingShallowSlopeRightHighest() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_shallow_right_higher" ) )
					{
						components.emplace_back( generateBlockComponentCeilingShallowSlopeRightHigher() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_shallow_right_lower" ) )
					{
						components.emplace_back( generateBlockComponentCeilingShallowSlopeRightLower() );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_bottom_shallow_right_lowest" ) )
					{
						components.emplace_back( generateBlockComponentCeilingShallowSlopeRightLowest() );
					}
					else if ( mezun::areStringsEqual( comp_type, "conveyor" ) )
					{
						const int speed =
						(
							comp_obj.HasMember( "speed" ) &&
							comp_obj[ "speed" ].IsInt()
						)
						? comp_obj[ "speed" ].GetInt()
						: 1;
						const Direction::Horizontal direction =
						(
							comp_obj.HasMember( "direction" ) &&
							comp_obj[ "direction" ].IsString() &&
							mezun::areStringsEqual( comp_obj[ "direction" ].GetString(), "right" )
						)
						? Direction::Horizontal::RIGHT
						: Direction::Horizontal::LEFT;
						components.emplace_back( std::make_unique<BlockComponentConveyor> ( direction, speed ) );
					}
					else if ( mezun::areStringsEqual( comp_type, "climb_down" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentClimbDown> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "push_down" ) )
					{
						int force = 500;
						if
						(
							comp_obj.HasMember( "force" ) &&
							comp_obj[ "force" ].IsInt()
						)
						{
							force = comp_obj[ "force" ].GetInt();
						}
						components.emplace_back( std::make_unique<BlockComponentPushDown> ( force ) );
					}
					else if ( mezun::areStringsEqual( comp_type, "change_direction" ) )
					{
						if
						(
							comp_obj.HasMember( "direction" ) &&
							comp_obj[ "direction" ].IsString()
						)
						{
							if ( mezun::areStringsEqual( comp_obj[ "direction" ].GetString(), "right" ) )
							{
								components.emplace_back( std::make_unique<BlockComponentChangeDirection> ( Direction::Simple::RIGHT ) );
							}
							else if ( mezun::areStringsEqual( comp_obj[ "direction" ].GetString(), "left" ) )
							{
								components.emplace_back( std::make_unique<BlockComponentChangeDirection> ( Direction::Simple::LEFT ) );
							}
							else
							{
								components.emplace_back( std::make_unique<BlockComponentChangeDirection> ( Direction::Simple::UP ) );
							}
						}
					}
					else if ( mezun::areStringsEqual( comp_type, "change_palette" ) )
					{
						if
						(
							comp_obj.HasMember( "palette" ) &&
							comp_obj[ "palette" ].IsString()
						)
						{
							const std::string palette = comp_obj[ "palette" ].GetString();
							int bg_color = 1;


							if
							(
								comp_obj.HasMember( "bg_color" ) &&
								comp_obj[ "bg_color" ].IsInt()
							)
							{
								bg_color = comp_obj[ "bg_color" ].GetInt();
							}

							components.emplace_back( std::make_unique<BlockComponentChangePalette> ( Palette( palette, bg_color ) ) );
						}
					}
					else if ( mezun::areStringsEqual( comp_type, "message" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentMessage> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "kill" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentKill> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "kill_animated" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentKillAnimated> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "change_block" ) )
					{
						if
						(
							comp_obj.HasMember( "value" ) &&
							comp_obj[ "value" ].IsInt()
						)
						{
							components.emplace_back( std::make_unique<BlockComponentChangeBlock> ( comp_obj[ "value" ].GetInt() ) );
						}
					}
					else if ( mezun::areStringsEqual( comp_type, "force_upward" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentForceUpward> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "force_leftward" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentForceLeftward> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "force_rightward" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentForceRightward> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "bouncy" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentBouncy> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "lock_box" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentLockBox> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "sink" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSink> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "sludge_water" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSludgeWater> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "door" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentDoor> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "switch_sound" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSwitchSound> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "layer" ) )
					{
						if
						(
							comp_obj.HasMember( "layer" ) &&
							comp_obj[ "layer" ].IsInt()
						)
						{
							components.emplace_back( std::make_unique<BlockComponentLayer> ( Unit::IntToLayer( comp_obj[ "layer" ].GetInt() ) ) );
						}
					}
					else if ( mezun::areStringsEqual( comp_type, "current_left" ) )
					{
						components.emplace_back( generateBlockComponentCurrentLeftward() );
					}
					else if ( mezun::areStringsEqual( comp_type, "current_right" ) )
					{
						components.emplace_back( generateBlockComponentCurrentRightward() );
					}
					else if ( mezun::areStringsEqual( comp_type, "current_up" ) )
					{
						components.emplace_back( generateBlockComponentCurrentUpward() );
					}
					else if ( mezun::areStringsEqual( comp_type, "current_down" ) )
					{
						components.emplace_back( generateBlockComponentCurrentDownward() );
					}
					else if ( mezun::areStringsEqual( comp_type, "switch_on" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSwitchOn> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "switch_off" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSwitchOff> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "secret_goal" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSecretGoal> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "sticky" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSticky> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "shmup_bullet" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentShmupBullet> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "sewer_door" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSewerDoor> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "set_gravity" ) )
					{
						if
						(
							comp_obj.HasMember( "value" ) &&
							comp_obj[ "value" ].IsDouble()
						)
						{
							components.emplace_back( std::make_unique<BlockComponentSetGravity> ( comp_obj[ "value" ].GetDouble() ) );
						}
					}
					else if ( mezun::areStringsEqual( comp_type, "add_time" ) )
					{
						if
						(
							comp_obj.HasMember( "amount" ) &&
							comp_obj[ "amount" ].IsInt()
						)
						{
							components.emplace_back( std::make_unique<BlockComponentAddTime> ( comp_obj[ "amount" ].GetInt() ) );
						}
					}
					else if ( mezun::areStringsEqual( comp_type, "hole" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentHole> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "spark_wall" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSparkWall> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "hero_to_shooter" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentHeroToShooter> () );
					}
				}

				std::vector<std::unique_ptr<BlockCondition>> this_condition;

				if ( c.HasMember( "conditions" ) && c[ "conditions" ].IsArray() )
				{
					for ( auto& cond : c[ "conditions" ].GetArray() )
					{

						if
						(
							cond.IsObject() &&
							cond.HasMember( "type" ) &&
							cond[ "type" ].IsString()
						)
						{

							auto cond_type = cond[ "type" ].GetString();

							if ( mezun::areStringsEqual( cond_type, "collide_any" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionCollideAny> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "hero" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionHero> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "collide_top" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionCollideTop> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "collide_bottom" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionCollideBottom> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "collide_left" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionCollideLeft> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "collide_right" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionCollideRight> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "collide_center" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionCollideCenter> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "switch_on" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionSwitchOn> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "switch_off" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionSwitchOff> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "switch" ) )
							{
								if ( cond.HasMember( "value" ) && cond[ "value" ].IsInt() )
								{
									this_condition.emplace_back( std::make_unique<BlockConditionSwitch> ( cond[ "value" ].GetInt() ) );
								}
							}
							else if ( mezun::areStringsEqual( cond_type, "key" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionKey> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "not_full_health" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionNotFullHealth> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "not_ducking" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionNotDucking> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "not_on_ladder" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionNotOnLadder> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "collide_not_bottom" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionCollideNotBottom> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "not_hero" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionNotHero> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "layer" ) )
							{
								if ( cond.HasMember( "layer" ) && cond[ "layer" ].IsInt() )
								{
									this_condition.emplace_back( std::make_unique<BlockConditionLayer> ( Unit::IntToLayer( cond[ "layer" ].GetInt() ) ) );
								}
							}
							else if ( mezun::areStringsEqual( cond_type, "bull" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionBull> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "rival" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionRival> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "fade_out" ) )
							{
								int threshold = 0;
								if ( cond.HasMember( "threshold" ) && cond[ "threshold" ].IsInt() )
								{
									threshold = cond[ "threshold" ].GetInt();
								}
								this_condition.emplace_back( std::make_unique<BlockConditionFadeOut> ( threshold ) );
							}
							else if ( mezun::areStringsEqual( cond_type, "fade_cloud" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionFadeCloud> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "collide_specific" ) )
							{
								int left = 0;
								int right = 0;
								int top = 0;
								int bottom = 0;

								if ( cond.HasMember( "left" ) && cond[ "left" ].IsInt() )
								{
									left = cond[ "left" ].GetInt();
								}
								if ( cond.HasMember( "right" ) && cond[ "right" ].IsInt() )
								{
									right = cond[ "right" ].GetInt();
								}
								if ( cond.HasMember( "top" ) && cond[ "top" ].IsInt() )
								{
									top = cond[ "top" ].GetInt();
								}
								if ( cond.HasMember( "bottom" ) && cond[ "bottom" ].IsInt() )
								{
									bottom = cond[ "bottom" ].GetInt();
								}

								this_condition.emplace_back( std::make_unique<BlockConditionCollideSpecific> ( left, right, top, bottom ) );
							}

						}

					}

				}

				conditions.emplace_back( std::move( this_condition ) );
			}
		}


	/* SEND
	=================================================================*/

		return std::make_unique<BlockType>
		(
			std::move( graphics ),
			std::move ( components ), std::move( conditions )
		);
};
