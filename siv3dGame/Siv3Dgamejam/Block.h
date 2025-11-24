#pragma once
#include <Siv3D.hpp>

class Block
{
public:
	Block(P2World& world, const Vec2& pos, const SizeF& size, bool fixed = false)
		: m_body{ world.createRect(P2Kinematic, pos, size, P2Material{.restitution = 0.0 }) },
		m_visual{ Arg::center(pos), size },
		m_pos(pos),
		m_startY(pos.y),
		m_targetY(pos.y),
		m_fixed(fixed)
	{
		m_flowerTexture = Texture{ U"Texture/flower_n.png" };
		
	}

	// プレイヤーの位置を受け取る
	void setPlayerPos(const Vec2& pos)
	{
		m_playerPosition = pos;
	}

	
	// 上に動かす目標位置を設定
	void moveTo(double newY)
	{
		if (!m_movable) return; // 動かせないブロックは無視
		m_targetY = newY;
		m_isMoving = true;
	}

	// 横方向へ移動
	void moveXTo(double targetX, double speed = 100.0)
	{
		m_targetX = targetX;
		m_moveSpeedX = speed;
		m_movingX = true;
	}

	// 上昇
	void riseTo(double targetY, double speed = 120.0)
	{
		m_riseTargetY = targetY;
		m_riseSpeed = speed;
		m_rising = true;
	}

	// 下降
	void downTo(double targetY, double speed = 120.0)
	{
		m_downTargetY = targetY;
		m_downSpeed = speed;
		m_down = true;
	}

	// 花を咲かせるフラグ
	void bloomFlower()
	{
		m_hasFlower = true;
	}

	


	void update(double deltaTime)
	{

		// 縦移動
		if (m_movable && m_isMoving)
		{
			m_pos.y -= moveSpeed * deltaTime;

			if (m_pos.y <= m_targetY)
			{
				m_pos.y = m_targetY;
				m_isMoving = false;
			}

			m_visual.setCenter(m_pos);
			m_body.setPos(m_pos);
		}


		// 上昇
		if (m_rising)
		{
			m_pos.y -= m_riseSpeed * deltaTime;
			if (m_pos.y <= m_riseTargetY)
			{
				m_pos.y = m_riseTargetY;
				m_rising = false;
			}
			m_visual.setCenter(m_pos);
			m_body.setPos(m_pos);
		}

		// 下降
		if (m_down)
		{
			m_pos.y += m_downSpeed * deltaTime;
			if (m_pos.y >= m_downTargetY)
			{
				m_pos.y = m_downTargetY;
				m_down = false;
			}
			m_visual.setCenter(m_pos);
			m_body.setPos(m_pos);
		}

		// 横移動
		if (m_movingX)
		{
			// 右向き左向き
			double dir = (m_targetX > m_pos.x) ? 1.0 : -1.0;
			m_pos.x += dir * m_moveSpeedX * deltaTime;

			// 到達判定
			if ((dir > 0 && m_pos.x >= m_targetX) ||
				(dir < 0 && m_pos.x <= m_targetX))
			{
				m_pos.x = m_targetX;
				m_movingX = false;
			}

			m_visual.setCenter(m_pos);
			m_body.setPos(m_pos);
		}

	}


	void draw() const
	{
		m_visual.draw(m_movable ? ColorF{233.0, 240.0, 241.0 } :// 動くブロック
			ColorF{ 233.0,  240.0, 241.0 });// 固定ブロック
		m_body.drawFrame(2, m_movable ? Palette::Skyblue : Palette::Gray);// 外枠
		
	}

	// 花の描画
	void drawFlower(const Texture& texture) const
	{
		if (m_hasFlower)
		{
			texture.drawAt(m_visual.center().x, m_visual.y - 20);
		}
	}

	bool hasFlower() const { return m_hasFlower; }

	P2BodyID id() const
	{
		return m_body.id();
	}

	const P2Body& body() const { return m_body; }

	bool isFixed() const { return m_fixed; }
private:
	P2Body m_body;// 物理ボディ
	RectF m_visual;// 描画用
	Vec2 m_pos; // 現在位置
	bool m_fixed = false;
	// プレイヤーの現在の位置
	Vec2 m_playerPosition;

	bool m_movable = false;   // 動くブロックかどうか
	double m_startY;   // 元の高さ
	double m_targetY;  // 目標の高さ
	double moveSpeed = 100.0; // 移動速度
	bool m_isMoving = false;

	// 上昇アニメーション用
	bool m_rising = false;
	double m_riseSpeed = 120.0;
	double m_riseTargetY = 0.0;

	//横移動用
	bool m_movingX = false;
	double m_moveSpeedX = 50.0;
	double m_targetX = 0.0;

	//下降アニメーション用
	bool m_down = false;
	double m_downSpeed = 120.0;
	double m_downTargetY = 0.0;

	// 花のエフェクト
	bool m_hasFlower = false;

	Texture m_flowerTexture;
};
