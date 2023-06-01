#include "Map.h"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string Map::pretty_print() {
  std::stringstream ss;

	std::cout << "jestme w prety print" << std::endl;
	std::cout << internal_map.size() << std::endl;
	std::cout << internal_map[0].size() << std::endl;
	std::cout << internal_map[0][0].size() << std::endl;

  // Left to right, bottom to top
  for (int layer = 0; layer < internal_map[0][0].size(); layer++) {
    ss << "The layer is " << layer << "\n";
    for (int x = 0; x < internal_map[0].size(); x++) {
      for (int y = 0; y < internal_map.size(); y++) {
        switch (internal_map[x][y][layer]) {
        case MapObject::START_ALTAR:
          ss << "S";
          break;
        case MapObject::END_ALTAR:
          ss << "E";
          break;
        case MapObject::WALL:
          ss << "W";
          break;
        default:
          ss << " ";
          break;
        }

        ss << internal_map[x][y][layer];
      }
      ss << "\n";
    }
    ss << "\n";
  }

  return ss.str();
}

std::vector<std::vector<std::vector<MapObject>>> Map::get_map() {
  return internal_map;
}

std::pair<int, int> Map::get_start_position() { return start_position; }

std::pair<int, int> Map::get_end_position() { return end_position; }

Map::Map(std::vector<std::vector<std::vector<MapObject>>> map) {
  internal_map = map;
  for (int i = 0; i < map.size(); i++) {
    for (int j = 0; j < map[i].size(); j++) {
      for (int k = 0; k < map[i][j].size(); k++) {
        if (map[i][j][k] == MapObject::START_ALTAR) {
          this->start_position = std::make_pair(i, j);
        } else if (map[i][j][k] == MapObject::END_ALTAR) {
          this->end_position = std::make_pair(i, j);
        }
      }
    }
  }

  layers = map[0][0].size();
}
