#pragma once
#include <Siv3D.hpp>
#include <HamFramework.hpp>
#include "Common.h"

class Clear : public App::Scene
{
public:
	Clear(const InitData& init);

	void update() override;
	void draw() const override;

private:
	Array<std::tuple<String, double, bool>> m_credits;// スタッフロールの内容
	double m_height = 0.0;
	Stopwatch m_stopwatch{ StartImmediately::Yes };// スクロール用ストップウォッチ

	// 現在のスクロール位置
	double yOffset() const
	{
		return Scene::Height() + 60 - m_stopwatch.ms() / 20.0;
	}

	void initCredits();// クレジット内容をセットアップ

	Audio audio{ U"Audio/赤ちゃん・泣き声.mp3" };
	double volume = 0.6;// 音量

};
