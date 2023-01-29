#include <unistd.h>

#include <TestArea.hpp>

TestArea::TestArea(unsigned int _width, unsigned int _height)
    : width(_width), height(_height), window("Test Area", _width, _height) {
	window.Clear(color::black);
}

TestArea::~TestArea() { Show(); }

void TestArea::Show() {
	window.UpdateSurface();
	while (true) {
		Event();
		usleep(500000);
	}
}

static unsigned int ColorSaturation(unsigned int color, float value) {
	unsigned int o_r = (color & 0x00FF0000) >> 16;
	unsigned int o_g = (color & 0x0000FF00) >> 8;
	unsigned int o_b = color & 0x000000FF;

	unsigned int r = value * o_r;
	unsigned int g = value * o_g;
	unsigned int b = value * o_b;
	return 0xFF000000 | r << 16 | g << 8 | b;
}

void TestArea::DrawLine(Vector2 a, Vector2 b, unsigned int color) {
	auto ipart = [](float x) -> int { return int(std::floor(x)); };
	auto round = [](float x) -> float { return std::round(x); };
	auto fpart = [](float x) -> float { return x - std::floor(x); };
	auto rfpart = [=](float x) -> float { return 1 - fpart(x); };

	const bool steep = abs(b.y - a.y) > abs(b.x - a.x);
	if (steep) {
		std::swap(a.x, a.y);
		std::swap(b.x, b.y);
	}
	if (a.x > b.x) {
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);
	}

	const float dx = b.x - a.x;
	const float dy = b.y - a.y;
	const float gradient = (dx == 0) ? 1 : dy / dx;

	int xpx11;
	float intery;
	{
		const float xend = round(a.x);
		const float yend = a.y + gradient * (xend - a.x);
		const float xgap = rfpart(a.x + 0.5);
		xpx11 = int(xend);
		const int ypx11 = ipart(yend);
		if (steep) {
			SetPixel(ypx11, xpx11,
			         ColorSaturation(color, rfpart(yend) * xgap));
			SetPixel(ypx11 + 1, xpx11,
			         ColorSaturation(color, fpart(yend) * xgap));
		} else {
			SetPixel(xpx11, ypx11,
			         ColorSaturation(color, rfpart(yend) * xgap));
			SetPixel(xpx11, ypx11 + 1,
			         ColorSaturation(color, fpart(yend) * xgap));
		}
		intery = yend + gradient;
	}

	int xpx12;
	{
		const float xend = round(b.x);
		const float yend = b.y + gradient * (xend - b.x);
		const float xgap = rfpart(b.x + 0.5);
		xpx12 = int(xend);
		const int ypx12 = ipart(yend);
		if (steep) {
			SetPixel(ypx12, xpx12,
			         ColorSaturation(color, rfpart(yend) * xgap));
			SetPixel(ypx12 + 1, xpx12,
			         ColorSaturation(color, fpart(yend) * xgap));
		} else {
			SetPixel(xpx12, ypx12,
			         ColorSaturation(color, rfpart(yend) * xgap));
			SetPixel(xpx12, ypx12 + 1,
			         ColorSaturation(color, fpart(yend) * xgap));
		}
	}

	if (steep) {
		for (int x = xpx11 + 1; x < xpx12; x++) {
			SetPixel(ipart(intery), x,
			         ColorSaturation(color, rfpart(intery)));
			SetPixel(ipart(intery) + 1, x,
			         ColorSaturation(color, fpart(intery)));
			intery += gradient;
		}
	} else {
		for (int x = xpx11 + 1; x < xpx12; x++) {
			SetPixel(x, ipart(intery),
			         ColorSaturation(color, rfpart(intery)));
			SetPixel(x, ipart(intery) + 1,
			         ColorSaturation(color, fpart(intery)));
			intery += gradient;
		}
	}
}

void TestArea::SetPixel(int x, int y, unsigned int color) {
	y *= -1;

	x += width / 2;
	y += height / 2;

	if (x < 0 || x >= (int)width || y < 0 || y >= (int)height)
		return;

	window.SetPixel(x, y, color);
}

void TestArea::SetPixel(Vector2 pos, unsigned int color) {
	SetPixel(std::floor(pos.x + EPSILON), std::floor(pos.y + EPSILON), color);
}

void TestArea::Event() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			exit(0);
		}

		if ((e.type == SDL_KEYDOWN) &&
		    (e.key.keysym.sym == SDLK_ESCAPE)) {
			exit(0);
		}
	}
}

void TestArea::UpdateSurface() { window.UpdateSurface(); }

