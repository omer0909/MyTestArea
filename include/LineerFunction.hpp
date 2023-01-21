#pragma once

#include <Vector2.hpp>
#include <iostream>

class LineerFunction
{
      public:
	LineerFunction(Vector2 point1, Vector2 const &point2);

	LineerFunction(LineerFunction const &);
	LineerFunction &operator=(LineerFunction const &);
	~LineerFunction();

	void SetPoint(Vector2 point);
	LineerFunction Ratate90();
	float Distance(Vector2 point) const;
	float GetSlope() const;
	float GetAdd() const;

	float GetXPos(float yPos) const;
	float GetYPos(float xPos) const;

      private:
	LineerFunction(bool swap, float slope, float add, float distanceFactor);
	bool swap;
	float slope;
	float add;
	float distanceFactor;
};

std::ostream &operator<<(std::ostream &o, LineerFunction const &i);