#pragma once
#include "StageBase.h"

class Stage1 : public StageBase
{
public:
	Stage1(const InitData& init);

protected:
	void setupStage() override;
	Vec2 playerStartPos() const override;
	Vec2 goalPos() const override;
	Texture backgroundTexture() const override;
	const StageDifficulty& stageDifficulty() const override;

	bool isFinalStage() const override { return false; }
	State nextStageState() const override { return State::Stage2; }
};
