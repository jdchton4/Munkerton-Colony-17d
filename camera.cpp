// camera.cpp

#include "raylib.h"
#include "camera.h"
#include "player.h"
#include "timer.h"

void CameraInit(Camera2D& c, Player& p) {
	int monitor = GetCurrentMonitor();
	int monitorwidth = GetMonitorWidth(monitor);
	int monitorheight = GetMonitorHeight(monitor);
	c = { 0 };
	c.target = p.Pos;
	c.offset = { (float)monitorwidth / 2, (float)monitorheight / 2 };
	c.rotation = 0.0;
	c.zoom = 1.0f;
}

void CameraScreenShake(Camera2D& c, Player& p, Timer& t, float Duration)
{
	float dt = GetFrameTime();
	if (t.CamShakeTick < Duration) {
		t.CamShakeTick += dt;
		// Define how violent you want the shake to be (in pixels)
		float shakeIntensity = 8.0f;

		// Generate a random offset between -shakeIntensity and +shakeIntensity
		float offsetX = (float)GetRandomValue(-100, 100) / 100.0f * shakeIntensity;
		float offsetY = (float)GetRandomValue(-100, 100) / 100.0f * shakeIntensity;

		// Apply the random shake on top of the player's tracking position
		c.target.x = p.Pos.x + offsetX;
		c.target.y = p.Pos.y + offsetY;
	}
	else {
		c.target.x = p.Pos.x;
		t.CamShakeTick = 0;
	}
}

