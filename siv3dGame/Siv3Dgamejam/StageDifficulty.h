#pragma once
#include <Siv3D.hpp>

// ステージ難易度設定構造体
struct StageDifficulty
{
	int itemCount;              // アイテム数
	double jumpHeightScale;     // 高さ変化の大きさ
	double randomOffset;        // ランダム揺らぎ
	Array<int> melodySteps;     // 音階シーケンス
};

// 難易度プリセット
namespace DifficultyPreset
{
	static StageDifficulty Easy{
		20,     // アイテム
		60.0,    // 高さ
		50.0,   // ランダム
		{ 0,2,4,5,7,9,7,5,4,2,0 } // ドレミファソラソファミレド
	};

	static StageDifficulty Normal{
		10,
		60.0,
		90.0,
		{ 0,2,4,7,5,4,2,0 } 
	};

	static StageDifficulty Hard{
		5,
		20.0,
		30.0,
		{ 0,4,7,5,9,7,4,2,0 }
	};

	/*static StageDifficulty Hell{
		4,
		30.0,
		40.0,
		{ 0,7,2,9,4,11,0 } 
	};*/
}

