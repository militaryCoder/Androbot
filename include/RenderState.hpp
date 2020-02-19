// As soon as object of RenderState class created, it must be initialized
// Example:
// RenderState renderState;
// or
// RenderState *renderState = new RenderState();

#pragma once

#include <Windows.h>

struct RenderState
{
	RenderState();

	unsigned int width;
	unsigned int height;

	void* memory;

	BITMAPINFO bitmapInfo;
};