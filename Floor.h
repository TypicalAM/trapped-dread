#pragma once

#include "CollidableObj.h"
#include "common.h"

class Floor : public CollidableObj {
private:
  int width;
  int length;
  std::pair<int, int> start_pos;

public:
  Floor(float floor_level, int width, int height,
        std::pair<int, int> start_pos);
  virtual ~Floor() {}
  void setFloorLevel(float newLevel) { m_position.y = newLevel; }
  float getFloorLevel() { return m_position.y; }
  virtual std::array<std::pair<bool, bool>, 3>
  calc_colision(const glm::vec3 &other_pos) override;
  virtual void draw(const glm::mat4 &baseM, ShaderProgram *sp) override;
};
