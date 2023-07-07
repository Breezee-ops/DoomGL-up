#include "engine.h"

void engine::descriptRender(std::vector<float>points, std::vector<unsigned> indices, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int shaderProgram) {
    glUseProgram(shaderProgram);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(unsigned), (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(10.0f);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

    glBindVertexArray(0);
    glUseProgram(0);
    std::cout << points.size() << std::endl;
}

int pointdep(int x1, int x2, int y1, int y2) {
    // calculates depth of middle point to player (origin)
    int midx = (x1 + x2) / 2;
    int midy = (y1 + y2) / 2;

    return (midx * midx) + (midy * midy);
}

void bubblesort(Walls* wallsin, int size) {
    int i, j;
    bool swapped;
    for (i = 0; i < size - 1; i++) {
        swapped = false;
        for (j = 0; j < size - i - 1; j++) {
            if (wallsin[j].zdep < wallsin[j + 1].zdep) {
                std::swap(wallsin[j], wallsin[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false) break;
    }
}

void engine::render(Walls* walls, int size, player player, trig math, unsigned VAO, unsigned VBO, unsigned EBO, unsigned shaderProgram) {
    float wx[4];
    float wy[4];
    float wz[4];
    int SW2 = 400;
    int SH2 = 300;
    float ps = math.SIN[player.a];
    float pc = math.COS[player.a];

    std::vector<float> points;
    std::vector<unsigned> indices;
    std::vector<int> x1, y1, x2, y2;
    for (int k = 0; k < size; k++) {
        walls[k].zdep = pointdep(walls[k].x1 - player.x, walls[k].x2 - player.x, walls[k].y1 - player.y, walls[k].y2 - player.y);
    }
    bubblesort(walls, size);
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
            points.push_back(wx[0] / (wy[0]));
            points.push_back(wz[0] / (wy[0]));
            points.push_back(walls[i].col);
            points.push_back(wx[1] / (wy[1]));
            points.push_back(wz[1] / (wy[1]));
            points.push_back(walls[i].col);
            points.push_back(wx[0] / (wy[0]));
            points.push_back(wz[2] / (wy[0]));
            points.push_back(walls[i].col);
            points.push_back(wx[1] / (wy[1]));
            points.push_back(wz[3] / (wy[1]));
            points.push_back(walls[i].col);
        }
    }

    int quadcount = points.size() / 4;
    int base = 0;
    for (int j = 0; j < quadcount; j++) {
        indices.push_back(0 + base);
        indices.push_back(1 + base);
        indices.push_back(2 + base);
        indices.push_back(1 + base);
        indices.push_back(2 + base);
        indices.push_back(3 + base);
        base += 4;
    }
    if (!points.empty()) descriptRender(points, indices, VAO, VBO, EBO, shaderProgram);
}

void engine::keyboardHandle(player& p, trig m, GLFWwindow* window) {
    int comp = 90 - p.a;
    if (comp < 0) comp += 360;
    if (comp > 359) comp -= 360;
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
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        p.x -= 3 * m.SIN[comp];
        p.y += 3 * m.COS[comp];
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        p.x += 3 * m.SIN[comp];
        p.y -= 3 * m.COS[comp];
    }
    // looking
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) p.l += 1;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) p.l -= 1;
}