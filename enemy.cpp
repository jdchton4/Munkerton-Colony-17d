// Enemy.cpp
#include "enemy.h"
#include "raylib.h"
#include "player.h"
#include "timer.h"

#define MOVE_CONST 10.0;

int EnemyCount = 0;

std::vector<Enemy> ActiveEnemies;
void EnemyInit(Enemy& e, GlobalEnemy& ge, Player& p) {
	e.enemyname = "enemyplacehold";
	e.Pos = { 0.0, 0.0 };
	e.Health = 1.0;
	e.HealthMax = 1.0;
	e.Speed = float(1.0);
	e.SightDistance = 1.0;
	e.atk1 = 1; // Damage values for different attacks the enemy may have
	e.atk2 = 1;
	e.atk3 = 1;
	e.AtkRange = 50;
	e.IsAlive = true;
	e.InSight = false;
	e.StateChange = false;
	Texture2D enemytexture = LoadTexture("resources/spritesheets/mudeaterbaseidlespritesheet.png"); // starting player texture
	e.TextureAtk = enemytexture;
	e.TextureIdle = enemytexture;
	e.TextureMoving = enemytexture;

	switch (p.WorldProgress) {
	// spawnrate is the number that has to be reached for an enemy to spawn; lower numbers ~ higher frequency
	case 1: ge.SpawnRate = 5; break; // GetRandomValue(500, 600); break;
	case 2: ge.SpawnRate = GetRandomValue(500, 600); break;
	case 3: ge.SpawnRate = GetRandomValue(500, 600); break;
	case 4: ge.SpawnRate = GetRandomValue(450, 550); break;
	case 5: ge.SpawnRate = GetRandomValue(450, 550); break;
	case 6: ge.SpawnRate = GetRandomValue(450, 550); break;
	case 7: ge.SpawnRate = GetRandomValue(400, 500); break;
	case 8: ge.SpawnRate = GetRandomValue(400, 500); break;
	case 9: ge.SpawnRate = GetRandomValue(400, 500); break;
	case 10: ge.SpawnRate = GetRandomValue(350, 450); break;
	default: ge.SpawnRate = GetRandomValue(350, 450); break;
	}
}

void GenerateEnemies(Enemy& e, Player& p, Timer& t, GlobalEnemy& ge) {

	int MaxEnemies = 0;
	
	// Checks world progress to set max enemies
	{
		switch (p.WorldProgress) {
		case 1: MaxEnemies = 5; break;
		case 2: MaxEnemies = 8; break;
		case 3: MaxEnemies = 11; break;
		case 4: MaxEnemies = 13; break;
		case 5: MaxEnemies = 14; break;
		case 6: MaxEnemies = 15; break;
		case 7: MaxEnemies = 16; break;
		case 8: MaxEnemies = 17; break;
		case 9: MaxEnemies = 18; break;
		case 10: MaxEnemies = 20; break;
		default: MaxEnemies = 5; break;
		}
	}



	if (ActiveEnemies.size() < MaxEnemies) {
		if (t.EnemyGenTick >= ge.SpawnRate) { // when the ticker reaches the world progs determined spawnrate, spawn an enemy
			EnemyCount++;
			// Name; (Vector) Pos; Health; MaxHealth; Speed; SightDistance; AtkDmg 1 ; 2 ; 3 ; AtkRange ;IsAlive; In Sight; StateChange; State; Facing; CurText ; TIdle; TMove; TAtk;
			Enemy MudEater = { "MudEater_" + std::to_string(EnemyCount), GetNewEnemyPos(p), 100.0, 100.0, 3.0f, 900.0, 4, 5, 6, 30, true, false, false, Enemy::IDLE, Enemy::NO_DIR, e.TextureIdle, e.TextureIdle, e.TextureIdle };
			ActiveEnemies.push_back(MudEater);
			t.EnemyGenTick = 0; // reset enemy spawn timer for the next one
		}
		t.EnemyGenTick += .1f;
	}
	return;
}
// test
// Generates a random position for an enemy to spawn in
Vector2 GetNewEnemyPos(Player& p) {
	Vector2 NewEnemyPos = { 0.0f, 0.0f };
	float GenRadiusMax = 900.0f;
	float GenRadiusMin = 700.0f;

	float RandomAngle = GetRandomValue(0, 360) * ((float)PI / (float)180.0); 
	float RandomRadius = GetRandomValue((float)GenRadiusMin, (float)GenRadiusMax);

	NewEnemyPos.x = p.Pos.x + RandomRadius * std::cos(RandomAngle); // spawns enemy random distance ( rad ) and angle ( cos ) away
	NewEnemyPos.y = p.Pos.y + RandomRadius * std::sin(RandomAngle);// spawns enemy random distance ( rad ) and angle ( sin ) away

	return NewEnemyPos;
}

