#include "engine.h"

void engine::descriptRender(float* points, unsigned int* indices, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int shaderProgram) {
    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 8, points);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 6, indices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(10.0f);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

void engine::render(sectors* walls, int size, player player, trig math, float* points, unsigned* indices, unsigned VAO, unsigned VBO, unsigned EBO, unsigned shaderProgram) {
    float wx[4];
    float wy[4];
    float wz[4];
    int SW2 = 400;
    int SH2 = 300;
    float ps = math.SIN[player.a];
    float pc = math.COS[player.a];
    // placed offset point
    for (int i = 0; i < size; i++) {
        int x1 = walls[i].x1 - player.x;
        int y1 = walls[i].y1 - player.y;

        int x2 = walls[i].x2 - player.x;
        int y2 = walls[i].y2 - player.y;
        // rotation using rotate matrix
        wx[0] = x1 * pc - y1 * ps;
        wy[0] = y1 * pc + x1 * ps;
        wz[0] = 0 - player.z;
        wz[2] = walls[i].z - player.z;
        wx[1] = x2 * pc - y2 * ps;
        wy[1] = y2 * pc + x2 * ps;
        wz[1] = 0 - player.z;
        wz[3] = walls[i].z - player.z;

        // transform into screen coordinates
        if (wy[0] > 0 && wy[1] > 0) {
            points[0] = 2*wx[0] / (wy[0]);
            points[1] = 2*wz[0] / (wy[0]);
            points[2] = 2*wx[1] / (wy[1]);
            points[3] = 2*wz[1] / (wy[1]);
            points[4] = 2*wx[0] / (wy[0]);
            points[5] = 2*wz[2] / wy[0];
            points[6] = 2*wx[1] / (wy[1]);
            points[7] = 2*wz[3] / wy[1];
        }
        std::cout << wz[2] << std::endl;
        descriptRender(points, indices, VAO, VBO, EBO, shaderProgram);
    }
}

void engine::keyboardHandle(player& p, trig m, GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        p.a -= 4;
        if (p.a < 0) p.a += 360;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        p.a += 4;
        if (p.a > 359) p.a -= 360;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        p.x += 3 * m.SIN[p.a];
        p.y += 3 * m.COS[p.a];
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        p.x -= 3 * m.SIN[p.a];
        p.y -= 3 * m.COS[p.a];
    }
    // looking
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) p.l += 1;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) p.l -= 1;
}