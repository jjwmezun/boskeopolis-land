#include "animated_graphics.hpp"
#include "block_component_bouncy.hpp"
#include "block_component_change_block.hpp"
#include "block_component_change_direction.hpp"
#include "block_component_change_palette.hpp"
#include "block_component_climbable.hpp"
#include "block_component_conveyor.hpp"
#include "block_component_diamond.hpp"
#include "block_component_door.hpp"
#include "block_component_force_upward.hpp"
#include "block_component_full_heal.hpp"
#include "block_component_goal.hpp"
#include "block_component_high_slope_left.hpp"
#include "block_component_high_slope_right.hpp"
#include "block_component_heal.hpp"
#include "block_component_hurt.hpp"
#include "block_component_key.hpp"
#include "block_component_kill.hpp"
#include "block_component_lose.hpp"
#include "block_component_low_slope_left.hpp"
#include "block_component_low_slope_right.hpp"
#include "block_component_mcguffin.hpp"
#include "block_component_message.hpp"
#include "block_component_money.hpp"
#include "block_component_move_water.hpp"
#include "block_component_single_use.hpp"
#include "block_component_sink.hpp"
#include "block_component_solid.hpp"
#include "block_component_steep_slope_left.hpp"
#include "block_component_steep_slope_right.hpp"
#include "block_component_swimmable.hpp"
#include "block_component_switch.hpp"
#include "block_component_switch_flip.hpp"
#include "block_component_warp.hpp"
#include "block_condition_collide_any.hpp"
#include "block_condition_collide_bottom.hpp"
#include "block_condition_collide_left.hpp"
#include "block_condition_collide_right.hpp"
#include "block_condition_collide_top.hpp"
#include "block_condition_enemy.hpp"
#include "block_condition_fade_cloud.hpp"
#include "block_condition_hero.hpp"
#include "block_condition_key.hpp"
#include "block_condition_not_ducking.hpp"
#include "block_condition_rival.hpp"
#include <fstream>
#include "main.hpp"
#include "mezun_helpers.hpp"
#include "mezun_exceptions.hpp"
#include "rapidjson/istreamwrapper.h"
#include "sprite_graphics.hpp"
#include "tileset.hpp"

Tileset::Tileset( std::string tileset )
try :
	name_ ( tileset ),
	universal_block_types_ ( std::move( makeBlockTypes( "universal" ) ) ),
	block_types_ ( std::move( makeBlockTypes( tileset ) ) )
{}
catch ( const mezun::CantLoadTileset& e )
{
	throw e;
};

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

