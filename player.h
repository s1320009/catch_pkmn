#pragma once
#include "raylib.h"
#include "MEWTWO.h"

// プレイヤーの状態を表す列挙型
typedef enum {
	PLAYER_STATE_FINE,
	PLAYER_STATE_DEAD
} PlayerState;

// プレイヤーの構造体
typedef struct {
	Vector2 position;	// プレイヤーの位置
	Vector2 speed;		// プレイヤーの移動速度
	Vector2 size;		// プレイヤーのサイズ
	Color color;		// プレイヤーの色
	int life;			// プレイヤーの残りライフ
	float invincibleTimer; // 無敵時間のタイマー
	PlayerState state;	// プレイヤーの状態
} Player;

Player CreatePlayer();
void UpdatePlayer(Player* player);
void DrawPlayer(Player player);

void CheckPlayerHurt(ProjectileManager* manager, Player* player);