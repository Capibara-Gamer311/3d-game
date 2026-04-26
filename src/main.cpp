#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>

const unsigned int WIDTH = 1000;
const unsigned int HEIGHT = 700;

float playerX = 0.0f;
float speed = 0.08f;
float gameSpeed = 0.05f;
bool gameOver = false;
float score = 0;

struct Obstacle {
    float x, z;
};

std::vector<Obstacle> obstacles;

void spawnObstacle() {
    Obstacle o;
    o.x = (rand() % 6 - 3);
    o.z = -30.0f;
    obstacles.push_back(o);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (!gameOver) {
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) playerX -= speed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) playerX += speed;
    }
}

void drawCube(float x,float y,float z,float s,float r,float g,float b) {
    glColor3f(r,g,b);
    glPushMatrix();
    glTranslatef(x,y,z);
    glutWireCube(s); // simpele cube
    glPopMatrix();
}

bool checkCollision(float px,float pz,float ox,float oz) {
    return fabs(px-ox)<1.0f && fabs(pz-oz)<1.0f;
}

int main() {
    srand(time(0));

    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Cube Runner 3D", NULL, NULL);
    glfwMakeContextCurrent(window);

    glEnable(GL_DEPTH_TEST);
    spawnObstacle();

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(70, (float)WIDTH/HEIGHT, 0.1, 100);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,5,10, 0,0,0, 0,1,0);

        // vloer
        glBegin(GL_QUADS);
        glColor3f(0.2f,0.7f,0.3f);
        glVertex3f(-20,-1,-100);
        glVertex3f(20,-1,-100);
        glVertex3f(20,-1,10);
        glVertex3f(-20,-1,10);
        glEnd();

        // speler
        drawCube(playerX,0,0,1, 0,0,1);

        if (!gameOver) {
            for (auto &o : obstacles) {
                o.z += gameSpeed;
                if (checkCollision(playerX,0,o.x,o.z)) {
                    gameOver = true;
                    std::cout << "Game Over! Score: " << score << std::endl;
                }
            }

            if (obstacles.back().z > -10)
                spawnObstacle();

            score += 0.01f;
        }

        for (auto &o : obstacles)
            drawCube(o.x,0,o.z,1, 1,0,0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}