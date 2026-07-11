#include "MEWTWO.h"
#include "raylib.h"
#include <cmath>

// ==========================================================
// グローバル ProjectileManager
// ==========================================================
static ProjectileManager g_mewtwoProjectileManager = { 0 };

// ==========================================================
// MEWTWO パラメータの定義
// ==========================================================

MewtwoAttackParameters GetMewtwoAttackParameters() {
	MewtwoAttackParameters params;
	params.rotationPerSec = 30.0f;          // 1秒で30度回転
	params.startupFrame = 5;               // 攻撃開始までの前隙（フレーム数）
	params.executeFrame = 35;               // 攻撃中の実行時間（フレーム数）
	params.wholeFrame = 60;                 // 攻撃終了までの後隙（フレーム数）
	params.projectilesPerAttack = 4;        // 上下左右4方向
	params.projectileDistance = 50.0f;      // 中心から 50px 離れた位置から発射
	params.projectileRadius = 8.0f;         // 弾の半径 8px
	params.projectileRotationSpeed = 120.0f; // 弾の回転速度 120度/秒
	params.projectileMaxLifeTime = 3.0f;    // 弾の寿命 3秒
	return params;
}

MewtwoMoveParameters GetMewtwoMoveParameters() {
	MewtwoMoveParameters params;
	params.startupFrame = 15;      // 前隙 15フレーム
	params.executeFrame = 25;	  // 後隙抜き時間 25フレーム
	params.wholeFrame = 35;        // 後隙 35フレーム
	return params;
}

// ==========================================================
// MEWTWO ATTACK 処理
// ==========================================================

void UpdateMewtwoAttack(Pkmn* pkmn) {
	if (pkmn->blueprint.type != PKMN_MEWTWO) return;

	MewtwoAttackParameters params = GetMewtwoAttackParameters();

	pkmn->frameCounter++;

	// 回転速度を適用
	pkmn->rotation += params.rotationPerSec * GetFrameTime();

	// 二回目
	if (pkmn->frameCounter == params.startupFrame) {
		// 上下左右から4発同時発射
		// 方向: 上(-90度)、下(90度)、右(0度)、左(180度)

		float directions[4] = { -90.0f, 90.0f, 0.0f, 180.0f };
		Color colors[4] = { PURPLE, PURPLE, PURPLE, PURPLE };

		for (int i = 0; i < params.projectilesPerAttack; i++) {
			// 初期位置を計算（MEWTWO位置から指定方向に距離分）
			float angleRad = (directions[i] * 3.14159f / 180.0f);
			Vector2 initialPos = {
				pkmn->position.x + params.projectileDistance * cosf(angleRad),
				pkmn->position.y + params.projectileDistance * sinf(angleRad)
			};

			// Projectile を生成して追加
			Projectile proj = CreateProjectile(
				pkmn->position,             // 回転中心（MEWTWO位置）
				initialPos,                 // 初期位置
				params.projectileDistance,  // 距離
				directions[i],              // 初期方向
				params.projectileRotationSpeed,  // 回転速度
				params.projectileMaxLifeTime,    // 寿命
				params.projectileRadius,    // 半径
				colors[i]                   // 色
			);

			AddProjectile(&g_mewtwoProjectileManager, proj);
		}
	}

	// 二回目
	if (pkmn->frameCounter == params.executeFrame) {
		// 上下左右から4発同時発射（同じパターン）
		float directions[4] = { -90.0f, 90.0f, 0.0f, 180.0f };
		Color colors[4] = { PURPLE, PURPLE, PURPLE, PURPLE };

		for (int i = 0; i < params.projectilesPerAttack; i++) {
			float angleRad = (directions[i] * 3.14159f / 180.0f);
			Vector2 initialPos = {
				pkmn->position.x + params.projectileDistance * cosf(angleRad),
				pkmn->position.y + params.projectileDistance * sinf(angleRad)
			};

			Projectile proj = CreateProjectile(
				pkmn->position,
				initialPos,
				params.projectileDistance,
				directions[i],
				params.projectileRotationSpeed,
				params.projectileMaxLifeTime,
				params.projectileRadius,
				colors[i]
			);

			AddProjectile(&g_mewtwoProjectileManager, proj);
		}
	}

	// ATTACK終了判定
	if (pkmn->frameCounter == params.wholeFrame) {
		pkmn->frameCounter = 0;
		pkmn->timer = 0.0f;
		pkmn->rotation = 0.0f;
		pkmn->state = PKMN_STATE_STAY;
	}
}

// ==========================================================
// MEWTWO MOVE 処理
// ==========================================================

