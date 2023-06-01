#pragma once

#include <string>
#include <vector>

// MapObject is a enum class that represents a type of object on the map
enum MapObject {
	EMPTY,
  WALL,
  START_ALTAR,
  END_ALTAR,
};

// Map is a class that represents a map
class Map {
private:
  std::vector<std::vector<std::vector<MapObject>>>
      internal_map;                   // 3d vector of map objects
  std::pair<int, int> start_position; // start position
  std::pair<int, int> end_position;   // end position
  int layers;

public:
  Map(std::vector<std::vector<std::vector<MapObject>>> map);
  std::string pretty_print();
  std::vector<std::vector<std::vector<MapObject>>>
  get_map();                                // get the 3d vector of map objects
  std::pair<int, int> get_start_position(); // get the start position
  std::pair<int, int> get_end_position();   // get the end position
};