void EnemyState(Enemy& e, Player& p) {
	for (int i = 0; i < ActiveEnemies.size(); i++) { // loops through each live enemy 
		// Check if enemy is dead
		if (ActiveEnemies[i].Health <= 0) {
			ActiveEnemies[i].State = Enemy::DEAD;
			ActiveEnemies[i].StateChange = true;
			ActiveEnemies[i].IsAlive = false;
		}
		// Check if player is within enemy sightlines
		if (ActiveEnemies[i].State != Enemy::DEAD) {
			float DistSq = ((p.Pos.x - ActiveEnemies[i].Pos.x) * (p.Pos.x - ActiveEnemies[i].Pos.x)) + ((p.Pos.y - ActiveEnemies[i].Pos.y) * (p.Pos.y - ActiveEnemies[i].Pos.y));
			float RadSq = (ActiveEnemies[i].SightDistance) * (ActiveEnemies[i].SightDistance);
			float AtkRangeRadSq = (ActiveEnemies[i].AtkRange) * (ActiveEnemies[i].AtkRange);
			// (px - ex)^2 + (py - ey)^2 <= r^2 is the condition for player being in enemy sight
			if (ActiveEnemies[i].State == Enemy::CHASE || DistSq <= AtkRangeRadSq) {
				ActiveEnemies[i].State = Enemy::ATTACK;
				ActiveEnemies[i].StateChange = true;
			} else if (DistSq <= RadSq) {
				ActiveEnemies[i].InSight = true;
				ActiveEnemies[i].State = Enemy::CHASE;
				ActiveEnemies[i].StateChange = true;
			}
			else {
				ActiveEnemies[i].InSight = false;
				ActiveEnemies[i].State = Enemy::IDLE;
				ActiveEnemies[i].StateChange = true;
			}
		}

	}
	for (auto it = ActiveEnemies.begin(); it != ActiveEnemies.end(); ) { // cleans out dead enemies from ActiveEnemies[] vector
		if (it->State == Enemy::DEAD) {
			it = ActiveEnemies.erase(it);
		}
		else {
			++it; 
		}
	}
	return;
}

void UpdateEnemies(Enemy& e, Player& p) {
	EnemyState(e, p);
	float dt = GetFrameTime();
	// Directional check for flipping sprites
	for (int i = 0; i < ActiveEnemies.size(); i++) {
		if (p.Pos.x - ActiveEnemies[i].Pos.x >= 0) {
			ActiveEnemies[i].Facing = Enemy::E;
		}
		else {
			ActiveEnemies[i].Facing = Enemy::W;
		}
	}
	// Finding player & shortest distance
	for (int i = 0; i < ActiveEnemies.size(); i++) {
		Vector2 PathToPlayer = { 0.0f,0.0f };
		float DistanceToPlayer = 0.0f;
		if (ActiveEnemies[i].State == Enemy::CHASE) {
			PathToPlayer = { p.Pos.x - ActiveEnemies[i].Pos.x, p.Pos.y - ActiveEnemies[i].Pos.y };
			DistanceToPlayer = sqrtf((PathToPlayer.x * PathToPlayer.x) + (PathToPlayer.y * PathToPlayer.y));
			if (DistanceToPlayer > 0) {
				PathToPlayer.x /= DistanceToPlayer;
				PathToPlayer.y /= DistanceToPlayer;
			}
			float EnemySpeed = ActiveEnemies[i].Speed;
			ActiveEnemies[i].Pos.x += PathToPlayer.x * EnemySpeed;
			ActiveEnemies[i].Pos.y += PathToPlayer.y * EnemySpeed;
		}
	}
}
void DrawEnemies(Player& p) {
	for (int i = 0; i < ActiveEnemies.size(); i++) {
		ActiveEnemies[i].CurrentTexture = ActiveEnemies[i].TextureIdle;
		float FrameSpeed = .30f;
		float scale = 5.0;
		float FrameWidth = (float)ActiveEnemies[i].CurrentTexture.width / 5.00;
		float FrameHeight = (float)ActiveEnemies[i].CurrentTexture.height;
		float ScaledWidth = FrameWidth * p.scale;
		float ScaledHeight = FrameHeight * p.scale;
		static int CurrentFrame = 0;
		static int FrameCount = 0;
		Vector2 origin = { ScaledWidth / 2.0f, ScaledHeight / 2.0f }; // middle of the box
		FrameCount++; // # slice projected
		Rectangle FrameRect = { CurrentFrame * FrameWidth, 0.0f, FrameWidth, FrameHeight }; // Slice of sprite sheet to be projected
		Rectangle DestRect = { ActiveEnemies[i].Pos.x, ActiveEnemies[i].Pos.y, ScaledWidth, ScaledHeight}; // Area where sheet is projected


		//if (ActiveEnemies[i].StateChange == true) {
		//	CurrentFrame = 0;
		//	FrameCount = 0;
		//}
		if (FrameCount >= (60 / FrameSpeed))
		{
			FrameCount = 0;
			CurrentFrame++;
			if (CurrentFrame > 4) CurrentFrame = 0;
		}

		if (ActiveEnemies[i].Facing == Enemy::W) FrameRect.width = FrameWidth * -1;
		DrawTexturePro(ActiveEnemies[i].CurrentTexture, FrameRect, DestRect, origin, 0.0f, WHITE);
		ActiveEnemies[i].StateChange = false;
	}

	return;
}

void UpdateEnemiesMeleeDumb(Player& p, Enemy& e) {

	/*
	* Check if Alive //
	*	Use IsAlive property//
	* Delete Enemy if not and clean Enemy vector//
	* Find Player with GetPlayerPos(p);
	* Update direction facing//
	* Find Shortest vector to player
	*	e.Pos.x - p.Pos.x = distx; same for y
	* Check for obstacles -> Adjust to a location near the obstacle and set target there if one is in the way
	*	world is set up on tile grid
	*	each obstacle has a set dimension that marks corresponding tiles as occupied_by_object
	*	
	* Set target and move to that location 
	*	Use EnemyMove(float distx, float disty);
	* Dont update target until player gets a certain distance ( check with vector ) away from target or Timer.EnemyTargetTime depletes
	* if Enemy is close 
	* Call EnemyAttack();
	* if Player is too far away
	* Try again
	*/
	return;
}

void EnemyAttack(Enemy& e, Player& p) {
	return;
}
