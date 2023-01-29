#pragma once
#include <MapData.hpp>
#include <TestArea.hpp>
#include <Vector2.hpp>
#include <iostream>
#include <map>

class Station {
   public:
	inline Station(Station *_beforeStation, Vector2 const &_startPos, Vector2 const &_endPos, int _corner, bool _turnDir, float _distance)
	    : beforeStation(_beforeStation), startPos(_startPos), endPos(_endPos), corner(_corner), turnDir(_turnDir), distance(_distance) {
	}
	Station *beforeStation;
	Vector2 startPos;
	Vector2 endPos;
	int corner;
	bool turnDir;
	float distance;
};

class Map {
   public:
	Map(std::string const &file, TestArea &_screen);
	~Map();
	Map(Map const &);
	Map &operator=(Map const &);
	void DrawWalls();
	void CalculatePath(Vector2 pos, Vector2 target);
	bool CrossControl(Vector2 const &a, int index) const;
	bool CrossControl(Vector2 const &a, Vector2 const &b, unsigned int outerSetA, unsigned int outerSetB) const;

   private:
	bool CollisionControl(Vector2 const &a, Vector2 const &b);
	bool CollisionControl(Vector2 const &a, Vector2 const &b, int aIndex);
	bool CollisionControl(Vector2 const &a, Vector2 const &b, int aIndex, int bIndex);
	bool IsFront(int pos, Vector2 const &target);
	bool IsFrontLeft(int corner, Vector2 const &dir);
	bool IsFrontRight(int corner, Vector2 const &dir);
	bool IsFrontLeft(int corner, Vector2 const &before, Vector2 const &dir);
	bool IsFrontRight(int corner, Vector2 const &before, Vector2 const &dir);
	std::multimap<float, Station>::iterator
	ControlTargetWay(Station &station, Vector2 const &target, std::multimap<float, Station> &stations);
	std::multimap<float, Station>::iterator
	FindWay(std::multimap<float, Station> &stations, Vector2 const &pos, Vector2 const &target);
	MapData data;
	float r;
	Vector2 *walls;
	std::vector<int> corners;
	bool *went;
	TestArea &screen;
};