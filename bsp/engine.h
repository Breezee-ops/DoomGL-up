#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <math.h>
#include <vector>
#include "structinc.h"
class engine
{
public:
	void descriptRender(float* points, unsigned int* indices, unsigned int VAO, unsigned int VBO, unsigned int EBO, unsigned int shaderProgram);
	void render(sectors* walls, int size, player player, trig math, float* points, unsigned* indices, unsigned VAO, unsigned VBO, unsigned EBO, unsigned shaderProgram);
	void keyboardHandle(player& p, trig m, GLFWwindow* window);
};