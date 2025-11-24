#pragma once
#include <Siv3D.hpp>

class UIManager
{
public:
	UIManager() = default;

	UIManager(const Vec2& pos, double size, const Emoji& emoji)
		: m_rect(pos.x, pos.y, size, size)
		, m_emojiTexture{ emoji }
		, m_mask{ MakeRoundBuffer(CreateEmojiPolygons(emoji), 4).scaled(EmojiScale) }
	{
	}

	bool draw() const
	{
		const RoundRect rr{ m_rect, 8 };// ボタンの背景
		bool hovered = rr.mouseOver();// マウスが乗っているか
		m_transition.update(hovered);

		rr.draw(hovered ? ColorF{ 0.95 } : ColorF{ 1.0 })// 背景の描画
			.drawFrame(2, ColorF{ 0.4,0.5,1.0 });

		Vec2 center = m_rect.center();// アイコン描画
		double angle = Math::Sin(m_transition.value() * 8_pi) * 4_deg * m_transition.value();
		const Transformer2D tf{ Mat3x2::Rotate(angle, center) };

		m_mask.draw(center, ColorF{ 0.2, 0.3, 0.4, 0.4 });// 影マスクを描画
		m_emojiTexture.scaled(EmojiScale).rotated(angle).drawAt(center);

		return rr.leftClicked();// 左クリックでボタン押下
	}
	// 座標が UI の中に含まれているか判定
	bool contains(const Vec2& p) const { return m_rect.intersects(p); }

private:
	RectF m_rect;// ボタンの領域
	Texture m_emojiTexture;  
	MultiPolygon m_mask;// アイコンのマスク用ポリゴン
	mutable Transition m_transition{ 0.2s, 0.2s };

	static constexpr double EmojiScale = 0.3;

	// 絵文字から形状データを生成する
	static MultiPolygon CreateEmojiPolygons(const Emoji& emoji)
	{
		Image img{ emoji };  
		return img.alphaToPolygonsCentered(160, AllowHoles::No);
	}

	// ポリゴンを丸めてマスク用ポリゴンを作成する
	static MultiPolygon MakeRoundBuffer(const MultiPolygon& polys, int32 dist)
	{
		MultiPolygon result;
		for (auto& p : polys)
		{
			result = Geometry2D::Or(result, p.calculateRoundBuffer(dist));
		}
		return result;
	}
};
