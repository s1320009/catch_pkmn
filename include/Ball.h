#pragma once
#include "raylib.h"

////発射状態のステートマシン
//typedef enum {
//	BALL_WAIT_X,		//0. AD待機状態
//	BALL_CHARGE_X,		//1. X軸(A/D)のチャージ状態
//	BALL_WAIT_Y,		//2. Y軸(W/S)の入力待機状態
//	BALL_CHARGE_Y,		//3. Y軸(W/S)のチャージ状態
//	BALL_AIMING,		//4. 予測線出してスペースキー待機状態
//	BALL_FLYING,		//5. 発射状態（減速処理）
//	BALL_BOUNCE,		//6. PKMNに当たって跳ね返る状態
//	BALL_OUT			//7. 画面外に出た状態
//} BallLaunchState;
//
//typedef struct {
//	Vector2 position;
//	float radius;
//	Vector2 speed;
//	Color color;
//
//	bool isAimingLeft;		 //ゲージを左においておくための変数
//	bool isAimingDown;		 //ゲージを下においておくための変数
//	float outTimer;			 //画面外に出たときのタイマー
//	float bounceStartY;		 //跳ね返ったときのY座標の基準
//
//	BallLaunchState state;	//現在の発射状態
//	Vector2 chargePower;	//チャージされた力
//	float chargeGaugeX;		//横ゲージの現在の長さ(0.0f～1.0f)
//	float chargeGaugeY;		//縦ゲージの現在の長さ(0.0f～1.0f)
//	bool isGaugeIncreasing; //ゲージが往復して増減するためのフラグ
//} Ball;
//
//Ball CreateBall();
//void UpdateBall(Ball* ball, GameObject* playerObject);
//void DrawBall(Ball ball);

	enum class BALL_STATE{
		WAIT_X,		//0. AD待機状態
		CHARGE_X,	//1. X軸(A/D)のチャージ状態
		WAIT_Y,		//2. Y軸(W/S)の入力待機状態
		CHARGE_Y,	//3. Y軸(W/S)のチャージ状態
		AIMING,		//4. 予測線出してスペースキー待機状態
		FLYING,		//5. 発射状態（減速処理）
		BOUNCE,		//6. PKMNに当たって跳ね返る状態
		OUT			//7. 画面外に出た状態
	};

class Ball{
public:
	Ball();
	void Reset();
	void Update(Vector2 playerPosition);
	void Draw() const;
	BALL_STATE GetState() const;
	void SetState(BALL_STATE state);
	Vector2 GetPosition() const;
	float GetRadius() const;
	void SetSpeed(Vector2 speed);
	void SetBounceStart(Vector2 playerPosition);

private:
  BALL_STATE m_state;
	Vector2 m_position;
	float m_radius;
	Vector2 m_speed;
	Color m_color;

	bool m_isAimingLeft;		 //ゲージを左においておくための変数
	bool m_isAimingDown;		 //ゲージを下においておくための変数
	float m_outTimer;			 //画面外に出たときのタイマー
	Vector2 m_bounceStart;		 //跳ね返ったときのY座標の基準
 
	Vector2 m_chargePower;	//チャージされた力
	float m_chargeGaugeX;		//横ゲージの現在の長さ(0.0f～1.0f)
	float m_chargeGaugeY;		//縦ゲージの現在の長さ(0.0f～1.0f)
	bool m_isGaugeIncreasing; //ゲージが往復して増減するためのフラグ
};