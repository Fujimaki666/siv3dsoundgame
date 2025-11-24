#pragma once
#include <Siv3D.hpp>

// ドアの開閉アニメーション
class DoorAnimation
{
public:
	DoorAnimation(const Vec2& pos)
		: m_pos(pos)
	{
		m_textures = {
			//Texture{ U"Texture/Door1.png" }, 
			Texture{ U"Texture/Door2.png" }, 
			Texture{ U"Texture/Door3.png" }, 
			Texture{ U"Texture/Door4.png" } 
		};
	}

	const Texture& getCurrentTexture() const { return m_textures[m_frame]; }
	const Vec2& getPos() const { return m_pos; }

	void update(double elapsedTime)
	{
		
		if (elapsedTime < 1.0)
			m_frame = 0;
		else if (elapsedTime < 2.0)
			m_frame = 1;
		/*else if (elapsedTime < 5.0)
			m_frame = 2;*/
		else
			m_frame = 2;//ドアが開く
	}

	void draw() const
	{
		m_textures[m_frame].drawAt(m_pos);
	}

private:
	Vec2 m_pos; // ドアの中心座標
	Array<Texture> m_textures;
	int m_frame = 0;
};
#pragma once
