#include <Map.hpp>

Map::Map(std::string const &file) : data(file)
{
	r = 10;

	walls = new Vector2[data.size * 2];
	for (int i = 0; i < data.size; i++) {
		Vector2 const &a = data.points[i];
		Vector2 const &b = data.Forward(i);
		Vector2 dir = (a - b).Normalized() * r;
		Vector2 crossDir(-dir.y, dir.x);
		walls[i * 2] = a + crossDir;
		walls[i * 2 + 1] = b + crossDir;
	}
}

Map::~Map() { delete[] walls; }

void Map::DrawWalls(TestArea &screen)
{
	for (unsigned int i = 0; i < data.size; i++) {
		screen.DrawCircle(data.points[i], 3, color::green);
		screen.DrawLine(data.points[i], data.Forward(i));
	}
}

static inline bool PointDir(Vector2 const &pA, Vector2 const &pB,
			    Vector2 const &pV)
{
	const Vector2 lineA = pA - pB;
	const Vector2 lineB = pB - pV;
	const float zValue = lineA.y * lineB.x - lineA.x * lineB.y;

	return zValue < 0;
}

static inline bool SegmentCross(Vector2 const &pA1, Vector2 const &pA2,
				Vector2 const &pB1, Vector2 const &pB2)
{
	const bool cases[] = {PointDir(pA1, pA2, pB1), PointDir(pA1, pA2, pB2),
			      PointDir(pB1, pB2, pA1), PointDir(pB1, pB2, pA2)};
	return cases[0] != cases[1] && cases[2] != cases[3];
}

bool Map::IsFront(int pos, Vector2 const &target)
{
	const bool case1 =
	    PointDir(data.Forward(pos), data.points[pos], target);
	const bool case2 = PointDir(data.points[pos], data.Back(pos), target);

	return case1 || case2;
}

bool Map::IsFrontLeft(int pos, Vector2 const &target)
{
	const int posA = pos * 2;
	const int posB = data.BackIndex(pos) * 2;
	const bool case1 = PointDir(walls[posA + 1], walls[posA], target);
	const bool case2 = PointDir(walls[posB], walls[posB + 1], target);

	return case1 && case2;
}

bool Map::IsFrontRight(int pos, Vector2 const &target)
{
	const int posA = pos * 2;
	const int posB = data.BackIndex(pos) * 2;
	const bool case1 = PointDir(walls[posA], walls[posA + 1], target);
	const bool case2 = PointDir(walls[posB + 1], walls[posB], target);

	return case1 && case2;
}

bool Map::CollisionControl(Vector2 const &a, Vector2 const &b, TestArea &screen)
{
	const Vector2 dir = (b - a).Normalized();
	const Vector2 crossDir(-dir.y, dir.x);
	const Vector2 add = crossDir * r;

	screen.DrawLine(a + add, b + add, color::red);
	screen.DrawLine(a - add, b - add, color::red);

	for (unsigned int i = 0; i < data.size; i++) {
		if (SegmentCross(a + add, b + add, data.points[i],
				 data.Forward(i)))
			return false;
	}

	for (unsigned int i = 0; i < data.size; i++) {
		if (SegmentCross(a - add, b - add, data.points[i],
				 data.Forward(i)))
			return false;
	}
	return true;
}

bool Map::CollisionControl(Vector2 const &a, Vector2 const &b, int aIndex,
			   TestArea &screen)
{
	for (int i = 0; i < data.size * 2; i += 2) {
		if (SegmentCross(b, a, walls[i], walls[i + 1]) && i != aIndex &&
		    i != data.BackIndex(i)) {
			return false;
		}
	}
	return true;
}

bool Map::CollisionControl(int aIndex, int bIndex, TestArea &screen)
{
	Vector2 const &a = data.points[aIndex];
	Vector2 const &b = data.points[bIndex];
	for (int i = 0; i < data.size; i++) {
		if (SegmentCross(a, b, walls[i * 2], walls[(i * 2) + 1]) &&
		    i != aIndex && i != data.BackIndex(aIndex) && i != bIndex &&
		    i != data.BackIndex(bIndex)) {
			return false;
		}
	}
	return true;
}

void MoveR(Vector2 const &point, Vector2 const &circle, float r, Vector2 &left,
	   Vector2 &right)
{
	const Vector2 diff = circle - point;
	const float distanceDiv = 1.0f / diff.Magnitude();
	const Vector2 dir = diff * distanceDiv;
	const Vector2 leftDir(-dir.y, dir.x);
	const float cos = (r * r) * distanceDiv;
	const float sin = std::sqrt((r * r) - (cos * cos));
	left = circle + leftDir * sin - dir * cos;
	right = circle - leftDir * sin - dir * cos;
}

