#pragma once
#include "CollidableObj.h"
#include "common.h"
#include "myTeapot.h"

class CollidableTeapot: public CollidableObj
{
public:
	CollidableTeapot(glm::vec3 init_pos) :
		CollidableObj(init_pos, glm::vec3(0, 0, 0), glm::vec3(2, 2, 2)) {}
	virtual ~CollidableTeapot() {}
	
	virtual void draw(const glm::mat4& baseM, ShaderProgram* sp) override;
};

