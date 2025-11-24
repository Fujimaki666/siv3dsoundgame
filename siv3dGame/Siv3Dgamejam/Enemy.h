#pragma once
#include <Siv3D.hpp>

class Enemy
{
public:
	static Texture& tex() {
		
		static bool loaded = false;
		static Texture s_tex;
		if (!loaded) {
			try { s_tex = Texture{ U"Texture/Enemy.png" }; }
			catch (...) { s_tex = Texture{ Emoji{ U"👾" } }; }// 読み込み失敗時の代替表示
			loaded = true;
		}
		return s_tex;
	}

	// 敵がボスかどうか
	bool isBoss = false;

	// 通常敵用 
	P2Body m_body;

	//ボス
	Vec2 pos;
	double radius =0;// ボスの当たり判定
	double bossSpeed = 60;
	double scale = 6.0;

	Enemy(P2World& world, const Vec2& start, bool boss = false);

	void update(const Vec2& playerPos, bool chase);
	void draw() const;
	void resetPos();// 初期位置に戻す
	void startChase();// 追跡開始位置を記録

	Vec2 getPos() const;// 現在位置を返す
	//P2BodyID bodyID() const { return m_body.id(); }
	P2BodyID bodyID() const {
		return isBoss ? P2BodyID{} : m_body.id();
	}
private:
	//Texture m_tex = Texture{ U"Texture/Enemy.png" };

	// 通常敵用
	double speed = 50.0;
	Vec2 m_startPos;
	Vec2 m_chaseStartPos;// 追跡モードに移った瞬間の位置
	double maxChaseDist = 30.0; // 追跡最大距離
};
