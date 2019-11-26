
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
  // #include <glm/vec4.hpp>
  // #include <glm/mat4x4.hpp>
#include <glm/ext.hpp>
  // #include <glm/ext/matrix_transform.hpp>
  // #include <glm/ext/matrix_clip_space.hpp>
  // #include <glm/ext/constants.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Camera.h>
#include <Shader.h>

#include <Grid.h>
#include <NormalDistribution.h>

using namespace std;

void initGL();
bool initGlad();
GLFWwindow *createWindow (int, int, string, int, int);

int win_frame[4] = {1320, 780, 0, 0};

int main () {

  initGL();
  GLFWwindow *window = createWindow(win_frame[0], win_frame[1], "Marbles", win_frame[2], win_frame[3]);
  if (!window || !initGlad())
    return -1;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  float triangle[] = {
    -0.5, -0.5,
     0.7,  0.7,
     1.0,  0.0
  };

  unsigned int vao, vbo;
  { // Load triangle model into vertex array
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 3*2*sizeof(float), triangle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
  }

  string project_dir = "/home/henry/Desktop/Marbles";
  //string project_dir = "/home/lil/Desktop/NormalDistribution";
  Shader shader(project_dir+"/shaders/triangle_vert.glsl", project_dir+"/shaders/triangle_frag.glsl");

  float fov = 3.14159f/2, near = 0.2f, far = 10000.0f;
  glm::mat4 perspective =
  // glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
  glm::perspective(fov, (float)win_frame[0]/(float)win_frame[1], near, far);
  glm::vec3 cam_pos (0,0,10);
  glm::vec3 cam_dir (0,0,-1);
  glm::mat4 view = glm::lookAt(
    cam_pos,
    cam_pos+cam_dir,
    glm::vec3(0,1,0)
  );
  glm::mat4 model(1.0f);
  glm::mat4 local(1.0f);

  shader.use();
  shader.setPerspective(perspective);
  shader.setView(view);
  shader.setModel(model);
  shader.setLocal(local);

  // Background unit grid
  Grid grid;
  // Simulation
  NormalDistribution simulation (5);
  bool tick_stop = false;

  // Track cursor movement
  double cursor_prev[2] = {win_frame[0]/2.0, win_frame[1]/2.0}, cursor_pos[2];
  glfwGetCursorPos(window, cursor_prev, cursor_prev+1);
  double cam_yaw = -3.14159/2, cam_pitch;

  bool control = false;
  bool nums[10] = {false};
  bool nums_last[10] = {false};

  int count = 0;
  glClearColor(0.3, 0.1, 0.5, 1);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(window)){
    if (glfwGetKey(window, GLFW_KEY_C) != GLFW_PRESS)
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view;
    {
    cam_pos += .1f * glm::vec3(
      glfwGetKey(window, GLFW_KEY_D) - glfwGetKey(window, GLFW_KEY_A),
      glfwGetKey(window, GLFW_KEY_SPACE) - glfwGetKey(window, GLFW_KEY_LEFT_SHIFT),
      glfwGetKey(window, GLFW_KEY_S) - glfwGetKey(window, GLFW_KEY_W)
    );

    glfwGetCursorPos(window, cursor_pos, cursor_pos+1);
    cam_yaw   +=  0.01f*(cursor_pos[0] - cursor_prev[0]);
    cam_pitch += -0.01f*(cursor_pos[1] - cursor_prev[1]);
    cursor_prev[0] = cursor_pos[0];
    cursor_prev[1] = cursor_pos[1];

    cam_dir = {
      cosf(cam_pitch)*cosf(cam_yaw),
      sinf(cam_pitch),
      cosf(cam_pitch)*sinf(cam_yaw)
    };
    view = glm::lookAt(
      cam_pos,
      cam_pos+cam_dir,
      glm::vec3(0,1,0)
    );
    }

    { // Controls
      for (int i=1; i<10; i++)
        nums_last[i] = nums[i];

      // GLFW_KEY_# for 0 <= # <= 9 equals 48+#
      control = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
      for (int i=1; i<10; i++)
        nums[i] = glfwGetKey(window, GLFW_KEY_0 + i) == GLFW_PRESS;
    }

    shader.use();
    shader.setView(view);
    grid.Render(shader);
    simulation.Render(shader);

    do { // Simulation Running
      if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        simulation.tick();
        break;
      }

      if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        if (!tick_stop)
        simulation.tick();
        tick_stop = true;
        break;
      } else
        tick_stop = false;

      for (int i=1; i<10; i++) {
        if (!nums_last[i] && nums[i]) {
          for (int t=0; t<i; t++)
            simulation.tick();
          break;
        } else if (control) {
          if (nums[i]) {
            for (int t=0; t<i; t++)
              simulation.tick();
            break;
          }
        } else {
          if (nums[i]) {
            if (count % i == 0)
              simulation.tick();
            break;
          }
        }
      }

    } while(false);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, GL_TRUE);

    glfwSwapBuffers(window);
    glfwPollEvents();
    count++;
    // Just check for overflow, silently fix it.
    if (count > 2000000000)
      count = 0;
  }
}

void initGL () {
  glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);
  glfwSwapInterval(0);
}

bool initGlad () {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cerr << "Failed to initialize GLAD" << endl;
    return false;
  }
  return true;
}

void framebufferSizeCallback (GLFWwindow *win, int width, int height) {
  win_frame[0] = width;
  win_frame[1] = height;
	glViewport(20, 20, win_frame[0]-40, win_frame[1]-40);
}

GLFWwindow *createWindow (int win_width, int win_height, string win_name, int win_x, int win_y) {

  GLFWwindow *win = glfwCreateWindow (win_width, win_height, win_name.c_str(), NULL, NULL);

  if (win) {
    glfwSetWindowPos(win, win_x, win_y);
    glfwSetFramebufferSizeCallback(win, framebufferSizeCallback);
    glfwMakeContextCurrent(win);
  } else
    cerr << "Failed to create window!" << endl;

  return win;
}
