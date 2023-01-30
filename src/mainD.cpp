#include <Image.hpp>
#include <TestArea.hpp>

int main() {
	TestArea screen(1000, 1000);
	Image image("test.png");

	Vector2 rotate = Vector2(12, 3).Normalized();
	Vector2 cross(-rotate.y, rotate.x);
	for (int x = 0; x < 10 * 100; x++) {
		for (int y = 0; y < 10 * 100; y++) {
			Vector2 pos = rotate * x + cross * y;
			screen.window.SetPixel(x, y, image.GetPixel(pos / 100));
		}
	}
}
