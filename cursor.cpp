#include "raylib.h"
#include "cursor.h"

void CursorInit(Cursor& m) {
	m.num = 1;
	m.Pos = GetMousePosition();
}

void UpdateCursor(Cursor& m) {
	m.Pos = GetMousePosition();
}