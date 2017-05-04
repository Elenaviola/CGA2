#ifndef _GAME_H_
#define _GAME_H_
#include "GameWindow.h"
#include "Input.h"
#include "glerror.h"
#include "libheaders.h"

class Game : public GameWindow
{
public:
	Game();
	~Game();

	void init() override;
	void shutdown() override;

	void update(GLdouble time) override;
	void render(GLdouble time) override;

	void onKey(Key key, Action action, Modifier modifier) override;
	void onMouseMove(MousePosition mouseposition) override;
	void onMouseButton(MouseButton button, Action action, Modifier modifier) override;
	void onMouseScroll(double xscroll, double yscroll) override;

	void onFrameBufferResize(int width, int height) override;

private:
};

#endif