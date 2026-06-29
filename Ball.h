#pragma once
#include "raylib.h"

//発射状態のステートマシン
typedef enum {
	BALL_WAIT_X,		//0. AD待機状態
	BALL_CHARGE_X,		//1. X軸(A/D)のチャージ状態
	BALL_WAIT_Y,		//2. Y軸(W/S)の入力待機状態
	BALL_CHARGE_Y,		//3. Y軸(W/S)のチャージ状態
	BALL_AIMING,		//4. 予測線出してスペースキー待機状態
	BALL_FLYING			//5. 発射状態（減速処理）
} BallLaunchState;

typedef struct {
	Vector2 position;
	float radius;
	Vector2 speed;
	Color color;

	BallLaunchState state;	//現在の発射状態
	Vector2 chargePower;	//チャージされた力
	float chargeGaugeX;		//横ゲージの現在の長さ(0.0f～1.0f)
	float chargeGaugeY;		//縦ゲージの現在の長さ(0.0f～1.0f)
	bool isGaugeIncreasing; //ゲージが往復して増減するためのフラグ
} Ball;

Ball CreateBall();
void UpdateBall(Ball* ball);
void DrawBall(Ball ball);