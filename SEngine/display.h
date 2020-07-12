#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <string>

class Display
{
public:
	Display(int width, int height, const std::string& title);
	virtual ~Display();

	void Update();
	void Clear(float r, float g, float b, float a);
	void changeDisplayRes(int x, int y);

	bool& IsClosed();

	inline unsigned int GetWidth() { return m_width; };
	inline unsigned int GetHeight() { return m_height; };
	inline SDL_DisplayMode GetDisplayMode() { return m_displayMode; };

protected:

private:
	//Display(const Display& other) {}
	//Display& operator = (const Display& other){}
	bool m_isClosed;

	unsigned int m_width;
	unsigned int m_height;

	SDL_Window* m_window;

	SDL_GLContext m_glContext;

	SDL_DisplayMode m_displayMode;
};

