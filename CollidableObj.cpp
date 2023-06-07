#include "CollidableObj.h"
#include <iostream>


glm::mat4 CollidableObj::calc_base_M(const glm::mat4 &baseM) {
  glm::mat4 newM = glm::translate(baseM, m_position);
  // rotate by all angles
  newM = glm::rotate(newM, glm::radians(m_rotation_per_axis.x),
                     glm::vec3(1.f, 0.f, 0.f));
  newM = glm::rotate(newM, glm::radians(m_rotation_per_axis.y),
                     glm::vec3(0.f, 1.f, 0.f));
  newM = glm::rotate(newM, glm::radians(m_rotation_per_axis.z),
                     glm::vec3(0.f, 0.f, 1.f));

  return newM;
}


bool CollidableObj::hasColided(const glm::vec3& other_pos)
{
    glm::vec3 diff = m_position - other_pos;
    return
        diff.x >= -m_bounding_box_radius.x && diff.x <= m_bounding_box_radius.x &&
        diff.y >= -m_bounding_box_radius.y && diff.y <= m_bounding_box_radius.y &&
        diff.z >= -m_bounding_box_radius.z && diff.z <= m_bounding_box_radius.z;
}


float CollidableObj::slab_intersetion(const glm::vec3 rayStart, const glm::vec3& rayDir) const {
	// algo z wyk³adu


	glm::vec3 low = aabbMin();
	glm::vec3 high = aabbMax();

	float tnear = -FLT_MAX;
	float tfar = FLT_MAX;
	float t1, t2;
	for (int i = 0; i < 3; i++) {
		if (rayDir[i] == 0) {
            if (rayStart[i] < low[i] || rayStart[i] > high[i])
                // not inside the slab
                return 0;
		}
		t1 = (low[i] - rayStart[i]) / rayDir[i];
		t2 = (high[i] - rayStart[i]) / rayDir[i];
		if (t1 > t2) {
			std::swap(t1, t2);
		}
		if (t1 > tnear) {
			tnear = t1;
		}
		if (t2 < tfar) {
			tfar = t2;
		}
        // promien poza pud³em
        if (tnear > tfar || tfar < 0) {
			return 0;
		}

	}
	return tnear;
}


glm::vec3 CollidableObj::modify_cam_pos(const glm::vec3& old_cam_pos, const glm::vec3& new_cam_pos)
{
    auto ray = new_cam_pos - old_cam_pos;
    float t = slab_intersetion(old_cam_pos, ray);



    constexpr float epsilon = 0.0001;
    return old_cam_pos + ray * (t - epsilon);
}



