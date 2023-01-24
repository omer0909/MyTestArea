#include <Vector2.hpp>
#include <iostream>
class MapData
{
      public:
	MapData(std::string const &file);
	~MapData();
	inline Vector2 const &Forward(int i) { return points[forward[i]]; }
	inline Vector2 const &Back(int i) { return points[back[i]]; }
	inline int ForwardIndex(int i) { return forward[i]; }
	inline int BackIndex(int i) { return back[i]; }
	Vector2 *points;
	int size;

      private:
	void ReadFile(std::string const &name);
	int *forward;
	int *back;
};