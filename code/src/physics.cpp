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



glm::vec3 oldPos;
glm::vec3 Pos;
glm::mat3 Rot;
glm::vec3 LinMom;
glm::vec3 AngMom;
glm::quat rotQuat;
glm::vec3 Vel;

glm::mat4 mattrans;
glm::vec3 gravity = {0.f, -9.81f, 0.f};
glm::vec3 force01 = { -3.f, 4.f, 3.f }; //PEDAZO DE IDIOTAS ESTO TIENE QUE SER UN RANDOM <------------------------------------------------------------------------------------- !!! /!\  !!!
glm::vec3 AngularVel;

float mass = 4.f;
glm::vec3 torque;
glm::vec3 FG;

glm::mat3 ITensorInv;
glm::mat3 IBody;

const float tolerancia = 0.0001;

namespace Cube {
	extern void setupCube();
	extern void cleanupCube();
	extern void updateCube(const glm::mat4& transform);
	extern void drawCube();
	extern glm::vec3 cubeVerts[];
}

void generateMatTrans();
bool ColisionBox(const glm::vec3 vertex, float dt);
void ApplyGravity(float dt);
float BolzanoTime(float first, float last,glm::vec3 vertex);

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
	oldPos = Pos;
	IBody = { (1 / 12.f * mass * 2), 0, 0,
			0, (1 / 12.f * mass * 2), 0,
			0, 0, (1 / 12.f * mass * 2) };

	generateMatTrans();
	LinMom = force01;
	FG = mass * gravity;
	torque = glm::cross(Cube::cubeVerts[1], force01);
	
}

void PhysicsUpdate(float dt) {
	ApplyGravity(dt);
	generateMatTrans();

	float t = 0;
	for (int i = 0; i < 7; i++)
		if (ColisionBox(oldPos + Cube::cubeVerts[i], dt))
			t = BolzanoTime(0, dt, oldPos + Cube::cubeVerts[i]);

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
	mattrans = matPos*glm::toMat4(rotQuat);
}

bool ColisionBox(const glm::vec3 preVertex, float dt)
{
	glm::vec3 vertex = preVertex; //CALUCLAR NOU VERTEX AMB ANTIC + DT <------------------------------------------------------------------------------------- !!! /!\  !!!

	float distantplaneX01 = (vertex.x * 1) + 5; //distancia plano frontal
	float distantplaneY01 = (vertex.y * 1); //plano inferior
	float distantplaneZ01 = (vertex.z * 1) + 5; //plano lateral derecho
	float distantplaneZ02 = (vertex.z * -1) + 5; //plano lateral izquierdo
	float distantplaneX02 = (vertex.x * -1) + 5;//plano trasero
	float distantplaneY02 = (vertex.y * -1) + 10; //plano superior


																	//Rebotes planos frontales
	if (distantplaneX01 <= 0) //frotal
	{
		return true;
	}
	if (distantplaneX02 <= 0)//trasero
	{
		return true;
	}


	if (distantplaneY01 <= 0)
	{
		return true;

	}

	if (distantplaneY02 <= 0)
	{
		return true;
	}


	////Rebotes Planos Laterales
	if (distantplaneZ01 <= 0)//derecha
	{
		return true;

	}

	if (distantplaneZ02 <= 0)//izquierda
	{
		return true;

	}
	return false;
}


void ApplyGravity(float dt) {
	LinMom += dt * FG;
	AngMom += dt*torque;
	torque = glm::vec3(0);
	Vel = LinMom / mass;
	oldPos = Pos;
	Pos += dt *	Vel;
	ITensorInv = glm::toMat3(rotQuat)*glm::inverse(IBody)*glm::transpose(glm::toMat3(rotQuat));
	AngularVel = ITensorInv*LinMom;
	rotQuat = glm::normalize(rotQuat + dt*(glm::quat(0, AngularVel)*rotQuat));
}

float BolzanoTime(float first, float last, const glm::vec3 vertex) {
	float mid = first + (last - first) / 2;

	if (last - first > tolerancia) {
		if (ColisionBox(vertex,mid)) {
			last = BolzanoTime(first, mid, vertex);
		}
		else {
			first = BolzanoTime(mid, last, vertex);
		}
	}

	if (last - first <= tolerancia){
		return last;
	}
	else {
		return mid;
	}
}