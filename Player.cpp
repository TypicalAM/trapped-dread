#include "Player.h"
#include "common.h"

#include <iostream>

void Player::mouse_move_callback(double xpos, double ypos) {

  float delta_horizontal =
      static_cast<float>(xpos - static_cast<double>(m_screen_width / 2));
  m_euler_angles.y += delta_horizontal * m_sensitivity; // TODO czemu -= ??

  float delta_vertical =
      static_cast<float>(ypos - static_cast<double>(m_sceen_height / 2));
  m_euler_angles.x -= delta_vertical * m_sensitivity;
  m_euler_angles.x = std::max(std::min(m_euler_angles.x, 89.0f), -89.0f);
}

glm::vec3 Player::calc_forward() {
  glm::vec3 forwards{cos(glm::radians(m_euler_angles.y)) *
                         cos(glm::radians(m_euler_angles.x)),
                     sin(glm::radians(m_euler_angles.x)),
                     sin(glm::radians(m_euler_angles.y)) *
                         cos(glm::radians(m_euler_angles.x))};

  return forwards;
}

void Player::update(
    double timeDelta, int movement_mask, float speed,
    const std::vector<std::unique_ptr<CollidableObj>> &other_objs) {
  // constat for gravity
  constexpr float gravity = 3.;
  constexpr float jump_speed = 1.5;
  constexpr float crouch_slow_down = 0.5;

  float fw_diff = 0.0f;
  float right_diff = 0.0f;
  float up_diff = 0.0f;
  if (movement_mask & MOVE_FORWARD) {
    fw_diff += speed * timeDelta;
  }
  if (movement_mask & MOVE_BACKWARD) {
    fw_diff -= speed * timeDelta;
  }
  if (movement_mask & MOVE_LEFT) {
    right_diff -= speed * timeDelta;
  }
  if (movement_mask & MOVE_RIGHT) {
    right_diff += speed * timeDelta;
  }

  // if both rigth_diff and fw_diff are none zero
  // we need to normalize the vector
  if (fw_diff != 0.0f && right_diff != 0.0f) {
    fw_diff /= sqrt(2.0f);
    right_diff /= sqrt(2.0f);
  }

  if (allow_fly) {
    if (movement_mask & MOVE_UP) {
      up_diff += speed * timeDelta;
    }
    if (movement_mask & MOVE_DOWN) {
      up_diff -= speed * timeDelta;
    }

    // early return in fly mode
    // when in noclip - no collision checks
    update_cam_pos(fw_diff, right_diff, up_diff);
    return;
  }

  // use the space for jumping
  // UP dierction is now global up, not camaera up

  // check if we are on the ground
  if (is_on_floor) {

    // m_cam_pos.y = base_floor_level; // reset if we clipped through
    if (movement_mask & MOVE_UP) {
      // preform jump
      vertical_velocity = jump_speed;
      is_on_floor = false;
    } else {
      vertical_velocity = 0.0f;
    }
  } else { // in the air
    vertical_velocity -= gravity * timeDelta;
  }

  // update vertical position
  up_diff += vertical_velocity * timeDelta;
  std::cout << "vertical_velocity: " << vertical_velocity << std::endl;

  glm::vec3 old_cam_pos = m_cam_pos;

  update_cam_pos(fw_diff, right_diff, 0);
  m_cam_pos.y += vertical_velocity * timeDelta;

  for (auto &other_obj : other_objs) {

    std::cout << "other object type: " << typeid(*other_obj).name()
              << std::endl;

    std::array<std::pair<bool, bool>, 3> collisions =
        other_obj->calc_colision(m_cam_pos);

    if (collisions[0].first || collisions[0].second) {
      std::cout << "collision detected X" << std::endl;
      m_cam_pos.x = old_cam_pos.x;
    }

    // hit the floor
    if (collisions[1].first) {
      std::cout << "collision detected Y floor" << std::endl;
      m_cam_pos.y = old_cam_pos.y;
      vertical_velocity = 0.0f;
      is_on_floor = true;
    }

    // hit the roof
    if (collisions[1].second) {
      std::cout << "collision detected Y ceiling" << std::endl;

      // detect this as allowing for jump
      m_cam_pos.y = old_cam_pos.y;
      vertical_velocity *= -1;
    }

    if (collisions[2].first || collisions[2].second) {
      std::cout << "collision detected Z" << std::endl;
      m_cam_pos.z = old_cam_pos.z;
    }
  }
}

void Player::update_cam_pos(float fw_diff, float right_diff, float up_diff) {
  auto forwards = calc_forward();

  if (!allow_fly) {
    // set the y component to 0
    forwards.y = 0.0f;
    forwards = glm::normalize(forwards);
  }

  glm::vec3 globalUp(0.0f, 1.0f, 0.0f);
  glm::vec3 right(glm::cross(forwards, globalUp));
  glm::vec3 up(glm::cross(right, forwards));

  // forward backward
  m_cam_pos += fw_diff * glm::normalize(forwards);

  // left rigth
  m_cam_pos += right_diff * glm::normalize(glm::cross(forwards, up));

  // up down
  m_cam_pos += up_diff * glm::normalize(up);
}

glm::mat4 Player::calcViewMatrix() {

  auto forwards = calc_forward();

  glm::vec3 globalUp(0.0f, 1.0f, 0.0f);
  glm::vec3 right{glm::cross(forwards, globalUp)};
  glm::vec3 up{glm::cross(right, forwards)};

  return glm::lookAt(m_cam_pos, m_cam_pos + forwards, up);
}