#pragma once
#include <MapData.hpp>
#include <TestArea.hpp>
#include <Vector2.hpp>
#include <iostream>
#include <map>

class Map
{
      public:
	Map(std::string const &file);
	~Map();
	Map(Map const &);
	Map &operator=(Map const &);
	void DrawWalls(TestArea &);
	void CalculatePath(Vector2 pos, Vector2 target, TestArea &screen);
	bool CrossControl(Vector2 const &a, int index) const;
	bool CrossControl(Vector2 const &a, Vector2 const &b,
			  unsigned int outerSetA, unsigned int outerSetB) const;

      private:
	bool CollisionControl(Vector2 const &a, Vector2 const &b,
			      TestArea &screen);
	bool CollisionControl(Vector2 const &a, Vector2 const &b, int aIndex,
			      TestArea &screen);
	bool CollisionControl(Vector2 const &a, Vector2 const &b, int aIndex,
			      int bIndex, TestArea &screen);
	bool IsFront(int pos, Vector2 const &target);
	bool IsFrontLeft(int corner, Vector2 const &dir);
	bool IsFrontRight(int corner, Vector2 const &dir);
	bool IsFrontLeft(int corner, Vector2 const &before, Vector2 const &dir);
	bool IsFrontRight(int corner, Vector2 const &before, Vector2 const &dir);
	MapData data;
	float r;
	Vector2 *walls;
	std::vector<int> corners;
	bool *went;
};