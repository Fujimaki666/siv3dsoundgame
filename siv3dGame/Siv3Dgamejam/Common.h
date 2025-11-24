# pragma once
# include <Siv3D.hpp>

// シーンのステート
enum class State
{
	Title,
	Game,
	Stage1,
	Stage2,
	StageFinal,
	Clear
	
};

//enum class BlockType {
//	High,    // 通常
//	Jump,      // ジャンプ強化
//	Speed      // 加速・滑る
//};

// 共有するデータ
struct GameData
{
	GMInstrument instrument = GMInstrument::Piano1;
};

using App = SceneManager<State, GameData>;
#pragma once
