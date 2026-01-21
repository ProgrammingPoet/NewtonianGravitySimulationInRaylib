#include "raylib.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <cmath> 


int NumberOfMasses = 0;
long double G = 6.6743e-11;

struct Particle {
	int Index;
	long long int Mass;
	float PositionX;
	float PositionY;
	float Radius;
	float VelocityX;
	float VelocityY;
	float AccelerationX;
	float AccelerationY;
	float ForceX;
	float ForceY;
};

std::vector<Particle> ParticleVector;
Vector2 CursorPosition;

bool NeedVelocity;
float StartPositionX;
float StartPositionY;
float EndPositionX;
float EndPositionY;
float Distance;
float Angle;
float DeltaX;
float DeltaY;
float UX;
float UY; 
float VX;
float VY;
bool Paused;
long long int Mass = 9223372036854775807;




int main() {
	InitWindow(1280, 720, "Gravity Sim");

	//SetTargetFPS(60);
	auto CurrentTime = std::chrono::steady_clock::now();

	
	while (WindowShouldClose() != true) {

		if (IsKeyPressed(KEY_SPACE)) {
			std::cout << "Simulation Is Currently Paused" << std::endl;
			std::cout << "Enter Mass " << std::endl;
			std::cin >> Mass;

		}

		auto PreviousTime = CurrentTime;
		CurrentTime = std::chrono::steady_clock::now();
		auto DeltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - PreviousTime);
		double TimePassed = DeltaTime.count() / 1000.0; //seconds



		BeginDrawing();
		ClearBackground(BLACK);
		DrawFPS(20, 20);
		
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			CursorPosition = GetMousePosition();
			NeedVelocity = true;
			StartPositionX = CursorPosition.x;
			StartPositionY = CursorPosition.y;
		}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && NeedVelocity) {
			CursorPosition = GetMousePosition();
			EndPositionX = CursorPosition.x;
			EndPositionY = CursorPosition.y;
		}

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && NeedVelocity) {
			ParticleVector.push_back(Particle{ NumberOfMasses, Mass, StartPositionX, StartPositionY, 5, (EndPositionX - StartPositionX), (EndPositionY - StartPositionY), 0, 0, 0, 0});
			NumberOfMasses++;
			NeedVelocity = false;
		}

		

		for (int i = 0; i < ParticleVector.size(); i++) {
			ParticleVector[i].ForceX = 0;
			ParticleVector[i].ForceY = 0;
			for (int j = 0; j < ParticleVector.size(); j++) {
				if (i == j) continue;
				Distance = std::sqrt(std::pow((ParticleVector[i].PositionX - ParticleVector[j].PositionX), 2) + std::pow((ParticleVector[i].PositionY - ParticleVector[j].PositionY), 2));
				DeltaX = ParticleVector[j].PositionX - ParticleVector[i].PositionX;
				DeltaY = ParticleVector[j].PositionY - ParticleVector[i].PositionY;

				//Angle = std::acos(ParticleVector[i].PositionX - ParticleVector[j].PositionX) / Distance;
				ParticleVector[i].ForceX = ((G * ParticleVector[i].Mass * ParticleVector[j].Mass) / std::pow((Distance), 2)) * (DeltaX / Distance) + ParticleVector[i].ForceX;
				ParticleVector[i].ForceY = ((G * ParticleVector[i].Mass * ParticleVector[j].Mass) / std::pow((Distance), 2)) * (DeltaY / Distance) + ParticleVector[i].ForceY;
				ParticleVector[i].AccelerationX = ParticleVector[i].ForceX / ParticleVector[i].Mass;
				ParticleVector[i].AccelerationY = ParticleVector[i].ForceY / ParticleVector[i].Mass;
			}
		}

		for (int i = 0; i < ParticleVector.size(); i++) {

			UX = ParticleVector[i].VelocityX;
			UY = ParticleVector[i].VelocityY;

			VX = ParticleVector[i].VelocityX + ParticleVector[i].AccelerationX * TimePassed;
			VY = ParticleVector[i].VelocityY + ParticleVector[i].AccelerationY * TimePassed;

			if (UX > 299792458) {
				UX = 299792458;
			}

			if (UY > 299792458) {
				UY = 299792458;
			}

			if (VX > 299792458) {
				VX = 299792458;
			}

			if (VY > 299792458) {
				VY = 299792458;
			}

			ParticleVector[i].PositionX += (UX + ((UX + VX)) / 2) * TimePassed;
			ParticleVector[i].PositionY += (UY + ((UY + VY)) / 2) * TimePassed;
			ParticleVector[i].VelocityX = VX;
			ParticleVector[i].VelocityY = VY;
			
			
		}

		for (int i = 0; i < ParticleVector.size(); i++) {
			DrawCircle(ParticleVector[i].PositionX, ParticleVector[i].PositionY, ParticleVector[i].Radius, WHITE);
		}


		//std::cout << NumberOfMasses << std::endl;
		EndDrawing();
	}

	CloseWindow();
	return 0;
}