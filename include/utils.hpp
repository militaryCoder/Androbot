#pragma once

namespace utils
{
	template<typename ValType, typename MinType, typename MaxType>
	inline ValType clamp(ValType value, MinType min, MaxType max)
	{
		if (value < min)
			return min;
		if (value > max)
			return max;

		return value;
	}
}