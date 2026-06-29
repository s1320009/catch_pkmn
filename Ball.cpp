#include "Ball.h"
#include "raylib.h"

// 最大チャージ時の初速（必要に応じて調整してください）
const float MAX_LAUNCH_SPEED = 15.0f;
// 飛行中の減速係数（摩擦。1.0未満で徐々に減速）
const float BALL_FRICTION = 0.98f;

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
                if (ball->chargeGaugeX <= -1.0f) {
                    ball->chargeGaugeX = -1.0f;
                    ball->isGaugeIncreasing = true;
                }
            }

            //キーが離されたら確定させる
			if (IsKeyReleased(KEY_A) || IsKeyReleased(KEY_D)) {
				ball->chargePower.x = ball->chargeGaugeX * MAX_LAUNCH_SPEED;
				ball->state = BALL_WAIT_Y;
			}
            break;

        case BALL_WAIT_Y:
            // Bキーが押されたら、横の受付（最初の状態）に巻き戻す
            if (IsKeyPressed(KEY_B)) {
                ball->state = BALL_WAIT_X;
            }
            // WかSが押されたら縦チャージ開始
            else if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S)) {
                ball->state = BALL_CHARGE_Y;
                ball->isGaugeIncreasing = true;
            }
            break;

        case BALL_CHARGE_Y:
            // Bキーが押されたら、縦のチャージを中断して縦の入力待ちへ巻き戻す
            if (IsKeyPressed(KEY_B)) {
                ball->chargeGaugeY = 0.0f;
                ball->state = BALL_WAIT_Y;
                break;
            }

            // 縦ゲージの増減（-1.0～1.0を往復）
            if (ball->isGaugeIncreasing) {
                ball->chargeGaugeY += gaugeSpeed;
                if (ball->chargeGaugeY >= 1.0f) {
                    ball->chargeGaugeY = 1.0f;
                    ball->isGaugeIncreasing = false;
                }
            }
            else {
                ball->chargeGaugeY -= gaugeSpeed;
                if (ball->chargeGaugeY <= -1.0f) {
                    ball->chargeGaugeY = -1.0f;
                    ball->isGaugeIncreasing = true;
                }
            }

			//キーが離されたら確定させる
			if (IsKeyReleased(KEY_W) || IsKeyReleased(KEY_S)) {
				ball->chargePower.y = ball->chargeGaugeY * MAX_LAUNCH_SPEED;
				ball->state = BALL_AIMING;
			}

            break;
    }
}