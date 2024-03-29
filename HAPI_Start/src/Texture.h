#pragma once


#include "HAPI_lib.h"
#include "Global.h"
#include <string>
#include <vector>
#include <memory>
#include "Texture.h"

struct Texture;
class Textures
{
public:
	static Textures& getInstance()
	{
		static Textures instance;
		return instance;
	}

	bool loadAllTextures();

	Texture& getTileSheet();
	Texture& getHealthBar();

private:
	std::unique_ptr<Texture> m_tileSheet;
	std::unique_ptr<Texture> m_healthBarTexture;
	bool m_texturesLoaded = false;
};

struct Frame
{
	Frame(int x, int y, bool alpha = false);

	int x;
	int y;
	bool alpha;
};

class Texture
{
public:
	~Texture();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&&);
	Texture&& operator=(Texture&&) = delete;

	static std::unique_ptr<Texture> load(const std::string& xmlFileName, const std::string& imageName, 
		const std::string& textureFileName);

	Frame getFrame(int ID) const;
	Vector2i getTileSize() const;
	Rectangle getFrameRect(int tileID) const;	
	HAPISPACE::BYTE* getTexture() const;
	Vector2i getSize() const;

private:
	Texture();
	HAPISPACE::BYTE* m_tileSheet;
	std::vector<Frame> m_frames;
	Vector2i m_textureSize;
	int m_columns;
	Vector2i m_tileSize;

	bool isFrameAlpha(int ID) const;
	void loadInFrames();
	bool loadXMLTexture(const std::string& xmlFileName, const std::string& imageName, const std::string& textureFileName);
	bool loadTexture(const std::string& textureFileName);
};
