#include "StageBase.h"
#include "DoorAnimation.h"

//ステージのベースになる

StageBase::StageBase(const InitData& init)
	: IScene(init)
	
	, m_bgm(U"Audio/bgm.mp3", Loop::Yes)
{
	m_bgm.setVolume(0.2);
}


//アイテム自動配置
//難易度に応じて高さを変える
Array<Vec2> StageBase::generateItemPositions(Vec2 start, Vec2 goal, const StageDifficulty& diff)
{
	Array<Vec2> pos;
	double dx = (goal.x - start.x) / diff.itemCount;// X方向の間隔


	for (int i = 0; i < diff.itemCount; ++i)
	{
		// メロディに応じた高さ
		int step = diff.melodySteps[i % diff.melodySteps.size()];
		double height = step * diff.jumpHeightScale;

		Vec2 p{
			start.x + dx * i,// X位置
			start.y - height + Random(-diff.randomOffset, diff.randomOffset) // Y位置
		};

		// 高さ制限
		p.y = Clamp(p.y, 100.0, 680.0);
		pos << p;
	}
	return pos;
}


void StageBase::initStage()
{
	// UIボタン
	m_helpIcon = UIManager({ 1200,20 }, 55, Emoji{ U"❔" });
	m_backButton = UIManager({ 1180,20 }, 60, Emoji{ U"❌" });
	//m_audioIcon = UIManager({ 140,20 }, 55, Emoji{ U"🔊" });
	m_texCount = Texture{ Emoji{U"🤍"} }; // ハート所持数表示用Texture

	m_setumeiTexture = Texture{ U"Texture/setumei.png" };
	

	// マイク開始
	mike.start();
	// プレイヤーとゴール位置
	m_player = Player(world, playerStartPos());
	m_goalPos = goalPos();
	m_background = backgroundTexture();

	// 画像
	m_flowerTexture = Texture{ U"Texture/flower_n.png" };
	m_doorClosed = Texture{ U"Texture/Door1.png" };
	m_goal = Texture{ U"Texture/kirakira.png" };
	m_door = std::make_unique<DoorAnimation>(doorPos());// ゴール演出のドア

	//タイトルで選んだ楽器をセット
	m_keySound.setInstrument(getData().instrument);
	// アイテム配置
	m_itemPositions = generateItemPositions(playerStartPos(), m_goalPos, stageDifficulty());
	for (auto& pos : m_itemPositions)
		m_items << Item{ pos };
	//m_itemCount = (int)m_items.size();
	
	m_bgm.play();// BGM開始
}


//一番近いブロックを探す
Block* StageBase::findClosestBlock()
{
	Block* closest = nullptr;
	double nearest = 99999;

	for (auto& b : m_blocks)
	{
		double d = b.body().getPos().distanceFrom(m_player.getPos());
		if (d < nearest && d < 250)
		{
			nearest = d;
			closest = &b;
		}
	}
	return closest;
}


//花を咲かせる 
void StageBase::bloomFlower()
{
	if (auto b = findClosestBlock())
		b->bloomFlower();
}


//ブロック生成 
void StageBase::createBlockFromNote(int midiNote)
{
	int diff = midiNote - 60;
	if (diff < 0 || diff > 11) return;

	Vec2 base = m_player.getPos();

	// 12音階ごとのブロック出現位置
	static const Vec2 offset[12] =
	{
		{0, 50},   {70,75}, {60,50}, {-60,90}, {70,75}, {0,50},
		{0,60}, {20,90}, {80,70}, {0,70}, {55,70}, {0,90}
	};

	Vec2 spawn = base + offset[diff];
	Block& b = m_blocks.emplace_back(world, spawn, SizeF{ 50, 50 });

	// 音階ごとのブロックの動き（上昇・下降・横移動など）
	switch (diff)
	{
	case 0: b.riseTo(b.body().getPos().y - 80); break;
	case 1: b.riseTo(b.body().getPos().y - 120); break;
	case 2: b.moveXTo(b.body().getPos().x + 120); break;
	case 3: b.moveXTo(b.body().getPos().x - 150, 200); break;
	case 4: b.downTo(b.body().getPos().y + 100); break;
	case 5: break;
	case 6: b.riseTo(b.body().getPos().y - 300, 300); break;
	case 7: b.moveXTo(m_player.getPos().x + 90, 150); break;
	case 8: b.downTo(b.body().getPos().y + 70); break;
	case 9: b.riseTo(b.body().getPos().y - 250); break;
	case 10: b.downTo(b.body().getPos().y + 200); break;
	case 11: b.moveXTo(m_player.getPos().x - 100, 200); break;
	}
}


