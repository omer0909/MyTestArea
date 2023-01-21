#include <Map.hpp>
#include <TestArea.hpp>

#include <chrono>

// auto start = std::chrono::system_clock::now();
// //
// auto now = std::chrono::system_clock::now();
// std::chrono::duration<float> elapsed_seconds = now - start;
// std::cout << elapsed_seconds.count() << std::endl;

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cerr << "./out map.txt" << std::endl;
		return -1;
	}

	TestArea screen(500, 500);
	Map map(argv[1]);

	map.DrawWalls(screen);
	map.CalculatePath(Vector2(-100, -120), Vector2(100, 170), screen);

	return 0;
}

int mainE()
{
	TestArea screen(500, 500);

	Vector2 point(-50, 50);
	Vector2 circle(100, 0);
	float r = 100;

	screen.SetPixel(point, color::red);
	screen.SetPixel(circle, color::green);
	screen.DrawCircle(circle, r, color::green);
	///////////////

	const Vector2 diff = circle - point;
	const float sqrDistance = diff.SqrMagnitude();
	const float distanceDiv = 1 / std::sqrt(sqrDistance);
	const Vector2 dir = diff * distanceDiv;
	const float leftDistance = std::sqrt(sqrDistance - r * r);
	const float leftHeight = (leftDistance * r) * distanceDiv;
	const Vector2 leftDir(-dir.y, dir.x);
	const float distancePoint = leftDistance * (leftHeight / r);
	Vector2 left = point + leftDir * leftHeight + dir * distancePoint;
	Vector2 right = point - leftDir * leftHeight + dir * distancePoint;

	screen.DrawLine(point, circle, color::blue);
	screen.DrawLine(point, left, color::yellow);
	screen.DrawLine(point, right, color::yellow);
	return 0;
}

#include <map>
int mainC()
{
	std::map<float, std::string> map;

	map.insert({2.0f, "iki"});
	map.insert({3.5f, "üç buçuk"});
	map.insert({4.0f, "dört"});
	map.insert({1.0f, "bir"});
	map.insert({3.0f, "üç"});
	map.insert({3.0f, "üç new"});

	for (auto &value : map) {

		std::cout << value.second << std::endl;
	}
	return 0;
}

int mainD()
{

	static auto start = std::chrono::system_clock::now();
	// char *v = new char[90000000];
	std::vector<char> v(90000000);
	auto now = std::chrono::system_clock::now();

	std::chrono::duration<float> elapsed_seconds = now - start;

	std::cout << elapsed_seconds.count() << std::endl;
	return 0;
}
