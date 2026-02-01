#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <vector>
#include <iostream>
#include <chrono>
#include <cmath> 


long double G = 6.6743e-11;

struct Particle {
	long double Mass;
	long double PositionX;
	long double PositionY;
	long double Radius;
	long double VelocityX;
	long double VelocityY;
	long double AccelerationX;
	long double AccelerationY;
	long double ForceX;
	long double ForceY;
};

std::vector<Particle> ParticleVector;
Vector2 PreviousCursorPosition;
Vector2 CurrentCursorPosition;
Vector2 CursorPosition;
Vector2 ScreenCoOrdinates;
int MouseWheelDirection;



bool NeedVelocity;
long double StartPositionX;
long double StartPositionY;
long double EndPositionX;
long double EndPositionY;
long double Distance;
long double Angle;
long double DeltaX;
long double DeltaY;
long double UX;
long double UY;
long double VX;
long double VY;
bool Paused;
//long long int Mass = 9223372036854775807;
long double Mass = 2e30;
long double TotalMass;
long double Radius;
long double PositionX;
long double PositionY;
long double VelocityX;
long double VelocityY;




long double LengthScale = 1e9; 
long double MassScale = 1e20; 
long double TimeScale = 2.1e7;
float ZoomStrength = 0.05;
int SimulationStartX = 250;
int SimulationStartY = 0;
int ConfiguratorStartX = 0;
int ConfiguratorStartY = 0;
int ScreenWidth;
int ScreenHeight;





