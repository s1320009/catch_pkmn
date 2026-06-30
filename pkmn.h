#pragma once
#include "raylib.h"

// 1.pkmnの種類を表す列挙型
typedef enum {
	PKMN_PIKACHU,
	PKMN_MEWTWO,		
}PkmnType;

// 2. pkmnの状態を表す列挙型
typedef enum {
	PKMN_STATE_THINK,
	PKMN_STATE_STAY,
	PKMN_STATE_DASH,
	PKMN_STATE_ATTACK
}PkmnState;

typedef struct {
	Vector2 position;	// ポケモンの位置
	float radius;		// ポケモンの半径
	Vector2 speed;		// ポケモンの移動速度
	Color color;		// ポケモンの色

	PkmnType type;		// ポケモンの種類
	PkmnState state;	// ポケモンの状態
	float timer;		// ポケモンの状態遷移用タイマー

	// ---pkmnの種類ごとのパラメータ---
	float dashSpeed;		// ダッシュ速度
	float attackduration;	// 攻撃持続時間
}Pkmn;

//関数宣言
Pkmn CreatePkmn(PkmnType type, Vector2 startPos);
void UpdatePkmn(Pkmn *pkmn);
void DrawPkmn(Pkmn pkmn);