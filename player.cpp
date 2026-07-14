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
	player.isInvincible = false;
	player.invincibleFrame = 0; // 無敵時間を3秒に設定
	player.state = PLAYER_STATE_FINE;
	
	return player;
}

void UpdatePlayer(Player* player) {
	switch (player->state) {
		case PLAYER_STATE_FINE: {

			// 💡 関数が終わってもクリックした位置を記憶し続ける変数
			static Vector2 clickStartPos = { 0.0f, 0.0f };

			float moveMultiplier = 0.5f; // ➔ 飛んでいく「初速」の倍率（好みに合わせて調整）
			float friction = 0.92f;      // ➔ 減速の割合（0.90〜0.98 の間で調整。小さいほどすぐ止まる）

			// ⏳ 【毎フレーム実行】前フレームの速度を少しずつ減速させる（摩擦）
			player->speed.x *= friction;
			player->speed.y *= friction;

			// 🖱️ ① 左クリックが「押された瞬間」の位置を記録
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				clickStartPos = GetMousePosition();
			}

			// 🖱️ ② 左クリックが「離された瞬間」に、引っ張った距離に応じた「初速」をドカンと与える！
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				Vector2 clickEndPos = GetMousePosition();

				// 💡 引っ張った方向とは「逆」に飛ばしたい場合（弓矢やゴムのように）は、引き算を逆にしてください
				// ここでは「クリックして動かした方向」にそのまま飛ぶ計算にしています
				player->speed.x = (clickEndPos.x - clickStartPos.x) * moveMultiplier;
				player->speed.y = (clickEndPos.y - clickStartPos.y) * moveMultiplier;
			}

			// 💡 半分のサイズ（中心からの距離）を計算
			float halfWidth = player->size.x / 2.0f;
			float halfHeight = player->size.y / 2.0f;

			// 🛡️ 未来の座標を計算して画面外チェックを行う
			Vector2 futurePos = player->position;
			futurePos.x += player->speed.x;
			futurePos.y += player->speed.y;

			// X方向のチェック：はみ出る場合は速度をゼロにして壁にピタッと止める（または跳ね返らせる）
			if (futurePos.x + halfWidth < GetScreenWidth() && futurePos.x - halfWidth > 0) {
				player->position.x = futurePos.x;
			}
			else {
				player->speed.x = 0.0f; // 壁にぶつかったら横方向の勢いを止める
			}

			// Y方向のチェック
			if (futurePos.y + halfHeight < GetScreenHeight() && futurePos.y - halfHeight > 0) {
				player->position.y = futurePos.y;
			}
			else {
				player->speed.y = 0.0f; // 壁にぶつかったら縦方向の勢いを止める
			}

			// プレイヤーのライフが0以下になった場合、状態を死んでいる状態に変更
			if (player->life <= 0) {
				player->state = PLAYER_STATE_DEAD;
			}
			break;
		}

		case PLAYER_STATE_DEAD: {
			break;
		}
	}
}

void DrawPlayer(Player player) {
	// プレイヤーの状態に応じて描画する
	switch (player.state) {
	case PLAYER_STATE_FINE:
		if (!player.isInvincible) {
			DrawRectangleV({ player.position.x - player.size.x / 2, player.position.y - player.size.y / 2 }, player.size, player.color);
		}
		break;
	case PLAYER_STATE_DEAD:
		// プレイヤーが死んでいる場合の描画（必要に応じて追加）
		break;
	}
}

void CheckPlayerHurt(ProjectileManager* manager, PkmnManager* pkmnManager, Player* player) {
	//無敵時間を減らす
	if (player->invincibleFrame > 0) {
		player->invincibleFrame--;
		//当たった時にちらつかせる
		if (player->invincibleFrame % 10 < 5) {
			player->isInvincible = true;
		}
		else {
			player->isInvincible = false;
		}
	}
	else {
		// 無敵時間が終了したらみえない状態を解除
		player->isInvincible = false;

		// プレイヤーが無敵状態でない場合、弾との衝突判定を行う
		for (int i = 0; i < manager->count; i++) {
			Projectile* proj = &manager->projectiles[i];
			if (proj->isActive) {
				// プレイヤーと弾の衝突判定
				if (CheckCollisionCircleRec(proj->position, proj->radius, { player->position.x - player->size.x / 2, player->position.y - player->size.y / 2, player->size.x, player->size.y })) {
					// 衝突した場合、プレイヤーのライフを減らす
					player->life--;
					// 無敵時間をリセット
					player->invincibleFrame = 60;
					// 弾を非アクティブにする
					proj->isActive = false;
					break; // 一度のフレームで複数の弾に当たらないようにする
				}
			}
		}

		// 💥 2. ポケモン（Pkmn）との衝突判定（マネージャーをループ！）
		for (int i = 0; i < pkmnManager->count; i++) {
			Pkmn* enemy = &pkmnManager->list[i];

			// 生きていて、画面内にいる敵だけチェックする
			if (enemy->isActive && enemy->isVisible) {

				// 弾と同じように、円（敵）と四角（プレイヤー）の判定を行う！
				if (CheckCollisionCircleRec(enemy->position, enemy->blueprint.radius, { player->position.x - player->size.x / 2, player->position.y - player->size.y / 2, player->size.x, player->size.y })) {
					player->life--;
					player->invincibleFrame = 60; // 1秒無敵

					// ※お好みで、体当たりした後に敵をちょっとだけ弾き飛ばす処理などをここに追加できます
					return;
				}
			}
		}
	}
}
