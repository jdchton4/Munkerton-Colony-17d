// @author jdeepak

#pragma once

#include <cmath>
#include <string>
#include <vector>
#include <iostream>

#include "raylib.h"

struct Timer;
struct Player;

struct Enemy {
	std::string enemyname;
	Vector2 Pos;
	float Health;
	float HealthMax;
	float Speed;
	float SightDistance;
	int atk1; // Damage values for different attacks the enemy may have
	int atk2;
	int atk3;
	int AtkRange;
	bool IsAlive;
	bool InSight;
	bool StateChange;
	enum EnemyState {
		MOVING,
		IDLE,
		DEAD,
		NO_STATE,
		ATTACK,
		HURT,
		CHASE,
	};
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

	EnemyState State;
	Orientation Facing;

	Texture2D CurrentTexture;
	Texture2D TextureIdle;
	Texture2D TextureMoving;
	Texture2D TextureAtk;
}; 

struct GlobalEnemy {
	int SpawnRate;
};

extern int EnemyCount;
extern std::vector<Enemy> ActiveEnemies;

void GenerateEnemies(Enemy& e, Player& p, Timer& t, GlobalEnemy& ge); // Sets max enemy count based on world progress and periodically generates enemies in a random bound around the player
void DrawEnemies(Player& p);
void UpdateEnemies(Enemy& e, Player& p);
void UpdateEnemiesMeleeDumb(Player& p, Enemy& e);
void EnemyAttack(Enemy& e, Player& p);
Vector2 GetNewEnemyPos(Player& p);
void EnemyInit(Enemy& e, GlobalEnemy& ge, Player& p);
void EnemyState(Enemy& e);