#pragma once

#define SCREEN_W	800
#define SCREEN_H	450

#define COLOR_WHITE		GetColor( 255, 255, 255 )
#define COLOR_BLACK		GetColor(   0,   0,   0 )
#define COLOR_YELLOW	GetColor( 255, 255,   0 )
#define COLOR_AQUA		GetColor(   0, 255, 255 )

#define COLOR_PLAYER	GetColor( 128, 128, 255 )
#define COLOR_NPC		GetColor( 255, 128, 128 )

void GameInit();	//	‰Šú‰»ˆ—
void GameUpdate();	//	XVˆ—
void GameRender();	//	•`‰æˆ—
void GameExit();	//	I—¹ˆ—
