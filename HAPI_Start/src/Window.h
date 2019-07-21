#pragma once

#include "HAPI_lib.h"
#include "Global.h"

struct Sprite;
struct Window
{
	Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window&& operator=(Window&&) = delete;

	bool isSpriteFullyContained(Vector2i position, Vector2i size) const;
	bool isSpritePartiallyContained(Rectangle windowRect, Rectangle textureRect) const;
	bool initialize();

	void clearToBlack();
	void blit(const Sprite& sprite, Vector2i position);
	void blitAlpha(const Sprite& sprite, Vector2i position);

	HAPISPACE::BYTE* m_window;
	Vector2i m_size;
};