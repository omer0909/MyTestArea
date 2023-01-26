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
	map.CalculatePath(Vector2(-150, -180), Vector2(100, 0), screen);

	return 0;
}

inline bool InSegmant(Vector2 const &a, Vector2 const &b, Vector2 const &pos)
{
	const Vector2 diff = b - a;
	float rate = Vector2::DotProduct(diff, (pos - a));
	return rate > 0 && rate < diff.SqrMagnitude();
}

inline float SegmantDistance(Vector2 const &a, Vector2 const &b,
			     Vector2 const &pos)
{
	const Vector2 diff = b - a;
	const Vector2 crossDir(-diff.y, diff.x);
	const Vector2 pa = pos - a;
	if (pa.y * crossDir.x - pa.x * crossDir.y > 0) {
		return pa.Magnitude();
	}
	const Vector2 pb = pos - b;
	if (pb.y * crossDir.x - pb.x * crossDir.y < 0) {
		return pb.Magnitude();
	}
	return std::abs(Vector2::DotProduct(crossDir, pa) / diff.Magnitude());
}

inline float SegmantSqrDistance(Vector2 const &a, Vector2 const &b,
				Vector2 const &pos)
{
	const Vector2 diff = b - a;
	const Vector2 crossDir(-diff.y, diff.x);
	const Vector2 pa = pos - a;
	if (pa.y * crossDir.x - pa.x * crossDir.y > 0) {
		return pa.SqrMagnitude();
	}
	const Vector2 pb = pos - b;
	if (pb.y * crossDir.x - pb.x * crossDir.y < 0) {
		return pb.SqrMagnitude();
	}
	const float dot = Vector2::DotProduct(crossDir, pa);
	return (dot * dot) / crossDir.SqrMagnitude();
}

int mainA()
{
	TestArea screen(500, 500);
	Vector2 a(50, -50);
	Vector2 b(-100, 0);

	for (int x = -250; x < 250; x++) {
		for (int y = -250; y < 250; y++) {

			screen.SetPixel(x, y,
					100 << (int)(SegmantSqrDistance(
							 a, b, Vector2(x, y)) /
						     100));
		}
	}

	screen.DrawLine(a, b);
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
	std::multimap<float, std::string> map;

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

int mainT()
{
	constexpr int size = 50000;
	typedef float test;
	test *arr = new test[size];
	test *b = new test[size];
	for (int i = 0; i < size; i++) {
		arr[i] = rand();
		b[i] = arr[i];
	}
	auto start = std::chrono::system_clock::now();
	for (int i = 0; i < size; i++) {
		for (int j = 1; j < size; j++) {
			arr[i] = std::sqrt(arr[i]);
		}
	}
	{
		auto now = std::chrono::system_clock::now();

		std::chrono::duration<float> elapsed_seconds = now - start;

		std::cout << elapsed_seconds.count() << std::endl;
	}
}
// int main()
// {
// 	TestArea screen(500, 500);

// 	Vector2 circleA(100, 100);
// 	Vector2 circleB(-100, -100);
// 	float r = 50;

// 	screen.DrawCircle(circleA, r);
// 	screen.DrawCircle(circleB, r);

// 	Vector2 A;
// 	Vector2 B;

// 	MoveL(circleB, circleA, r, A, B, false);
// 	screen.DrawLine(A, B);
// }