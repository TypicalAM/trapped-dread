#include "Map.h"
#include "MapLoader.h"
#include <iterator>
#include <map>
#include <optional>
#include <string>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"

#include "Player.h"
#include "common.h"

#include "Floor.h"

float speed_forward = 0;
float speed_right = 0;
float speed_up = 0;

// encode movement using bit mask
// 0b0000 0000
// 0b0000 0001 - forward
// 0b0000 0010 - backward
// 0b0000 0100 - left
// 0b0000 1000 - right
// 0b0001 0000 - up
// 0b0010 0000 - down
// 0b0100 0000 - fly

int movement_mask = 0;

// added
float walk_speed = 2.f;

float aspectRatio = 1;

float INITIAL_HEIGHT = 1000;
float INITAL_WIDTH = 1000;

bool can_fly = true;

ShaderProgram *sp;
Player *camera_ptr; // singleton troche - mozna to lepiej napisac
std::vector<std::unique_ptr<CollidableObj>> GameObjects;

// Procedura obsługi błędów
void error_callback(int error, const char *description) {
  fputs(description, stderr);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if (action == GLFW_PRESS) {
    if (key == GLFW_KEY_W)
      movement_mask |= MOVE_FORWARD;
    if (key == GLFW_KEY_S)
      movement_mask |= MOVE_BACKWARD;
    if (key == GLFW_KEY_A)
      movement_mask |= MOVE_LEFT;
    if (key == GLFW_KEY_D)
      movement_mask |= MOVE_RIGHT;
    if (key == GLFW_KEY_SPACE)
      movement_mask |= MOVE_UP;
    if (key == GLFW_KEY_LEFT_CONTROL)
      movement_mask |= MOVE_DOWN;
    if (key == GLFW_KEY_T)
      camera_ptr->toggle_fly();
    if (key == GLFW_KEY_EQUAL)
      camera_ptr->inc_pitch(); // toggle ala noclip
    if (key == GLFW_KEY_KP_SUBTRACT)
      camera_ptr->inc_yaw(); // toggle ala noclip
    if (key == GLFW_KEY_0)
      camera_ptr->reset_cam(); // toggle ala noclip
  }

  if (action == GLFW_RELEASE) {
    if (key == GLFW_KEY_W)
      movement_mask &= ~MOVE_FORWARD;
    if (key == GLFW_KEY_S)
      movement_mask &= ~MOVE_BACKWARD;
    if (key == GLFW_KEY_A)
      movement_mask &= ~MOVE_LEFT;
    if (key == GLFW_KEY_D)
      movement_mask &= ~MOVE_RIGHT;
    if (key == GLFW_KEY_SPACE)
      movement_mask &= ~MOVE_UP;
    if (key == GLFW_KEY_LEFT_CONTROL)
      movement_mask &= ~MOVE_DOWN;
  }
}

void windowResizeCallback(GLFWwindow *window, int width, int height) {

  INITAL_WIDTH = width;
  INITIAL_HEIGHT = height;

  camera_ptr->reset_after_resize(width, height);

  if (height == 0)
    return;
  aspectRatio = (float)width / (float)height;
  glViewport(0, 0, width, height);
}

void mouse_move_callback(GLFWwindow *window, double xpos, double ypos) {

  // reset cursor to screen center
  glfwSetCursorPos(window, static_cast<double>(INITAL_WIDTH / 2),
                   static_cast<double>(INITIAL_HEIGHT / 2));

  camera_ptr->mouse_move_callback(xpos, ypos);
}

// Procedura inicjująca
void initOpenGLProgram(GLFWwindow *window) {
  //************Tutaj umieszczaj kod, który należy wykonać raz, na początku
  // programu************
  glClearColor(0, 0, 0, 1);
  glEnable(GL_DEPTH_TEST);

  glfwSetWindowSizeCallback(window, windowResizeCallback);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetCursorPosCallback(window, mouse_move_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

  sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
}

// Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow *window) {
  //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli
  // głównej************
  delete camera_ptr;
  delete sp;
}