void TestArea::DrawCircle(Vector2 center, float radious, unsigned int color) {
	float sinf;
	float cosf = 0;
	int cX = std::floor(center.x + EPSILON);
	int cY = std::floor(center.y + EPSILON);
	while ((sinf = std::sqrt(radious * radious - cosf * cosf)) > cosf - 1) {
		float value = sinf - std::floor(sinf);
		float nValue = 1 - value;

		int sin = std::floor(sinf + EPSILON);
		int cos = std::floor(cosf + EPSILON);
		int addSin = sin + 1;

		SetPixel(cX + cos, cY + sin, ColorSaturation(color, nValue));
		SetPixel(cX + cos, cY + addSin, ColorSaturation(color, value));

		SetPixel(cX + cos, cY - sin, ColorSaturation(color, nValue));
		SetPixel(cX + cos, cY - addSin, ColorSaturation(color, value));

		SetPixel(cX - cos, cY + sin, ColorSaturation(color, nValue));
		SetPixel(cX - cos, cY + addSin, ColorSaturation(color, value));

		SetPixel(cX - cos, cY - sin, ColorSaturation(color, nValue));
		SetPixel(cX - cos, cY - addSin, ColorSaturation(color, value));

		SetPixel(cX + sin, cY + cos, ColorSaturation(color, nValue));
		SetPixel(cX + addSin, cY + cos, ColorSaturation(color, value));

		SetPixel(cX + sin, cY - cos, ColorSaturation(color, nValue));
		SetPixel(cX + addSin, cY - cos, ColorSaturation(color, value));

		SetPixel(cX - sin, cY + cos, ColorSaturation(color, nValue));
		SetPixel(cX - addSin, cY + cos, ColorSaturation(color, value));

		SetPixel(cX - sin, cY - cos, ColorSaturation(color, nValue));
		SetPixel(cX - addSin, cY - cos, ColorSaturation(color, value));

		++cosf;
	}
	// for (int x = -radious; x <= radious; x++) {
	// 	for (int y = -radious; y <= radious; y++) {
	// 		if ((int)std::sqrt(x * x + y * y) == radious) {
	// 			SetPixel(x + center.x, y + center.y, color);
	// 		}
	// 	}
	// }
}

bool ControlAngle(Vector2 const& dir, Vector2 const& pos, float maxCos, bool bigAngle) {
	bool side = Vector2::DotProduct(Vector2(-dir.y, dir.x), pos) > 0;
	return (side && Vector2::DotProduct(dir, pos) > maxCos) != bigAngle;
}

void TestArea::DrawCircleAngle(Vector2 const& center, Vector2 const& start, Vector2 const& end, float radious, bool turnDir, unsigned int color) {
	float sin;
	float cos = 0;

	Vector2 startDir = start - center;
	Vector2 endDir = end - center;

	if (turnDir)
		std::swap(startDir, endDir);

	bool bigAngle = Vector2::DotProduct(startDir, Vector2(-endDir.y, endDir.x)) > 0;

	if (bigAngle)
		std::swap(startDir, endDir);

	float maxCos = Vector2::DotProduct(startDir, endDir);

	while ((sin = std::sqrt(radious * radious - cos * cos)) > cos) {
		float value = sin - std::floor(sin);
		float nValue = 1 - value;
		float addSin = sin + 1;

		Vector2 pos = Vector2(cos, sin);
		if (ControlAngle(startDir, pos, maxCos, bigAngle)) {
			SetPixel(center + pos, ColorSaturation(color, nValue));
			SetPixel(center + Vector2(cos, addSin), ColorSaturation(color, value));
		}

		pos = Vector2(cos, -sin);
		if (ControlAngle(startDir, pos, maxCos, bigAngle)) {
			SetPixel(center + pos, ColorSaturation(color, nValue));
			SetPixel(center + Vector2(cos, -addSin), ColorSaturation(color, value));
		}

		pos = Vector2(-cos, sin);
		if (ControlAngle(startDir, pos, maxCos, bigAngle)) {
			SetPixel(center + pos, ColorSaturation(color, nValue));
			SetPixel(center + Vector2(-cos, addSin), ColorSaturation(color, value));
		}

		pos = Vector2(-cos, -sin);
		if (ControlAngle(startDir, pos, maxCos, bigAngle)) {
			SetPixel(center + pos, ColorSaturation(color, nValue));
			SetPixel(center + Vector2(-cos, -addSin), ColorSaturation(color, value));
		}

		pos = Vector2(sin, cos);
		if (ControlAngle(startDir, pos, maxCos, bigAngle)) {
			SetPixel(center + pos, ColorSaturation(color, nValue));
			SetPixel(center + Vector2(addSin, cos), ColorSaturation(color, value));
		}

		pos = Vector2(sin, -cos);
		if (ControlAngle(startDir, pos, maxCos, bigAngle)) {
			SetPixel(center + pos, ColorSaturation(color, nValue));
			SetPixel(center + Vector2(addSin, -cos), ColorSaturation(color, value));
		}

		pos = Vector2(-sin, cos);
		if (ControlAngle(startDir, pos, maxCos, bigAngle)) {
			SetPixel(center + pos, ColorSaturation(color, nValue));
			SetPixel(center + Vector2(-addSin, cos), ColorSaturation(color, value));
		}

		pos = Vector2(-sin, -cos);
		if (ControlAngle(startDir, pos, maxCos, bigAngle)) {
			SetPixel(center + pos, ColorSaturation(color, nValue));
			SetPixel(center + Vector2(-addSin, -cos), ColorSaturation(color, value));
		}
		++cos;
	}
}

int TestArea::GetDebugColor() {
	int value = rand() % 256;
	int index = rand() % 3;

	unsigned int colors[3] = {0, 0, 0};
	colors[index] = value;
	colors[(index + 1) % 3] = 255 - value;
	return 0xFF000000 | (colors[0] << 16) | (colors[1] << 8) | colors[2];
}