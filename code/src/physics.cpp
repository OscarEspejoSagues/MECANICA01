#include <GL\glew.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\intersect.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>


#include <glm/gtx/quaternion.hpp>
 

glm::vec3 Pos;
glm::mat3 Rot;
glm::vec3 LinMom;
glm::vec3 AngMom;
glm::quat rotQuat;

glm::mat4 mattrans;
glm::vec3 gravity = {0.f, -9.81f, 0.f};
//float rot;

namespace Cube {
	extern void setupCube();
	extern void cleanupCube();
	extern void updateCube(const glm::mat4& transform);
	extern void drawCube();
	extern glm::vec3 cubeVerts[];
}

void generateMatTrans();
void ColisionBox(const glm::vec3 vertex);

bool show_test_window = false;
void GUI() {
	bool show = true;
	ImGui::Begin("Physics Parameters", &show, 0);

	// Do your GUI code here....
	{	
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//FrameRate
		
	}
	// .........................
	
	ImGui::End();

	// Example code -- ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

void PhysicsInit() {
	Cube::setupCube();
	Pos = { 0.f,8.f,0.f };

	generateMatTrans();
	/*rot = 0.f;*/
}

void PhysicsUpdate(float dt) {
	
	Pos += dt * gravity;
	/*rot += dt * 100;
	rotQuat = glm::quat(cos(glm::radians(rot / 2)), 0, sin(glm::radians(rot / 2)) * 1, 0);*/

	generateMatTrans();

	for (int i = 0; i < 7; i++)
		ColisionBox(Pos+Cube::cubeVerts[i]);

	Cube::updateCube(mattrans);
}

void PhysicsCleanup() {
	Cube::cleanupCube();
}

void generateMatTrans() {
	glm::mat4 matPos = {
		1.f,0.f,0.f,0.f,
		0.f,1.f,0.f,0.f,
		0.f,0.f,1.f,0.f,
		Pos.x,Pos.y,Pos.z,1.f,
	};
	mattrans = glm::toMat4(rotQuat) * matPos;
}

void ColisionBox(const glm::vec3 vertex)
{

	float distantplaneX01 = (vertex.x * 1) + 5; //distancia plano frontal
	float distantplaneY01 = (vertex.y * 1); //plano inferior
	float distantplaneZ01 = (vertex.z * 1) + 5; //plano lateral derecho
	float distantplaneZ02 = (vertex.z * -1) + 5; //plano lateral izquierdo
	float distantplaneX02 = (vertex.x * -1) + 5;//plano trasero
	float distantplaneY02 = (vertex.y * -1) + 10; //plano superior


																	//Rebotes planos frontales
	if (distantplaneX01 <= 0) //frotal
	{

	}
	if (distantplaneX02 <= 0)//trasero
	{

	}


	if (distantplaneY01 <= 0)
	{
		Pos = { 0.f, 0.f, 0.f }; // <--------------------------------------

	}

	if (distantplaneY02 <= 0)
	{

	}


	////Rebotes Planos Laterales
	if (distantplaneZ01 <= 0)//derecha
	{

	}

	if (distantplaneZ02 <= 0)//izquierda
	{


	}
}