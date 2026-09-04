#pragma once
#include "Component.h"
#include "raylib.h"
#include "Mewtwo.h"
#include "Pkmn.h"

// プレイヤーの状態を表す列挙型
enum PlayerState {
	PLAYER_STATE_FINE,
	PLAYER_STATE_DEAD
};
// プレイヤーの構造体
//struct Player{
//	Vector2 position;	// プレイヤーの位置
//	Vector2 speed;		// プレイヤーの移動速度
//	Vector2 size;		// プレイヤーのサイズ
//	Color color;		// プレイヤーの色
//	int life;			// プレイヤーの残りライフ
//	bool isInvincible;	// プレイヤーが無敵状態かどうか
//	int invincibleFrame; // 無敵時間のタイマー
//	PlayerState playerState;	// プレイヤーの状態
//};
//
//Player CreatePlayer();
//void UpdatePlayer(Player* player);
//void DrawPlayer(Player player);
//
//void CheckPlayerHurt(ProjectileManager* manager, PkmnManager* pkmnManager, Player* player);


//=========================================================================================
//=========================================================================================

class Player : public Component {
public:
	Vector2 speed = { 0.0f, 0.0f };
	int life = 1;
	bool isInvincible = false;
	int invincibleFrame = 0;
	PlayerState playerState = PLAYER_STATE_FINE;
	Color color = RED;

	Player();
	void Reset();
	void Update() override;
	void Draw() override;

	void CheckPlayerHurt(ProjectileManager* manager, PkmnManager* pkmnManager);
};