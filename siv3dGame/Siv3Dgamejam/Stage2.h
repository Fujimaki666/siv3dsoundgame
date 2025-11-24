#pragma once
#include "StageBase.h"
#include "Enemy.h"
class Stage2 : public StageBase
{
public:
	Stage2(const InitData& init);

protected:
	void setupStage() override;
	Vec2 playerStartPos() const override;
	Vec2 goalPos() const override;
	Texture backgroundTexture() const override;
	const StageDifficulty& stageDifficulty() const override;

	bool isFinalStage() const override { return false; }
	State nextStageState() const override { return State::StageFinal; }
	

};
#pragma once
