#include "Altar.h"
#include "Map.h"
#include "MapLoader.h"
#include <algorithm>

#include <memory>
#include <vector>

#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"

#include "Floor.h"
#include "LightSource.h"
#include "Player.h"
#include "Skull.h"
#include "common.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

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
float walk_speed = 3.f;
float aspectRatio = 1;

float INITIAL_HEIGHT = 1000;
float INITAL_WIDTH = 1000;

ShaderProgram *sp;
Player *camera_ptr;

GLuint wallTexture;
GLuint floorTexture;
GLuint ceilingTexture;
GLuint skullTexture;
GLuint altarTexture;
GLuint catTexture;

std::vector<std::unique_ptr<CollidableObj>> GameObjects;
std::vector<std::unique_ptr<CollidableObj>> heldGameObjects;
std::vector<std::unique_ptr<PointLightSource>> StationaryPointLightSourcess;
std::vector<std::unique_ptr<ConeLightSource>> StatonaryConeLightSources;

std::vector<float> cone_light_loc;
std::vector<float> cone_light_colors;

// Check if we can grab a skull and grab it
bool can_grab_skull() {
  std::cout << "we can grab an object" << std::endl;
  glm::vec3 player_pos = camera_ptr->get_position();
  for (int i = 0; i < GameObjects.size(); i++) {
    if (!GameObjects[i]->can_grab(player_pos)) {
      continue;
    }

    // Move the skull from the map to the player's hand
    heldGameObjects.push_back(std::move(GameObjects[i]));
    heldGameObjects.back()->set_pos(glm::vec3(0, SKULL_OFFSET_Y, 0));
    GameObjects.erase(GameObjects.begin() + i);
    return true;
  }

  return false;
}

// Check if we can place a skull on a matching end altar
bool can_place_skull() {
  std::cout << "we have an object in our hand" << std::endl;
  glm::vec3 player_pos = camera_ptr->get_position();
  for (int i = 0; i < GameObjects.size(); i++) {
    if (!GameObjects[i]->can_place_skull(player_pos)) {
      continue;
    }

    std::cout << "we can place an object" << std::endl;
    auto altar = dynamic_cast<Altar *>(GameObjects[i].get());
    if (altar->get_type() != ALTAR_END)
      continue;

    auto altar_pos = altar->get_pos();
    auto skull = dynamic_cast<Skull *>(heldGameObjects[0].get());

    if (altar->get_color() == skull->get_color()) {
      GameObjects.push_back(std::move(heldGameObjects[0]));
      GameObjects.back()->set_pos(
          glm::vec3(altar_pos.x, SKULL_OFFSET_Y, altar_pos.y));
      heldGameObjects.erase(heldGameObjects.begin());
      return true;
    }

    if (heldGameObjects.size() != 2)
      return false;

    auto skull2 = dynamic_cast<Skull *>(heldGameObjects[1].get());
    if (altar->get_color() == skull2->get_color()) {
      skull2->get_color() == BLUE ? camera_ptr->place_blue_skull()
                                  : camera_ptr->place_red_skull();
      GameObjects.push_back(std::move(heldGameObjects[1]));
      GameObjects.back()->set_pos(
          glm::vec3(altar_pos.x, SKULL_OFFSET_Y, altar_pos.y));
      heldGameObjects.erase(heldGameObjects.begin() + 1);
      return true;
    }
  }

  return false;
}

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
    if (key == GLFW_KEY_E) {
      if (heldGameObjects.size() >= 1 && can_place_skull())
        return;

      if (heldGameObjects.size() <= 1 && can_grab_skull())
        return;
    }
  }
}

GLuint readTexture(const char *filename) {
  GLuint tex;
  glActiveTexture(GL_TEXTURE0);
  std::vector<unsigned char> image;
  unsigned width, height;
  unsigned error = lodepng::decode(image, width, height, filename);
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               (unsigned char *)image.data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  return tex;
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

  catTexture = readTexture("assets/textures/cat.png");
  altarTexture = readTexture("assets/textures/altar.png");
  skullTexture = readTexture("assets/textures/marble.png");
  floorTexture = readTexture("assets/textures/floor.png");
  wallTexture = readTexture("assets/textures/wall.png");

  sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
}

// Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow *window) {
  //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu
  // pętli
  // głównej************
  delete camera_ptr;
  delete sp;
}

