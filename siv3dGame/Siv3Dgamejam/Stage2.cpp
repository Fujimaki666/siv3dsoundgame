#include "Stage2.h"
#include "StageDifficulty.h"

//ステージ２　敵が近づいてくる
Stage2::Stage2(const InitData& init)
	: StageBase(init)
{
	setupStage();   // まず地形を作る
	initStage();   

	m_enemies << Enemy(world, Vec2{ 1300, 600 });
	//m_enemies << Enemy(world, Vec2{ 1200, 300 });
	//m_enemies << Enemy(world, Vec2{ 800, 500 });
	//m_enemies << Enemy(world, Vec2{ 700, 100 });

}
void Stage2::setupStage()
{
	// ステージ固有オブジェクト
	m_blocks << Block(world, Vec2{ 0, 700 }, SizeF{ 600,60 }, true);
	m_blocks << Block(world, Vec2{ 0, 360 }, SizeF{ 2,720 }, true);
	m_blocks << Block{ world, Vec2{1850, 300 }, SizeF{300,  10} , true };// ゴール台
	m_blocks << Block{ world, Vec2{600, 0 }, SizeF{50,  1000} , true };
}


Vec2 Stage2::playerStartPos() const
{
	return Vec2{ 200, 600 };
}

Vec2 Stage2::goalPos() const
{
	return Vec2{ 1900, 260 };
}

Texture Stage2::backgroundTexture() const
{
	return Texture{ U"Texture/background.png" };
}

const StageDifficulty& Stage2::stageDifficulty() const
{
	return DifficultyPreset::Normal;
}


