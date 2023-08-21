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
    //std::cout << points.size() << std::endl;
}

int pointdep(int x1, int x2, int y1, int y2) {
    // calculates depth of middle point to player (origin)
    int midx = (x1 + x2) / 2;
    int midy = (y1 + y2) / 2;

    return (midx * midx) + (midy * midy);
}

void bubblesort(std::vector<Walls>& wallsin, player player) {
    int i, j;
    bool swapped;
    for (i = 0; i < wallsin.size() - 1; i++) {
        swapped = false;
        for (j = 0; j < wallsin.size() - i - 1; j++) {
            if ((((wallsin[j].p2.first - wallsin[j].p1.first / 2) - player.x) * ((wallsin[j].p2.first - wallsin[j].p1.first / 2) - player.x)) + (((wallsin[j].p2.second - wallsin[j].p1.second / 2) - player.y) * ((wallsin[j].p2.second - wallsin[j].p1.second / 2) - player.y)) < (((wallsin[j + 1].p2.first - wallsin[j + 1].p1.first / 2) - player.x) * ((wallsin[j + 1].p2.first - wallsin[j + 1].p1.first / 2) - player.x)) + (((wallsin[j + 1].p2.second - wallsin[j + 1].p1.second / 2) - player.y) * ((wallsin[j + 1].p2.second - wallsin[j + 1].p1.second / 2) - player.y))) {
                std::swap(wallsin[j], wallsin[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false) break;
    }
}

void Secbubblesort(sectors* s, int size) {
    int i, j;
    bool swapped;
    for (i = 0; i < size; i++) {
        swapped = false;
        for (j = 0; j < size; j++) {
            if (s[j].d < s[j + 1].d) {
                std::swap(s[j], s[j + 1]);
                swapped = true;
            }
        }
        if (swapped == false) break;
    }
}

void engine::render(sectors* s, int Ssize, trees t, player player, trig math, unsigned VAO, unsigned VBO, unsigned EBO, unsigned shaderProgram) {
    float wx[4];
    float wy[4];
    float wz[4];
    int SW2 = 400;
    int SH2 = 300;
    float ps = math.SIN[player.a];
    float pc = math.COS[player.a];
    std::pair<float, float> v(player.x, player.y);
    std::vector<float> points;
    std::vector<unsigned> indices;
    std::vector<Walls> walls;

    // traverse the tree and update a vector
    // 
    // 
    t.print(v, walls);
    //for (Walls wall : walls) {
    //    trees::printwalls(wall);
    //}
    //for (int sec = 0; sec < Ssize; sec++) {
    //    s[sec].d = (s[sec].x - player.x) * (s[sec].x - player.x) + (s[sec].y - player.y) * (s[sec].y - player.y);
    //}
    std::cout << "tree constructed with: " << walls.size() << "ssecs" << std::endl;
    //bubblesort(walls, player);
    for (auto& wall : walls) {
        float x1 = wall.p1.first - player.x;
        float y1 = wall.p1.second - player.y;
        float x2 = wall.p2.first - player.x;
        float y2 = wall.p2.second - player.y;
        // rotation using rotate matrix
        wx[0] = x1 * pc - y1 * ps;
        wy[0] = y1 * pc + x1 * ps;
        /* for look think about it in terms of the player rotating as they look down
        * so as they're looking down, the point that they were looking at gets pushed up in their vision
        * the offset for that is the depth to the player (wy)
        */
        wz[0] = (float)wall.floor - player.z + (player.l * wy[0] / 32); // subtract to offset basically convert z to world coords as well
        wz[2] = (float)wall.height - player.z + (player.l * wy[0] / 32); // multiply look by wy to basically proportionally push wy further away
        wx[1] = x2 * pc - y2 * ps;
        wy[1] = y2 * pc + x2 * ps;
        wz[1] = (float)wall.floor - player.z + (player.l * wy[1] / 32);
        wz[3] = (float)wall.height - player.z + (player.l * wy[1] / 32);

        if (wy[0] > 0 && wy[1] > 0) {
            points.push_back(wx[0] / (wy[0]));
            points.push_back(wz[0] / (wy[0]));
            points.push_back(wall.col);
            points.push_back(wx[1] / (wy[1]));
            points.push_back(wz[1] / (wy[1]));
            points.push_back(wall.col);
            points.push_back(wx[0] / (wy[0]));
            points.push_back(wz[2] / (wy[0]));
            points.push_back(wall.col);
            points.push_back(wx[1] / (wy[1]));
            points.push_back(wz[3] / (wy[1]));
            points.push_back(wall.col);
        }
    }
    std::cout << "points: " << points.size() << std::endl;
    int quadcount = points.size() / (3 * 4);
    //std::cout << points.size()<<std::endl;
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

    std::cout << "quads: " << quadcount << std::endl;
    // TODO: place caps

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
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) p.l += 1;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) p.l -= 1;

    // height
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) p.z += 1;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) p.z -= 1;
}