#include <GL\glew.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_sdl_gl3.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\intersect.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

 

glm::vec3 Pos;
glm::mat3 Rot;
glm::vec3 LinMom;
glm::vec3 AngMom;


namespace Cube {
	extern void setupCube();
	extern void cleanupCube();
	extern void updateCube(const glm::mat4& transform);
	extern void drawCube();
}


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
}

void PhysicsUpdate(float dt) {
	glm::mat4 scale = { 
		2.f,0.f,0.f,0.f,
		0.f,2.f,0.f,0.f,
		0.f,0.f,2.f,0.f,
		0.f,0.f,0.f,2.f,
	};
	Cube::updateCube(scale);
}

void PhysicsCleanup() {
	Cube::cleanupCube();
}