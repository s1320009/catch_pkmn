#include "raylib.h"
#include "raymath.h"
#include "player.h"

Player CreatePlayer() {
	Player player;
	player.position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };	//float型にするために2.0fとする
	player.speed = { 0.0f, 0.0f };
	player.size = { 50.0f, 50.0f };
	player.color = RED;
	player.life = 3;
	player.invincibleTimer = 3.0f; // 無敵時間を3秒に設定
	player.state = PLAYER_STATE_FINE;
	
	return player;
}

void UpdatePlayer(Player* player) {
	switch (player->state) {
		case PLAYER_STATE_FINE: {
			// プレイヤーの移動処理
			//マウスを左クリックしながらスライドした分だけ移動する

			// 💡 【重要】関数が終わってもクリックした位置を記憶し続ける安全な変数
			static Vector2 clickStartPos = { 0.0f, 0.0f };

			Vector2 mouseDelta = { 0.0f, 0.0f };
			float moveMultiplier = 3.0f; // ➔ 【ここがポイント！】何倍にするかの数値

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				clickStartPos = GetMousePosition();
			}

			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
				Vector2 clickEndPos = GetMousePosition();

				mouseDelta.x = (clickEndPos.x - clickStartPos.x) * moveMultiplier;
				mouseDelta.y = (clickEndPos.y - clickStartPos.y) * moveMultiplier;
			}

			// プレイヤーの速度をリセット
			player->speed = { 0.0f, 0.0f };

			// 💡 半分のサイズ（中心からの距離）を計算
			float halfWidth = player->size.x / 2.0f;
			float halfHeight = player->size.y / 2.0f;

			// X方向の画面外チェック（中心座標を基準に、左右の端がはみ出さないか）
			if (player->position.x + halfWidth + mouseDelta.x < GetScreenWidth() &&
				player->position.x - halfWidth + mouseDelta.x > 0) {
				player->speed.x = mouseDelta.x;
			}

			// Y方向の画面外チェック（中心座標を基準に、上下の端がはみ出さないか）
			if (player->position.y + halfHeight + mouseDelta.y < GetScreenHeight() &&
				player->position.y - halfHeight + mouseDelta.y > 0) {
				player->speed.y = mouseDelta.y;
			}

			player->position.x += player->speed.x;
			player->position.y += player->speed.y;

			// プレイヤーのライフが0以下になった場合、状態を死んでいる状態に変更
			if (player->life <= 0) {
				player->state = PLAYER_STATE_DEAD;
			}
			break;
		}

		case PLAYER_STATE_DEAD:
			// プレイヤーが死んでいる場合の処理（必要に応じて追加）
			break;
	}
}

void CheckPlayerHurt(ProjectileManager* manager, Player* player) {
	//無敵時間を減らす
	if (player->invincibleTimer > 0.0f) {
		player->invincibleTimer -= GetFrameTime();
	}
	else {
		// プレイヤーが無敵状態でない場合、弾との衝突判定を行う
		for (int i = 0; i < manager->count; i++) {
			Projectile* proj = &manager->projectiles[i];
			if (proj->isActive) {
				// プレイヤーと弾の衝突判定
				if (CheckCollisionCircleRec(proj->position, proj->radius, { player->position.x - player->size.x / 2, player->position.y - player->size.y / 2, player->size.x, player->size.y })) {
					// 衝突した場合、プレイヤーのライフを減らす
					player->life--;
					// 無敵時間をリセット
					player->invincibleTimer = 3.0f;
					// 弾を非アクティブにする
					proj->isActive = false;
					break; // 一度のフレームで複数の弾に当たらないようにする
				}
			}
		}
	}
}

void DrawPlayer(Player player) {
	// プレイヤーの状態に応じて描画する
	switch (player.state) {
	case PLAYER_STATE_FINE:
		DrawRectangleV({ player.position.x - player.size.x / 2, player.position.y - player.size.y / 2 }, player.size, player.color);
		break;
	case PLAYER_STATE_DEAD:
		// プレイヤーが死んでいる場合の描画（必要に応じて追加）
		break;
	}
}