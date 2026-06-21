#include "raylib.h"
#include "inventory.h"
#include "timer.h"
#include "player.h"
#include "cursor.h"
#include "item.h"

#define BUTTON_COLOR_DEF { 227, 222, 127, 255 }
#define BUTTON_COLOR_SEL { 203, 196, 65, 255 }

void InventoryInit(Inventory& i, Camera2D c) {
	i.MsgTimer = false;
	i.bgpos.x = c.offset.x - 500;
	i.bgpos.y = c.offset.y - 400;
	i.invtick = 0;
	i.UniqueItemsCount = 0;
	i.ScrapMetalCount = 3;
	i.StaminaVialCount = 9;
	i.VitalityVialCount = 14;
	i.bgpos = { 0.0f, 0.0f };
	i.StaminaVialSprite = LoadTexture("resources/itemsprites/staminavialsprite.png");
	i.VitalityVialSprite = LoadTexture("resources/itemsprites/vitalityvialsprite.png");
	i.ScrapMetalSprite = LoadTexture("resources/itemsprites/scrapmetal.png");
	if (i.ScrapMetalCount > 0) 	i.UniqueItemsCount++;
	if (i.StaminaVialCount > 0) 	i.UniqueItemsCount++;
	if (i.VitalityVialCount > 0) 	i.UniqueItemsCount++;
}

void OpenInventory(Inventory& i, Player& p, Camera2D c, Timer& t, Cursor& cu, StaminaVial& sv, VitalityVial& hv) {
	int invx = i.bgpos.x = c.offset.x / 2;
	int invy = i.bgpos.y = c.offset.y - 400;
	if (IsKeyPressed(KEY_T)) {
		p.InventoryIsOpen = true;
	}
	if (IsKeyPressed(KEY_T) && p.InventoryIsOpen == true && t.InvTick >= .35) { // invtick is a buffer so that holding the key doesnt instantly close the inventory
		p.InventoryIsOpen = false;
		t.InvTick = 0.0;
	}
	if (p.InventoryIsOpen) {
		DrawRectangle(invx, invy , 960, 800, PURPLE);
		DrawRectangle(invx, invy , 960, 80, DARKPURPLE);
		ListItems(i);
		DrawUseButtons(p, i, cu, t, sv, hv);
		t.InvTick += .1;
	}
}

void ListItems(Inventory& i) {
	Font deffont = GetFontDefault();

	Vector2 InvSpritePos = { i.bgpos.x - 80 , i.bgpos.y + 20 };
	Vector2 InvTextPos = { i.bgpos.x + 150, i.bgpos.y + 150 };
	Vector2 HeaderPos = { i.bgpos.x + 370 , i.bgpos.y + 20 };

	DrawTextEx(deffont, TextFormat("Inventory"), HeaderPos, 50.0f, 5.0f, WHITE);

	if (i.StaminaVialCount > 0) {
		DrawTextureEx(i.StaminaVialSprite, InvSpritePos, 0.0f, 3.0f, WHITE);
		DrawTextEx(deffont, TextFormat("Stamina Vial: %i", i.StaminaVialCount), InvTextPos, 25.0, 2.0, WHITE);
		InvSpritePos.y += 100;
		InvTextPos.y += 100;
	}
	if (i.VitalityVialCount > 0) {
		DrawTextureEx(i.VitalityVialSprite, InvSpritePos, 0.0f, 3.0f, WHITE);
		DrawTextEx(deffont, TextFormat("Vitality Vial: %i", i.VitalityVialCount), InvTextPos, 25.0, 2.0, WHITE);
		InvSpritePos.y += 100;
		InvTextPos.y += 100;
	}
	if (i.ScrapMetalCount > 0) {
		DrawTextureEx(i.ScrapMetalSprite, InvSpritePos, 0.0f, 3.0f, WHITE);
		DrawTextEx(deffont, TextFormat("Scrap Metal: %i", i.ScrapMetalCount), InvTextPos, 25.0, 2.0, WHITE);
		InvSpritePos.y += 100;
		InvTextPos.y += 100;
	}

	return;
}

