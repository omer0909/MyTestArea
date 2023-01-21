#include <LineerFunction.hpp>

LineerFunction::LineerFunction(Vector2 point1, Vector2 const &point2)
{
	Vector2 dif = point2 - point1;
	bool swapDistance = Vector2::DotProduct(dif, Vector2(-1, 1)) < 0;

	swap = std::abs(dif.x) < std::abs(dif.y);

	if (swap) {
		std::swap(point1.x, point1.y);
		std::swap(dif.x, dif.y);
	}

	slope = dif.y / dif.x;
	add = point1.y - slope * point1.x;
	distanceFactor = 1 / std::sqrt(slope * slope + 1);

	if (swapDistance)
		distanceFactor *= -1;
}

LineerFunction::LineerFunction(LineerFunction const &other) { *this = other; }

LineerFunction::LineerFunction(bool _swap, float _slope, float _add,
			       float _distanceFactor)
    : swap(_swap), slope(_slope), add(_add), distanceFactor(_distanceFactor)
{
}

LineerFunction &LineerFunction::operator=(LineerFunction const &other)
{
	slope = other.slope;
	add = other.add;
	swap = other.swap;
	distanceFactor = other.distanceFactor;

	return *this;
}

float LineerFunction::GetSlope() const
{
	if (!swap)
		return slope;
	return 1 / slope;
}

float LineerFunction::GetXPos(float yPos) const
{
	return (yPos - GetAdd()) / GetSlope();
}

float LineerFunction::GetYPos(float xPos) const
{
	return xPos * GetSlope() + GetAdd();
}

float LineerFunction::GetAdd() const
{
	if (!swap)
		return add;
	return (-add) / slope;
}

LineerFunction::~LineerFunction() {}

void LineerFunction::SetPoint(Vector2 point)
{
	if (swap)
		std::swap(point.x, point.y);
	add = point.y - slope * point.x;
}

LineerFunction LineerFunction::Ratate90()
{
	return LineerFunction(!swap, -slope, add,
			      swap ? -distanceFactor : distanceFactor);
}

float LineerFunction::Distance(Vector2 point) const
{
	if (swap)
		std::swap(point.x, point.y);

	return (slope * point.x - point.y + add) * distanceFactor;
}

std::ostream &operator<<(std::ostream &o, LineerFunction const &f)
{
	o << "--- slope: " << f.GetSlope() << std::endl;
	o << "--- add:   " << f.GetAdd() << std::endl;
	return (o);
}