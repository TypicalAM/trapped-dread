#include "Map.h"
#include "Floor.h"
#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

std::string GameMap::pretty_print() {
  std::stringstream ss;

  // Left to right, bottom to top
  for (int layer = 0; layer < internal_map.size(); layer++) {
    ss << "The layer is " << layer << "\n";
    for (int y = 0; y < internal_map[0].size(); y++) {
      for (int x = 0; x < internal_map[0][0].size(); x++) {
        switch (internal_map[layer][y][x]) {
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
          ss << "0";
          break;
        }
      }
      ss << "\n";
    }
  }

  return ss.str();
}

Map3D GameMap::get_map() { return internal_map; }

std::pair<int, int> GameMap::get_start_position() { return start_position; }

std::pair<int, int> GameMap::get_end_position() { return end_position; }

std::unique_ptr<Floor> GameMap::gen_floor() {
  int width = internal_map[0][0].size();
  int length = internal_map[0].size();
  return std::make_unique<Floor>(0.4f, width, length);
}

GameMap::GameMap(Map3D map) {
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