void DrawUseButtons(Player& p, Inventory& i, Cursor& m, Timer& t, StaminaVial& sv, VitalityVial& hv ) {
	
	Font deffont = GetFontDefault();
	Vector2 ButtonPos = { i.bgpos.x + 400 , i.bgpos.y + 130 };
	Vector2 TextPos = { ButtonPos.x + 30, ButtonPos.y + 15 };
	if (i.StaminaVialCount > 0) {
		if (m.Pos.x > ButtonPos.x && m.Pos.x < ButtonPos.x + 100 && m.Pos.y > ButtonPos.y && m.Pos.y < ButtonPos.y + 50) {
			DrawRectangle(ButtonPos.x, ButtonPos.y, 100, 50, BUTTON_COLOR_SEL);
			DrawTextEx(GetFontDefault(), TextFormat("USE"), TextPos, 20, 2, BLACK);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) UseStaminaVial(sv, p, i);
			ButtonPos.y += 100;
			TextPos.y += 100;
		}
		else {
			DrawRectangle(ButtonPos.x, ButtonPos.y, 100, 50, BUTTON_COLOR_DEF);
			DrawTextEx(GetFontDefault(), TextFormat("USE"), TextPos, 20, 2, BLACK);
			ButtonPos.y += 100;
			TextPos.y += 100;
		}
	}
	if (i.VitalityVialCount > 0) {
		if (m.Pos.x > ButtonPos.x && m.Pos.x < ButtonPos.x + 100 && m.Pos.y > ButtonPos.y && m.Pos.y < ButtonPos.y + 50) {
			DrawRectangle(ButtonPos.x, ButtonPos.y, 100, 50, BUTTON_COLOR_SEL);
			DrawTextEx(GetFontDefault(), TextFormat("USE"), TextPos, 20, 2, BLACK);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) UseVitalityVial(hv, p, i);
			ButtonPos.y += 100;
			TextPos.y += 100;
		}
		else {
			DrawRectangle(ButtonPos.x, ButtonPos.y, 100, 50, BUTTON_COLOR_DEF);
			DrawTextEx(GetFontDefault(), TextFormat("USE"), TextPos, 20, 2, BLACK);
			ButtonPos.y += 100;
			TextPos.y += 100;
		}
	}
	if (i.ScrapMetalCount > 0) {
		if (m.Pos.x > ButtonPos.x && m.Pos.x < ButtonPos.x + 100 && m.Pos.y > ButtonPos.y && m.Pos.y < ButtonPos.y + 50) {
			DrawRectangle(ButtonPos.x, ButtonPos.y, 100, 50, BUTTON_COLOR_SEL);
			DrawTextEx(GetFontDefault(), TextFormat("USE"), TextPos, 20, 2, BLACK);
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				i.MsgTimer = true;
			}
			ButtonPos.y += 100;
			TextPos.y += 100;
		}
		else {
			DrawRectangle(ButtonPos.x, ButtonPos.y, 100, 50, BUTTON_COLOR_DEF);
			DrawTextEx(GetFontDefault(), TextFormat("USE"), TextPos, 20, 2, BLACK);
			ButtonPos.y += 100;
			TextPos.y += 100;
		}
		if (i.MsgTimer == true && t.InvMsg >= -60) {
			t.InvMsg -= .3;
			t.TextOpacity -= 1;
			DrawText(TextFormat("ITEM MUST BE CRAFTED"), ButtonPos.x + 150, ButtonPos.y - 90 + t.InvMsg, 20, {0, 0, 0, t.TextOpacity});
		}
		else {
			t.InvMsg = 0;
			i.MsgTimer = false;
			t.TextOpacity = 255.0;
		}
	}
}