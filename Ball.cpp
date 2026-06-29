#include "Ball.h"
#include "raylib.h"

Ball CreateBall() {
	Ball ball;
	ball.position = {0.0f,0.0f };
	ball.radius = 12.0f;
    ball.speed = { 0.0f, 0.0f };             // 最初は止まっている
    ball.color = MAROON;                     // 色も固定

    // ステートマシンの初期化
    ball.state = BALL_WAIT_X;
    ball.chargePower = { 0.0f, 0.0f };
    ball.chargeGaugeX = 0.0f;
    ball.chargeGaugeY = 0.0f;
    ball.isGaugeIncreasing = true;

    return ball;
}

void UpdateBall(Ball* ball) {
    //ゲージが往復して増減するスピード
    float gaugeSpeed = 2.0f * GetFrameTime(); //約0.5秒でMAXになる速度

    //ボールの初期位置
	Vector2 initialPos = { 400.0f, 300.0f };

	//ステートマシンの処理
    switch (ball->state) {
        case BALL_WAIT_X:
            // 待機状態ではボールを手元に固定して各種パワーをリセット
            ball->position = initialPos;
            ball->speed = { 0.0f, 0.0f };
            ball->chargePower = { 0.0f, 0.0f };
            ball->chargeGaugeX = 0.0f;
            ball->chargeGaugeY = 0.0f;

            // AかDが押されたら横チャージ状態へ移行
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
                ball->state = BALL_CHARGE_X;
                ball->isGaugeIncreasing = true;
            }
            break;

        case BALL_CHARGE_X:
            //横ゲージの増減処理
            if (ball->isGaugeIncreasing) {
                ball->chargeGaugeX += gaugeSpeed;
                if (ball->chargeGaugeX >= 1.0f) {
                    ball->chargeGaugeX = 1.0f;
                    ball->isGaugeIncreasing = false;
                }
            } else {
                ball->chargeGaugeX -= gaugeSpeed;
                if (ball->chargeGaugeX <= 0.0f) {
                    ball->chargeGaugeX = 0.0f;
                    ball->isGaugeIncreasing = true;
                }
            }
            break;
    }
}