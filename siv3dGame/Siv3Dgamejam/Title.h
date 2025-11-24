# pragma once
# include "Common.h"
# include "UIManager.h"

// タイトルシーン
class Title : public App::Scene
{
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;

	bool isInstrumentSelect = false;// 楽器選択画面かどうか

private:

	RoundRect m_startButton{ Arg::center(Scene::Width() / 2, 300), 300, 60, 8 };
	RoundRect m_setumeiButton{ Arg::center(Scene::Width() / 2, 400), 300, 60, 8 };
	RoundRect m_instrumentButton{ Arg::center(Scene::Width() / 2, 500), 300, 60, 8 };
	Transition m_instrumentTransition{ 0.4s, 0.2s };
	RoundRect m_exitButton{ Arg::center(Scene::Width() / 2, 600), 300, 60, 8 };

	// ホバー時のアニメーション
	Transition m_startTransition{ 0.4s, 0.2s };
	Transition m_setumeiTransition{ 0.4s, 0.2s };
	Transition m_exitTransition{ 0.4s, 0.2s };

	//  タイトル画面の背景
	Rect m_titleRect{ 0, 0, 1280, 720 };

	bool isSetumei = false;// 説明画面が表示されているか
	//Rect m_backButton{ 300, 500, 200, 60 };
	UIManager m_backButton;//  説明画面の戻るボタン
	Transition m_backTransition{ 0.4s, 0.2s };

	Texture m_setumeiTexture;//  説明画面の画像

	Texture m_titleImage;// タイトル背景画像

	Texture m_title;//タイトル文字画像


};
#pragma once
