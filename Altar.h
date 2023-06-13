#pragma once

#include "CollidableObj.h"
#include <glm/fwd.hpp>
#include <vector>

#define ALTAR_SCALAR glm::vec3(0.1f, 0.05f, 0.1f)
#define ALTAR_OFFSET_Y -0.01f

const glm::vec3 ALTAR_PLACE_BOX(0.5f, 0.5f, 0.5f);

enum AltarType { START, END };
enum AltarColor { BLUE_ALTAR, RED_ALTAR };

class Altar : public CollidableObj {
private:
  AltarType type;
  AltarColor color;
  int x_pos;
  int y_pos;

  std::vector<glm::vec4> vertices;
  std::vector<glm::vec4> colors;
  std::vector<glm::vec4> normals;
  std::vector<glm::vec2> texCoords;
  unsigned int numVertices;

public:
  Altar(int x_pos, int y_pos, AltarType type, AltarColor color);
  virtual ~Altar() {}
  virtual void draw(const glm::mat4 &baseM, ShaderProgram *sp) override;
  bool can_place_skull(const glm::vec3 &player_pos) override;
  AltarColor get_color() { return color; }
  AltarType get_type() { return type; }
  std::pair<int, int> get_pos() { return std::make_pair(x_pos, y_pos); }
};