BlockType* Tileset::blockType( int type, int x, int y )
{
	try
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
	}
	catch ( const std::out_of_range& e )
	{
		throw mezun::InvalidBlockType( type, name_, x, y );
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

		std::string texture = "tilesets" + Main::pathDivider() + tileset + ".png";

		if ( block.HasMember( "graphics" ) && block[ "graphics" ].IsObject() )
		{

			auto g = block[ "graphics" ].GetObject();

			bool flip_x = false;
			bool flip_y = false;
			double rotation = 0;
			bool priority = false;
			Uint8 alpha = 255;

			if ( g.HasMember( "flip_x" ) && g[ "flip_x" ].IsBool() )
			{
				flip_x = g[ "flip_x" ].GetBool();
			}

			if ( g.HasMember( "flip_y" ) && g[ "flip_y" ].IsBool() )
			{
				flip_y = g[ "flip_y" ].GetBool();
			}

			if ( g.HasMember( "rotation" ) && g[ "rotation" ].IsInt() )
			{
				rotation = g[ "rotation" ].GetInt();
			}

			if ( g.HasMember( "priority" ) && g[ "priority" ].IsBool() )
			{
				priority = g[ "priority" ].GetBool();
			}

			if ( g.HasMember( "alpha" ) && g[ "alpha" ].IsInt() )
			{
				alpha = g[ "alpha" ].GetInt();
			}


			if ( g.HasMember( "type" ) && g[ "type" ].IsString() )
			{
				if ( mezun::areStringsEqual( g[ "type" ].GetString(), "sprite" ) )
				{
					int x = 0;
					int y = 0;

					if ( g.HasMember( "x" ) && g[ "x" ].IsInt() )
					{
						x = g[ "x" ].GetInt();
					}

					if ( g.HasMember( "y" ) && g[ "y" ].IsInt() )
					{
						y = g[ "y" ].GetInt();
					}

					graphics = std::make_unique<SpriteGraphics>
					(
						std::forward<std::string> ( texture ),
						Unit::BlocksToPixels( x ),
						Unit::BlocksToPixels( y ),
						flip_x,
						flip_y,
						rotation,
						priority,
						0,
						0,
						0,
						0,
						alpha
					);
				}
				else if ( mezun::areStringsEqual( g[ "type" ].GetString(), "animated" ) )
				{
					int animation_speed = 8;

					if ( g.HasMember( "speed" ) && g[ "speed" ].IsInt() )
					{
						animation_speed = g[ "speed" ].GetInt();
					}

					if ( g.HasMember( "frames" ) && g[ "frames" ].IsArray() )
					{
						std::vector<std::pair<int, int>> frames;

						for ( auto& fr : g[ "frames" ].GetArray() )
						{
							if ( fr.IsObject() )
							{
								auto f = fr.GetObject();
								int x = 0;
								int y = 0;

								if ( f.HasMember( "x" ) && f[ "x" ].IsInt() )
								{
									x = Unit::BlocksToPixels( f[ "x" ].GetInt() );
								}

								if ( f.HasMember( "y" ) && f[ "y" ].IsInt() )
								{
									y = Unit::BlocksToPixels( f[ "y" ].GetInt() );
								}

								frames.emplace_back( std::make_pair ( x, y ) );
							}
						}

						graphics = std::make_unique<AnimatedGraphics>
						(
							std::forward<std::string> ( texture ),
							frames,
							flip_x,
							flip_y,
							rotation,
							priority,
							0,
							0,
							0,
							0,
							animation_speed
						);
					}
				}
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
						if
						(
							comp_obj.HasMember( "amount" ) &&
							comp_obj[ "amount" ].IsInt()
						)
						{
							components.emplace_back( std::make_unique<BlockComponentHeal> ( comp_obj[ "amount" ].GetInt() ) );
						}
						else
						{
							components.emplace_back( std::make_unique<BlockComponentHeal> () );
						}
					}
					else if ( mezun::areStringsEqual( comp_type, "hurt" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentHurt> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "full_heal" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentFullHeal> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "diamond" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentDiamond> () );
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
					else if ( mezun::areStringsEqual( comp_type, "slope_high_left" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentHighSlopeLeft> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_low_left" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentLowSlopeLeft> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_high_right" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentHighSlopeRight> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_low_right" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentLowSlopeRight> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_steep_left" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSteepSlopeLeft> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "slope_steep_right" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSteepSlopeRight> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "conveyor" ) )
					{
						if
						(
							comp_obj.HasMember( "direction" ) &&
							comp_obj[ "direction" ].IsString() &&
							mezun::areStringsEqual( comp_obj[ "direction" ].GetString(), "right" )
						)
						{
							components.emplace_back( std::make_unique<BlockComponentConveyor> ( Direction::Horizontal::RIGHT ) );
						}
						else
						{
							components.emplace_back( std::make_unique<BlockComponentConveyor> ( Direction::Horizontal::LEFT ) );
						}
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
					else if ( mezun::areStringsEqual( comp_type, "bouncy" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentBouncy> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "sink" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentSink> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "move_water" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentMoveWater> () );
					}
					else if ( mezun::areStringsEqual( comp_type, "door" ) )
					{
						components.emplace_back( std::make_unique<BlockComponentDoor> () );
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
							else if ( mezun::areStringsEqual( cond_type, "hero" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionHero> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "key" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionKey> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "not_ducking" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionNotDucking> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "rival" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionRival> () );
							}
							else if ( mezun::areStringsEqual( cond_type, "fade_cloud" ) )
							{
								this_condition.emplace_back( std::make_unique<BlockConditionFadeCloud> () );
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