// @author jdeepak

#pragma once
#include "raylib.h"

struct Player;
struct Level;
struct Cam;
struct Inventory;
struct Timer;
struct GlobalEnemy;
struct Cursor;

void DEBUGdisplaydiog(Player& p, Level& l, Camera2D Cam, Inventory& i, Timer& t, GlobalEnemy& ge, Cursor& m);
void DEBUGcheatswitch(Player& p, Camera2D Cam, Inventory& i);
void DrawPlayerHitbox(Player& p);
void DEBUGCallDebug(Player& p, Level& l, Camera2D Cam, Inventory& i, Timer& t, GlobalEnemy& ge, Cursor& m);