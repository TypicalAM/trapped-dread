#pragma once

// set up these constants 
// encode movement using bit mask
// 0b0000 0000
// 0b0000 0001 - forward
// 0b0000 0010 - backward
// 0b0000 0100 - left
// 0b0000 1000 - right
// 0b0001 0000 - up
// 0b0010 0000 - down
// 0b0100 0000 - fly

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>

#define MOVE_FORWARD 0x01
#define MOVE_BACKWARD 0x02
#define MOVE_LEFT 0x04
#define MOVE_RIGHT 0x08
#define MOVE_UP 0x10
#define MOVE_DOWN 0x20
#define MOVE_FLY 0x40


