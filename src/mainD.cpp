#include <Image.hpp>
#include <TestArea.hpp>

int main() {
	TestArea screen(1000, 1000);
	Image image("test.png");

	Vector2 rotate = Vector2(12, 3).Normalized();
	Vector2 move = Vector2(-50, -100);
	float size = 50;

	Vector2 cross(-rotate.y, rotate.x);
	for (int x = 0; x < screen.window.GetXSize(); x++) {
		for (int y = 0; y < screen.window.GetYSize(); y++) {
			Vector2 pos = rotate * (x + move.x) + cross * (y + move.y);
			screen.window.SetPixel(x, y, image.GetPixel(pos / size));
		}
	}
}
