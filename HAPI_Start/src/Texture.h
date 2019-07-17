#pragma once

#include "HAPI_lib.h"
#include <string>

struct Texture
{
	Texture();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&&) = delete;
	Texture&& operator=(Texture&&) = delete;

	bool load(const std::string& fileName);

	HAPISPACE::BYTE* m_texture;
	int m_width;
	int m_height;
	bool m_alpha;
};