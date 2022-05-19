#include <GLFW/glfw3.h>
#include <glad/glad.h>
#define GLT_IMPLEMENTATION
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.h"
#include "shader.h"
#include <time.h>
#include "glText.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
#define PI 3.141592653

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);



// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

void game_over_message(int score){
    GLTtext *text1 = gltCreateText();
	gltSetText(text1, "Hello World!");
    char str[50];

    gltBeginDraw();

    gltColor(1.0f, 1.0f, 1.0f, 1.0f);
    sprintf(str, "Game over!");
    gltSetText(text1, str);
    gltDrawText2DAligned(text1,
			(GLfloat)(SCR_WIDTH / 2),
			(GLfloat)(SCR_HEIGHT / 2.1),
			5.0f,
			GLT_CENTER, GLT_CENTER);
    sprintf(str, "Score: %d", score);
    gltSetText(text1, str);
    gltDrawText2DAligned(text1,
			(GLfloat)(SCR_WIDTH / 2),
			(GLfloat)(SCR_HEIGHT / 1.9),
			3.0f,
			GLT_CENTER, GLT_CENTER);
    gltEndDraw();   
}

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
bool toggleKeyRotate = false;
bool turnTable = false;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		toggleKeyRotate = !toggleKeyRotate;

	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		turnTable = !turnTable;
}

bool resetFlag = false;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

void PopulateVertexV(float vertices[], int n)
{
	int k = 0;
	float i = (2 * PI) / n;
	vertices[k++] = -3.1f;
	vertices[k++] = 0.0f;
	vertices[k++] = 0.0f;
	vertices[k++] = 0.0f;
	vertices[k++] = 1.0f;
	vertices[k++] = 0.0f;

	vertices[k++] = -2.9f;
	vertices[k++] = 0.0f;
	vertices[k++] = 0.0f;
	vertices[k++] = 0.0f;
	vertices[k++] = 1.0f;
	vertices[k++] = 0.0f;

	vertices[k++] = -3.1 + 0.2f * cos(i);
	vertices[k++] = 0.2f * sin(i);
	vertices[k++] = 0.0f;
	vertices[k++] = 0.0f;
	vertices[k++] = 1.0f;
	vertices[k++] = 0.0f;
	i += ((2 * PI) / n);
	for (int j = 1; j < n; j++)
	{
		int s1 = k - 18;
		int s2 = k - 6;

		vertices[k++] = vertices[s1++];
		vertices[k++] = vertices[s1++];
		vertices[k++] = vertices[s1++];
		vertices[k++] = vertices[s1++];
		vertices[k++] = vertices[s1++];
		vertices[k++] = vertices[s1++];

		vertices[k++] = vertices[s2++];
		vertices[k++] = vertices[s2++];
		vertices[k++] = vertices[s2++];
		vertices[k++] = vertices[s2++];
		vertices[k++] = vertices[s2++];
		vertices[k++] = vertices[s2++];

		vertices[k++] = 0.2f * cos(i) + (-3.1);
		vertices[k++] = 0.2f * sin(i);
		vertices[k++] = 0.0f;
		vertices[k++] = 0.0f;
		vertices[k++] = 1.0f;
		vertices[k++] = 0.0f;
		i += ((2 * PI) / n);
	}
}

