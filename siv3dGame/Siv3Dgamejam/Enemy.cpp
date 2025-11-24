#include "Enemy.h"

Enemy::Enemy(P2World& world, const Vec2& start, bool boss)
	: isBoss(boss), m_startPos(start), m_chaseStartPos(start)
{
	if (isBoss)
	{
		// ボスは物理を使わない
		pos = start;
		radius = 100;
	}
	else
	{
		// 通常敵は物理を使う
		m_body = world.createCircle(P2Kinematic, start, 25);
		m_body.setFixedRotation(true);
	}
}

// 現在位置を取得
Vec2 Enemy::getPos() const
{
	return isBoss ? pos : m_body.getPos();
}

// 追跡開始位置を記録
void Enemy::startChase()
{
	if (!isBoss)
		m_chaseStartPos = m_body.getPos();
}

void Enemy::update(const Vec2& playerPos, bool chase)
{
	//  ボス敵の挙動（常に右方向へ移動）
	if (isBoss)
	{
		pos.x += bossSpeed * Scene::DeltaTime();
		return;
	}

	// 通常敵の挙動
	Vec2 p = m_body.getPos();

	if (chase)
	{
		double dist = p.distanceFrom(m_chaseStartPos);
		if (dist < maxChaseDist)// 最大追跡距離以内なら追いかける
		{
			// プレイヤーに向かって移動
			Vec2 dir = (playerPos - p).normalized();
			m_body.setPos(p + dir * speed * Scene::DeltaTime());
		}
	}
}

void Enemy::draw() const
{
	Vec2 p = getPos();

	// 当たり判定表示
	//Circle(p, radius).drawFrame(2, Palette::Red.withAlpha(200));

	if (isBoss)// ボスは大きく
		Enemy::tex().mirrored().scaled(scale).drawAt(p);
	else
		Enemy::tex().drawAt(p);
}

// 初期位置へ戻す
void Enemy::resetPos()
{
	if (isBoss)
	{
		pos = m_startPos;
	}
	else
	{
		m_body.setPos(m_startPos);
		m_chaseStartPos = m_startPos;
	}
}
