#include "StageFinal.h"
#include "StageDifficulty.h"

//最終ステージ

StageFinal::StageFinal(const InitData& init)
	: StageBase(init)
{
	setupStage();   
	initStage();    // 初期化


	
	//m_enemies << Enemy(world, Vec2{ 1200, 300 });
	//m_enemies << Enemy(world, Vec2{ 800, 500 });
	//m_enemies << Enemy(world, Vec2{ 700, 100 });

}
void StageFinal::setupStage()
{
	// ステージ固有オブジェクト
	m_blocks << Block(world, Vec2{ 0, 700 }, SizeF{ 600,60 }, true);
	m_blocks << Block(world, Vec2{ 0, 360 }, SizeF{ 2,720 }, true);
	m_blocks << Block{ world, Vec2{2000, 300 }, SizeF{300,  10} , true };

	m_enemies.emplace_back(world, Vec2{ -500, 300 }, true);
	auto& boss = m_enemies.back();
	boss.scale = 6.0;
	boss.bossSpeed = 60;
	boss.radius = 0;  // ボス当たり判定サイズ

}

Vec2 StageFinal::playerStartPos() const
{
	return Vec2{ 200, 600 };
	
}

Vec2 StageFinal::goalPos() const
{
	return Vec2{ 2000, 260 };
}

Texture StageFinal::backgroundTexture() const
{
	return Texture{ U"Texture/background.png" };
}

const StageDifficulty& StageFinal::stageDifficulty() const
{
	return DifficultyPreset::Normal;
}

