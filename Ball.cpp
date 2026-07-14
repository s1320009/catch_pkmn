#include "Ball.h"
#include "raylib.h"

//windowは上がマイナス
const float UP = -1.0f;
// 最大チャージ時の初速（必要に応じて調整してください）
const float MAX_LAUNCH_SPEED = 40.0f;
// 飛行中の減速係数（摩擦。1.0未満で徐々に減速）
const float BALL_FRICTION = 0.98f;

Ball CreateBall() {
	Ball ball;
	ball.position = {0.0f,0.0f };
	ball.radius = 12.0f;
    ball.speed = { 0.0f, 0.0f };             // 最初は止まっている
    ball.color = MAROON;                     // 色も固定

    //ゲージを左においておくための変数
	ball.isAimingLeft = false;              //ゲージを左においておくための変数
	ball.isAimingDown = false;              //ゲージを下においておくための変数
	ball.outTimer = 0.0f;                       //画面外に出たときのタイマー

    // ステートマシンの初期化
    ball.state = BALL_WAIT_X;
    ball.chargePower = { 0.0f, 0.0f };
    ball.chargeGaugeX = 0.0f;
    ball.chargeGaugeY = 0.0f;
	ball.isGaugeIncreasing = true;           // ゲージが増加中か減少中かのフラグ

    return ball;
}

