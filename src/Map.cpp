#include <Map.hpp>

static inline bool PointDir(Vector2 const &pA, Vector2 const &pB,
                            Vector2 const &pV) {
	const Vector2 lineA = pA - pB;
	const Vector2 lineB = pB - pV;
	const float zValue = lineA.y * lineB.x - lineA.x * lineB.y;

	return zValue < 0;
}

Map::Map(std::string const &file, TestArea &_screen) : data(file), screen(_screen) {
	r = 20;

	walls = new Vector2[data.size * 2];
	for (int i = 0; i < data.size; i++) {
		Vector2 const &a = data.points[i];
		Vector2 const &b = data.Forward(i);
		Vector2 dir = (a - b).Normalized() * r;
		Vector2 crossDir(-dir.y, dir.x);
		walls[i * 2] = a + crossDir;
		walls[i * 2 + 1] = b + crossDir;
	}

	for (int i = 0; i < data.size; i++) {
		if (PointDir(data.Back(i), data.points[i], data.Forward(i)))
			corners.push_back(i);
	}
	went = new bool[data.size];
}

Map::~Map() {
	delete[] walls;
	delete[] went;
}

void Map::DrawWalls() {
	for (int i = 0; i < data.size; i++) {
		screen.DrawCircle(data.points[i], 3, color::green);
		screen.DrawLine(data.points[i], data.Forward(i));
	}
}

static inline bool SegmentCross(Vector2 const &pA1, Vector2 const &pA2,
                                Vector2 const &pB1, Vector2 const &pB2) {
	const bool cases[] = {PointDir(pA1, pA2, pB1), PointDir(pA1, pA2, pB2),
	                      PointDir(pB1, pB2, pA1), PointDir(pB1, pB2, pA2)};
	return cases[0] != cases[1] && cases[2] != cases[3];
}

bool Map::IsFront(int pos, Vector2 const &target) {
	const bool case1 = PointDir(data.Forward(pos), data.points[pos], target);
	const bool case2 = PointDir(data.points[pos], data.Back(pos), target);

	return case1 || case2;
}

bool Map::IsFrontLeft(int corner, Vector2 const &dir) {
	const Vector2 crossDir(-dir.y, dir.x);
	const bool case1 = Vector2::DotProduct(crossDir, data.Forward(corner) - data.points[corner]) > -EPSILON;
	const bool case2 = Vector2::DotProduct(crossDir, data.points[corner] - data.Back(corner)) < EPSILON;
	return case1 && case2;
}

bool Map::IsFrontRight(int corner, Vector2 const &dir) {
	const Vector2 crossDir(-dir.y, dir.x);
	const bool case1 = Vector2::DotProduct(crossDir, data.Forward(corner) - data.points[corner]) < EPSILON;
	const bool case2 = Vector2::DotProduct(crossDir, data.points[corner] - data.Back(corner)) > -EPSILON;
	return case1 && case2;
}

bool Map::IsFrontLeft(int corner, Vector2 const &before, Vector2 const &dir) {
	const Vector2 crossDir(-dir.y, dir.x);
	bool case1 = Vector2::DotProduct(crossDir, data.Back(corner) - data.points[corner]) > -EPSILON;
	bool case2 = Vector2::DotProduct(crossDir, before) < EPSILON;
	return case1 && case2;
}

bool Map::IsFrontRight(int corner, Vector2 const &before, Vector2 const &dir) {
	const Vector2 crossDir(-dir.y, dir.x);
	bool case1 = Vector2::DotProduct(crossDir, data.Forward(corner) - data.points[corner]) < EPSILON;
	bool case2 = Vector2::DotProduct(crossDir, before) > -EPSILON;
	return case1 && case2;
}

// bool Map::IsFrontLeft(Station const &station, int pos, Vector2 const &target)
// {
// 	const int posA = pos * 2;
// 	const int posB = data.BackIndex(pos) * 2;
// 	const bool case1 = PointDir(walls[posA + 1], walls[posA], target);
// 	const bool case2 = PointDir(walls[posB], walls[posB + 1], target);

// 	return case1 && case2;
// }