// alters global `GameObjects`
void setupInitialPositionsOfObjects(GameMap &map) {
  // GameObjects.push_back(std::unique_ptr<Floor>(new Floor(0.4f)));
  GameObjects.push_back(map.gen_floor());
  for (auto &wall : map.gen_walls())
    GameObjects.push_back(std::move(wall));

  // GameObjects.push_back(std::unique_ptr<CollidableTeapot>(
  //    new CollidableTeapot(glm::vec3(0, 2.f, 0))));
  //
}

// Procedura rysująca zawartość sceny
void drawScene(GLFWwindow *window) {
  //************Tutaj umieszczaj kod rysujący obraz******************l
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 V = camera_ptr->calcViewMatrix();

  glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f,
                                 50.0f); // Wylicz macierz rzutowania

  glm::mat4 M = glm::mat4(1.0f);

  sp->use(); // Aktywacja programu cieniującego

  glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
  glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
  glUniform1f(sp->u("divisor"), 3.2);

  // draw all the game objects
  // for now just floor
  for (auto &obj : GameObjects) {
    obj->draw(M, sp);
  }

  glfwSwapBuffers(window); // Przerzuć tylny bufor na przedni
}

int main(void) {
  auto map = (new MapLoader("example.map"))->load_map();
  if (!map.has_value()) {
    fprintf(stderr, "Nie można załadować mapy.\n");
    exit(EXIT_FAILURE);
  }

  std::cout << "Mapa załadowana poprawnie." << std::endl;
  std::cout << map->pretty_print() << std::endl;

  // Try to load
  GLFWwindow *window; // Wskaźnik na obiekt reprezentujący okno

  glfwSetErrorCallback(error_callback); // Zarejestruj procedurę obsługi błędów

  if (!glfwInit()) { // Zainicjuj bibliotekę GLFW
    fprintf(stderr, "Nie można zainicjować GLFW.\n");
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(
      INITAL_WIDTH, INITIAL_HEIGHT, "OpenGL", NULL,
      NULL); // Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

  if (!window) // Jeżeli okna nie udało się utworzyć, to zamknij program
  {
    fprintf(stderr, "Nie można utworzyć okna.\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  camera_ptr =
      new Player(INITAL_WIDTH, INITIAL_HEIGHT, map->calc_player_start_pos());
  camera_ptr->moveCam(glm::vec3(0.f, 0.4f, 0.f));
  camera_ptr->setFloorLevel(0.4f);

  glfwMakeContextCurrent(
      window); // Od tego momentu kontekst okna staje się aktywny i polecenia
               // OpenGL będą dotyczyć właśnie jego.
  glfwSwapInterval(
      1); // Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

  if (glewInit() != GLEW_OK) { // Zainicjuj bibliotekę GLEW
    fprintf(stderr, "Nie można zainicjować GLEW.\n");
    exit(EXIT_FAILURE);
  }

  initOpenGLProgram(window); // Operacje inicjujące

  setupInitialPositionsOfObjects(map.value());

  // Główna pętla
  glfwSetTime(0); // Zeruj timer
  while (!glfwWindowShouldClose(
      window)) // Tak długo jak okno nie powinno zostać zamknięte
  {
    camera_ptr->update(glfwGetTime(), movement_mask, walk_speed, GameObjects);

    glfwSetTime(0);    // Zeruj timer
    drawScene(window); // Wykonaj procedurę rysującą

    glfwPollEvents(); // Wykonaj procedury callback w zalezności od zdarzeń
                      // jakie zaszły.
  }

  freeOpenGLProgram(window);

  glfwDestroyWindow(window); // Usuń kontekst OpenGL i okno
  glfwTerminate();           // Zwolnij zasoby zajęte przez GLFW
  exit(EXIT_SUCCESS);
}