void UpdateMewtwoMove(Pkmn* pkmn) {
	//pkmn->blueprint.type != PKMN_MEWTWO の場合は無視
	if (pkmn->blueprint.type != PKMN_MEWTWO) return;

	// MEWTWO MOVE のパラメータを取得
	MewtwoMoveParameters params = GetMewtwoMoveParameters();

	pkmn->speed = { 0.0f, 0.0f };
	pkmn->frameCounter++; // ➔ 毎フレーム「1」ずつ純粋に増える

	//前隙15f
	if (pkmn->frameCounter <= params.startupFrame) {
		pkmn->isVisible = true;
	}

	//10f消える　次の場所を探す
	else if (pkmn->frameCounter <= params.executeFrame) {
		pkmn->isVisible = false;    //描画と当たり判定を消す

		if (pkmn->frameCounter == params.startupFrame + 1) {
			Vector2 nextPos;
			bool isOverlapping = true;

			while (isOverlapping) {
				//ランダムで次の座標を決める
				nextPos.x = GetRandomValue(pkmn->blueprint.radius, GetScreenWidth() - pkmn->blueprint.radius);
				nextPos.y = GetRandomValue(pkmn->blueprint.radius, GetScreenHeight() - pkmn->blueprint.radius);
				//重なり判定
				if (CheckCollisionCircles(nextPos, pkmn->blueprint.radius, pkmn->position, pkmn->blueprint.radius)) {
					isOverlapping = true;
				}
				else {
					isOverlapping = false;
				}
			}

			pkmn->position = nextPos;
		}
	}
	//後隙10f
	else if (pkmn->frameCounter <= params.wholeFrame) {
		pkmn->isVisible = true;   //描画と当たり判定を復活
		pkmn->speed = { 0.0f, 0.0f };
	}
	else {
		pkmn->frameCounter = 0;
		int choice = GetRandomValue(1, 2);
		if (choice == 1) {
			pkmn->state = PKMN_STATE_ATTACK;
		}
		else {
			pkmn->state = PKMN_STATE_STAY;
		}
	}
}

// ==========================================================
// MEWTWO 描画（ATTACK時の回転表示）
// ==========================================================

void DrawMewtwoAttack(Pkmn pkmn) {
	if (pkmn.blueprint.type != PKMN_MEWTWO) return;
	if (pkmn.state != PKMN_STATE_ATTACK) return;

	// MEWTWO ATTACK: 回転を視覚化
	DrawCircleV(pkmn.position, pkmn.blueprint.radius * 1.5f, ORANGE);

	// 回転角度を示すラインを描画
	float angleRad = (pkmn.rotation * 3.14159f / 180.0f);
	Vector2 lineEnd = {
		pkmn.position.x + pkmn.blueprint.radius * 1.5f * cosf(angleRad),
		pkmn.position.y + pkmn.blueprint.radius * 1.5f * sinf(angleRad)
	};
	DrawLineV(pkmn.position, lineEnd, YELLOW);

	// デバッグ用：現在のフェーズと回転角度を表示
	DrawText(TextFormat("frame: %d, Rotation: %.1f", pkmn.frameCounter, pkmn.rotation), 
			 (int)pkmn.position.x - 50, (int)pkmn.position.y - 80, 12, WHITE);
}

// ==========================================================
// Projectile（プロジェクタイル）関連の実装
// ==========================================================

// ==========================================================
// 【CreateProjectile】弾を生成する
// ==========================================================
// 
// 【重要】この関数は「弾の状態を初期化する」だけです。
// 実際の position は初期値に設定されますが、UpdateProjectile で毎フレーム
// 再計算されるため、初期値は実質参照されません。
//
// 【パラメータの意味】
//   centerPos: MEWTWO の座標（軌道の中心）
//   initialPos: 弾が生成される位置（この時点でのみ使われる）
//   distance: 中心からの距離（px）。毎フレーム distanceGrowthRate ずつ増える
//   direction: 弾の方向（度）。-90=上, 0=右, 90=下, 180=左
//   rotationSpeed: 毎秒何度回転するか（angularVelocity に設定される）
//   maxLifeTime: 弾が消えるまでの秒数
//   radius: 弾の大きさ（px）
//   color: 弾の色
//

