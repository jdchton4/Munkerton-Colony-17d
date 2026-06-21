#pragma once
#include "raylib.h"
#include "world.h"


void RoomInit(Level& l) {
    Texture2D levelsprite = LoadTexture("resources/worldbgs/levelbig.png"); // Starting spawn texture: spawn room

    l.id = 1;
    l.width = 100000;
    l.height = 100000;
    l.texture = levelsprite;
    l.scale = 1.0f;
    l.legalmove = true;
    enum TileType {
        GRASS,
        STONE,
    };
    TileType Tile;
}

void DrawWorld(Level& l) {
    SetTextureFilter(l.texture, TEXTURE_FILTER_ANISOTROPIC_4X);
    Rectangle sourcerect = { 0.0f, 0.0f, float(l.texture.width), float(l.texture.height) };
    Rectangle DestRect = { -50000.0f, -50000.0f, l.height, l.height};
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(l.texture, sourcerect, DestRect, origin, 0.0f, WHITE);
}

void Room1Setup(Level& l, Tile& T) {

    return;
}