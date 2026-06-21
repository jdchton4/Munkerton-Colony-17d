// @author jdeepak
// @file inventory.h
#pragma once

#include "raylib.h"

struct Player;
struct Camera2D;
struct Timer;
struct Cursor;
struct StaminaVial;
struct VitalityVial;

struct Inventory {
	bool MsgTimer; // returns based on whether the inventory message is active or not
	int VitalityVialCount;
	int StaminaVialCount;
	int ScrapMetalCount;
	int UniqueItemsCount;
	double invtick;
	Vector2 bgpos;
	Texture2D VitalityVialSprite;
	Texture2D StaminaVialSprite;
	Texture2D ScrapMetalSprite;
};

void OpenInventory(Inventory& i, Player& p, Camera2D c, Timer& t, Cursor& m, StaminaVial& sv, VitalityVial& hv);
void InventoryInit(Inventory& i, Camera2D c);
void ListItems(Inventory& i);
void DrawUseButtons(Player& p, Inventory& i, Cursor& c, Timer& t, StaminaVial& sv, VitalityVial& hv);
