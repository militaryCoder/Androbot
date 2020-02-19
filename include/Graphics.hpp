// As soon as object of Graphics class created, it must be initialized
// Example:
// Graphics graphics;
// or
// Graphics *graphics = new Graphics();

#pragma once

#include "RenderState.hpp"

class Graphics
{
	public:
		Graphics() = default;
		Graphics(RenderState *);

		void setRenderState(RenderState *);

		void clearScreen(unsigned int color);
		void drawRectangleInPixelsToBuffer(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color);

	private:
		RenderState *m_renderState;
		
};