#pragma once
#include "StageBase.h"
#include "Enemy.h"

//最終ステージ
class StageFinal : public StageBase
{
public:
	StageFinal(const InitData& init);

protected:
	void setupStage() override;
	Vec2 playerStartPos() const override;
	Vec2 goalPos() const override;
	Texture backgroundTexture() const override;
	const StageDifficulty& stageDifficulty() const override;

	bool isFinalStage() const override { return true; }
	//State nextStageState() const override { return State::StageFinal; }
	Vec2 doorPos() const override
	{
		return Vec2{ 2000, 195 };
	}

};
#pragma once
