// @author jdeepak
// @file player.h

#pragma once
#include "raylib.h"
#include "world.h"
#include "timer.h"


struct Timer;
struct Camera2D;

struct Player { // Player Stats, States, Textures, Sounds, Orientations

	enum PlayerState {
		MOVING,
		SPRINTING,
		IDLE,
		DASHING,
		DEAD,
		NO_STATE,
		ATTACK,
	};
	// Direction Player is facing
	enum Orientation { 
		NO_DIR, 
		N, 
		NE,
		E,
		SE,
		S,
		SW,
		W,
		NW,
	};

	Vector2 Pos;
	Vector2 NextPos;

	int scale;
	int gamestage;
	int WorldProgress;

	float Speed;
	float Health;
	float HealthMax;
	float Stamina;
	float StaminaMax;
	float StaminaUpgrade;
	float SprintBuff;
	float DashBuff;
	float DashTimer;

	bool IsDashing;
	bool IsSprinting;
	bool StamCD;
	bool IsWalking;
	bool IsIdle;
	bool IsAlive;
	bool InventoryIsOpen;

	Texture2D TextureStatic;
	Texture2D TextureDead;
	Texture2D TextureIdle;
	Texture2D TextureMoving;
	Texture2D TextureMovingNE;
	Texture2D TextureMovingSE;
	Texture2D TextureDashing;
	Texture2D TextureDashingNE;
	Texture2D TextureDashingSE;
	Texture2D CurrentTexture;

	Sound DashSfx;
	Sound MoveSfx;
	Sound SprintSfx;
	Sound StamDepleteSfx;
	Sound DeathSfx;
	Sound SpawnSfx;
	Rectangle HitboxH; // Head Hitbox
	Rectangle HitboxT; // Torso Hitbox
	Rectangle HitboxL; // Legs Hitbox
	PlayerState State;
	Orientation Facing;
};

struct PlayerTextures {
	Texture2D TextureStatic;
	Texture2D TextureIdle;
	Texture2D TextureMoving;
	Texture2D TextureMovingNE;
	Texture2D TextureMovingSE;
	Texture2D TextureDashing;
};

struct PlayerSounds {
	Sound DashSfx;
	Sound MoveSfx;
	Sound SprintSfx;
	Sound StamDepleteSfx;
};

void PlayerInit(Player& p, Timer& t);
void UpdatePlayer(Player& p, Level& l, Timer& t); // Main function handling player movement, Stamina, dash, sprint, bounds, input, death check in one
void PlayerHitbox(Player& p);
void DrawPlayer(Player& p); // Draws Player to screen; Handles player textures and animation
void SetPlayerPos(Player& p, Vector2 dest);
void PlayerDash(Player& p, float dt, Timer& t, float& CurrentSpeed); // Handles player dash and dash sound
void PlayerSprint(Player& p, float dt, Timer& t, float& CurrentSpeed);
void GetPlayerInput(Player& p, float& MoveX, float& MoveY);
void PlayerStamina(Player& p, float dt, Timer& t);
Vector2 GetPlayerPos(Player& p);
void PlayerDeath(Player& p, Timer& t);
