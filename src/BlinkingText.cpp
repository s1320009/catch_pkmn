#include "BlinkingText.h"

void UpdateBlinkingText(BlinkingText& text) {
	if (text.isFadingOut) {
		text.alpha -= text.fadeSpeed;
	}
	else {
		text.alpha += text.fadeSpeed;
	}

	if (text.alpha <= 0) {
		text.alpha = 0;
		text.isFadingOut = false; // フェードインに切り替え
	}
	else if (text.alpha >= 255) {
		text.alpha = 255;
		text.isFadingOut = true; // フェードアウトに切り替え
	}
}

void DrawBlinkingText(const BlinkingText& text, Font font, const char* str, Vector2 position, float fontSize, Color color) {
	Color textColor = color;		//rgbの値をコピー 透明度はa
	textColor.a = (unsigned char)text.alpha; // 透明度を設定  unsigned charにキャストすることで、本来-128〜127の範囲の値を0〜255に変換する　色で使われる
	DrawTextEx(font, str, position, fontSize, 1, textColor);  //フォント、文字列、位置、フォントサイズ、文字間隔、色を指定
}