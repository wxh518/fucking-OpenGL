#pragma once

#include "Renderer.h"
#include <string>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width;
	int m_Height;
	int m_BPP;
public:
	Texture(const std::string& filepath);
	~Texture();

	void Bind(const unsigned int& slot = 0) const;
	void UnBind();
};