void UpdateBall(Ball* ball, Player* player) {
    //消える処理で使う
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    //ゲージが往復して増減するスピード
    float gaugeSpeed = 2.0f * GetFrameTime(); //約0.5秒でMAXになる速度

    //ボールの初期位置
	Vector2 initialPos = player->position;

	//ステートマシンの処理
    switch (ball->state) {
        case BALL_WAIT_X:
            // 待機状態ではボールを手元に固定して各種パワーをリセット
            ball->position = initialPos;
            ball->speed = { 0.0f, 0.0f };
            ball->chargePower = { 0.0f, 0.0f };
            ball->chargeGaugeX = 0.0f;
            ball->chargeGaugeY = 0.0f;

            //プレイヤーが４ンでいるときはchargeできないようにする
            if (player->state == PLAYER_STATE_DEAD) {
				ball->state = BALL_WAIT_X;
				break;
            }

            // AかDが押されたら横チャージ状態へ移行
            if (IsKeyDown(KEY_A) || IsKeyDown(KEY_D)) {
                ball->state = BALL_CHARGE_X;
                ball->isGaugeIncreasing = true;
                ball->isAimingLeft = IsKeyDown(KEY_A) ? true : false;
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

            //キーが離されたら確定させる
			if (IsKeyReleased(KEY_A) || IsKeyReleased(KEY_D)) {
				float directionX = ball->isAimingLeft ? -1.0f : 1.0f; // Aキーなら左(-1)、Dキーなら右(+1)
                ball->chargePower.x = ball->chargeGaugeX * MAX_LAUNCH_SPEED * directionX;
				ball->state = BALL_WAIT_Y;
			}
            break;

        case BALL_WAIT_Y:
			ball->position = initialPos;    //手元に戻す
            // Bキーが押されたら、横の受付（最初の状態）に巻き戻す
            if (IsKeyPressed(KEY_B)) {
                ball->state = BALL_WAIT_X;
            }
            // WかSが押されたら縦チャージ開始
            else if (IsKeyDown(KEY_W) || IsKeyDown(KEY_S)) {
                ball->state = BALL_CHARGE_Y;
                ball->isGaugeIncreasing = true;
                ball->isAimingDown = IsKeyDown(KEY_S) ? true : false;
            }
            break;

        case BALL_CHARGE_Y:
            ball->position = initialPos;    //手元に戻す
            // Bキーが押されたら、縦のチャージを中断して縦の入力待ちへ巻き戻す
            if (IsKeyPressed(KEY_B)) {
                ball->chargeGaugeY = 0.0f;
                ball->state = BALL_WAIT_Y;
                break;
            }

            // 縦ゲージの増減（0.0～1.0を往復）
            if (ball->isGaugeIncreasing) {
                ball->chargeGaugeY += gaugeSpeed;
                if (ball->chargeGaugeY >= 1.0f) {
                    ball->chargeGaugeY = 1.0f;
                    ball->isGaugeIncreasing = false;
                }
            }
            else {
                ball->chargeGaugeY -= gaugeSpeed;
                if (ball->chargeGaugeY <= 0.0f) {
                    ball->chargeGaugeY = 0.0f;
                    ball->isGaugeIncreasing = true;
                }
            }

			//キーが離されたら確定させる
			if (IsKeyReleased(KEY_W) || IsKeyReleased(KEY_S)) {
				float directionY = ball->isAimingDown ? 1.0f : -1.0f; // Sキーなら下(+1)、Wキーなら上(-1)
				ball->chargePower.y = ball->chargeGaugeY * MAX_LAUNCH_SPEED * directionY;
				ball->state = BALL_AIMING;
			}
            break;

		case BALL_AIMING:
            ball->position = initialPos;    //手元に戻す
            // Bキーが押されたら、縦のパワーをクリアして縦の入力待ちに戻る
            if (IsKeyPressed(KEY_B)) {
                ball->chargePower.y = 0.0f;
                ball->chargeGaugeY = 0.0f;
                ball->state = BALL_WAIT_Y;
            }
            // スペースキーが押されたら、力を速度にコピーしてついに発射！
            else if (IsKeyPressed(KEY_SPACE)) {
                ball->speed = ball->chargePower;
                ball->state = BALL_FLYING;
            }
            break;

        case BALL_FLYING:
		{       //変数float speedSquaredをこのcase内で使うために{}で囲む
            // 速度を座標に足して移動させる
            ball->position.x += ball->speed.x;
            ball->position.y += ball->speed.y;

            // 毎フレーム、摩擦を掛け算して徐々に減速させる
            ball->speed.x *= BALL_FRICTION;
            ball->speed.y *= BALL_FRICTION;

            // 速度がほぼゼロになったら自動的に待機状態（手元）に戻る

            // 速度ベクトルの「二乗の長さ」（Xの二乗 + Yの二乗）を出す
            float speedSquared = (ball->speed.x * ball->speed.x) + (ball->speed.y * ball->speed.y);

            // 速度がほぼゼロ（0.2の二乗である 0.04f 未満）になったら手元に戻す
            if (IsKeyPressed(KEY_SPACE) || speedSquared < 0.04f) {
                ball->state = BALL_WAIT_X;
            }
            else if (ball->position.x < -ball->radius || ball->position.x > screenWidth + ball->radius ||
                ball->position.y < -ball->radius || ball->position.y > screenHeight + ball->radius) {

                ball->outTimer = 0.0f; // 画面外に出た瞬間にタイマーをリセット
                ball->state = BALL_OUT;
            }
            break;
        }       //小部屋終わり

		case BALL_OUT:
			// 画面外に出たら、一定時間経過後に手元に戻す
			ball->outTimer += GetFrameTime();
			if (ball->outTimer >= 0.5f) { // 0.5秒経過したら手元に戻す
				ball->state = BALL_WAIT_X;
			}
            break;
    }
}

void DrawBall(Ball ball) {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    float gaugeMax = 200.0f; // ゲージの最大値
	float gaugeThickness = 20.0f; // ゲージの厚み
    
    // WAIT以外でボール本体を描画
    if (ball.state != BALL_WAIT_X && ball.state != BALL_WAIT_Y) {
        DrawCircleV(ball.position, ball.radius, ball.color);
    }

    if (ball.state == BALL_WAIT_X) {
        DrawText("PRESS [A/D] TO CHARGE / [B] TO CANCEL", 50, screenHeight - 30, 20, MAROON);
    }
    if (ball.state == BALL_WAIT_Y) {
        DrawText("PRESS [W/S] TO CHARGE / [B] TO CANCEL", 50, screenHeight - 30, 20, MAROON);
    }

    // 2. 状態に応じたチャージゲージUIの描画
    // 横チャージ中、またはそれ以降の状態なら画面下に横ゲージを表示
    if (ball.state >= BALL_CHARGE_X && ball.state <= BALL_AIMING) {
        int gaugeW = (int)(gaugeMax * ball.chargeGaugeX);

		//ゲージの描画位置を計算
		float bgY = ball.position.y - gaugeThickness / 2;
		float rectY = bgY;
        float bgX = 0;
        float rectX = 0;

        if (ball.isAimingLeft) {
            bgX = ball.position.x - ball.radius - 15 - gaugeMax;
            // ゲージは右から左に伸びるように見せるため、右端からゲージ幅を引く
            rectX = (ball.position.x - ball.radius - 15) - gaugeW;
		}
		else {
			bgX = ball.position.x + ball.radius + 15;
			rectX = bgX;
		}

        DrawRectangle(bgX, bgY, gaugeMax, gaugeThickness, LIGHTGRAY);
        DrawRectangle(rectX, rectY, gaugeW, gaugeThickness, ORANGE);
    }

    // 縦チャージ中、またはそれ以降の状態なら画面左に縦ゲージを表示
    if (ball.state >= BALL_CHARGE_Y && ball.state <= BALL_AIMING) {
        int gaugeH = (int)(gaugeMax * ball.chargeGaugeY);

		// ゲージの描画位置を計算
		float bgX = ball.position.x - gaugeThickness / 2;
		float rectX = bgX;
		float bgY = 0;
		float rectY = 0;

		if (ball.isAimingDown) {
			bgY = ball.position.y + ball.radius + 15;
			rectY = bgY;
		}
		else {
			bgY = ball.position.y - ball.radius - 15 - gaugeMax;
			// ゲージは下から上に伸びるように見せるため、下端からゲージ高さを引く
			rectY = (ball.position.y - ball.radius - 15) - gaugeH;
		}

        DrawRectangle(bgX, bgY, gaugeThickness, gaugeMax, LIGHTGRAY);
        // 下から上に向かって伸びるように座標を計算
        DrawRectangle(rectX, rectY, gaugeThickness, gaugeH, LIME);
    }

    // 3. 発射予測線の破線描画
    if (ball.state == BALL_AIMING) {
        Vector2 simPos = ball.position;
        Vector2 simSpeed = ball.chargePower;

        // 30フレーム先までの軌道をシミュレーションして点で描く
        for (int i = 0; i < 30; i++) {
            simSpeed.x *= BALL_FRICTION;
            simSpeed.y *= BALL_FRICTION;
            simPos.x += simSpeed.x;
            simPos.y += simSpeed.y;

            if (i % 3 == 0) { // 3フレームに1回だけ描画して綺麗な破線にする
                DrawCircleV(simPos, 3, MAROON);
            }
        }
                         DrawText("PRESS [SPACE] TO LAUNCH / [B] TO CANCEL", 50, screenHeight - 30, 20, MAROON);
                     }
                 }