// bool Map::IsFrontRight(Station const &station, int pos, Vector2 const
// &target)
// {
// 	const int posA = pos * 2;
// 	const int posB = data.BackIndex(pos) * 2;
// 	const bool case1 = PointDir(walls[posA], walls[posA + 1], target);
// 	const bool case2 = PointDir(walls[posB + 1], walls[posB], target);

// 	return case1 && case2;
// }

bool Map::CollisionControl(Vector2 const &a, Vector2 const &b) {
	const Vector2 dir = (b - a).Normalized();
	const Vector2 crossDir(-dir.y, dir.x);
	const Vector2 add = crossDir * r;

	for (int i = 0; i < data.size; i++) {
		if (SegmentCross(a + add, b + add, data.points[i], data.Forward(i)))
			return false;
	}

	for (int i = 0; i < data.size; i++) {
		if (SegmentCross(a - add, b - add, data.points[i], data.Forward(i)))
			return false;
	}
	return true;
}

inline float SegmantSqrDistance(Vector2 const &a, Vector2 const &b, Vector2 const &pos) {
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

bool Map::CollisionControl(Vector2 const &a, Vector2 const &b, int aIndex) {
	for (int i = 0; i < data.size * 2; i += 2) {
		if (SegmentCross(b, a, walls[i], walls[i + 1]) && i != aIndex && i != data.BackIndex(i)) {
			return false;
		}
	}
	for (int i : corners) {
		if (SegmantSqrDistance(a, b, data.points[i]) < r * r && i != aIndex && i != data.BackIndex(i))
			return false;
	}
	return true;
}

bool Map::CollisionControl(Vector2 const &a, Vector2 const &b, int aIndex, int bIndex) {
	for (int i = 0; i < data.size; i++) {
		if (SegmentCross(a, b, walls[i * 2], walls[(i * 2) + 1]) && i != aIndex && i != data.BackIndex(aIndex) && i != bIndex && i != data.BackIndex(bIndex)) {
			return false;
		}
	}
	for (int i : corners) {
		if (SegmantSqrDistance(a, b, data.points[i]) < r * r && i != aIndex && i != data.BackIndex(aIndex) && i != bIndex && i != data.BackIndex(bIndex))
			return false;
	}
	return true;
}

void MoveR(Vector2 const &point, Vector2 const &circle, float r, Vector2 &left,
           Vector2 &right) {
	const Vector2 diff = circle - point;
	const float distanceDiv = 1.0f / diff.Magnitude();
	const Vector2 dir = diff * distanceDiv;
	const Vector2 leftDir(-dir.y, dir.x);
	const float cos = (r * r) * distanceDiv;
	const float sin = std::sqrt((r * r) - (cos * cos));
	left = circle + leftDir * sin - dir * cos;
	right = circle - leftDir * sin - dir * cos;
}

void MoveC(Vector2 const &circleA, Vector2 const &circleB, float r, Vector2 &resultA, Vector2 &resultB, bool turnDir) {
	Vector2 center = Vector2::Lerp(circleA, circleB, 0.5f);

	const Vector2 diff = circleB - center;
	const float distanceDiv = 1.0f / diff.Magnitude();
	const Vector2 dir = diff * distanceDiv;
	const Vector2 leftDir(-dir.y, dir.x);
	const float cos = (r * r) * distanceDiv;
	const float sin = std::sqrt((r * r) - (cos * cos));
	resultA = circleA + leftDir * sin * (turnDir ? 1 : -1) + dir * cos;
	resultB = circleB - leftDir * sin * (turnDir ? 1 : -1) - dir * cos;
}

void MoveL(Vector2 const &circleA, Vector2 const &circleB, float r, Vector2 &resultA, Vector2 &resultB, bool turnDir) {
	const Vector2 dir = (circleB - circleA).Normalized();
	const Vector2 crossDir(-dir.y, dir.x);
	resultA = circleA + crossDir * r * (turnDir ? 1 : -1);
	resultB = circleB + crossDir * r * (turnDir ? 1 : -1);
}

std::multimap<float, Station>::iterator
Map::ControlTargetWay(Station &station, Vector2 const &target, std::multimap<float, Station> &stations) {
	Vector2 left, right;

	MoveR(target, data.points[station.corner], r, left, right);
	float distance = Vector2::Distance(target, left);

	Vector2 beforeDir = station.endPos - station.startPos;
	if (station.turnDir) {
		if (IsFrontRight(station.corner, beforeDir, target - right) &&
		    CollisionControl(right, target, station.corner)) {
			return stations.insert({station.distance + distance,
			                        Station(&station, right, target,
			                                -1, false, distance)});
		}
	} else {
		if (IsFrontLeft(station.corner, beforeDir, target - left) &&
		    CollisionControl(left, target, station.corner)) {
			return stations.insert({station.distance + distance,
			                        Station(&station, left, target,
			                                -1, false, distance)});
		}
	}
	return stations.end();
}

std::multimap<float, Station>::iterator
Map::FindWay(std::multimap<float, Station> &stations, Vector2 const &pos, Vector2 const &target) {
	for (int i : corners) {
		Vector2 left, right;

		MoveR(pos, data.points[i], r, left, right);
		float distance = Vector2::Distance(left, pos);

		if (IsFrontLeft(i, pos - left) &&
		    CollisionControl(left, pos, i)) {
			went[i] = true;
			auto &tmp =
			    stations
			        .insert({distance, Station(nullptr, pos, left, i, true, distance)})
			        ->second;
			auto founded = ControlTargetWay(tmp, target, stations);
			if (founded != stations.end())
				return founded;
		}

		if (IsFrontRight(i, pos - right) &&
		    CollisionControl(right, pos, i)) {
			went[i] = true;

			auto &tmp = stations.insert({distance, Station(nullptr, pos, right, i, false, distance)})
			                ->second;
			auto founded = ControlTargetWay(tmp, target, stations);
			if (founded != stations.end())
				return founded;
		}
	}
	for (auto &sPair : stations) {
		Station const &station = sPair.second;
		Vector2 beforeDir = station.endPos - station.startPos;

		if (station.turnDir) {
			for (int i : corners) {
				if (went[i])
					continue;
				{
					Vector2 A;
					Vector2 B;
					MoveC(data.points[station.corner], data.points[i], r, A, B, station.turnDir);
					if (IsFrontLeft(station.corner, beforeDir, A - B) && IsFrontRight(i, A - B) &&
					    CollisionControl(A, B, station.corner, i)) {
						{
							float distance = Vector2::Distance(A, B);
							went[i] = true;
							auto &tmp = stations.insert({sPair.first + distance,
							                             Station(&sPair.second, A, B, i, false, distance)})
							                ->second;
							auto founded = ControlTargetWay(tmp, target, stations);
							if (founded != stations.end())
								return founded;
						}
					}
				}
				{
					Vector2 A;
					Vector2 B;
					MoveL(data.points[station.corner], data.points[i], r, A, B, station.turnDir);
					if (IsFrontLeft(station.corner, beforeDir, A - B) &&
					    IsFrontLeft(i, A - B) &&
					    CollisionControl(A, B, station.corner, i)) {
						{
							float distance = Vector2::Distance(A, B);
							went[i] = true;
							auto &tmp = stations.insert({sPair.first + distance,
							                             Station(&sPair.second, A, B, i, true, distance)})
							                ->second;
							auto founded = ControlTargetWay(tmp, target, stations);
							if (founded != stations.end())
								return founded;
						}
					}
				}
			}
		} else {
			for (int i : corners) {
				if (went[i])
					continue;
				{
					Vector2 A;
					Vector2 B;
					MoveC(data.points[station.corner], data.points[i], r, A, B, station.turnDir);
					if (IsFrontRight(station.corner, beforeDir, A - B) &&
					    IsFrontLeft(i, A - B) &&
					    CollisionControl(A, B, station.corner, i)) {
						{
							float distance = Vector2::Distance(A, B);
							went[i] = true;
							auto &tmp = stations.insert({sPair.first + distance,
							                             Station(&sPair.second, A, B, i, true, distance)})
							                ->second;
							auto founded = ControlTargetWay(tmp, target, stations);
							if (founded != stations.end())
								return founded;
						}
					}
				}
				{
					Vector2 A;
					Vector2 B;
					MoveL(data.points[station.corner], data.points[i], r, A, B, station.turnDir);
					if (IsFrontRight(station.corner, beforeDir, A - B) &&
					    IsFrontRight(i, A - B) &&
					    CollisionControl(A, B, station.corner, i)) {
						{
							float distance = Vector2::Distance(A, B);
							went[i] = true;
							auto &tmp = stations.insert({sPair.first + distance,
							                             Station(&sPair.second, A, B, i, false, distance)})
							                ->second;
							auto founded = ControlTargetWay(tmp, target, stations);
							if (founded != stations.end())
								return founded;
						}
					}
				}
			}
		}
	}
	return stations.end();
}

void Map::CalculatePath(Vector2 pos, Vector2 target) {
	std::multimap<float, Station> stations;
	auto found = stations.end();

	if (CollisionControl(pos, target)) {
		float distance = Vector2::Distance(pos, target);
		found = stations.insert({distance, Station(nullptr, pos, target, 0, false, distance)});

	} else {
		for (int i = 0; i < data.size; i++) {
			went[i] = false;
		}
		found = FindWay(stations, pos, target);
	}

	if (found != stations.end()) {
		Station *tmp = &found->second;
		while (tmp != nullptr) {
			screen.DrawLine(tmp->startPos, tmp->endPos, color::green);
			if (tmp->beforeStation != nullptr)
				screen.DrawCircleAngle(data.points[tmp->beforeStation->corner], tmp->beforeStation->endPos, tmp->startPos, r, tmp->beforeStation->turnDir, color::green);

			tmp = tmp->beforeStation;
		}
	}
	/*
	    // tüm köşelerin kendi arasındaki kombinsayonu
	    for (int i = 0; i < corners.size(); i++) {
	        for (int j = i + 1; j < corners.size(); j++) {
	            std::cout << "a: " << i << ", b: " << j
	   << std::endl;
	        }
	    }
	*/

	// for (auto i : corners) {
	// 	screen.DrawCircle(data.points[i], 10, color::yellow);
	// }

	// for (int i = 0; i < data.size * 2; i += 2) {
	// 	screen.DrawLine(walls[i], walls[i + 1], color::yellow);
	// }

	/*kararlaştırılan algoritma:
	öncelikle pozisyondan target e ray gönder. eğer başkalarıyla
	kesişim yoksa algoritma burada bitsin. ama ray gönerirken hacime
	göre r kadar sağa ve sola öteleyerek iki ray göndermeyi unutma;

	eğer önceki adım başarsız olursa Breadth-first search
	algoritması kullan. aynı ray tekniği ile etraftaki tüm köşelere
	ray yolla ama sağ veya sola r kadar ötelemek gerekiyor. bunu
	yapmak için kenarın açısına bak ve ona göre corner yerini sağa
	veya sola ötele. (tüm sağ ve sollar local olacak)

	bu şekilde ray gönderildikten sonra karakterin gideceği yolun da
	exrahesaplanması gerekiyor çünkü çember etrafında turlama işi de
	var. bunun için std::acos ve dot product kullanabilirsin

	cornerler arasında olanişlemleri önceden yapıp bir yerde
	depolayabilirsin. (performans için)

	search algoritmsını kullanırken bir elemana erişilip
	erişimediğini kontrol için boolean bir array kullanılabilir. bir
	de tüm elemanların
	*/

	// DrawWalls(screen);
	screen.DrawCircle(pos, r, color::red);
	screen.DrawCircle(target, r, color::green);
}

// void Map::CalculatePath(Vector2 pos, Vector2 target, TestArea
// &screen)
// {
// 	for (unsigned int i = 0; i < pointsSize; i++) {
// 		for (unsigned int j = i + 1; j < pointsSize; j++) {
// 			Vector2 const &pA1(points[i]);
// 			Vector2 const &pA2(points[j]);

// 			if (j == i || j == (i + 1) % pointsSize)
// 				continue;

// 			if (IsBack(i, points[j]) && IsBack(j,
// points[i])) 				continue;

// 			bool cross = false;
// 			for (unsigned int k = 0; k < pointsSize; k++) {
// 				if (k == i || k == j ||
// 				    k == (i - 1 + pointsSize) %
// pointsSize
// || 				    k == (j - 1 + pointsSize) %
// pointsSize) continue;

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
