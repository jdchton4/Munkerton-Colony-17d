// @author jdeepak

#pragma once
#include "raylib.h"
#include "player.h"

#define STAMINA_POS_X 50
#define STAMINA_POS_Y 125

#define HEALTH_POS_X 50
#define HEALTH_POS_Y 50
struct HUD {
	Texture2D bordertexture;
	Sound stamdepletesfx;
	Vector2 Pos;
};

void DrawStamHUD(Player& p, HUD& h);
void DrawHealthHUD(Player& p, HUD& h);
void PlayerInit(Player& User);
void StambarInit(HUD& h);
void HealthbarInit(HUD& h);