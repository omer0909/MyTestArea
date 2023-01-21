#pragma once
#include <TestArea.hpp>
#include <Vector2.hpp>
#include <iostream>

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
	bool DirectWayControl(Vector2 const &pos, Vector2 const &target,
			      TestArea &screen);
	bool IsFront(int pos, Vector2 const &target);
	bool IsFrontLeft(int pos, Vector2 const &target);
	bool IsFrontRight(int pos, Vector2 const &target);
	void ReadFile(std::string const &name);
	Vector2 *points;
	unsigned int pointsSize;
	float r;
	Vector2 *walls;
};