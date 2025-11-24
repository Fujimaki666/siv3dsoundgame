#pragma once
#include <Siv3D.hpp>
#include "Block.h"

class Player
{
public:
	Player() = default;

	Player(P2World& world, const Vec2& startPos)
		: m_body{ world.createRect(
			P2Dynamic,
			startPos,
			SizeF{ 35, 124 },
			P2Material{.density = 1.0, .restitution = 0.0, .friction = 0.2 }) },

		m_textures{
			Texture{ U"Texture/P_right.png" },
			Texture{ U"Texture/P_rightwalk.png" },
			Texture{ U"Texture/sing.png" }
			}
	{
		m_body.setFixedRotation(true);
	}

	

	
	Vec2 getPos() const { return m_body.getPos(); }

	void setPos(const Vec2& pos)
	{
		m_body.setPos(pos);
	}

	//接地判定
	bool isGrounded(const P2World& world, const Array<Block>& blocks) const
	{
		for (auto&& [pair, collision] : world.getCollisions())
		{
			// プレイヤーが関係しない衝突ならスキップ
			if (pair.a != m_body.id() && pair.b != m_body.id())
				continue;

			//衝突した相手の ID
			P2BodyID otherID = (pair.a == m_body.id()) ? pair.b : pair.a;

			// 衝突相手が地面ブロックか確認
			for (const auto& block : blocks)
			{
				if (otherID == block.id())
					return true;
			}
		}
		return false;
	}

	void update(P2World& world, const Array<Block>& blocks, bool recording)
	{
		Vec2 vel = m_body.getVelocity();
		bool grounded = isGrounded(world, blocks);

		m_isMoving = false;

		// プレイヤーの移動
		if (KeyRight.pressed())
		{
			vel.x = 180;
			m_isMoving = true;
			m_facingRight = true;
		}
		else if (KeyLeft.pressed())
		{
			vel.x = -180;
			m_isMoving = true;
			m_facingRight = false;
		}
		else vel.x = 0;


		

		// 最高速度の制限
		vel.x = Clamp(vel.x, -240.0, 240.0);

		// 最終反映
		m_body.setVelocity(Vec2{ vel.x, m_body.getVelocity().y });

		// ジャンプ
		if (KeySpace.down() && grounded)
		{
			m_body.applyLinearImpulse({ 0, -180 });
		}

		// 録音中
		if (recording)
		{
			m_body.setVelocity({ 0, 0 });
			m_frameIndex = 2;
		}
		else if (m_isMoving)// 歩いている
		{
			double t = Scene::Time();
			m_frameIndex = static_cast<int>(t / 0.15) % 2;
			m_body.setVelocity({ vel.x, m_body.getVelocity().y });
		}
		else// 立ち状態
		{
			m_frameIndex = 0;
			m_body.setVelocity({ vel.x, m_body.getVelocity().y });
		}
	}

	


	void draw() const
	{
		const Vec2 pos = m_body.getPos();
		const Texture& tex = m_textures[m_frameIndex];

		// 向きに応じて反転描画
		(m_facingRight ? tex : tex.mirrored()).drawAt(pos);

		// Debug
		//m_body.drawFrame(2, Palette::Red);
	}
	P2BodyID bodyID() const { return m_body.id(); }

private:
	P2Body m_body;
	

	Array<Texture> m_textures;

	int32 m_frameIndex = 0;
	bool m_isMoving = false;
	bool m_facingRight = true;

	

	
	
};
