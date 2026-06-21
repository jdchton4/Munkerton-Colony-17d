// @file hud.cpp
// @author jdeepak
#include "raylib.h"
#include "hud.h"

void StambarInit(HUD& h) {
	Texture2D stambarsprite = LoadTexture("resources/statbar1.png"); // Stamina bar border
	h.bordertexture = stambarsprite;
	h.Pos.x = STAMINA_POS_X;
	h.Pos.y = STAMINA_POS_Y;
}

void HealthbarInit(HUD& h) {
	Texture2D healthbarsprite = LoadTexture("resources/statbar1.png"); // Health bar border
	h.bordertexture = healthbarsprite;
	h.Pos.x = HEALTH_POS_X;
	h.Pos.y = HEALTH_POS_Y;
}

void DrawStamHUD(Player& p, HUD& h) {
	float barwidth = 240.0; // fills up the bar completely
	float barheight = 65.0;
	float stampercent = p.Stamina / p.StaminaMax; // How much Stamina is left as a percentage
	barwidth = 240 * stampercent;
	unsigned char StamColor = stampercent * 255;
	if (StamColor < 100) StamColor = 100;
	if (p.StamCD == true) { // If Dash/Sprint disabled
		DrawRectangle(STAMINA_POS_X, STAMINA_POS_Y, int(barwidth), int(barheight), GRAY); // Draws orange Stamina Bar
		DrawTextureEx(h.bordertexture, h.Pos, 0.0f, 3.0f, RED); // Draw Red Border
	}
	else if (p.StamCD == false) {
		DrawRectangle(STAMINA_POS_X, STAMINA_POS_Y, int(barwidth), int(barheight), {255, StamColor, 0, 255}); // Draws yellow Stamina Bar
		DrawTextureEx(h.bordertexture, h.Pos, 0.0f, 3.0f, WHITE); // Draw White Border
	}
	return; 
}

void DrawHealthHUD(Player& p, HUD& h) {
	float barwidth = 240.0; // fills up the bar completely
	float barheight = 65.0;
	float healthpercent = p.Health/p.HealthMax; // How much Health is left as a percentage
	barwidth = 240 * healthpercent;
	unsigned char HealthColor = healthpercent * 255;
	if (HealthColor < 100) HealthColor = 100;
	if (healthpercent > .30) { // above low Health
		DrawRectangle(HEALTH_POS_X, HEALTH_POS_Y, int(barwidth), int(barheight), {HealthColor, 0, 0, 255}); // Draws Green Health Bar
		DrawTextureEx(h.bordertexture, h.Pos, 0.0f, 3.0f, WHITE); // Draw white Border
	}
	else if (healthpercent <= .30) { // low Health
		DrawRectangle(HEALTH_POS_X, HEALTH_POS_Y, int(barwidth), int(barheight), {HealthColor, 0, 0, 255}); // Draws Health Bar
		DrawTextureEx(h.bordertexture, h.Pos, 0.0f, 3.0f, RED); // Draw red Border
	}

	return;
}
