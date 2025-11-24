#include "Stage1.h"
#include "StageDifficulty.h"

//ステージ1　敵なし
Stage1::Stage1(const InitData& init)
	: StageBase(init)
{
	setupStage();  
	initStage();    // 初期化
}

void Stage1::setupStage()
{
	// ステージ固有オブジェクト
	m_blocks << Block(world, Vec2{ 0, 700 }, SizeF{ 600,60 }, true);
	m_blocks << Block(world, Vec2{ 0, 360 }, SizeF{ 2,720 }, true);
	m_blocks << Block{ world, Vec2{1530, 300 }, SizeF{300,  10} , true };// ゴール台
}

Vec2 Stage1::playerStartPos() const
{
	return Vec2{ 200, 600 };
}

Vec2 Stage1::goalPos() const
{
	return Vec2{ 1580, 260 };
}

Texture Stage1::backgroundTexture() const
{
	return Texture{ U"Texture/background.png" };
}

const StageDifficulty& Stage1::stageDifficulty() const
{
	return DifficultyPreset::Easy;//難易度易しめ
}
