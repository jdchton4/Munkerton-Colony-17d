// main.cpp

#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#include "enemy.h"
#include "raylib.h"

#include "player.h"
#include "world.h"
#include "debug.h"
#include "hud.h"
#include "camera.h"
#include "inventory.h"
#include "timer.h"
#include "cursor.h"
#include "item.h"

#define DEBUG
int main() {

	// Window Init 
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);  // Parameters for window initialization
	InitWindow(700, 700, "Munker"); // Junk l
	int currentmonitor = GetCurrentMonitor(); // Stores monitor data
	int monitorwidth = GetMonitorWidth(currentmonitor); // Places monitor data in variable
	int monitorheight = GetMonitorHeight(currentmonitor);
	SetWindowSize(monitorwidth, monitorheight); // Set window size based on user display
	MaximizeWindow();
	SetTargetFPS(120);

	Inventory Inv;


	Texture2D enemytexture = LoadTexture("resources/alienfin.png"); // placeholder enemy texture

	Timer Tick;
	HUD StaminaBar;
	HUD HealthBar;
	Cursor Mouse;
	Level Room; 
	Player User; 
	Enemy Foe;
	GlobalEnemy GlobalEnemies;
	StaminaVial StamVial;
	VitalityVial VitVial;
	// Camera Init
	Camera2D Camera = { 0 };

	// Init everything else
	InitAudioDevice();
	TimerInit(Tick);
	PlayerInit(User, Tick);
	RoomInit(Room);
	StambarInit(StaminaBar);
	HealthbarInit(HealthBar);
	EnemyInit(Foe, GlobalEnemies, User);
	CameraInit(Camera, User);
	InventoryInit(Inv, Camera);
	StaminaVialInit(StamVial);
	VitalityVialInit(VitVial);

	// Audio Init
	Music DefBGost = LoadMusicStream("resources/bgm/worldaudio1v2.wav");
	Music CurrentTrack = DefBGost;



	// Game Loop
	while (!WindowShouldClose()) {
		if (!IsSoundPlaying(User.SpawnSfx) && !IsMusicStreamPlaying(DefBGost) && User.IsAlive == true) PlayMusicStream(CurrentTrack);
		Camera.target = User.Pos;
		UpdateMusicStream(CurrentTrack); // Update Music Buffer
		UpdatePlayer(User, Room, Tick); // Constantly check for player movement updates before drawing
		UpdateCursor(Mouse);
		GenerateEnemies(Foe, User, Tick, GlobalEnemies); // Check for 
		UpdateEnemies(Foe, User);
		BeginDrawing(); // Begin drawing after changes; Refresh screen 1st, draw world 2nd, draw player 3rd for proper layering.
		BeginMode2D(Camera); // Cam Start
		ClearBackground(GRAY);
		DrawWorld(Room);
		DrawPlayer(User);
		DrawEnemies(User); 
		EndMode2D(); // Cam End
		DrawStamHUD(User, StaminaBar);
		DrawHealthHUD(User, HealthBar);
		OpenInventory(Inv, User, Camera, Tick, Mouse, StamVial, VitVial);
#ifdef DEBUG
		DEBUGdisplaydiog(User, Room, Camera, Inv, Tick, GlobalEnemies, Mouse);
#endif
		EndDrawing();
	}
	UnloadTexture(User.TextureIdle); // Clean up
	UnloadTexture(Room.texture);
	CloseWindow();
	return 0;
};