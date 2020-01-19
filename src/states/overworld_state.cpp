#include "audio.hpp"
#include "main.hpp"
#include "input.hpp"
#include "overworld_state.hpp"
#include "overworld_menu_state.hpp"
#include "unit.hpp"

OverworldState::OverworldState()
:
	GameState( StateID::OVERWORLD_STATE, { "Overworld Red", 2 } ),
	inventory_ (),
	hero_ ( 300, 300 ),
	camera_ ( 800, 448 ),
	camera_state_ ( CameraState::MOVE_PLAYER )
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
};

void OverworldState::stateRender()
{
	Render::colorCanvas( 2 );
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
	Audio::changeSong( "overworld" );
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