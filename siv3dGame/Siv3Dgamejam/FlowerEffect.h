#pragma once
#include <Siv3D.hpp>

//最初の花が咲く演出
class FlowerEffect
{
public:
	FlowerEffect(const Vec2& pos)
		: m_pos(pos)
	{
		m_textures = {
			Texture{ U"Texture/flower1.png" }, // つぼみ
			Texture{ U"Texture/flower2.png" }, // 半開き
			Texture{ U"Texture/flower3.png" }, // 開花
			Texture{ U"Texture/flower4.png" }  // 満開
		};
	}

	void update(double elapsedTime)
	{
		// 録音開始からの経過時間に応じて段階的に咲く
		if (elapsedTime < 3.0)
			m_frame = 0;
		else if (elapsedTime < 4.0)
			m_frame = 1;
		else if (elapsedTime < 5.0)
			m_frame = 2;
		else
			m_frame = 3;
	}

	void draw() const
	{
		m_textures[m_frame].drawAt(m_pos);
	}

private:
	Vec2 m_pos;
	Array<Texture> m_textures;
	int m_frame = 0;
};
