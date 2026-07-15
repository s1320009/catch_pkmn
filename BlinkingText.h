#pragma once
#include "raylib.h"

// 点滅するテキストの構造体
struct BlinkingText {
	int alpha = 255; // 透明度（0〜255）
	int fadeSpeed = 3;  // 透明度の変化速度
	bool isFadingOut = true; // フェードアウト中かどうか trueなら透明度が減少、falseなら増加
};

void UpdateBlinkingText(BlinkingText& text);

void DrawBlinkingText(const BlinkingText& text, Font font, const char* str, Vector2 position, float fontSize, Color color);