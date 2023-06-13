#pragma once

#include "CollidableObj.h"
#include <glm/fwd.hpp>
#include <vector>

#define SKULL_SCALAR glm::vec3(0.2f, 0.2f, 0.2f)
#define SKULL_OFFSET_Y 0.44f
const glm::vec3 SKULL_GRAB_BOX(0.5f, 0.5f, 0.5f);

class Skull : public CollidableObj {
private:
  int x_pos;
  int y_pos;
  Color color;

  std::vector<glm::vec4> vertices;
  std::vector<glm::vec4> colors;
  std::vector<glm::vec4> normals;
  std::vector<glm::vec2> texCoords;
  unsigned int numVertices;

public:
  Skull(int x_pos, int y_pos, Color color);
  ~Skull() {}
  void draw(const glm::mat4 &baseM, ShaderProgram *sp) override;
  bool can_grab(const glm::vec3 &player_pos) override;
  Color get_color() { return color; }
};
