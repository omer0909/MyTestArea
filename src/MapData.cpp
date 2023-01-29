#include <MapData.hpp>
#include <fstream>
#include <list>
#include <sstream>

MapData::MapData(std::string const &file) { ReadFile(file); }

MapData::~MapData()
{
	delete[] points;
	delete[] forward;
	delete[] back;
}

template <typename T>
static void ListCopyToArray(std::list<T> const &src, T *&arr)
{
	arr = new T[src.size()];
	int i = 0;
	for (T const &a : src) {
		arr[i++] = a;
	}
}

void MapData::ReadFile(std::string const &name)
{
	std::fstream file;

	file.open(name, std::ios_base::in);
	if (file.fail()) {
		std::cerr << name << " file can not read" << std::endl;
	}

	std::list<Vector2> points_list;
	std::string line;
	int counter = 0;
	std::list<int> islands;

	while (std::getline(file, line)) {
		if (!line.size()) {
			islands.push_back(counter);
			counter = 0;
			continue;
		}
		counter++;
		std::stringstream ssline(line);
		Vector2 pos;
		std::string tmp;
		ssline >> tmp;
		pos.x = std::strtof(tmp.c_str(), nullptr);
		ssline >> tmp;
		pos.y = std::strtof(tmp.c_str(), nullptr);
		points_list.push_back(pos * 2);
	}
	islands.push_back(counter);

	ListCopyToArray<Vector2>(points_list, points);
	size = points_list.size();
    
	forward = new int[size];
	back = new int[size];
	int add = 0;
	for (auto i : islands) {
		for (int j = 0; j < i; j++) {
			forward[add + j] = add + ((j + 1) % i);
			back[add + j] = add + ((j - 1 + i) % i);
		}
		add += i;
	}
}
