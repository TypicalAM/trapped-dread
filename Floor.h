#pragma once

#include "CollidableObj.h"
#include "common.h"

class Floor : public CollidableObj {
private:
  int width;
  int length;
  float bb_thicknes = 0.2f; // defacto how tall the player is

public:
  Floor(float floor_level, int width, int height);
  virtual ~Floor() {}
  void setFloorLevel(float newLevel) { m_position.y = newLevel; }
  float getFloorLevel() { return m_position.y; }
  virtual void draw(const glm::mat4 &baseM, ShaderProgram *sp) override;
  virtual bool hasColided(const glm::vec3 &other_pos) override;

  virtual glm::vec3 modify_cam_pos(const glm::vec3 &old_cam_pos,
                                   const glm::vec3 &new_cam_pos);
};
