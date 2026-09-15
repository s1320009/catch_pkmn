#pragma once
#include <algorithm>
#include <cassert>

namespace Common {
	constexpr int SCREEN_WIDTH = 1280;
	constexpr int SCREEN_HEIGHT = 720;
	constexpr int TARGET_FPS = 60;

	//@brief オブジェクトの位置を画面内に制限する関数
	template <typename T> 
	T clamp(T value, T min, T max) {
		assert(min <= max && "min should be less than or equal to max");
		return std::min(std::max(min, value), max);
	}
}

