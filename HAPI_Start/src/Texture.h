#pragma once

#include "HAPI_lib.h"
#include <string>

class Texture
{
public:
	Texture();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&&) = delete;
	Texture&& operator=(Texture&&) = delete;

	bool containsAlpha() const;
	bool load(const std::string& fileName);
	HAPISPACE::BYTE* getTexture();
	int getWidth() const;
	int getHeight() const;

private:
	HAPISPACE::BYTE* m_texture;
	int m_width;
	int m_height;
	bool m_alpha;
};