class Station
{
      public:
	Station();
};

void Map::CalculatePath(Vector2 pos, Vector2 target, TestArea &screen)
{

	// std::map<float, int>;

	if (CollisionControl(pos, target, screen)) {
		std::cout << "Direct way is clean" << std::endl;
		return;
	} else
		std::cout << "Direct way is not clean" << std::endl;
	std::vector<int> corners;

	for (int i = 0; i < data.size; i++) {
		if (PointDir(data.Back(i), data.points[i], data.Forward(i)))
			corners.push_back(i);
	}

	for (int i = 0; i < data.size * 2; i += 2) {
		screen.DrawLine(walls[i], walls[i + 1], color::yellow);
	}

	for (int i : corners) {

		Vector2 left, right;

		MoveR(pos, data.points[i], r, left, right);

		// if (!x(i, pos + (points[i] - right)) &&
		//     !PointDir(points[i], points[(i + 1) % pointsSize],
		//     right))
		// 	screen.DrawLine(right, pos, color::blue);

		// PointDir(points[(i - 1 + pointsSize) % pointsSize],
		// points[i], 	 left);

		if (IsFrontLeft(i, pos) &&
		    CollisionControl(left, pos, i, screen))
			screen.DrawLine(left, pos, color::blue);

		if (IsFrontRight(i, pos) &&
		    CollisionControl(right, pos, i, screen))
			screen.DrawLine(right, pos, color::blue);

		// if (PointDir(points[i], points[(i + 1) % pointsSize], right))
		// 	screen.DrawLine(right, points[i], color::blue);

		// if (PointDir(points[(i - 1 + pointsSize) % pointsSize],
		// 	     points[i], left))
		// 	screen.DrawLine(left, points[i], color::blue);
	}

	Vector2 beforeA;
	Vector2 beforeB;
	{
	}

	/*
		// tüm köşelerin kendi arasındaki kombinsayonu
		for (int i = 0; i < corners.size(); i++) {
			for (int j = i + 1; j < corners.size(); j++) {
				std::cout << "a: " << i << ", b: " << j <<
	   std::endl;
			}
		}
	*/

	for (auto i : corners) {
		screen.DrawCircle(data.points[i], 10, color::yellow);
	}

	/*kararlaştırılan algoritma:
	öncelikle pozisyondan target e ray gönder. eğer başkalarıyla kesişim
	yoksa algoritma burada bitsin. ama ray gönerirken hacime göre r kadar
	sağa ve sola öteleyerek iki ray göndermeyi unutma;

	eğer önceki adım başarsız olursa Breadth-first search algoritması
	kullan. aynı ray tekniği ile etraftaki tüm köşelere ray yolla ama sağ
	veya sola r kadar ötelemek gerekiyor. bunu yapmak için kenarın açısına
	bak ve ona göre corner yerini sağa veya sola ötele. (tüm sağ ve sollar
	local olacak)

	bu şekilde ray gönderildikten sonra karakterin gideceği yolun da
	exrahesaplanması gerekiyor çünkü çember etrafında turlama işi de var.
	bunun için std::acos ve dot product kullanabilirsin

	cornerler arasında olanişlemleri önceden yapıp bir yerde
	depolayabilirsin. (performans için)

	search algoritmsını kullanırken bir elemana erişilip erişimediğini
	kontrol için boolean bir array kullanılabilir. bir de tüm elemanların
	*/

	// DrawWalls(screen);
	screen.DrawCircle(pos, r, color::red);
}

// void Map::CalculatePath(Vector2 pos, Vector2 target, TestArea &screen)
// {
// 	for (unsigned int i = 0; i < pointsSize; i++) {
// 		for (unsigned int j = i + 1; j < pointsSize; j++) {
// 			Vector2 const &pA1(points[i]);
// 			Vector2 const &pA2(points[j]);

// 			if (j == i || j == (i + 1) % pointsSize)
// 				continue;

// 			if (IsBack(i, points[j]) && IsBack(j, points[i]))
// 				continue;

// 			bool cross = false;
// 			for (unsigned int k = 0; k < pointsSize; k++) {
// 				if (k == i || k == j ||
// 				    k == (i - 1 + pointsSize) % pointsSize ||
// 				    k == (j - 1 + pointsSize) % pointsSize)
// 					continue;

// 				Vector2 const &pB1(points[k]);
// 				Vector2 const &pB2(
// 				    points[(k + 1) % pointsSize]);

// 				if (SegmentCross(pA1, pA2, pB1, pB2)) {
// 					cross = true;
// 					break;
// 				}
// 			}
// 			if (!cross)
// 				screen.DrawLine(points[i], points[j],
// 						color::blue);
// 		}
// 	}
// 	DrawWalls(screen);
// }
