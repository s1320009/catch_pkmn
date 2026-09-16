#include "player.h"
#include "GameObject.h"
#include "Common.h"
#include "TextureAnimeComponent.h"

Player::Player() {
	m_speed = { 0.0f, 0.0f };
	m_life = 1;
	m_isInvincible = false;
	m_invincibleFrame = 0; // 無敵時間を3秒に設定
	m_STATE = STATE::FINE;
	m_color = RED;
}

void Player::Reset() {
	if (gameObject == nullptr) return;
	gameObject->position = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
	gameObject->scale = { 64.0f, 64.0f };
	m_speed = { 0.0f, 0.0f };
	m_life = 1;
	m_isInvincible = false;
	m_invincibleFrame = 0; // 無敵時間を3秒に設定
	m_STATE = STATE::FINE;
	m_color = RED;

	m_ball.Reset();
}

void Player::Update() {
	switch (m_STATE) {
		case STATE::FINE: {
			// 💡 関数が終わってもクリックした位置を記憶し続ける変数
			static Vector2 clickStartPos = { 0.0f, 0.0f };

			float moveMultiplier = 0.2f; // ➔ 飛んでいく「初速」の倍率（好みに合わせて調整）
			float friction = 0.92f;      // ➔ 減速の割合（0.90〜0.98 の間で調整。小さいほどすぐ止まる）

			// ⏳ 【毎フレーム実行】前フレームの速度を少しずつ減速させる（摩擦）
			this->m_speed.x *= friction;
			this->m_speed.y *= friction;

			// 🖱️ ① 左クリックが「押された瞬間」の位置を記録
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				clickStartPos = GetMousePosition();
			}

			// 🖱️ ② 左クリックが「離された瞬間」に、引っ張った距離に応じた「初速」をドカンと与える！
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
				Vector2 clickEndPos = GetMousePosition();

				// 💡 引っ張った方向とは「逆」に飛ばしたい場合（弓矢やゴムのように）は、引き算を逆にしてください
				// ここでは「クリックして動かした方向」にそのまま飛ぶ計算にしています
				m_speed.x = (clickEndPos.x - clickStartPos.x) * moveMultiplier;
				m_speed.y = (clickEndPos.y - clickStartPos.y) * moveMultiplier;
			}

			// 💡 半分のサイズ（中心からの距離）を計算
			float halfWidth = this->gameObject->scale.x / 2.0f;
			float halfHeight = this->gameObject->scale.y / 2.0f;

			// 次フレームの位置を予測
			Vector2 futurePos = {
				gameObject->position.x + m_speed.x,
				gameObject->position.y + m_speed.y
			};

			float minX = halfWidth;
			float maxX = Common::SCREEN_WIDTH - halfWidth;
			float minY = halfHeight;
			float maxY = Common::SCREEN_HEIGHT - halfHeight;

			gameObject->position.x = Common::clamp(futurePos.x, minX, maxX);
			gameObject->position.y = Common::clamp(futurePos.y, minY, maxY);

			m_ball.Update(gameObject->position);

			// プレイヤーのライフが0以下になった場合、状態を死んでいる状態に変更
			if (m_life <= 0) {
				m_STATE = STATE::DEAD;
			}
			break;
		}

		case STATE::DEAD: {
			break;
		}
	}
}

void Player::Draw() {
	if (gameObject == nullptr) return;
	switch (m_STATE) {
		case STATE::FINE: {
			if (!m_isInvincible){
				//DrawRectangle(gameObject->position.x - gameObject->scale.x / 2, gameObject->position.y - gameObject->scale.y / 2, gameObject->scale.x, gameObject->scale.y, m_color);
				auto* pAnime = gameObject->GetComponent<TextureAnimeComponent>();
				//アニメーションの切り替え
				const BALL_STATE ballState = m_ball.GetState();
				switch (ballState) {
				case BALL_STATE::WAIT_X:
					if (pAnime->animeTexture != &pIdleAnime) {
						pAnime->SetAnimeTexture(pIdleAnime);
					}
					break;
				case BALL_STATE::FLYING:
					if (pAnime->animeTexture != &pThrowAnime) {
						pAnime->SetAnimeTexture(pThrowAnime);
					}
					break;
				}
			}
			break;
		}
		case STATE::DEAD: {
			break;
		}
	}
}

void Player::CheckPlayerHurt(ProjectileManager* manager, PkmnManager* pkmnManager) {
	//無敵時間を減らす
	if (this->m_invincibleFrame > 0) {
		this->m_invincibleFrame--;
		//当たった時にちらつかせる
		if (this->m_invincibleFrame % 10 < 5) {
			this->m_isInvincible = true;
		}
		else {
			this->m_isInvincible = false;
		}
	}
	else {
		// 無敵時間が終了したらみえない状態を解除
		this->m_isInvincible = false;

		// プレイヤーが無敵状態でない場合、弾との衝突判定を行う
		for (int i = 0; i < manager->count; i++) {
			Projectile* proj = &manager->projectiles[i];

			if(not proj->isActive) continue;

			// プレイヤーと弾の衝突判定
			if (CheckCollisionCircleRec(proj->position, proj->radius, { this->gameObject->position.x - this->gameObject->scale.x / 2, this->gameObject->position.y - this->gameObject->scale.y / 2, this->gameObject->scale.x, this->gameObject->scale.y })) {
				this->m_life--;
				this->m_invincibleFrame = 60;
				proj->isActive = false;
				break; // 一度のフレームで複数の弾に当たらないようにする
			}
		}

		// 💥 2. ポケモン（Pkmn）との衝突判定（マネージャーをループ！）
		for (int i = 0; i < pkmnManager->count; i++) {
			Pkmn* enemy = &pkmnManager->list[i];

			if(not enemy->isActive) continue;
			if (not enemy->isVisible) continue;

			// 弾と同じように、円（敵）と四角（プレイヤー）の判定を行う！
			if (CheckCollisionCircleRec(enemy->position, enemy->blueprint.radius, { gameObject->position.x - gameObject->scale.x / 2, gameObject->position.y - gameObject->scale.y / 2, gameObject->scale.x, gameObject->scale.y })) {
				this->m_life--;
				this->m_invincibleFrame = 60; // 1秒無敵
				return;
			}
		}
	}
}                

bool Player::IsDead() const {
	return m_STATE == STATE::DEAD;
}

bool Player::IsInvincible() const {
	return m_isInvincible;
}

Vector2 Player::GetPosition() const {
	if (gameObject == nullptr) return { 0.0f, 0.0f };
	return gameObject->position;
}

Ball& Player::GetBall() {
	return m_ball;
}

const Ball& Player::GetBall() const {
	return m_ball;
}