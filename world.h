#pragma once
#include "raylib.h"

struct Level {
	int width;
	int height;
	int id;
	Texture2D texture;
	float scale;
	bool legalmove;
};
struct Tile {
	Vector2 Pos;
	Texture2D StoneTexture;
	Texture2D GrassTexture;
};

void DrawWorld(Level& l);
void RoomInit(Level& l);