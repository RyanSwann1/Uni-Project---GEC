#pragma once

#include "HAPI_lib.h"
#include "Global.h"
#include <memory>

class Sprite;
class Window
{
public:
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&);
	Window&& operator=(Window&&) = delete;

	static std::unique_ptr<Window> create();
	Vector2i getSize() const;
	
	void render(const Sprite& sprite) const;
	void clearToBlack();

private:
	Window();
	HAPISPACE::BYTE* m_window;
	Vector2i m_windowSize;

	void blit(const Sprite& sprite) const;
	void blitAlpha(const Sprite& sprite) const;
	bool isSpriteFullyContained(const Sprite& sprite) const;
	bool isSpriteViewable(Rectangle windowRect, Rectangle textureRect) const;

	bool createWindow();
};