#pragma once

#include "HAPI_lib.h"
#include <string>
#include "Global.h"

struct Frame
{
	Frame(int y, int x, bool alpha = false);

	int y;
	int x;
	bool alpha;
};

class Texture
{
public:
	Texture();
	~Texture();
	//Texture(const Texture&) = delete;
	//Texture& operator=(const Texture&) = delete;
	//Texture(Texture&&) = delete;
	//Texture&& operator=(Texture&&) = delete;

	Rectangle getFrameRect(int tileID) const;
	bool isAlpha() const;
	bool load(const std::string& xmlFileName, const std::string& textureFileName);
	HAPISPACE::BYTE* getTexture();
	Vector2i getSize() const;

private:
	HAPISPACE::BYTE* m_texture;
	std::vector<Frame> m_frames;
	Vector2i m_textureSize;
	int m_columns;
	int m_tileSize;
	bool m_alpha;

	void loadInFrames();
	bool isFrameAlpha(int ID) const;
};