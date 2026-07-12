#include "pkmn.h"
#include "raylib.h"
#include <cmath>
#include "MEWTWO.h"

Pkmn CreatePkmn(PkmnBlueprint blueprint, Vector2 startPos) {
	Pkmn pkmn = { 0 };                      // すべてのメンバを0で初期化

	//ステートの初期化
	pkmn.position = startPos;
	pkmn.speed = { 0.0f, 0.0f };
	pkmn.state = PKMN_STATE_THINK;

	pkmn.timer = 0.0f;
	pkmn.frameCounter = 0;                     // フレームカウンター
	pkmn.rotation = 0.0f;					 // 回転角度
	pkmn.isVisible = true;					 // 画面内にいる状態

	//パラメータの初期化
	pkmn.blueprint = blueprint;          // 設計図をコピー

	return pkmn;
}

void UpdatePkmn(Pkmn* pkmn) {
	// 状態遷移の処理
	pkmn->timer += GetFrameTime(); // タイマーを更新

	switch (pkmn->state) {
		case PKMN_STATE_THINK:
			pkmn->speed = { 0.0f, 0.0f }; // 移動速度をゼロにする
			if (pkmn->timer >= pkmn->blueprint.thinkduration) {
				pkmn->timer = 0.0f;		// タイマーをリセット

				// マウスカーソルとの距離を計算
				Vector2 mousePos = GetMousePosition();
				float dx = pkmn->position.x - mousePos.x;
				float dy = pkmn->position.y - mousePos.y;
				float distanceToMouse = sqrtf(dx * dx + dy * dy);

				// ポケモンタイプに応じて確率を変更
				if (distanceToMouse < 200.0f) {
					// =========================================
					// 【マウスが近い場合】
					// =========================================
					int choice = GetRandomValue(1, 100);

					if (pkmn->blueprint.type == PKMN_PIKACHU) {
						pkmn->frameCounter = 0; // PIKACHU のフレームカウンターをリセット
						// PIKACHU: ATTACK 30%, DASH 50%, MOVE 20%
						if (choice <= 30) {
							pkmn->state = PKMN_STATE_ATTACK;
						}
						else if (choice <= 80) {
							pkmn->state = PKMN_STATE_DASH;
						}
						else {
							pkmn->state = PKMN_STATE_MOVE;
						}
					}
					else if (pkmn->blueprint.type == PKMN_MEWTWO) {
						pkmn->frameCounter = 0; // MEWTWO のフレームカウンターをリセット
						// MEWTWO: ATTACK 70%, DASH 10%, MOVE 20%
						if (choice <= 70) {
							pkmn->state = PKMN_STATE_ATTACK;
						}
						else if (choice <= 80) {
							pkmn->state = PKMN_STATE_DASH;
						}
						else {
							pkmn->state = PKMN_STATE_MOVE;
						}
					}
				}
				else {
					// =========================================
					// 【マウスが遠い場合】
					// =========================================
					int choice = GetRandomValue(1, 100);

					if (pkmn->blueprint.type == PKMN_PIKACHU) {
						pkmn->frameCounter = 0; // PIKACHU のフレームカウンターをリセット
						// PIKACHU: ATTACK 20%, DASH 40%, MOVE 40%
						if (choice <= 20) {
							pkmn->state = PKMN_STATE_ATTACK;
						}
						else if (choice <= 60) {
							pkmn->state = PKMN_STATE_DASH;
						}
						else {
							pkmn->state = PKMN_STATE_MOVE;
						}
					}
					else if (pkmn->blueprint.type == PKMN_MEWTWO) {
						pkmn->frameCounter = 0; // MEWTWO のフレームカウンターをリセット
						// MEWTWO: ATTACK 10%, DASH 40%, MOVE 50%
						if (choice <= 10) {
							pkmn->state = PKMN_STATE_ATTACK;
						}
						else if (choice <= 50) {
							pkmn->state = PKMN_STATE_DASH;
						}
						else {
							pkmn->state = PKMN_STATE_MOVE;
						}
					}
				}
			}
			break;

			// --------------------------------------------------
			// ● ATTACK: ポケモンタイプに応じた攻撃処理
			// --------------------------------------------------
		case PKMN_STATE_ATTACK:
			pkmn->speed = { 0.0f, 0.0f }; // 攻撃中はその場で固定

			if (pkmn->blueprint.type == PKMN_MEWTWO) {
				// MEWTWO 専用の ATTACK 処理
				UpdateMewtwoAttack(pkmn);
			}
			else {
				// 通常の ATTACK（PIKACHU等）
				if (pkmn->timer >= pkmn->blueprint.attackduration) {
					pkmn->frameCounter = 0; // フレームカウンターをリセット	
					pkmn->timer = 0.0f;
					pkmn->state = PKMN_STATE_STAY;
				}
			}
			break;

			// --------------------------------------------------
			// ● MOVE: 進行方向に少しだけ移動する
			// --------------------------------------------------
		case PKMN_STATE_MOVE:

			if (pkmn->blueprint.type == PKMN_MEWTWO) {
				UpdateMewtwoMove(pkmn);
			}
			else {
				pkmn->speed.x = -pkmn->blueprint.dashSpeed * 0.5f;
				pkmn->speed.y = 0.0f;
			}

			if (pkmn->timer >= pkmn->blueprint.moveduration) {
				pkmn->frameCounter = 0; // フレームカウンターをリセット
				pkmn->timer = 0.0f;
				pkmn->state = PKMN_STATE_THINK;
			}
			break;

			// --------------------------------------------------
			// ● STAY: エディタで決めた時間だけ止まり、ランダムで次へ
			// --------------------------------------------------
		case PKMN_STATE_STAY:
			pkmn->speed = { 0.0f, 0.0f }; // 待機中も停止

			// エディタ設定の待機時間に達したら
			if (pkmn->timer >= pkmn->blueprint.stayduration) {
				pkmn->frameCounter = 0; // フレームカウンターをリセット
				pkmn->timer = 0.0f;

				// 🎲 確率分岐：1か2のサイコロを振る
				int choice = GetRandomValue(1, 2);
				if (choice == 1) {
					pkmn->state = PKMN_STATE_THINK;
				}
				else {
					pkmn->state = PKMN_STATE_DASH;
				}
			}
			break;

			// --------------------------------------------------
			// ● DASH: 1秒間だけ走って THINK か DASH へ（ご自身のルール）
			// --------------------------------------------------
		case PKMN_STATE_DASH:
			pkmn->frameCounter++; // ➔ 毎フレーム「1」ずつ純粋に増える

			if (pkmn->frameCounter == 1) {
				// 🎲 確率分岐：1から４のサイコロを振る
				int choice = GetRandomValue(1, 4);
				if (choice == 1) {
					// 1/4の確率で上方向にダッシュ
					pkmn->speed.x = 0.0f;
					pkmn->speed.y = -pkmn->blueprint.dashSpeed;
				}
				else if (choice == 2) {
					// 1/4の確率で下方向にダッシュ
					pkmn->speed.x = 0.0f;
					pkmn->speed.y = pkmn->blueprint.dashSpeed;
				}
				else if (choice == 3) {
					// 1/4の確率で右方向にダッシュ
					pkmn->speed.x = pkmn->blueprint.dashSpeed;
					pkmn->speed.y = 0.0f;
				}
				else if (choice == 4) {
					// 1/4の確率で左方向にダッシュ
					pkmn->speed.x = -pkmn->blueprint.dashSpeed;
					pkmn->speed.y = 0.0f;
				}
			}
			

			// 1秒走ったら
			if (pkmn->timer >= 1.0f) {
				pkmn->timer = 0.0f;
				pkmn->frameCounter = 0; // フレームカウンターをリセット

				// 🎲 確率分岐：1か2のサイコロを振る
				int choice = GetRandomValue(1, 2);
				if (choice == 1) {
					pkmn->state = PKMN_STATE_THINK;
				}
				else {
					pkmn->state = PKMN_STATE_DASH; // もう一度ダッシュ！（連続ダッシュ）
				}
			}
			break;
	}

	// ==========================================================
// 1. 【X軸方向】だけの未来予測チェック
// ==========================================================
	Vector2 futurePosX = pkmn->position;
	futurePosX.x += pkmn->speed.x; // Xだけ動かしてみる

	bool canMoveX = true;

	// X方向の画面枠チェック
	if (futurePosX.x - pkmn->blueprint.radius < 0 ||
		futurePosX.x + pkmn->blueprint.radius > GetScreenWidth())
	{
		canMoveX = false;
	}

	// ==========================================================
	// 2. 【Y軸方向】だけの未来予測チェック
	// ==========================================================
	Vector2 futurePosY = pkmn->position;
	futurePosY.y += pkmn->speed.y; // Yだけ動かしてみる

	bool canMoveY = true;

	// Y方向の画面枠チェック
	if (futurePosY.y - pkmn->blueprint.radius < 0 ||
		futurePosY.y + pkmn->blueprint.radius > GetScreenHeight())
		// ...
	{
		canMoveY = false;
	}

	// ==========================================================
	// 3. 安全が確認できた軸【だけ】を実際に動かす！
	// ==========================================================
	if (canMoveX) {
		pkmn->position.x = futurePosX.x;
	}
	else {
		pkmn->speed.x = 0.0f; // ぶつかったらX速度は殺す
	}

	if (canMoveY) {
		pkmn->position.y = futurePosY.y;
	}
	else {
		pkmn->speed.y = 0.0f; // ぶつかったらY速度は殺す
	}
}

// ==========================================================
// 3. 描画関数
// ==========================================================
void DrawPkmn(Pkmn pkmn) {
	//見えなくなる
	if(pkmn.isVisible == false) return;

	// 状態（State）に応じて色や大きさを変えて「固有の行動」を視覚化する
	if (pkmn.state == PKMN_STATE_ATTACK && pkmn.blueprint.type == PKMN_MEWTWO) {
		// MEWTWO ATTACK: 専用描画関数を呼び出し
		DrawMewtwoAttack(pkmn);
	}
	else if (pkmn.state == PKMN_STATE_ATTACK) {
		// 攻撃中は半径1.5倍でオレンジ色に
		DrawCircleV(pkmn.position, pkmn.blueprint.radius * 1.5f, ORANGE);
	}
	else if (pkmn.state == PKMN_STATE_DASH) {
		// ダッシュ中はエディタの色を使いつつ、ちょっと大きく見せるなど
		DrawCircleV(pkmn.position, pkmn.blueprint.radius * 1.2f, pkmn.blueprint.color);
	}
	else {
		// 通常はエディタ設定のまま
		DrawCircleV(pkmn.position, pkmn.blueprint.radius, pkmn.blueprint.color);
	}
}