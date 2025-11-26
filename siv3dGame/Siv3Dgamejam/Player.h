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
			if (pair.a != m_body.id() && pair.b != m_body.id())
				continue;

			P2BodyID otherID = (pair.a == m_body.id()) ? pair.b : pair.a;

			// 相手がブロックかどうかチェック
			bool isBlock = false;
			for (const auto& block : blocks)
			{
				if (otherID == block.id())
				{
					isBlock = true;
					break;
				}
			}
			if (!isBlock)
				continue;

			// 衝突法線ベクトルを取得
			const Vec2 n = collision.normal();  

			// プレイヤーが A 側
			if (pair.a == m_body.id()&& n.y > 0.5)
			{
				// 下向き（y > 0.5）に押されている,足元にブロック
					return true;
			}
			// プレイヤーが B 側
			else if( pair.b == m_body.id()&& n.y < -0.5)
			{
				// 上向き（y < -0.5）に押されている,足元にブロック
				
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

		const double moveForce = 800.0;
		const double maxSpeed = 180.0;

		// 右移動
		if (KeyRight.pressed())
		{
			if (vel.x < maxSpeed)
				m_body.applyForce({ moveForce, 0 });

			m_isMoving = true;
			m_facingRight = true;
		}
		// 左移動
		else if (KeyLeft.pressed())
		{
			if (vel.x > -maxSpeed)
				m_body.applyForce({ -moveForce, 0 });

			m_isMoving = true;
			m_facingRight = false;
		}

		// 摩擦で徐々に止める（キーを離したときだけ）
		if (!KeyRight.pressed() && !KeyLeft.pressed())
		{
			m_body.applyForce({ -vel.x * 8.0, 0 });
		}

		// ジャンプ
		if (KeySpace.down() && grounded)
		{
			m_body.applyLinearImpulse({ 0, -200 });
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
			//m_body.setVelocity({ vel.x, m_body.getVelocity().y });
		}
		else// 立ち状態
		{
			m_frameIndex = 0;
			//m_body.setVelocity({ vel.x, m_body.getVelocity().y });
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
	P2Body m_body;// 物理エンジン上の本体
	

	Array<Texture> m_textures;

	int32 m_frameIndex = 0; // どのテクスチャを使うか
	bool m_isMoving = false;// 歩いているか
	bool m_facingRight = true;// 右向きか

	

	
	
};
