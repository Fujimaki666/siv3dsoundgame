#pragma once
#include <Siv3D.hpp>

//ハート
class Item
{
public:
	Item(const Vec2& pos)
		: m_texture{ U"🤍"_emoji }, m_pos(pos) {
	}

	void draw() const
	{
		m_texture.scaled(0.4).drawAt(m_pos);
	}

	const Vec2& pos() const { return m_pos; }

private:
	Texture m_texture;
	Vec2 m_pos;
};
#pragma once
