#pragma once

#include <Vector2.hpp>
#include <Window.hpp>
#include <algorithm>

#define EPSILON 0.00001f
#define ANGLE_TO_RADIAN (M_PI / 180)

enum color {
	black = 0xFF000000,
	white = 0xFFFFFFFF,
	red = 0xFFFF0000,
	green = 0xFF00FF00,
	blue = 0xFF0000FF,
	cyan = 0xFF00FFFF,
	magenta = 0xFFFF00FF,
	yellow = 0xFFFFFF00
};

class TestArea {
   private:
	unsigned int width;
	unsigned int height;
	Window window;
	void Event();
	void Show();

   public:
	TestArea(unsigned int _width, unsigned int _height);
	~TestArea();
	void SetPixel(Vector2 pos, unsigned int color = color::white);
	void SetPixel(int x, int y, unsigned int color = color::white);
	void DrawLine(Vector2 a, Vector2 b, unsigned int color = color::white);
	void DrawCircle(Vector2 center, float radious, unsigned int color = color::white);
	void DrawCircleAngle(Vector2 const& center, Vector2 const& start, Vector2 const& end, float radious, bool turnDir, unsigned int color = color::white);
	void UpdateSurface();
	static int GetDebugColor();
};