void PopulateRoom(float RoomWallsV[], int n, float obs[])
{
	int k = 0;
	// float i = (2*PI)/n;
	// 1
	RoomWallsV[k++] = -3.5f;
	RoomWallsV[k++] = 2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = -3.5f;
	RoomWallsV[k++] = 2.5f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = 3.5f;
	RoomWallsV[k++] = 2.5f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;
	// 2
	RoomWallsV[k++] = 3.5f;
	RoomWallsV[k++] = 2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = 3.5f;
	RoomWallsV[k++] = 2.5f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = -3.5f;
	RoomWallsV[k++] = 2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;
	// 3
	RoomWallsV[k++] = -3.5f;
	RoomWallsV[k++] = -2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = -3.5f;
	RoomWallsV[k++] = -2.5f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = 3.5f;
	RoomWallsV[k++] = -2.5f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;
	// 4
	RoomWallsV[k++] = 3.5f;
	RoomWallsV[k++] = -2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = 3.5f;
	RoomWallsV[k++] = -2.5f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = -3.5f;
	RoomWallsV[k++] = -2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;
	// 5
	RoomWallsV[k++] = -3.5f;
	RoomWallsV[k++] = 2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = -3.5f;
	RoomWallsV[k++] = -2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = -3.3f;
	RoomWallsV[k++] = -2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;
	// 6
	RoomWallsV[k++] = -3.3f;
	RoomWallsV[k++] = -2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = -3.3f;
	RoomWallsV[k++] = 2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = -3.5f;
	RoomWallsV[k++] = 2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;
	// 7
	RoomWallsV[k++] = 3.3f;
	RoomWallsV[k++] = 2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = 3.3f;
	RoomWallsV[k++] = -2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = 3.5f;
	RoomWallsV[k++] = 2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;
	// 8
	RoomWallsV[k++] = 3.5f;
	RoomWallsV[k++] = 2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = 3.3f;
	RoomWallsV[k++] = -2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	RoomWallsV[k++] = 3.5f;
	RoomWallsV[k++] = -2.7f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 1.0f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = 0.0f;

	for (int i = 0; i < n; i++)
	{
		obs[i] = 5.6 * rand() / RAND_MAX - 2.8;
	}
	// for(int i=n/4;i<n;i++){
	// 	obs[i+n/2] = -2.8*rand()/RAND_MAX;
	// }
	for (int i = 0; i < n / 2; i++)
	{
		RoomWallsV[k++] = obs[i];
		RoomWallsV[k++] = 2.5;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 1.0f;

		RoomWallsV[k++] = obs[i] + 0.3;
		RoomWallsV[k++] = 2.5;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 1.0f;

		RoomWallsV[k++] = obs[i];
		RoomWallsV[k++] = 1.0;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 1.0f;

		RoomWallsV[k++] = obs[i];
		RoomWallsV[k++] = 1.0;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 1.0f;

		RoomWallsV[k++] = obs[i] + 0.3;
		RoomWallsV[k++] = 1.0;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 1.0f;

		RoomWallsV[k++] = obs[i] + 0.3;
		RoomWallsV[k++] = 2.5;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 1.0f;
	}

	for (int i = n / 2; i < n; i++)
	{
		RoomWallsV[k++] = obs[i];
		RoomWallsV[k++] = -2.5;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 1.0f;

		RoomWallsV[k++] = obs[i] + 0.3;
		RoomWallsV[k++] = -2.5;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 1.0f;

		RoomWallsV[k++] = obs[i];
		RoomWallsV[k++] = -1.0;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 1.0f;

		RoomWallsV[k++] = obs[i];
		RoomWallsV[k++] = -1.0;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 1.0f;

		RoomWallsV[k++] = obs[i] + 0.3;
		RoomWallsV[k++] = -1.0;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 1.0f;

		RoomWallsV[k++] = obs[i] + 0.3;
		RoomWallsV[k++] = -2.5;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 0.0f;
		RoomWallsV[k++] = 1.0f;
	}

	// Door
	float xColor = (float)rand() / RAND_MAX;
	float yColor = (float)rand() / RAND_MAX;
	float zColor = (float)rand() / RAND_MAX;

	RoomWallsV[k++] = 3.3f;
	RoomWallsV[k++] = 0.3f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = xColor;
	RoomWallsV[k++] = yColor;
	RoomWallsV[k++] = zColor;

	RoomWallsV[k++] = 3.3f;
	RoomWallsV[k++] = -0.3f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = xColor;
	RoomWallsV[k++] = yColor;
	RoomWallsV[k++] = zColor;

	RoomWallsV[k++] = 3.2f;
	RoomWallsV[k++] = -0.3f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = xColor;
	RoomWallsV[k++] = yColor;
	RoomWallsV[k++] = zColor;

	RoomWallsV[k++] = 3.2f;
	RoomWallsV[k++] = -0.3f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = xColor;
	RoomWallsV[k++] = yColor;
	RoomWallsV[k++] = zColor;

	RoomWallsV[k++] = 3.2f;
	RoomWallsV[k++] = 0.3f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = xColor;
	RoomWallsV[k++] = yColor;
	RoomWallsV[k++] = zColor;

	RoomWallsV[k++] = 3.3f;
	RoomWallsV[k++] = 0.3f;
	RoomWallsV[k++] = 0.0f;
	RoomWallsV[k++] = xColor;
	RoomWallsV[k++] = yColor;
	RoomWallsV[k++] = zColor;
}