// alters global `GameObjects`
void setupInitialPositionsOfObjects(GameMap &map) {
  auto floor = map.gen_floor();
  floor->bindTexture(floorTexture);
  GameObjects.push_back(std::move(floor));

  auto exit = map.gen_exit();
  exit->bindTexture(catTexture);
  GameObjects.push_back(std::move(exit));

  for (auto &wall : map.gen_walls()) {
    wall->bindTexture(wallTexture);
    GameObjects.push_back(std::move(wall));
  }

  for (auto &skull : map.gen_skulls()) {
    skull->bindTexture(skullTexture);
    GameObjects.push_back(std::move(skull));
  }

  for (auto &altars : map.gen_altars()) {
    altars->bindTexture(altarTexture);
    auto pos = altars->get_pos();

    // set light position
    cone_light_loc.push_back(pos.x);
    cone_light_loc.push_back(-1.0f);
    cone_light_loc.push_back(pos.z);
    cone_light_loc.push_back(1.0f);

    bool is_red_light = dynamic_cast<Altar *>(altars.get())->get_color() == RED;

    // set light color
    cone_light_colors.push_back(is_red_light ? 1.0f : 0.0f);
    cone_light_colors.push_back(0.0f);
    cone_light_colors.push_back(is_red_light ? 0.0f : 1.0f);
    cone_light_colors.push_back(1.0f);

    GameObjects.push_back(std::move(altars));
  }
}

void drawScene(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 V = camera_ptr->calcViewMatrix();
  glm::mat4 P =
      glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 50.0f);
  glm::mat4 M = glm::mat4(1.0f);

  sp->use();

  glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
  glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
  glUniform1f(sp->u("divisor"), 3.2);
  glUniform1f(sp->u("cutoffin"), 0.9f);
  glUniform1f(sp->u("cutoffout"), 0.8f);

  glUniform4fv(sp->u("coneLightpositions"), 4, cone_light_loc.data());
  glUniform4fv(sp->u("coneLightcolors"), 4, cone_light_colors.data());

  auto player_pos = camera_ptr->get_position();
  for (auto &obj : GameObjects) {
    float distance = glm::length(player_pos - obj->get_pos());
    bool is_floor = typeid(*obj) == typeid(Floor);
    if (!is_floor && distance > 7.0f)
      continue;

    obj->draw(M, sp);
  }

  for (int i = 0; i < heldGameObjects.size(); i++) {
    glm::mat4 M2 = glm::translate(M, camera_ptr->getCamPos());
    float angle = glm::radians(-1 * camera_ptr->getAngles().y);

    M2 = glm::rotate(M2, angle + 1.4f + -0.6f * i, glm::vec3(0.f, 1.f, 0.f));
    M2 = glm::translate(M2, glm::vec3(0.2f, -0.4f, 0.4f));
    M2 = glm::scale(M2, glm::vec3(0.5));
    M2 = glm::rotate(M2, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));

    heldGameObjects[i]->draw(M2, sp);
  }

  glfwSwapBuffers(window);
}

int main(int argc, char **argv) {
  std::string map_name = argc != 2 ? "example" : argv[1];
  std::string map_path = "assets/maps/" + map_name + ".map";

  auto map = (new MapLoader(map_path))->load_map();
  if (!map.has_value()) {
    fprintf(stderr, "Nie można załadować mapy.\n");
    exit(EXIT_FAILURE);
  }

  std::cout << "Mapa załadowana poprawnie." << std::endl;
  std::cout << map->pretty_print() << std::endl;

  GLFWwindow *window;

  glfwSetErrorCallback(error_callback);

  if (!glfwInit()) {
    fprintf(stderr, "Nie można zainicjować GLFW.\n");
    exit(EXIT_FAILURE);
  }

  window = glfwCreateWindow(INITAL_WIDTH, INITIAL_HEIGHT, "OpenGL", NULL, NULL);

  if (!window) {
    fprintf(stderr, "Nie można utworzyć okna.\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  camera_ptr =
      new Player(INITAL_WIDTH, INITIAL_HEIGHT, map->calc_player_start_pos());
  camera_ptr->moveCam(glm::vec3(0.f, 0.4f, 0.f));
  camera_ptr->setFloorLevel(0.4f);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Nie można zainicjować GLEW.\n");
    exit(EXIT_FAILURE);
  }

  initOpenGLProgram(window);

  setupInitialPositionsOfObjects(map.value());

  glfwSetTime(0);
  while (!glfwWindowShouldClose(window)) {
    camera_ptr->update(glfwGetTime(), movement_mask, walk_speed, GameObjects);
    glfwSetTime(0);
    drawScene(window);
    glfwPollEvents();
  }

  freeOpenGLProgram(window);
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
