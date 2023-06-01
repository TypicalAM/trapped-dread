#pragma once

#include "common.h"
#include "CollidableObj.h"

class Floor: public CollidableObj
{
public:
	Floor(float floor_level): 
		CollidableObj(glm::vec3(0, floor_level, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0)) {}
	virtual ~Floor(){}
	void setFloorLevel(float newLevel) {
		m_position.y = newLevel;
	}
	float getFloorLevel() {
		return m_position.y;
	}
	virtual std::array<std::pair<bool,bool>, 3> calc_colision(const glm::vec3& other_pos) override;
	virtual void draw(const glm::mat4& baseM, ShaderProgram* sp) override;
};