int main() {
	InitWindow(1620, 800, "Gravity Simulator");

	Camera2D Camera = { 0 };
	Camera.offset = { (float)SimulationStartX, (float)SimulationStartY };
	Camera.target = { 0.0f, 0.0f };
	Camera.rotation = 0.0f;
	Camera.zoom = 1.0f;

	//SetTargetFPS(60);
	auto CurrentTime = std::chrono::steady_clock::now();

	
	while (WindowShouldClose() != true) {

		ScreenWidth = GetScreenWidth();
		ScreenHeight = GetScreenHeight();



		if (IsKeyPressed(KEY_SPACE)) {
			ParticleVector.clear();
			std::cout << "Particle Vector Cleared" << std::endl;
		}

		auto PreviousTime = CurrentTime;
		CurrentTime = std::chrono::steady_clock::now();
		auto DeltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(CurrentTime - PreviousTime);
		long double TimePassed = (DeltaTime.count() / 1e9) * TimeScale;



		BeginDrawing();
		ClearBackground(BLACK);
		DrawFPS((SimulationStartX + 20), (SimulationStartY + 20));

		//GuiGetState() == STATE_PRESSED

		DrawRectangle(ConfiguratorStartX, ConfiguratorStartY, (SimulationStartX - ConfiguratorStartX), ScreenHeight, GRAY);
		GuiSetStyle(DEFAULT, TEXT_SIZE, 30); // Only labels
		GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0x000000FF); // black
		GuiSetStyle(DEFAULT, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
		GuiLabel(Rectangle{ (float)ConfiguratorStartX,(float)(ConfiguratorStartY + 15),(float)SimulationStartX,15 }, "Gravity Simulator");
		if (GuiButton(Rectangle { (float)(ConfiguratorStartX + 20), 70, (float)(SimulationStartX - 20*2), 50 }, "RESET")) {
			ParticleVector.clear();
			std::cout << "Particle Vector Cleared" << std::endl;
		}




		
		BeginMode2D(Camera);

		MouseWheelDirection = GetMouseWheelMove();

		/*if (MouseWheelDirection == 1) {
			CursorPosition = GetScreenToWorld2D(GetMousePosition(), Camera);
			Camera.target = { CursorPosition.x, CursorPosition.y };
			Camera.zoom = Camera.zoom + ZoomStrength;
		} else if (MouseWheelDirection == -1) {
			CursorPosition = GetScreenToWorld2D(GetMousePosition(), Camera);
			Camera.target = { CursorPosition.x, CursorPosition.y };
			Camera.zoom = Camera.zoom - ZoomStrength;
		}*/

		if (MouseWheelDirection == 1) {
			PreviousCursorPosition = GetScreenToWorld2D(GetMousePosition(), Camera);
			Camera.zoom = Camera.zoom + ZoomStrength;
			if (Camera.zoom < 0) {
				Camera.zoom = 0.1;
			}
			CurrentCursorPosition = GetScreenToWorld2D(GetMousePosition(), Camera);
			Camera.target.x = Camera.target.x + (PreviousCursorPosition.x - CurrentCursorPosition.x);
			Camera.target.y = Camera.target.y + (PreviousCursorPosition.y - CurrentCursorPosition.y);

		} else if (MouseWheelDirection == -1) {
			PreviousCursorPosition = GetScreenToWorld2D(GetMousePosition(), Camera);
			Camera.zoom = Camera.zoom - ZoomStrength;
			if (Camera.zoom < 0) {
				Camera.zoom = 0.1;
			}
			CurrentCursorPosition = GetScreenToWorld2D(GetMousePosition(), Camera);
			Camera.target.x = Camera.target.x + (PreviousCursorPosition.x - CurrentCursorPosition.x);
			Camera.target.y = Camera.target.y + (PreviousCursorPosition.y - CurrentCursorPosition.y);
		}

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && GetMousePosition().x > SimulationStartX) {
			CursorPosition = GetScreenToWorld2D(GetMousePosition(), Camera);
			NeedVelocity = true;
			StartPositionX = CursorPosition.x * LengthScale;
			StartPositionY = CursorPosition.y * LengthScale;
		}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && NeedVelocity) {
			CursorPosition = GetScreenToWorld2D(GetMousePosition(), Camera);
			EndPositionX = CursorPosition.x * LengthScale;
			EndPositionY = CursorPosition.y * LengthScale;
		}

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && NeedVelocity) {
			ParticleVector.push_back(Particle{ Mass, StartPositionX, StartPositionY, (20 * LengthScale), ((EndPositionX - StartPositionX) / TimeScale), ((EndPositionY - StartPositionY) / TimeScale), 0, 0, 0, 0 });
			NeedVelocity = false;
		}



		for (int i = 0; i < ParticleVector.size(); i++) {
			for (int j = 0; j < ParticleVector.size(); j++) {
				if (i == j) continue;
				Distance = std::sqrt(std::pow((ParticleVector[i].PositionX - ParticleVector[j].PositionX), 2) + std::pow((ParticleVector[i].PositionY - ParticleVector[j].PositionY), 2));
				if (Distance < (ParticleVector[i].Radius + ParticleVector[j].Radius) / 2) {
					VelocityX = 0;
					VelocityY = 0;
					TotalMass = ParticleVector[i].Mass + ParticleVector[j].Mass;
					Radius = std::sqrt(((PI * ParticleVector[i].Radius * ParticleVector[i].Radius) + (PI * ParticleVector[j].Radius * ParticleVector[j].Radius)) / PI);
					PositionX = (ParticleVector[i].PositionX + ParticleVector[j].PositionX) / 2;
					PositionY = (ParticleVector[i].PositionY + ParticleVector[j].PositionY) / 2;
					VelocityX = (ParticleVector[i].Mass * ParticleVector[i].VelocityX + ParticleVector[j].Mass * ParticleVector[j].VelocityX) / TotalMass;
					VelocityY = (ParticleVector[i].Mass * ParticleVector[i].VelocityY + ParticleVector[j].Mass * ParticleVector[j].VelocityY) / TotalMass;
					ParticleVector.push_back(Particle{ TotalMass, PositionX, PositionY, Radius, VelocityX, VelocityY, 0, 0, 0, 0 });
					if (i > j) {
						ParticleVector.erase(ParticleVector.begin() + i);
						ParticleVector.erase(ParticleVector.begin() + j);
						
					}
					if (j > i) {
						ParticleVector.erase(ParticleVector.begin() + j);
						ParticleVector.erase(ParticleVector.begin() + i);
						
					}
				}
			}
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
			Vector2 Particle = { ParticleVector[i].PositionX/LengthScale,ParticleVector[i].PositionY/LengthScale };
			if ((GetWorldToScreen2D(Particle, Camera).x > SimulationStartX) && (GetWorldToScreen2D(Particle, Camera).y > SimulationStartY)) {
				DrawCircle(((ParticleVector[i].PositionX / LengthScale)), ((ParticleVector[i].PositionY / LengthScale)), (ParticleVector[i].Radius / LengthScale), WHITE);
			}
		}


		//std::cout << NumberOfMasses << std::endl;
		EndMode2D();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}