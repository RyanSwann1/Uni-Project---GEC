#pragma once

#include "HAPI_lib.h"
#include "Global.h"

struct Sprite;
class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window&& operator=(Window&&) = delete;

	bool initialize();
	void render(const Sprite& sprite) const;
	void clearToBlack();

private:
	HAPISPACE::BYTE* m_window;
	Vector2i m_size;

	void blit(const Sprite& sprite) const;
	void blitAlpha(const Sprite& sprite) const;
	bool isSpriteFullyContained(const Sprite& sprite) const;
	bool isSpriteViewable(Rectangle windowRect, Rectangle textureRect) const;
};