#ifndef EE_WINDOW_H
#define EE_WINDOW_H

//#include "InputManager.h"
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/glew.h>
#endif
#include <SDL2/SDL.h>
//#include <SDL2/SDL_opengl.h>

class Window
{
public:
	Window() {}
	Window(const char*,int,int);
	~Window();

	void SwapBuffers();
	void Update();
	void SetFullscreen();
	void BindAsRenderTarget() const;
	inline int getWidth()
	{
		return m_width;
	}
	inline int getHeight()
	{
		return m_height;
	}
	inline SDL_Window* getWindow()
	{
		return m_window;
	}
	inline bool getIsCloseRequested()
	{
		return isCloseRequested;
	}
private:
	int m_width, m_height;
	bool isCloseRequested;
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
	SDL_Event e;
	//InputManager* input;
};

#endif
