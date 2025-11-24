#include "Title.h"
#include "Common.h"


//タイトルシーン
Title::Title(const InitData& init)
	: IScene{ init }
{
	m_backButton = UIManager{ Vec2{1180, 20}, 60, Emoji{U"❌"} };// 説明画面の戻るボタン
	m_setumeiTexture = Texture{ U"Texture/setumei.png" };// 説明画面の画像
	m_titleImage = Texture{ U"Texture/TitleImage.png" };// タイトル背景画像
	m_title = Texture{ U"Texture/Title.png" };// タイトル文字
}

void Title::update()
{
	// 楽器選択画面
	
	if (isInstrumentSelect)
	{
		auto& inst = getData().instrument; // 現在選んだ楽器

		// ボタンサイズ・X座標
		const Size buttonSize{ 400, 60 };
		const int centerX = Scene::Width() / 2 - buttonSize.x / 2;

		// 楽器ボタン押下判定
		auto checkButton = [&](GMInstrument type, int y)
			{
				RectF rect{ Arg::topLeft(centerX, y), buttonSize };
				if (rect.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
				if (rect.leftClicked()) inst = type;
			};

		checkButton(GMInstrument::Piano1, 260);// ピアノ
		checkButton(GMInstrument::MusicBox, 360);// オルゴール
		checkButton(GMInstrument::Trumpet, 460);// トランペット
		checkButton(GMInstrument::Recorder, 560);// リコーダー

		// 戻るボタン
		if (RectF{ Arg::center(1200,50), 64 }.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
			if (MouseL.down())
			{
				isInstrumentSelect = false;
			}
		}
		return;
	}



	// タイトル、説明画面
	if (!isSetumei)
	{
		// ボタンのホバーアニメーション更新
		m_startTransition.update(m_startButton.mouseOver());
		m_setumeiTransition.update(m_setumeiButton.mouseOver());
		m_instrumentTransition.update(m_instrumentButton.mouseOver());
		m_exitTransition.update(m_exitButton.mouseOver());

		// いずれかのボタンにマウスが載っている
		if (m_startButton.mouseOver()
			|| m_setumeiButton.mouseOver()
			|| m_instrumentButton.mouseOver()
			|| m_exitButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}

		// PLAYボタン
		if (m_startButton.leftClicked())
		{
			changeScene(State::Game);
		}
		else if (m_setumeiButton.leftClicked())// 説明
		{
			isSetumei = true;
		}
		else if (m_instrumentButton.leftClicked())// 楽器選択
		{
			isInstrumentSelect = true;
		}
		else if (m_exitButton.leftClicked())// EXIT
		{
			System::Exit();
		}
	}
	else// 説明画面
	{
		if (RectF{ Arg::center(1200,50), 64 }.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
			if (MouseL.down()) isSetumei = false;
		}
	}
}

void Title::draw() const
{
	//  タイトル背景、 タイトルロゴ
	m_titleImage.draw();
	m_title.drawAt(Scene::Width() / 2, 100);

	//通常タイトル 
	if (!isSetumei && !isInstrumentSelect)
	{
		const Font& boldFont = FontAsset(U"Bold");


		// PLAY
		m_startButton.draw(ColorF{ 1.0, m_startTransition.value() }).drawFrame(2);
		boldFont(U"PLAY").drawAt(36, m_startButton.center());

		// 説明
		m_setumeiButton.draw(ColorF{ 1.0, m_setumeiTransition.value() }).drawFrame(2);
		boldFont(U"説明").drawAt(36, m_setumeiButton.center());

		// 楽器を選ぶ 
		m_instrumentButton.draw(ColorF{ 1.0, m_instrumentTransition.value() }).drawFrame(2);
		boldFont(U"楽器を選ぶ").drawAt(26, m_instrumentButton.center());

		// EXIT
		m_exitButton.draw(ColorF{ 1.0, m_exitTransition.value() }).drawFrame(2);
		boldFont(U"EXIT").drawAt(36, m_exitButton.center());
	}

	//説明画面 
	if (isSetumei)
	{
		m_setumeiTexture.drawAt(Scene::Width() / 2, Scene::Height() / 2);
		m_backButton.draw();
	}

	
	//楽器選択画面
	if (isInstrumentSelect)
	{
		FontAsset(U"Bold")(U" 楽器を選んでください").drawAt(40, Scene::Center().x, 160);
		m_backButton.draw();

		const auto inst = getData().instrument;

		// ボタンサイズ・X座標（中央寄せ）
		const Size buttonSize{ 400, 60 };
		const int centerX = Scene::Width() / 2 - buttonSize.x / 2;

		auto drawButton = [&](String name, GMInstrument type, int y)
			{
				bool active = (inst == type);

				// 選択中は緑
				ColorF bgColor = active ? ColorF(0.7, 0.95, 0.7) : ColorF(1.0);

				// 文字色
				ColorF textColor = active ? ColorF(0.0) : ColorF(0.1);

				RectF rect{ Arg::topLeft(centerX, y), buttonSize };
				rect.draw(bgColor);
				rect.drawFrame(2);

				FontAsset(U"Bold")(name).drawAt(rect.center(), textColor);
			};

		// ボタン配置
		drawButton(U"ピアノ", GMInstrument::Piano1, 260);
		drawButton(U"オルゴール", GMInstrument::MusicBox, 360);
		drawButton(U"トランペット", GMInstrument::Trumpet, 460);
		drawButton(U"リコーダー", GMInstrument::Recorder, 560);

		return;
	}

}
