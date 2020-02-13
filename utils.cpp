#include "utils.hpp"

RECT utils::getClientRect(HWND hWnd)
{
	RECT rect;
	GetClientRect(hWnd, &rect);

	return rect;
}