void StageBase::update()
{

	
	//  プレイヤーが歌っている判定
	bool singing = (mike.getVolume() > 0.007);

	// アイテム取得
	m_items.remove_if([&](const Item& item)
	{
		if (item.pos().distanceFrom(m_player.getPos()) < 80)
		{
			++m_itemCount;
			return true;
		}
		return false;
	});

	// 音に反応して花が咲く
	if (singing) bloomFlower();

	world.update();
	m_player.update(world, m_blocks, !mike.isRecording());
	
	m_camera.follow(m_player.getPos());

	//  キー入力でブロック生成
	if (auto note = m_keySound.update())
	{
		playedNotes << *note;
		if (m_itemCount > 0)
		{
			createBlockFromNote(*note);
			--m_itemCount;
		}

		// 敵追跡
		m_enemyChase = true;

		for (auto& e : m_enemies)
			e.startChase();
	}

	// ブロック更新
	for (auto& b : m_blocks) b.update(Scene::DeltaTime());

	// 落下でリスタート
	if (m_player.getPos().y > 1600)
	{
		m_player.setPos(playerStartPos());

		// 敵もリセット
		for (auto& en : m_enemies)
			en.resetPos();

		m_enemyChase = false;
	}


	// 説明画面表示
	if (isSetumei)
	{
		if (m_backButton.draw())
			isSetumei = false;
		return;
	}

	// 説明画面ボタン
	if (MouseL.down() && m_helpIcon.contains(Cursor::Pos()))
	{
		isSetumei = true;
		return;
	}

	//  Reset ボタン 
	if (m_resetRequested)
	{
		m_blocks.remove_if([](const Block& b)
		{
			return !b.isFixed(); // 固定じゃないブロックを消す
		});

		// アイテム復元
		m_items.clear();
		for (auto& pos : m_itemPositions)
		{
			m_items << Item{ pos };
		}
		m_itemCount = 3; // アイテム収集数リセット
		m_resetRequested = false;
	}

	//ゴールで曲演奏
	if (!m_goalSequence && (m_player.getPos().distanceFrom(m_goalPos) < 60))
	{
		m_goalSequence = true;

		// BGM を止める
		m_bgm.stop();

		// 取った音が無ければ演奏スキップして遷移
		if (playedNotes.isEmpty())
		{
			if (isFinalStage())// 最終ステージか
			{
				m_doorTriggered = true;
				changeTimer = 0.0;
			}
			else
			{
				changeScene(nextStageState());
				return;
			}
		}
		else
		{
			// 演奏開始
			m_playback = true;
			m_playIndex = 0;
			m_timer = 0.0;
		}
	}

	// 録音自動再生
	if (m_playback)
	{
		m_timer += Scene::DeltaTime();

		if (m_timer > 0.4)
		{
			m_timer = 0.0;

			if (m_playIndex < playedNotes.size())
			{
				m_keySound.playNote(playedNotes[m_playIndex]);
				++m_playIndex;
			}
			else
			{
				// 演奏完了
				m_playback = false;

				if (isFinalStage())
				{
					// ドア演出に移行
					m_doorTriggered = true;
					changeTimer = 0.0;
				}
				else
				{
					// 次ステージへ遷移
					changeScene(nextStageState());
					return;
				}
			}
		}

		

		

	}

	//最終ステージ ドア演出 
	if (m_doorTriggered)
	{
		changeTimer += Scene::DeltaTime();
		m_door->update(changeTimer);

		if (changeTimer > 2.0) m_playerVisible = false;
		if (changeTimer > 3.0) changeScene(State::Clear);
	}

	for (auto& e : m_enemies)//  敵追跡
	{
		e.update(m_player.getPos(), m_enemyChase);
	}

	
	// 当たり判定チェック
	for (auto& [pair, collision] : world.getCollisions())
	{
		// プレイヤーと敵の当たり判定
		for (auto& e : m_enemies)
		{
			if (!e.isBoss) // 
			{
				if ((pair.a == m_player.bodyID() && pair.b == e.bodyID())
				 || (pair.b == m_player.bodyID() && pair.a == e.bodyID()))
				{
					// プレイヤー初期位置へ
					m_player.setPos(playerStartPos());

					// 敵たちもリセット
					for (auto& en : m_enemies)
						en.resetPos();

					m_enemyChase = false;
				}
			}
		}
	}

	Vec2 playerPos = m_player.getPos();
	//ボスだけ距離判定
	for (auto& e : m_enemies)
	{
		
		if (!e.isBoss) continue;

		double d = playerPos.distanceFrom(e.pos);
		double distToGoal = e.getPos().distanceFrom(m_goalPos);
		if (distToGoal < 550)// ゴール近づきすぎたら戻す
		{
			e.resetPos();
		}
		if (d < e.radius + 20) 
		{
			m_player.setPos(playerStartPos());

			for (auto& en : m_enemies)
				en.resetPos();

			m_enemyChase = false;
			break;
		}
	}


}


void StageBase::draw() const
{
	//  背景スクロール 
	double scrollX = m_camera.pos().x * 0.2; // スクロール量
	scrollX = Fmod(scrollX, m_background.width()); // ループさせる

	// 背景2枚並べてループ
	m_background.draw(-scrollX, 0);
	m_background.draw(m_background.width() - scrollX, 0);

	

	

	// カメラ内描画
	{
		const Transformer2D _{ m_camera.transform() };

		for (auto& b : m_blocks)// 花が咲いたブロック
			if (b.hasFlower()) b.drawFlower(m_flowerTexture);

		// ブロック・アイテム
		for (auto& b : m_blocks) b.draw();
		for (auto& i : m_items) i.draw();

		m_goal.scaled(0.08).drawAt(m_goalPos);// ゴール

		/*if (!m_doorTriggered) m_doorClosed.drawAt(m_goalPos);
		else m_door->draw();*/

		// 最終ステージのドア演出
		if(m_doorTriggered)m_door->draw();

		if (m_playerVisible) m_player.draw();

		
		// 敵
		for (auto& e : m_enemies)
			e.draw();

	}

	
	//UI描画
	m_texCount.scaled(0.5).drawAt(1050, 50);
	m_itemfont(U"x {}"_fmt(m_itemCount)).drawAt(1130, 50, Palette::Yellow);
	m_helpIcon.draw();
	

	// 説明画面
	if (isSetumei)
	{
		m_setumeiTexture.drawAt(Scene::Width() / 2, Scene::Height() / 2);
		m_backButton.draw();
		return;
	}

	// Resetボタン
	if (SimpleGUI::Button(U"Reset", Vec2{ 50,40 }))
	{
		const_cast<StageBase*>(this)->m_resetRequested = true;
	}
}
