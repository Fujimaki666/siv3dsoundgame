#include "Game.h"
#include "Common.h"
#include "Camera.h"


//最初のシーン

Game::Game(const InitData& init)
	: IScene(init)
{
	

	// 地面ブロック
	m_blocks << Block{ world, Vec2{ Scene::Width() / 2, 700 }, SizeF{ Scene::Width()+1300, 60 } }; // 地面

	m_blocks << Block{ world, Vec2{0, Scene::Height() / 2 }, SizeF{2,  Scene::Height() } };//左の壁
	m_flower = std::make_unique<FlowerEffect>(Vec2{ 1000, 500 });

}

void Game::update()
{

	//m_player.checkGround(m_blocks);
	
	world.update();
	
	m_player.update(world, m_blocks, micStarted);
	m_item.update();

	m_camera.follow(m_player.getPos());

	
	// UI（「声を出してみよう」）を出すトリガー
	if (m_player.getPos().x >= 900 && !micStarted)
	{
		UIActive = true;
	}

	// マイク起動トリガー
	if (m_player.getPos().x >= 960 && !micStarted)
	{
		//m_player.setPos(Vec2{ 1000, m_player.getPos().y });
		//mike.start();
		micStarted = true;
		changeTimer = 0.0;
	}

	//  マイクが起動している間の処理
	if (micStarted)
	{
		changeTimer += Scene::DeltaTime();
		flowerTimer = changeTimer + 2;
		m_flower->update(flowerTimer);
	}

	// 一定時間経過すると Stage1 に遷移
	if (changeTimer > 3)
	{
		changeScene(State::Stage1);
	}
}

void Game::draw() const
{
	Scene::SetBackground(ColorF{ 0.0 });// 背景色（黒）

	{
		const Transformer2D cameraScope{ m_camera.transform() };

		if (micStarted && flowerTimer > 2) { m_flower->draw(); }// 花が咲くタイミング

		// ブロックおよびアイテムの描画
		for (const auto& b : m_blocks) b.draw();
		m_item.draw();
		m_player.draw();
		
	} 

	//m_camera.apply();

	/*if( micStarted && flowerTimer > 2)
	{
		
		m_flower->draw();
	}*/

	// UI 表示
	if(UIActive && !micStarted)
	{
		m_buttonRect.rounded(10).drawFrame(3, Palette::Skyblue);
		m_font(U"声を出してみよう").drawAt(m_buttonRect.center(), ColorF{ 1.0 });
	}
	
	
	
	
}
