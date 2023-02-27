
#include <LineerFunction.hpp>
#include <TestArea.hpp>

struct Box {
	Vector2 pos;
	Vector2 dir;
	Vector2 size;
};

float RandomRange(float start, float end) {
	float value = end - start;
	return start + static_cast<float>(rand()) /
	                   (static_cast<float>(RAND_MAX / value));
}

#include <unistd.h>

int main() {
	// extra randomize
	//  for (int i = 0; i < 500; i++)
	//  	rand();

	// create test area
	TestArea screen(1000, 1000);

	// draw star
	//  screen.SetPixel(Vector2(50, 50), color::white);
	//  for (float i = 0; i < M_PI * 2; i += M_PI / 16) {
	//  	screen.DrawLine(Vector2(0, 0), Vector2(cos(i), sin(i)) *
	//  100, color::magenta);
	//  }

	// create random points
	std::vector<Vector2> points;
	for (int i = 0; i < 300; i++) {
		Vector2 tmp(RandomRange(-300, 300), RandomRange(-300, 300));
		if (tmp.SqrMagnitude() < 300 * 300)
			points.push_back(tmp);
	}

	// draw random points
	for (unsigned int i = 0; i < points.size(); i++) {
		screen.DrawCircle(points[i], 5, TestArea::GetDebugColor());
	}

	// initialize convex shape points list
	std::vector<int> convex(1);

	// find max height point
	float maxHeight = std::numeric_limits<float>::min();
	for (unsigned int i = 0; i < points.size(); i++) {
		if (points[i].y > maxHeight) {
			maxHeight = points[i].y;
			convex[0] = i;
		}
	}

	// draw a circle on max height point
	screen.DrawCircle(points[convex[0]], 20, color::red);

	// search other convex points with dot product
	Vector2 lastNormal = Vector2(0, 1);
	Vector2 lastPoint = points[convex[0]];

	for (unsigned int i = 0; i < points.size(); i++) {
		int selected;
		float maxAngle = -1;

		for (unsigned int j = 0; j < points.size(); j++) {
			Vector2 tmpVector = points[j] - lastPoint;
			if (tmpVector.SqrMagnitude() < EPSILON)
				continue;

			float tmpAngle = Vector2::DotProduct(
			    lastNormal, tmpVector.Normalized());
			if (tmpAngle > maxAngle) {
				maxAngle = tmpAngle;
				selected = j;
			}
		}
		if (selected == convex[0])
			break;

		lastPoint = points[selected];
		lastNormal =
		    (points[selected] - points[convex.back()]).Normalized();
		convex.push_back(selected);
	}

	// Draw convex shape
	screen.DrawLine(points[convex[0]], points[convex[1]], color::white);
	for (size_t i = 1; i < convex.size(); i++) {
		unsigned int col = color::blue;
		if (LineerFunction(points[convex[i]],
		                   points[convex[(i + 1) % convex.size()]])
		        .Distance(Vector2(0, 0)) > 0)
			col = color::green;

		screen.DrawLine(points[convex[i]],
		                points[convex[(i + 1) % convex.size()]], col);
		screen.DrawCircle(points[convex[i]], 15, col);
	}

	// search the minimum box on the all points
	float minArea = std::numeric_limits<float>::max();
	Box box;

	for (size_t i = 0; i < convex.size(); i++) {
		const Vector2 a = points[convex[i]];
		const Vector2 b = points[convex[(i + 1) % convex.size()]];

		LineerFunction func(a, b);
		LineerFunction crossFunc(func.Ratate90());
		crossFunc.SetPoint(a);

		Vector2 dir = (b - a).Normalized();

		float boxMin = std::numeric_limits<float>::max();
		float boxMax = std::numeric_limits<float>::min();

		float boxCMin = std::numeric_limits<float>::max();
		float boxCMax = std::numeric_limits<float>::min();

		for (auto &point : points) {
			float distance = func.Distance(point);
			if (distance < boxMin)
				boxMin = distance;
			if (distance > boxMax)
				boxMax = distance;

			float cDistance = crossFunc.Distance(point);

			if (cDistance < boxCMin)
				boxCMin = cDistance;
			if (cDistance > boxCMax)
				boxCMax = cDistance;
		}

		const Vector2 boxSize(boxMax - boxMin, boxCMax - boxCMin);
		float area = boxSize.x * boxSize.y;

		if (area < minArea) {
			minArea = area;

			Vector2 crossDir(-dir.y, dir.x);

			Vector2 boxPos = a;
			boxPos = boxPos + crossDir * boxMax;
			boxPos = boxPos + dir * boxCMin;

			box.pos = boxPos;
			box.size = boxSize;
			box.dir = dir;
		}
	}

	// draw minimum area size box
	Vector2 boxCrossDir(box.dir.y, -box.dir.x);

	screen.DrawCircle(box.pos, 40);

	Vector2 boxPoints[] = {box.pos,

	                       box.pos + (box.dir * box.size.y),

	                       box.pos + (box.dir * box.size.y) +
	                           boxCrossDir * box.size.x,

	                       box.pos + boxCrossDir * box.size.x};

	for (size_t i = 0; i < 4; i++) {
		screen.DrawLine(boxPoints[i], boxPoints[(i + 1) % 4],
		                color::yellow);
	}
}