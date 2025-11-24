#include "Clear.h"

// エンドロールに表示するクレジット
static const Array<std::pair<String, Array<String>>> Credits = {
	{ U"デザイン",{ U"夜狩" } },
	{ U"プログラム",{ U"kameo" } },
	{ U"Special Thanks",{ U"You" } },
};


Clear::Clear(const InitData& init)
	: IScene(init)
{
	initCredits();
	audio.play();
	audio.setVolume(volume);
}

void Clear::initCredits()
{
	double y = 0;

	for (const auto& credit : Credits)
	{
		m_credits.emplace_back(credit.first, y, true);
		y += 70;

		for (const auto& name : credit.second)
		{
			m_credits.emplace_back(name, y, false);
			y += 60;
		}

		y += 60;
	}

	m_height = y;
}
void Clear::update()
{
	// クリック or Esc でタイトルへ戻る
	if ((MouseL | KeyEscape).down())
	{
		changeScene(State::Title);
	}

	// 全部流れ切ったらリセット
	if ((m_height + yOffset()) < 0)
	{
		changeScene(State::Title);
	}
}

void Clear::draw() const
{
	const double offset = yOffset();

	for (const auto& credit : m_credits)
	{
		FontAsset(std::get<bool>(credit) ? U"CreditBig" : U"CreditSmall")(std::get<String>(credit))
			.drawAt(Scene::Center().x, std::get<double>(credit) + offset);
	}
}