void Enemylevel1(float enemy[], float xCoord)
{
	int k = 0, n = 8;
	float i = (2 * PI) / n;

	float x = xCoord;
	enemy[k++] = x;
	enemy[k++] = 0.0f;
	enemy[k++] = 0.0f;
	enemy[k++] = 0.8f;
	enemy[k++] = 0.2f;
	enemy[k++] = 0.3f;

	enemy[k++] = x - 0.2;
	enemy[k++] = 0.0f;
	enemy[k++] = 0.0f;
	enemy[k++] = 0.8f;
	enemy[k++] = 0.2f;
	enemy[k++] = 0.3f;

	enemy[k++] = x + 0.2f * cos(i);
	enemy[k++] = 0.2f * sin(i);
	enemy[k++] = 0.0f;
	enemy[k++] = 0.8f;
	enemy[k++] = 0.2f;
	enemy[k++] = 0.3f;
	i += ((2 * PI) / n);
	for (int j = 1; j < n; j++)
	{
		int s1 = k - 18;
		int s2 = k - 6;

		enemy[k++] = enemy[s1++];
		enemy[k++] = enemy[s1++];
		enemy[k++] = enemy[s1++];
		enemy[k++] = 0.8f;
		enemy[k++] = 0.2f;
		enemy[k++] = 0.3f;

		enemy[k++] = enemy[s2++];
		enemy[k++] = enemy[s2++];
		enemy[k++] = enemy[s2++];
		enemy[k++] = 0.8f;
		enemy[k++] = 0.2f;
		enemy[k++] = 0.3f;

		enemy[k++] = 0.2f * cos(i) + x;
		enemy[k++] = 0.2f * sin(i);
		enemy[k++] = 0.0f;
		enemy[k++] = 0.8f;
		enemy[k++] = 0.2f;
		enemy[k++] = 0.3f;
		i += ((2 * PI) / n);
	}
}

void Enemylevel2(float enemy2[], float x)
{
	int k = 0, n = 12;
	float i = (2 * PI) / n;

	enemy2[k++] = x;
	enemy2[k++] = 0.0f;
	enemy2[k++] = 0.0f;
	enemy2[k++] = 0.8f;
	enemy2[k++] = 0.2f;
	enemy2[k++] = 0.3f;

	enemy2[k++] = x - 0.2;
	enemy2[k++] = 0.0f;
	enemy2[k++] = 0.0f;
	enemy2[k++] = 0.8f;
	enemy2[k++] = 0.2f;
	enemy2[k++] = 0.3f;

	enemy2[k++] = x + 0.2f * cos(i);
	enemy2[k++] = 0.2f * sin(i);
	enemy2[k++] = 0.0f;
	enemy2[k++] = 0.8f;
	enemy2[k++] = 0.2f;
	enemy2[k++] = 0.3f;
	i += ((2 * PI) / n);
	for (int j = 1; j < n; j++)
	{
		int s1 = k - 18;
		int s2 = k - 6;

		enemy2[k++] = enemy2[s1++];
		enemy2[k++] = enemy2[s1++];
		enemy2[k++] = enemy2[s1++];
		enemy2[k++] = 0.8f;
		enemy2[k++] = 0.2f;
		enemy2[k++] = 0.3f;

		enemy2[k++] = enemy2[s2++];
		enemy2[k++] = enemy2[s2++];
		enemy2[k++] = enemy2[s2++];
		enemy2[k++] = 0.8f;
		enemy2[k++] = 0.2f;
		enemy2[k++] = 0.3f;

		enemy2[k++] = 0.2f * cos(i) + x;
		enemy2[k++] = 0.2f * sin(i);
		enemy2[k++] = 0.0f;
		enemy2[k++] = 0.8f;
		enemy2[k++] = 0.2f;
		enemy2[k++] = 0.3f;
		i += ((2 * PI) / n);
	}
}

