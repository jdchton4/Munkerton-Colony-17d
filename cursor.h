// @author jdeepak
#pragma once
#include "raylib.h"

struct Cursor {
	int num; // dummy variable
	Vector2 Pos;
	bool MouseOver;
};

void CursorInit(Cursor& m);
void UpdateCursor(Cursor& m);