Projectile CreateProjectile(Vector2 centerPos, Vector2 initialPos, float distance, float direction, float rotationSpeed, float maxLifeTime, float radius, Color color) {
	Projectile proj = { 0 };

	// ===== 軌道情報の設定 =====
	proj.centerPos = centerPos;                    // MEWTWO の座標が軌道の中心
	proj.distance = distance;                      // 初期距離（50px など）
	proj.angle = direction;                        // 初期角度（-90度など）
	proj.angularVelocity = rotationSpeed;          // MEWTWO の回転速度を引き継ぐ
	proj.distanceGrowthRate = 100.0f;              // 毎秒100px遠ざかる（固定値）

	// ===== 位置と寿命の初期化 =====
	proj.position = initialPos;                    // 初期位置（すぐに再計算される）
	proj.lifeTime = 0.0f;                          // 生成直後は経過時間0
	proj.maxLifeTime = maxLifeTime;                // 寿命を記録（3秒など）

	// ===== 描画情報 =====
	proj.radius = radius;                          // 弾のサイズ（8px など）
	proj.color = color;                            // 弾の色（PURPLE など）

	// ===== 状態 =====
	proj.isActive = true;                          // 生成直後は生きている状態

	return proj;
}

// ==========================================================
// 【UpdateProjectile】毎フレーム弾を更新する
// ==========================================================
// 
// 【カーブ軌道の核】ここで角度と距離を両方増やすことで、
// 回転しながら遠ざかる螺旋軌道が実現される。
//
// 【流れ】
// 1. 経過時間を加算
// 2. 寿命切れチェック → 死んだ弾は isActive = false で除外
// 3. 角度を増加させる → MEWTWO を中心に回転する効果
// 4. 距離を増加させる → 中心から遠ざかる効果
// 5. 新しい angle と distance から position を計算（極座標 → 直交座標）
//

void UpdateProjectile(Projectile* proj) {
	if (!proj->isActive) return;  // 既に死んでいる弾は処理しない

	// ===== 1. 経過時間を加算 =====
	float dt = GetFrameTime();  // フレーム時間（通常 1/60 ≈ 0.0167秒）
	proj->lifeTime += dt;

	// ===== 2. 寿命チェック =====
	if (proj->lifeTime >= proj->maxLifeTime) {
		proj->isActive = false;  // 寿命切れ → 消す
		return;
	}

	// ===== 3. 角度を更新（弾が回転する） =====
	// 例：angularVelocity=120度/秒、dt=0.0167秒
	//     → 120 * 0.0167 ≈ 2度ずつ増える
	proj->angle += proj->angularVelocity * dt;

	// ===== 4. 距離を更新（弾が遠ざかる） =====
	// 例：distanceGrowthRate=100px/秒、dt=0.0167秒
	//     → 100 * 0.0167 ≈ 1.67px ずつ増える
	proj->distance += proj->distanceGrowthRate * dt;

	// ===== 5. 極座標 → 直交座標への変換 =====
	// 公式：
	// x = centerX + distance * cos(angle)
	// y = centerY + distance * sin(angle)
	//
	// 例：centerPos=(400, 300), angle=-90度, distance=50px
	//     cos(-90°) = 0, sin(-90°) = -1
	//     → x = 400 + 50*0 = 400
	//     → y = 300 + 50*(-1) = 250
	//     → 結果：MEWTWO の上、50px の位置
	//
	float angleRad = (proj->angle * 3.14159f / 180.0f);  // 度をラジアンに変換
	proj->position.x = proj->centerPos.x + proj->distance * cosf(angleRad);
	proj->position.y = proj->centerPos.y + proj->distance * sinf(angleRad);
}

void DrawProjectile(Projectile proj) {
	if (!proj.isActive) return;
	DrawCircleV(proj.position, proj.radius, proj.color);
}

// ==========================================================
// ProjectileManager（プロジェクタイルマネージャー）の実装
// ==========================================================

ProjectileManager CreateProjectileManager() {
	ProjectileManager manager = { 0 };
	manager.count = 0;
	return manager;
}

void AddProjectile(ProjectileManager* manager, Projectile proj) {
	if (manager->count < MAX_PROJECTILES) {
		manager->projectiles[manager->count] = proj;
		manager->count++;
	}
}

void UpdateProjectileManager(ProjectileManager* manager) {
	for (int i = 0; i < manager->count; i++) {
		UpdateProjectile(&manager->projectiles[i]);
	}

	// 非アクティブな弾を削除（最適化）
	int activeCount = 0;
	for (int i = 0; i < manager->count; i++) {
		if (manager->projectiles[i].isActive) {
			manager->projectiles[activeCount] = manager->projectiles[i];
			activeCount++;
		}
	}
	manager->count = activeCount;
}

void DrawProjectileManager(ProjectileManager manager) {
	for (int i = 0; i < manager.count; i++) {
		DrawProjectile(manager.projectiles[i]);
	}
}

void ClearProjectileManager(ProjectileManager* manager) {
	manager->count = 0;
}

// ==========================================================
// ProjectileManager アクセス関数
// ==========================================================

ProjectileManager* GetMewtwoProjectileManager() {
	return &g_mewtwoProjectileManager;
}