void Enemylevel3(float enemy3[], float x)
{
	int k = 0, n = 10;
	float i = (2 * PI) / n;

	enemy3[k++] = x;
	enemy3[k++] = 0.0f;
	enemy3[k++] = 0.0f;
	enemy3[k++] = 0.8f;
	enemy3[k++] = 0.2f;
	enemy3[k++] = 0.3f;

	enemy3[k++] = x - 0.2;
	enemy3[k++] = 0.0f;
	enemy3[k++] = 0.0f;
	enemy3[k++] = 0.8f;
	enemy3[k++] = 0.2f;
	enemy3[k++] = 0.3f;

	enemy3[k++] = x + 0.2f * cos(i);
	enemy3[k++] = 0.2f * sin(i);
	enemy3[k++] = 0.0f;
	enemy3[k++] = 0.8f;
	enemy3[k++] = 0.2f;
	enemy3[k++] = 0.3f;
	i += ((2 * PI) / n);
	for (int j = 1; j < n; j++)
	{
		int s1 = k - 18;
		int s2 = k - 6;

		enemy3[k++] = enemy3[s1++];
		enemy3[k++] = enemy3[s1++];
		enemy3[k++] = enemy3[s1++];
		enemy3[k++] = 0.8f;
		enemy3[k++] = 0.2f;
		enemy3[k++] = 0.3f;

		enemy3[k++] = enemy3[s2++];
		enemy3[k++] = enemy3[s2++];
		enemy3[k++] = enemy3[s2++];
		enemy3[k++] = 0.8f;
		enemy3[k++] = 0.2f;
		enemy3[k++] = 0.3f;

		enemy3[k++] = 0.2f * cos(i) + x;
		enemy3[k++] = 0.2f * sin(i);
		enemy3[k++] = 0.0f;
		enemy3[k++] = 0.8f;
		enemy3[k++] = 0.2f;
		enemy3[k++] = 0.3f;
		i += ((2 * PI) / n);
	}
}

void Coins(float coins[], float coinsV[], int numCoins)
{
	int k = 0;
	int n = 5;
	float i = (2 * PI) / n;
	for (int l = 0; l < numCoins; l++)
	{
		float x = 6.4*rand()/RAND_MAX-3.2;
		coinsV[l]=x;
		coins[k++] = x;
		coins[k++] = 0.0f;
		coins[k++] = 0.0f;
		coins[k++] = 0.0f;
		coins[k++] = 1.0f;
		coins[k++] = 0.0f;

		coins[k++] = x-0.1;
		coins[k++] = 0.0f;
		coins[k++] = 0.0f;
		coins[k++] = 0.0f;
		coins[k++] = 1.0f;
		coins[k++] = 0.0f;

		coins[k++] = x + 0.1f * cos(i);
		coins[k++] = 0.1f * sin(i);
		coins[k++] = 0.0f;
		coins[k++] = 0.0f;
		coins[k++] = 1.0f;
		coins[k++] = 0.0f;
		i += ((2 * PI) / n);
		for (int j = 1; j < n; j++)
		{
			int s1 = k - 18;
			int s2 = k - 6;

			coins[k++] = coins[s1++];
			coins[k++] = coins[s1++];
			coins[k++] = coins[s1++];
			coins[k++] = coins[s1++];
			coins[k++] = coins[s1++];
			coins[k++] = coins[s1++];

			coins[k++] = coins[s2++];
			coins[k++] = coins[s2++];
			coins[k++] = coins[s2++];
			coins[k++] = coins[s2++];
			coins[k++] = coins[s2++];
			coins[k++] = coins[s2++];

			coins[k++] = 0.1f * cos(i) + (-3.1);
			coins[k++] = 0.2f * sin(i);
			coins[k++] = 0.0f;
			coins[k++] = 0.0f;
			coins[k++] = 1.0f;
			coins[k++] = 0.0f;
			i += ((2 * PI) / n);
		}
	}
}

glm::mat4 transform;
glm::mat4 transform1 = glm::mat4(1.0f);
glm::mat4 transform2;
glm::mat4 transform3;
glm::mat4 transform4;

float up = 0.0f, pleft = -3.1f;
float enemyUp = 0.0f;
bool toggleEnemy = true;
float enemy2Up = 0.0f;
bool toggleEnemy2 = true;
float enemy3Up = 0.0f;
bool toggleEnemy3 = true;

