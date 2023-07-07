#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>
#include "structinc.h"
class engine
{
public:
	void descriptRender(std::vector<float> points, std::vector<unsigned> indices, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int shaderProgram);
	void render(Walls* walls, int size, player player, trig math, unsigned VAO, unsigned VBO, unsigned EBO, unsigned shaderProgram);
	void keyboardHandle(player& p, trig m, GLFWwindow* window);
};