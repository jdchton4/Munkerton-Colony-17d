// debug.cpp

#include "raylib.h"
#include "debug.h"
#include "player.h"
#include "world.h"
#include "inventory.h"
#include "timer.h"
#include "enemy.h"
#include "cursor.h"

#define DISPLAYDIAG // Diagnostic viewer macro
#define CHEATSWITCH // Cheat switch macro
#define DRAWHITBOXPLAYER // Hit box view macro

void DEBUGcheatswitch(Player& p, Camera2D Cam, Inventory& i) {
	static int cheatswitch = 0;
	if (IsKeyPressed(KEY_SEMICOLON)) cheatswitch = 1;
	if (cheatswitch == 1) {
		p.Stamina = 1000;
		p.Health = 1000;
		p.DashBuff = 200.0;
		if (IsKeyPressed(KEY_SLASH)) Cam.zoom = (float).1;
		if (IsKeyPressed(KEY_APOSTROPHE)) cheatswitch = 0;
	}
	if (IsKeyPressed(KEY_ZERO) && cheatswitch == 0) {
		p.Stamina = p.StaminaMax;
		p.Health = p.HealthMax;
		p.DashBuff = 50.0;
		if (IsKeyPressed(KEY_SLASH)) Cam.zoom = 1;
	}
}
void DEBUGdisplaydiog(Player& p, Level& l, Camera2D Cam, Inventory& i, Timer& t, GlobalEnemy& ge, Cursor& m) {
	int ypos = 0;
	static int sel;
	char dummydata[] = "VALYOU";
	float frametime = GetFrameTime();
	int mon = GetCurrentMonitor();
	int monwidth = GetMonitorWidth(mon);
	int monheight = GetMonitorHeight(mon);

	// Diagnostics
	if (IsKeyPressed(KEY_SEVEN)) sel = 1;
	if (IsKeyPressed(KEY_EIGHT)) sel = 2;
	if (IsKeyPressed(KEY_NINE)) sel = 3;
	if (IsKeyPressed(KEY_SIX)) sel = 4;
	switch (sel) {
	case 1:
		ypos += 25;
		DrawText(TextFormat("PAGE ONE"), 1500, ypos, 30, YELLOW);
		ypos += 25;
		DrawText(TextFormat("X %.2f", p.Pos.x), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("Y %.2f", p.Pos.y), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("NAME: %.20s", dummydata), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("SPEED %.2f", p.Speed), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("REAL SPEED %.2f", p.Speed + p.SprintBuff), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("STAM %.2f", p.Stamina), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("STAM TICK %.2f", t.StamTick), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("Health %.2f", p.Health), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("STAM MAX %.2f", p.StaminaMax), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("NAME: %.20s", dummydata), 1500, ypos, 20, YELLOW);
		ypos += 25;
		break;
	case 2:
		ypos += 25;
		DrawText(TextFormat("PAGE TWO"), 1500, ypos, 30, YELLOW);
		ypos += 25;
		DrawText(TextFormat("CAMX: %.2f", i.bgpos.x), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("CAMY: %.2f", i.bgpos.y), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("FRAME TIME: %.20f", frametime), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("MONITOR WIDTH: %.20i", monwidth), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("MONITOR HEIGHT: %.20i", monheight), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("ENEMY GEN TICK: %.20f", t.EnemyGenTick), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("ENEMY SPAWN RATE: %i", ge.SpawnRate), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("MOUSE X: %.20f", m.Pos.x), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("MOUSE Y: %.20f", m.Pos.y), 1500, ypos, 20, YELLOW);
		ypos += 25;
		DrawText(TextFormat("FPS: %i", GetFPS()), 1500, ypos, 20, YELLOW);
		ypos += 25;
		break;
	case 3:
		break;
	case 4:
		PlayerHitbox(p);
		break;
	}
	DEBUGcheatswitch(p, Cam, i);
	return;
}

void DEBUGCallDebug(Player& p, Level& l, Camera2D Cam, Inventory& i, Timer& t, GlobalEnemy& ge, Cursor& m) {

#ifdef DISPLAYDIAG
	if (IsKeyPressed(KEY_SIX))
	DEBUGdisplaydiog(p, l, Cam, i, t, ge, m);
#endif
#ifdef CHEATSWITCH
	DEBUGcheatswitch(p, Cam, i);
#endif
#ifdef DRAWHITBOXPLAYER
#endif
}
