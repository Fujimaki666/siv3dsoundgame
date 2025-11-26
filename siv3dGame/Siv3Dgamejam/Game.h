#pragma once
#include "Common.h"
#include "Player.h"
#include "Block.h"
#include "Star.h"
#include "MikeAudio.h"
#include "FlowerEffect.h"
#include "Camera.h"


//最初のシーン
class Game : public App::Scene
{
public:
	Game(const InitData& init);

	void update() override;
	void draw() const override;

private:
	P2World world{ Vec2{ 0, 980 } }; // 重力設定
	Player m_player{ world ,Vec2{ 640, 10 } };// プレイヤー初期位置
	/*Block ground{ world, Vec2{ Scene::Width() / 2, 700 }, SizeF{ Scene::Width(), 60 } };
	P2BodyID groundID = ground.id();*/
	//Array<Block> m_blocks;
	Star m_item;
	Array<Block> m_blocks;
	Font m_font{ 20, Typeface::Bold };
	Rect m_buttonRect{ 910, 420, 200, 80 };

	// 物理シミュレーションの積算タイマー
	double accumulatedTime = 0.0;
	static constexpr double Step = 1.0 / 60.0;

	// マイク音声の起動フラグ
	bool micStarted = false;
	//MikeAudio mike;

	double changeTimer = 0.0;
	double flowerTimer = 0.0;// 花エフェクト発生タイマー
	bool UIActive = false;// UI が表示中かどうか
	std::unique_ptr<FlowerEffect> m_flower;// 花エフェクト

	Camera m_camera{ 4000, 720 };

	
};


