#include "Graphics.hpp"
#include "utils.hpp"

Graphics::Graphics(RenderState *rState)
: m_renderState(rState) {}

void Graphics::setRenderState(RenderState *rState)
{
	m_renderState = rState;
}

// Fills screen with chosen color
void Graphics::clearScreen(unsigned int color)
{
	unsigned int* pixel = (unsigned int*) m_renderState->memory;

	for (int y = 0; y < m_renderState->height; y++)
	{
		for (int x = 0; x < m_renderState->width; x++)
		{
			*pixel++ = color;
		}
	}
}

// Draws a rectangle into RenderState::memory member
void Graphics::drawRectangleInPixelsToBuffer(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int color)
{
	x = utils::clamp((unsigned int)0, x, m_renderState->width);
	y = utils::clamp((unsigned int)0, y, m_renderState->height);

	unsigned int right = utils::clamp((unsigned int)0, x + width, m_renderState->width);
	unsigned int bottom = utils::clamp((unsigned int)0, y + height, m_renderState->height);
	
	for (unsigned int yCount = y; yCount < bottom; yCount++)
	{
		unsigned int *currentPixel = (unsigned int *)m_renderState->memory + x + yCount * m_renderState->width;

		for (unsigned int xCount = x; xCount < right; xCount++)
		{
			*currentPixel++ = color;
		}
	}
}