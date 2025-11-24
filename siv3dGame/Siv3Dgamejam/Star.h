#pragma once
#include <Siv3D.hpp>

//最初のシーンの星
class Star
{
public:

	Star()
		: m_position(1000, 680)
		, m_texture{ U"Texture/kirakira.png" }
	{
	}

	void update()
	{
		
	}

	void draw() const
	{
		m_texture.scaled(0.15).drawAt(m_position);
	}

private:
	Vec2 m_position;
	Texture m_texture;
};
#pragma once
