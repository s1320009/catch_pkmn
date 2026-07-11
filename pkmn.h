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
	PKMN_STATE_MOVE,
	PKMN_STATE_ATTACK,
	PKMN_STATE_STAY,
	PKMN_STATE_DASH
}PkmnState;

// ==========================================================
// ★変更点①: 【エディタでいじる設計図】を構造体として切り離す
// ==========================================================
typedef struct {
	PkmnType type;          // ポケモンの種類
	float radius;           // ポケモンの半径
	Color color;            // ポケモンの色
	float dashSpeed;        // ダッシュ速度
	float attackduration;   // 攻撃持続時間
	float thinkduration;    // 思考時間
	float stayduration;     // 待機時間
	float moveduration;     // 異能時間
} PkmnBlueprint;			// (設定)ゲームループが回っている間、ずっと同じ値であるもの(Parameter / Blueprint)

// ==========================================================
// ★変更点②: 【ゲーム中にリアルタイム変化する状態】の構造体
// ==========================================================
typedef struct {
	Vector2 position;	// ポケモンの位置
	Vector2 speed;		// ポケモンの"現在の"移動速度
	PkmnState state;	// ポケモンの状態
	int frameCounter;   // ポケモンの技をフレームで管理
	float timer;		// ポケモンの状態遷移用タイマー
	float rotation;		// ポケモンの回転角度（度）- MEWTWO ATTACK用
	bool isVisible;		// ポケモンが画面内にいるかどうか（画面外に出たら false）

	PkmnBlueprint blueprint;	// エディタで設定した設計図を丸ごと内包する
}Pkmn;					// (状態)ゲーム中にリアルタイムで変化するもの(State / Instance)

// ==========================================================
// ★変更点③: 生成関数に「設計図」を渡せるようにする
// ==========================================================

// 変更前: Pkmn CreatePkmn(PkmnType type, Vector2 startPos);
// 変更後:
Pkmn CreatePkmn(PkmnBlueprint blueprint, Vector2 startPos);

void UpdatePkmn(Pkmn* pkmn);
void DrawPkmn(Pkmn pkmn);