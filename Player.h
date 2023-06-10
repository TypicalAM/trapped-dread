#include <glm/fwd.hpp>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include "CollidableObj.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <vector>

#define INITIAL_CAM_ANGLES glm::vec3(0.f, 90.f, 0.f)

class Player {
private:
  // consts
  float m_sensitivity = 0.1f;
  glm::vec3 initial_cam_pos;
  glm::vec3 m_cam_pos;
  glm::vec3 m_euler_angles;
  float vertical_velocity = 0.0f;

  bool m_fisrt_mouse = false;
  float m_screen_width;
  float m_sceen_height;

  bool allow_fly = true;
  bool is_crouched = false;
  float base_floor_level;

  bool is_on_floor = false;

  glm::vec3 calc_forward();

public:
  Player(float screen_width, float screen_height, glm::vec3 start_pos) {
    initial_cam_pos = start_pos;
    m_cam_pos = initial_cam_pos;
    m_euler_angles = INITIAL_CAM_ANGLES;

    std::cout << "called ctor\n";

    m_screen_width = screen_width;
    m_sceen_height = screen_height;
  }

  void reset_after_resize(float screen_width, float screen_height) {
    m_screen_width = screen_width;
    m_sceen_height = screen_height;
  }

  void toggle_fly() { allow_fly = allow_fly ? false : true; }

  void setCamPos(const glm::vec3 &camPos) { m_cam_pos = camPos; }

  void setFloorLevel(float x) { base_floor_level = x; }

  void moveCam(const glm::vec3 &offset) { m_cam_pos += offset; }

  glm::vec3 getCamPos() { return m_cam_pos; }

  void inc_yaw() {
    // allow_fly = allow_fly ? false : true;
    // std::cout << "noclip is " << allow_fly << std::endl;

    // DEBUG FIXME
    m_euler_angles.y += 5;

    std::cout << "angle is " << m_euler_angles.y << "\n";
  }

  void inc_pitch() {
    // allow_fly = allow_fly ? false : true;
    // std::cout << "noclip is " << allow_fly << std::endl;

    // DEBUG FIXME
    m_euler_angles.x += 5;

    std::cout << "angle is " << m_euler_angles.x << "\n";
  }

  void reset_cam() {
    m_euler_angles = INITIAL_CAM_ANGLES;
    m_cam_pos = initial_cam_pos;
    m_cam_pos.y = base_floor_level;
    vertical_velocity = 0.0f;
  }

  void update(double timeDelta, int movement_mask, float speed,
              const std::vector<std::unique_ptr<CollidableObj>> &other_objs);

  void update_cam_pos(float fw_diff, float right_diff, float up_diff);
  glm::mat4 calcViewMatrix();
  void mouse_move_callback(double xpos, double ypos);

  glm::vec3 getAngles() { return m_euler_angles; }
};
