#pragma once

#include <Windows.h>

typedef unsigned int uint;

namespace utils
{
	template<typename T>
	inline T clamp(T value, T min, T max)
	{
		if (value < min)
			return min;
		if (value > max)
			return max;

		return value;
	}
	
	RECT getClientRect(HWND hWnd);

	const float infinite_small = 0.0001f;
}