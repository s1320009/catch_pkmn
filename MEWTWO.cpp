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
	params.attackPhaseTime = 0.5f;          // 各フェーズ 0.5秒
	params.projectilesPerAttack = 4;        // 上下左右4方向
	params.projectileDistance = 50.0f;      // 中心から 50px 離れた位置から発射
	params.projectileRadius = 8.0f;         // 弾の半径 8px
	params.projectileRotationSpeed = 120.0f; // 弾の回転速度 120度/秒
	params.projectileMaxLifeTime = 3.0f;    // 弾の寿命 3秒
	return params;
}

// ==========================================================
// MEWTWO ATTACK 処理
// ==========================================================

void UpdateMewtwoAttack(Pkmn* pkmn) {
	if (pkmn->blueprint.type != PKMN_MEWTWO) return;

	MewtwoAttackParameters params = GetMewtwoAttackParameters();

	// 回転速度を適用
	pkmn->rotation += params.rotationPerSec * GetFrameTime();

	// =========================================
	// フェーズ管理：1秒間で 2回発射（各フェーズ 0.5秒）
	// =========================================

	// フェーズ 0（0～0.5秒）: 初回発射タイミング
	if (pkmn->timer >= 0.0f && pkmn->timer < params.attackPhaseTime && pkmn->attackPhase == 0) {
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

		pkmn->attackPhase = 1; // 次のフェーズへ
	}

	// フェーズ 1（0.5秒～1.0秒）: 2回目発射タイミング
	if (pkmn->timer >= params.attackPhaseTime && pkmn->timer < params.attackPhaseTime * 2.0f && pkmn->attackPhase == 1) {
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

		pkmn->attackPhase = 2; // 攻撃完了フェーズ
	}

	// ATTACK終了判定
	if (pkmn->timer >= pkmn->blueprint.attackduration) {
		pkmn->timer = 0.0f;
		pkmn->rotation = 0.0f;
		pkmn->attackPhase = 0;
		pkmn->state = PKMN_STATE_STAY;
	}
}

// ==========================================================
// MEWTWO MOVE 処理
// ==========================================================

void UpdateMewtwoMove(Pkmn* pkmn) {
	if (pkmn->blueprint.type != PKMN_MEWTWO) return;

	// MEWTWO は通常移動より少しだけ速く進む
	pkmn->speed.x = -pkmn->blueprint.dashSpeed * 0.75f;
	pkmn->speed.y = 0.0f;
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
	DrawText(TextFormat("Phase: %d, Rotation: %.1f", pkmn.attackPhase, pkmn.rotation), 
			 (int)pkmn.position.x - 50, (int)pkmn.position.y - 80, 12, WHITE);
}

// ==========================================================
// Projectile（プロジェクタイル）関連の実装
// ==========================================================

Projectile CreateProjectile(Vector2 centerPos, Vector2 initialPos, float distance, float direction, float rotationSpeed, float maxLifeTime, float radius, Color color) {
	Projectile proj = { 0 };
	proj.initialPosition = centerPos;
	proj.initialDistance = distance;
	proj.initialDirection = direction;
	proj.currentRotation = 0.0f;
	proj.rotationSpeed = rotationSpeed;
	proj.lifeTime = 0.0f;
	proj.maxLifeTime = maxLifeTime;
	proj.radius = radius;
	proj.color = color;
	proj.isActive = true;

	// 初期位置を計算
	float angleRad = (direction * 3.14159f / 180.0f);
	proj.position.x = centerPos.x + distance * cosf(angleRad);
	proj.position.y = centerPos.y + distance * sinf(angleRad);

	return proj;
}

void UpdateProjectile(Projectile* proj) {
	if (!proj->isActive) return;

	proj->lifeTime += GetFrameTime();

	// 寿命切れ
	if (proj->lifeTime >= proj->maxLifeTime) {
		proj->isActive = false;
		return;
	}

	// 回転角度を更新
	proj->currentRotation += proj->rotationSpeed * GetFrameTime();

	// 新しい角度を計算
	float newAngle = proj->initialDirection + proj->currentRotation;
	float angleRad = (newAngle * 3.14159f / 180.0f);

	// 中心位置からの相対位置を計算（円弧軌道）
	proj->position.x = proj->initialPosition.x + proj->initialDistance * cosf(angleRad);
	proj->position.y = proj->initialPosition.y + proj->initialDistance * sinf(angleRad);
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
