#pragma once
#include <Siv3D.hpp>
#include "Player.h"
#include "Block.h"
#include "Item.h"
#include "Camera.h"
#include "KeySound.h"
//#include "MikeAudio.h"
#include "DoorAnimation.h"
#include "StageDifficulty.h"
#include "UIManager.h"
#include "Enemy.h"
#include "Common.h"

//ステージのベースになる
class StageBase : public App::Scene
{
public:
	StageBase(const InitData& init);

	void update() override;
	void draw() const override;

protected:
	// ステージごとに変える部分
	virtual void setupStage() = 0;// ステージの初期化
	virtual const StageDifficulty& stageDifficulty() const = 0;// ステージ固有の難易度設定
	virtual Vec2 playerStartPos() const = 0;// プレイヤーの初期位置
	virtual Vec2 goalPos() const = 0;// ゴール位置
	virtual Texture backgroundTexture() const = 0;// 背景画像
	virtual bool isFinalStage() const { return false; }// 最終ステージかどうか
	virtual State nextStageState() const { return State::Clear; }// 次のステージの状態
	
	virtual void drawOverlay() const {}// ステージ特有の描画
	
	virtual Vec2 doorPos() const { return goalPos(); }// ゴールのドア位置

	// ステージ開始時に呼ぶ
	void initStage();

	//  アイテム位置生成
	Array<Vec2> generateItemPositions(Vec2 start, Vec2 goal,
	const StageDifficulty& diff);

	//ブロック を生成
	void createBlockFromNote(int midiNote);

	//もっとも近いブロックを探して花を咲かせる
	void bloomFlower();
	Block* findClosestBlock();

	// 敵関連
	Array<Enemy> m_enemies;
	bool m_enemyChase = false;

	//  音量関係
	bool m_showVolumeMenu = false;
	double m_bgmVolume = 0.5;
	double m_keyVolume = 0.5;


protected:
	//共通変数
	P2World world{ Vec2{0, 980} };

	//Player m_player{ world, Vec2{0, 0} };
	Player m_player;

	Array<Block> m_blocks;
	Array<Item> m_items;

	KeySound m_keySound;
	//MikeAudio mike;
	Camera m_camera{ 4000, 720 };

	// ゴール関係
	Texture m_goal;
	Vec2 m_goalPos{ 0,0 };

	Texture m_background;

	Texture m_flowerTexture;
	Texture m_doorClosed;// ドア閉状態

	
	std::unique_ptr<DoorAnimation> m_door;

	Array<Vec2> m_itemPositions;// 自動配置されたアイテムの位置
	Array<int> playedNotes;

	int m_itemCount = 3;// アイテム数
	bool m_playerVisible = true;
	bool m_doorTriggered = false;// ゴールドアが開いたか
	double changeTimer = 0;// クリア時のシーンチェンジタイマー
	bool m_resetRequested = false;// ブロックとアイテムのリセット要求

	 Audio m_bgm;

	 // UI 関係（ヘルプ、戻るボタンなど）
	UIManager m_helpIcon;
	UIManager m_backButton;

	Texture m_setumeiTexture;// 説明画像
	//Texture m_count;
	Font m_itemfont{ 40 };
	bool isSetumei = false;

	UIManager m_audioIcon;
	//Texture m_texAudio;
	bool m_goalSequence = false;     // ゴール演出を開始したか
	bool m_playback = false;//録音再生中か
	size_t m_playIndex = 0;          
	double m_timer = 0.0;// 録音再生用タイマー

	/*Emoji m_texHelp;
	Emoji m_texBack;
	Emoji m_texAudio;
	Emoji m_count;
	Emoji m_texHeart;*/
	/*Texture texHelp;
	Texture texBack;
	Texture texAudio;
	Texture texHeart;*/
	Font font{ 18 };

	Texture m_texCount;

	bool m_sing = false;
};
