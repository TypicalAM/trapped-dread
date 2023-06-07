#pragma once
#include "common.h"
#include "shaderprogram.h"
#include <array>


// pure virtual class for objects that support collisions
class CollidableObj {
protected:
  glm::vec3 m_position;
  glm::vec3 m_rotation_per_axis;   // how to rotate the obj when drawing
  glm::vec3 m_bounding_box_radius; // width , height, depth - everything is a
                                   // `prostopad³oœcian`

  // priatve helper functions for aabb collision algo
  float slab_intersetion( const glm::vec3 rayStart, const glm::vec3& rayDir) const;
  virtual glm::vec3 aabbMin() const { return m_position - m_bounding_box_radius; }
  virtual glm::vec3 aabbMax() const { return m_position + m_bounding_box_radius; }

public:
  CollidableObj(glm::vec3 init_pos, glm::vec3 init_rot,
                glm::vec3 bounding_box_radius)
      : m_position(init_pos), m_rotation_per_axis(init_rot),
        m_bounding_box_radius(bounding_box_radius) {}
  virtual ~CollidableObj() {}

  // most likely translae M by m_pos, rot by m_rot and then draw like normal -
  // possibly relode old M into shader
  virtual void draw(const glm::mat4 &baseM, ShaderProgram *sp) = 0;
  virtual glm::mat4 calc_base_M(const glm::mat4 &baseM);
  
  // returns true if this object has colided with point other_pos
  virtual bool hasColided(const glm::vec3& other_pos);

  // returns new camera position after collision
  virtual glm::vec3 modify_cam_pos(const glm::vec3 & old_cam_pos, const glm::vec3& new_cam_pos );

};
