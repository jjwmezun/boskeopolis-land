#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "mezun_json.hpp"
#include "overworld_state.hpp"
#include "overworld_menu_state.hpp"
#include "unit.hpp"

OverworldState::OverworldState()
:
	GameState( StateID::OVERWORLD_STATE, { "Overworld Red", 2 } ),
	inventory_ (),
	hero_ ( 300, 300 ),
	camera_ ( 800, 448 ),
	camera_state_ ( CameraState::MOVE_PLAYER ),
	background_animation_timer_ ( 0 ),
	background_animation_frame_ ( 0 ),
	water_background_ ( "bg/overworld-water.png", { 0, 0, 400, 224 } )
{
	camera_.center( hero_.getPosition() );
	hero_.updateGraphics( camera_.getBox() );
};

OverworldState::~OverworldState() {};

void OverworldState::stateUpdate()
{
	switch ( camera_state_ )
	{
		case ( CameraState::MOVE_PLAYER ):
		{
			testForMenuAction();
			camera_.adjust( hero_.getGraphicsBox() );
			hero_.update( camera_.getBox() );
			inventory_.update( 0 );
		}
		break;

		case ( CameraState::MOVE_CAMERA ):
		{
			if ( Input::pressed( Input::Action::MENU ) )
			{
				camera_state_ = CameraState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER;
			}
			else
			{
				camera_.move();
			}
		}
		break;

		case ( CameraState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER ):
		{
			if ( camera_.moveAutomaticallyToTarget( hero_.getPosition() ) )
			{
				camera_state_ = CameraState::MOVE_PLAYER;
			}
		}
		break;
	}

	updateBackgroundAnimation();
	updateBackgroundPosition();
};

void OverworldState::stateRender()
{
	Render::colorCanvas( 2 );
	water_background_.render();
	hero_.render( camera_ );
	switch ( camera_state_ )
	{
		case ( CameraState::MOVE_PLAYER ):
		{
			inventory_.render( 0 );
		}
		break;

		case ( CameraState::MOVE_CAMERA ):
		{
			renderCameraArrows();
		}
		break;

		case ( CameraState::CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER ):
		{
		}
		break;
	}
};

void OverworldState::init()
{
	const auto& data = mezun::getJSONData( Main::resourcePath() + "maps/land-ow.json", "The o’erworld map data is missing. Please redownload this game & try ’gain.", "The o’erworld map data is broken. Please redownload this game & try ’gain." );
	if ( !data.HasMember( "width" ) || !data[ "width" ].IsInt() )
	{
		throw std::runtime_error( "The o’erworld map data is broken. Please redownload this game & try ’gain." );
	}
	std::cout << data[ "width" ].GetInt() << std::endl;
	Audio::changeSong( "overworld" );
};

void loadMap( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data )
{

};

void OverworldState::testForMenuAction()
{
	if ( Input::pressed( Input::Action::MENU ) )
	{
		Main::pushState
		(
			std::make_unique<OverworldMenuState> ( palette() )
		);
	}
};

void OverworldState::backFromPop()
{
	Audio::changeSong( "overworld" );
};

void OverworldState::renderCameraArrows()
{

};

void OverworldState::updateBackgroundAnimation()
{
	if ( background_animation_timer_ == 7 )
	{
		background_animation_timer_ = 0;
		if ( background_animation_frame_ == 15 )
		{
			background_animation_frame_ = 0;
		}
		else
		{
			++background_animation_frame_;
		}
	}
	else
	{
		++background_animation_timer_;
	}
};

void OverworldState::updateBackgroundPosition()
{
	water_background_.src_.x = ( camera_.x() + background_animation_frame_ ) % 16;
	water_background_.src_.y = camera_.y() % 16;
};