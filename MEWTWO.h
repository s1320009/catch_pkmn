#pragma once
#include "pkmn.h"

// ==========================================================
// Projectile（弾）の定義 - MEWTWO専用
// ==========================================================
// 
// 弾は MEWTWO を中心に「カーブして飛んでいく」軌道を描きます。
// 極座標（角度と距離）で位置を管理し、毎フレーム角度と距離を増やすことで、
// 回転しながら遠ざかっていく効果を実現します。
//
// 【軌道の仕組み】
// 1. angle（角度）が毎秒 angularVelocity 度ずつ増える
//    → 弾が MEWTWO を中心に回転している
// 2. distance（距離）が毎秒 distanceGrowthRate px ずつ増える
//    → 弾が MEWTWO から遠ざかっていく
// 3. この2つが同時に起こる
//    → 螺旋軌道（カーブして飛ぶ）になる
//
// 【位置計算の公式】
// x = centerPos.x + distance * cos(angle)
// y = centerPos.y + distance * sin(angle)
//

typedef struct {
	// ===== 位置情報 =====
	Vector2 position;           // 弾が画面上にいる座標（毎フレーム更新される）
	Vector2 centerPos;          // 軌道の中心点（MEWTWO の座標、変わらない）

	// ===== 軌道情報（極座標） =====
	float distance;             // 中心からの距離（px）。毎秒 distanceGrowthRate ずつ増える
	float angle;                // 弾の方向角度（度）。-90=上、0=右、90=下、180=左
								 // 毎秒 angularVelocity ずつ増えることで回転する

	// ===== 運動パラメータ =====
	float angularVelocity;      // 角速度（度/秒）。MEWTWO の rotation と同じ値
								 // 値が大きいほどカーブがきつくなる
	float distanceGrowthRate;   // 距離増加速度（px/秒）。デフォルト100.0f
								 // 値が大きいほど弾が速く遠ざかる

	// ===== 生存時間 =====
	float lifeTime;             // 弾が生成されてからの経過時間（秒）
	float maxLifeTime;          // 弾の最大寿命（秒）。これを超えると消える

	// ===== 描画情報 =====
	float radius;               // 弾の半径（px）
	Color color;                // 弾の色（PURPLE など）

	// ===== 状態 =====
	bool isActive;              // true=生きている, false=消すべき状態
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
// 
// MEWTWO の ATTACK 時の弾の挙動を制御するパラメータをまとめています。
// GetMewtwoAttackParameters() で取得できます。
//

typedef struct {
	float rotationPerSec;           // MEWTWO 本体の回転速度（度/秒）
									 // 例：30.0f = 1秒で30度回転

	float attackPhaseTime;          // 各フェーズの時間（秒）
									 // ATTACK は2つのフェーズに分かれている
									 // フェーズ0（0～0.5秒）: 初回発射
									 // フェーズ1（0.5～1.0秒）: 2回目発射

	int projectilesPerAttack;       // 1回の発射で出す弾の数（通常4個：上下左右）

	float projectileDistance;       // 弾の初期距離（px）
									 // MEWTWO から何px離れた位置から発射するか

	float projectileRadius;         // 弾の大きさ（px）

	float projectileRotationSpeed;  // 弾が MEWTWO を中心に回転する角速度（度/秒）
									 // これが angularVelocity になる
									 // 値が大きいほどカーブがきつくなる

	float projectileMaxLifeTime;    // 弾が消えるまでの時間（秒）
									 // 例：3.0f = 3秒後に消える
} MewtwoAttackParameters;

// ==========================================================
// Projectile関連関数
// ==========================================================

// 【CreateProjectile】弾を1個生成する
// パラメータ説明：
//   centerPos: 軌道の中心（MEWTWO の座標）
//   initialPos: 弾の初期生成位置（centerPos から direction 方向に distance だけ離れた位置）
//   distance: 初期距離（中心からの距離）
//   direction: 初期角度（度。-90=上, 0=右, 90=下, 180=左）
//   rotationSpeed: 弾が回転する角速度（度/秒）← これが angularVelocity になる
//   maxLifeTime: 弾の寿命（秒）
//   radius: 弾の大きさ（px）
//   color: 弾の色
// 戻り値：新しく作られた Projectile 構造体
Projectile CreateProjectile(Vector2 centerPos, Vector2 initialPos, float distance, float direction, float rotationSpeed, float maxLifeTime, float radius, Color color);

// 【UpdateProjectile】毎フレーム弾を更新する
// 処理内容：
//   1. 経過時間を加算
//   2. 寿命切れチェック
//   3. 角度を増加させる（回転）
//   4. 距離を増加させる（遠ざかる）
//   5. 新しい角度と距離から position を計算
void UpdateProjectile(Projectile* proj);

// 【DrawProjectile】弾を画面に描画する
// 処理内容：弾の position に円を描く
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