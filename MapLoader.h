#pragma once

#include "Map.h"
#include <algorithm>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class MapLoader {
private:
  std::string filename;
  bool file_accessible(); // Check if the file is accessible and readable
  bool file_correct();    // Check if the file really is a map
  GameMap parse_file();   // Load the map from the file

public:
  MapLoader(std::string filename);
  std::optional<GameMap> load_map(); // Load the map from the file
};
