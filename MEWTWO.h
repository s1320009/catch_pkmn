#pragma once
#include "pkmn.h"

// ==========================================================
// Projectile（弾）の定義 - MEWTWO専用
// ==========================================================

typedef struct {
	Vector2 position;           // 弾の現在位置
	Vector2 centerPos;          // 公転中心（MEWTWO の位置）
	float distance;             // 中心からの現在距離
	float angle;                // 現在の角度（度）
	float angularVelocity;      // 角速度（度/秒）
	float distanceGrowthRate;   // 距離増加速度（px/秒）
	float lifeTime;             // 弾の寿命（経過時間）
	float maxLifeTime;          // 弾の最大寿命（秒）
	float radius;               // 弾の半径
	Color color;                // 弾の色
	bool isActive;              // アクティブフラグ
} Projectile;

// ==========================================================
// ProjectileManager（弾管理システム）
// ==========================================================

#define MAX_PROJECTILES 100
typedef struct {
	Projectile projectiles[MAX_PROJECTILES];
	int count;
} ProjectileManager;

// ==========================================================
// MEWTWO 専用パラメータ構造体
// ==========================================================

typedef struct {
	float rotationPerSec;		// 回転速度（度/秒）
	float attackPhaseTime;		// 各フェーズの継続時間（秒）
	int projectilesPerAttack;	// 一度の攻撃で発射する弾の数（4方向）
	float projectileDistance;	// 弾の初期距離
	float projectileRadius;		// 弾の半径
	float projectileRotationSpeed; // 弾の回転速度（度/秒）
	float projectileMaxLifeTime;   // 弾の最大寿命（秒）
} MewtwoAttackParameters;

// ==========================================================
// Projectile関連関数
// ==========================================================

Projectile CreateProjectile(Vector2 centerPos, Vector2 initialPos, float distance, float direction, float rotationSpeed, float maxLifeTime, float radius, Color color);
void UpdateProjectile(Projectile* proj);
void DrawProjectile(Projectile proj);

// ==========================================================
// ProjectileManager関連関数
// ==========================================================

ProjectileManager CreateProjectileManager();
void AddProjectile(ProjectileManager* manager, Projectile proj);
void UpdateProjectileManager(ProjectileManager* manager);
void DrawProjectileManager(ProjectileManager manager);
void ClearProjectileManager(ProjectileManager* manager);

// ==========================================================
// MEWTWO 専用関数
// ==========================================================

// MEWTWO ATTACK 状態の更新（内部でグローバルProjectileManagerを使用）
void UpdateMewtwoAttack(Pkmn* pkmn);

// MEWTWO MOVE 状態の更新（必要に応じて実装）
void UpdateMewtwoMove(Pkmn* pkmn);

// MEWTWO 描画（回転角度の視覚化など）
void DrawMewtwoAttack(Pkmn pkmn);

// ProjectileManager のアクセス関数
ProjectileManager* GetMewtwoProjectileManager();

// MEWTWO ATTACK パラメータを取得
MewtwoAttackParameters GetMewtwoAttackParameters();