int main()
{
	srand(time(0));
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	GLFWwindow *window =
		glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	Shader ourShader("../src/vertex.shader", "../src/fragment.shader");
	unsigned int VBO2, VAO2, EBO2;
	unsigned int VBO3, VAO3, EBO3;
	unsigned int VBO4, VAO4, EBO4;
	int score=0;
	// int level=2;
	for (int level = 1; level < 4; level++)
	{
		int n = 20;
		float vertices[18 * n];
		PopulateVertexV(vertices, n);

		int numObs = 3;
		if (level == 1)
		{
			numObs = 3;
		}
		else if (level == 2)
		{
			numObs = 4;
		}
		else
		{
			numObs = 5;
		}
		float obs[numObs];
		int Wall = 8 + (numObs * 2) + 2;
		float RoomWallsV[18 * Wall];
		PopulateRoom(RoomWallsV, numObs, obs);

		int enemySides = 8;
		float xCoordEnemy1;
		float enemy[18 * enemySides];

		int enemy2Sides = 12;
		float xCoordEnemy2;
		float enemy2[18 * enemy2Sides];

		int enemy3Sides = 10;
		float xCoordEnemy3;
		float enemy3[18 * enemy3Sides];

		if (level == 1)
		{
			// Enemy1

			float x = (float)3.2 * rand() / RAND_MAX;
			while (1)
			{
				if ((x < (obs[0] - 0.3) || x > (obs[0] + 0.7)) && (x < (obs[1] - 0.3) || x > (obs[1] + 0.7)) && (x < (obs[2] - 0.3) || x > (obs[2] + 0.7)))
				{
					break;
				}
				x = (float)3.2 * rand() / RAND_MAX;
			}
			xCoordEnemy1 = x;
			Enemylevel1(enemy, xCoordEnemy1);

			glGenVertexArrays(1, &VAO2);
			glGenBuffers(1, &VBO2);
			glBindVertexArray(VAO2);
			glBindBuffer(GL_ARRAY_BUFFER, VBO2);
			glBufferData(GL_ARRAY_BUFFER, sizeof(enemy), enemy, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		else if (level == 2)
		{

			float x = (float)3.2 * rand() / RAND_MAX;
			while (1)
			{
				if ((x < (obs[0] - 0.3) || x > (obs[0] + 0.7)) && (x < (obs[1] - 0.3) || x > (obs[1] + 0.7)) && (x < (obs[2] - 0.3) || x > (obs[2] + 0.7)) && (x < (obs[3] - 0.3) || x > (obs[3] + 0.7)))
				{
					break;
				}
				x = (float)3.2 * rand() / RAND_MAX;
			}
			xCoordEnemy1 = x;
			Enemylevel1(enemy, xCoordEnemy1);

			x = (float)3.2 * rand() / RAND_MAX;
			while (1)
			{
				if ((x < (obs[0] - 0.3) || x > (obs[0] + 0.7)) && (x < (obs[1] - 0.3) || x > (obs[1] + 0.7)) && (x < (obs[2] - 0.3) || x > (obs[2] + 0.7)) && (x < (obs[3] - 0.3) || x > (obs[3] + 0.7)))
				{
					break;
				}
				x = (float)3.2 * rand() / RAND_MAX;
			}
			xCoordEnemy2 = x;
			Enemylevel2(enemy2, xCoordEnemy2);

			// Enemy1
			glGenVertexArrays(1, &VAO2);
			glGenBuffers(1, &VBO2);
			glBindVertexArray(VAO2);
			glBindBuffer(GL_ARRAY_BUFFER, VBO2);
			glBufferData(GL_ARRAY_BUFFER, sizeof(enemy), enemy, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			// Enemy2
			glGenVertexArrays(1, &VAO3);
			glGenBuffers(1, &VBO3);
			glBindVertexArray(VAO3);
			glBindBuffer(GL_ARRAY_BUFFER, VBO3);
			glBufferData(GL_ARRAY_BUFFER, sizeof(enemy2), enemy2, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		else
		{
			float x = (float)3.2 * rand() / RAND_MAX;
			while (1)
			{
				if ((x < (obs[0] - 0.3) || x > (obs[0] + 0.7)) && (x < (obs[1] - 0.3) || x > (obs[1] + 0.7)) && (x < (obs[2] - 0.3) || x > (obs[2] + 0.7)) && (x < (obs[3] - 0.3) || x > (obs[3] + 0.7)) && (x < (obs[4] - 0.3) || x > (obs[4] + 0.7)))
				{
					break;
				}
				x = (float)3.2 * rand() / RAND_MAX;
			}
			xCoordEnemy1 = x;
			Enemylevel1(enemy, xCoordEnemy1);
			x = (float)3.2 * rand() / RAND_MAX;
			while (1)
			{
				if ((x < (obs[0] - 0.3) || x > (obs[0] + 0.7)) && (x < (obs[1] - 0.3) || x > (obs[1] + 0.7)) && (x < (obs[2] - 0.3) || x > (obs[2] + 0.7)) && (x < (obs[3] - 0.3) || x > (obs[3] + 0.7)) && (x < (obs[4] - 0.3) || x > (obs[4] + 0.7)))
				{
					break;
				}
				x = (float)3.2 * rand() / RAND_MAX;
			}
			xCoordEnemy2 = x;
			Enemylevel2(enemy2, xCoordEnemy2);

			x = (float)3.2 * rand() / RAND_MAX;
			while (1)
			{
				if ((x < (obs[0] - 0.3) || x > (obs[0] + 0.7)) && (x < (obs[1] - 0.3) || x > (obs[1] + 0.7)) && (x < (obs[2] - 0.3) || x > (obs[2] + 0.7)) && (x < (obs[3] - 0.3) || x > (obs[3] + 0.7)) && (x < (obs[4] - 0.3) || x > (obs[4] + 0.7)))
				{
					break;
				}
				x = (float)3.2 * rand() / RAND_MAX;
			}
			xCoordEnemy3 = x;
			Enemylevel3(enemy3, xCoordEnemy3);

			// Enemy1
			glGenVertexArrays(1, &VAO2);
			glGenBuffers(1, &VBO2);
			glBindVertexArray(VAO2);
			glBindBuffer(GL_ARRAY_BUFFER, VBO2);
			glBufferData(GL_ARRAY_BUFFER, sizeof(enemy), enemy, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			// Enemy2
			glGenVertexArrays(1, &VAO3);
			glGenBuffers(1, &VBO3);
			glBindVertexArray(VAO3);
			glBindBuffer(GL_ARRAY_BUFFER, VBO3);
			glBufferData(GL_ARRAY_BUFFER, sizeof(enemy2), enemy2, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			// Enemy3
			glGenVertexArrays(1, &VAO4);
			glGenBuffers(1, &VBO4);
			glBindVertexArray(VAO4);
			glBindBuffer(GL_ARRAY_BUFFER, VBO4);
			glBufferData(GL_ARRAY_BUFFER, sizeof(enemy3), enemy3, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		unsigned int NUM_VERTICES = (sizeof(vertices) / sizeof(vertices[0])) / 6;

		// Players
		unsigned int VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// Walls
		unsigned int VBO1, VAO1, EBO1;
		glGenVertexArrays(1, &VAO1);
		glGenBuffers(1, &VBO1);
		glBindVertexArray(VAO1);
		glBindBuffer(GL_ARRAY_BUFFER, VBO1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(RoomWallsV), RoomWallsV, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		// enable depth testing for 3d
		glEnable(GL_DEPTH_TEST);
		transform = glm::mat4(1.0);
		transform2 = glm::mat4(1.0);
		transform3 = glm::mat4(1.0);
		transform4 = glm::mat4(1.0);
		// render loop
		while (!glfwWindowShouldClose(window))
		{
			// print stuff for better understanding

			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			// inpu0
			processInput(window);
			// render
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (level == 1)
			{
				if ((pleft < (obs[0] - 0.15) || pleft > (obs[0] + 0.43) || up < 0.8))
				{
					if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && up < 2.3)
					{
						transform = glm::translate(transform, glm::vec3(0.0f, 0.05f, 0.0f));
						up += 0.05;
					}
				}
				if ((pleft < (obs[1] - 0.15) || pleft > (obs[1] + 0.43) || up > -0.8) && (pleft < (obs[2] - 0.15) || pleft > (obs[2] + 0.43) || up > -0.8))
				{
					if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && up > -2.3)
					{
						transform = glm::translate(transform, glm::vec3(0.0f, -0.05f, 0.0f));
						up -= 0.05;
					}
				}
				if ((pleft < (obs[0] - 0.2) || pleft > (obs[0] + 0.53) || up < 0.9) && (pleft < (obs[1] - 0.2) || pleft > (obs[1] + 0.53) || up > -0.9) && (pleft < (obs[2] - 0.2) || pleft > (obs[2] + 0.53) || up > -0.9))
				{
					if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && pleft > -3.1)
					{
						transform = glm::translate(transform, glm::vec3(-0.05f, 0.0f, 0.0f));
						pleft -= 0.05;
					}
				}
				if ((pleft < (obs[0] - 0.2) || pleft > (obs[0] + 0.3) || up < 0.9) && (pleft < (obs[1] - 0.2) || pleft > (obs[1] + 0.3) || up > -0.9) && (pleft < (obs[2] - 0.2) || pleft > (obs[2] + 0.3) || up > -0.9))
				{
					if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && pleft < 3.1)
					{
						transform = glm::translate(transform, glm::vec3(0.05f, 0.0f, 0.0f));
						pleft += 0.05;
					}
				}
			}
			else if (level == 2)
			{
				if ((pleft < (obs[0] - 0.15) || pleft > (obs[0] + 0.43) || up < 0.8) && (pleft < (obs[1] - 0.15) || pleft > (obs[1] + 0.43) || up < 0.8))
				{
					if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && up < 2.3)
					{
						transform = glm::translate(transform, glm::vec3(0.0f, 0.05f, 0.0f));
						up += 0.05;
					}
				}
				if ((pleft < (obs[2] - 0.15) || pleft > (obs[2] + 0.43) || up > -0.8) && (pleft < (obs[3] - 0.15) || pleft > (obs[3] + 0.43) || up > -0.8))
				{
					if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && up > -2.3)
					{
						transform = glm::translate(transform, glm::vec3(0.0f, -0.05f, 0.0f));
						up -= 0.05;
					}
				}
				if ((pleft < (obs[0] - 0.2) || pleft > (obs[0] + 0.53) || up < 0.9) && (pleft < (obs[1] - 0.2) || pleft > (obs[1] + 0.53) || up < 0.9) && (pleft < (obs[2] - 0.2) || pleft > (obs[2] + 0.53) || up > -0.9) && (pleft < (obs[3] - 0.2) || pleft > (obs[3] + 0.53) || up > -0.9))
				{
					if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && pleft > -3.1)
					{
						transform = glm::translate(transform, glm::vec3(-0.05f, 0.0f, 0.0f));
						pleft -= 0.05;
					}
				}
				if ((pleft < (obs[0] - 0.2) || pleft > (obs[0] + 0.3) || up < 0.9) && (pleft < (obs[1] - 0.2) || pleft > (obs[1] + 0.3) || up < 0.9) && (pleft < (obs[2] - 0.2) || pleft > (obs[2] + 0.3) || up > -0.9) && (pleft < (obs[3] - 0.2) || pleft > (obs[3] + 0.3) || up > -0.9))
				{
					if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && pleft < 3.1)
					{
						transform = glm::translate(transform, glm::vec3(0.05f, 0.0f, 0.0f));
						pleft += 0.05;
					}
				}
			}
			else
			{
				if ((pleft < (obs[0] - 0.15) || pleft > (obs[0] + 0.43) || up < 0.8) && (pleft < (obs[1] - 0.15) || pleft > (obs[1] + 0.43) || up < 0.8))
				{
					if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && up < 2.3)
					{
						transform = glm::translate(transform, glm::vec3(0.0f, 0.05f, 0.0f));
						up += 0.05;
					}
				}
				if ((pleft < (obs[2] - 0.15) || pleft > (obs[2] + 0.43) || up > -0.8) && (pleft < (obs[3] - 0.15) || pleft > (obs[3] + 0.43) || up > -0.8) && (pleft < (obs[4] - 0.15) || pleft > (obs[4] + 0.43) || up > -0.8))
				{
					if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && up > -2.3)
					{
						transform = glm::translate(transform, glm::vec3(0.0f, -0.05f, 0.0f));
						up -= 0.05;
					}
				}
				if ((pleft < (obs[0] - 0.2) || pleft > (obs[0] + 0.53) || up < 0.9) && (pleft < (obs[1] - 0.2) || pleft > (obs[1] + 0.53) || up < 0.9) && (pleft < (obs[2] - 0.2) || pleft > (obs[2] + 0.53) || up > -0.9) && (pleft < (obs[3] - 0.2) || pleft > (obs[3] + 0.53) || up > -0.9) && (pleft < (obs[4] - 0.2) || pleft > (obs[4] + 0.53) || up > -0.9))
				{
					if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && pleft > -3.1)
					{
						transform = glm::translate(transform, glm::vec3(-0.05f, 0.0f, 0.0f));
						pleft -= 0.05;
					}
				}
				if ((pleft < (obs[0] - 0.2) || pleft > (obs[0] + 0.3) || up < 0.9) && (pleft < (obs[1] - 0.2) || pleft > (obs[1] + 0.3) || up < 0.9) && (pleft < (obs[2] - 0.2) || pleft > (obs[2] + 0.3) || up > -0.9) && (pleft < (obs[3] - 0.2) || pleft > (obs[3] + 0.3) || up > -0.9) && (pleft < (obs[4] - 0.2) || pleft > (obs[4] + 0.3) || up > -0.9))
				{
					if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && pleft < 3.1)
					{
						transform = glm::translate(transform, glm::vec3(0.05f, 0.0f, 0.0f));
						pleft += 0.05;
					}
				}
			}

			ourShader.use();
			ourShader.setMat4("transform", transform);

			// view matrix
			glm::mat4 view = camera.GetViewMatrix(glm::vec3(0.0f));
			ourShader.setMat4("view", view);

			// projection matrix
			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			ourShader.setMat4("projection", projection);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 18 * n);

			// Walls

			ourShader.setMat4("transform", transform1);

			glBindVertexArray(VAO1);
			glDrawArrays(GL_TRIANGLES, 0, 18 * Wall);

			// Enemy
			if (toggleEnemy)
			{
				if (enemyUp < 2.3)
				{
					transform2 = glm::translate(transform2, glm::vec3(0.0f, 0.05f, 0.0f));
					enemyUp += 0.05;
				}
				else
				{
					toggleEnemy = false;
				}
			}
			else
			{
				if (enemyUp > -2.3)
				{
					transform2 = glm::translate(transform2, glm::vec3(0.0f, -0.05f, 0.0f));
					enemyUp -= 0.05;
				}
				else
				{
					toggleEnemy = true;
				}
			}
			ourShader.setMat4("transform", transform2);
			glBindVertexArray(VAO2);
			glDrawArrays(GL_TRIANGLES, 0, 18 * enemySides);

			if ((pleft > xCoordEnemy1 - 0.2 && pleft < xCoordEnemy1 + 0.2) && (up > enemyUp - 0.2 && up < enemyUp + 0.2))
			{
				std::cout<<"You Lost in level 1"<<std::endl;
				glfwSetWindowShouldClose(window, true);
			}

			if (level > 1)
			{
				if (toggleEnemy2)
				{
					if (enemy2Up < 2.4)
					{
						transform3 = glm::translate(transform3, glm::vec3(0.0f, 0.08f, 0.0f));
						enemy2Up += 0.08;
					}
					else
					{
						toggleEnemy2 = false;
					}
				}
				else
				{
					if (enemy2Up > -2.4)
					{
						transform3 = glm::translate(transform3, glm::vec3(0.0f, -0.08f, 0.0f));
						enemy2Up -= 0.08;
					}
					else
					{
						toggleEnemy2 = true;
					}
				}

				ourShader.setMat4("transform", transform3);
				glBindVertexArray(VAO3);
				glDrawArrays(GL_TRIANGLES, 0, 18 * enemy2Sides);

				if ((pleft > (xCoordEnemy2 - 0.2) && pleft < xCoordEnemy2 + 0.2) && (up > enemy2Up - 0.2 && up < enemy2Up + 0.2))
				{
					std::cout<<"You Lost in level2"<<std::endl;
					glfwSetWindowShouldClose(window, true);
				}
			}
			// Enemy3
			if (level > 2)
			{
				if (toggleEnemy3)
				{
					if (enemy3Up < 2.3)
					{
						transform4 = glm::translate(transform4, glm::vec3(0.0f, 0.1f, 0.0f));
						enemy3Up += 0.1f;
					}
					else
					{
						toggleEnemy3 = false;
					}
				}
				else
				{
					if (enemy3Up > -2.3)
					{
						transform4 = glm::translate(transform4, glm::vec3(0.0f, -0.1f, 0.0f));
						enemy3Up -= 0.1f;
					}
					else
					{
						toggleEnemy3 = true;
					}
				}
				ourShader.setMat4("transform", transform4);
				glBindVertexArray(VAO4);
				glDrawArrays(GL_TRIANGLES, 0, 18 * enemy3Sides);

				if ((pleft > xCoordEnemy3 - 0.2 && pleft < xCoordEnemy3 + 0.2) && (up > enemy3Up - 0.2 && up < enemy3Up + 0.2))
				{
					std::cout<<"You lost in level3"<<std::endl;
					glfwSetWindowShouldClose(window, true);
				}
			}
			if (pleft > 3.1 && (up > -0.3) && up < 0.3)
			{
				pleft = -3.1;
				up = 0.0;
				enemyUp = 0.0f;
				enemy2Up = 0.0f;
				enemy3Up = 0.0f;
				score = level*score+10;
				
				if(score == 100){
					game_over_message(score);
					std::cout<<"You won"<<std::endl;
				}
				break;
			}
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		glDeleteVertexArrays(1, &VAO1);
		glDeleteBuffers(1, &VBO1);

		glDeleteVertexArrays(1, &VAO2);
		glDeleteBuffers(1, &VBO2);

		glDeleteVertexArrays(1, &VAO3);
		glDeleteBuffers(1, &VBO3);

		glDeleteVertexArrays(1, &VAO4);
		glDeleteBuffers(1, &VBO4);
	}
	glfwTerminate();

	std::cout<<"Score: "<<score<<std::endl;
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}