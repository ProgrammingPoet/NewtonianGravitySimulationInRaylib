#include "raylib.h"
#include <vector>
#include <iostream>

struct Particle {
	float Mass;
	float PositionX;
	float PositionY;
	float Radius;
	float VelocityX;
	float VelocityY;
};

std::vector<Particle> ParticleVector;
Vector2 CursorPosition;

bool NeedVelocity;
float StartPositionX;
float StartPositionY;
float EndPositionX;
float EndPositionY;


int main() {
	InitWindow(600, 800, "Gravity Sim");
	SetTargetFPS(60);

	while (WindowShouldClose() != true) {
		BeginDrawing();
		ClearBackground(BLACK);
		DrawFPS(20, 20);
		
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			CursorPosition = GetMousePosition();
			NeedVelocity = true;
			StartPositionX = CursorPosition.x;
			StartPositionY = CursorPosition.y;
			std::cout << CursorPosition.x << ' ' << CursorPosition.y << std::endl;
		}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && NeedVelocity) {
			CursorPosition = GetMousePosition();
			EndPositionX = CursorPosition.x;
			EndPositionY = CursorPosition.y;
		}

		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && NeedVelocity) {
			ParticleVector.push_back(Particle{ 20, StartPositionX, StartPositionY, 5, (EndPositionX - StartPositionX), (EndPositionY - StartPositionY) });
			NeedVelocity = false;
		}

		for (int i = 0; i < ParticleVector.size(); i++) {
			ParticleVector[i].PositionX += ParticleVector[i].VelocityX;
			ParticleVector[i].PositionY += ParticleVector[i].VelocityY;
		}

		for (int i = 0; i < ParticleVector.size(); i++) {
			DrawCircle(ParticleVector[i].PositionX, ParticleVector[i].PositionY, ParticleVector[i].Radius, BLUE);
		}


		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}