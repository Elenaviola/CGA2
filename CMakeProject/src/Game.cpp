#include "Game.h"


Game::Game() : 
GameWindow(	800,				//width
			600,				//height
			false,				//fullscreen
			true,				//vsync
			3,					//OpenGL Version Major
			3,					//OpenGL Version Minor	=> Here the OpenGL Version is 3.3
			"Testgame")			//Tile of the window
{

}

Game::~Game()
{

}

//Initialization here. (i.e. load a scene, load textures ...)
void Game::init()
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

//cleanup. Free resources here.
void Game::shutdown()
{

}

//Update Game Logic here
void Game::update(GLdouble time)
{

}

//Render a frame
void Game::render(GLdouble time)
{
	glClear(GL_COLOR_BUFFER_BIT);
}

//Keyboard events
void Game::onKey(Key key, Action action, Modifier modifier)
{
	if (key == Key::Escape && action == Action::Down)
		quit();
}

//Mouse move events
void Game::onMouseMove(MousePosition mouseposition)
{

}

//Mouse Button events
void Game::onMouseButton(MouseButton button, Action action, Modifier modifier)
{

}

//Mouse scroll events
void Game::onMouseScroll(double xscroll, double yscroll)
{

}

//Window resize events
void Game::onFrameBufferResize(int width, int height)
{
	
}
