#pragma once
#include <Siv3D.hpp>

// キーボード入力に応じて楽器音を鳴らす
class KeySound
{
public:
	// 楽器を変えたら全てのキーの音源を作り直す
	void setInstrument(GMInstrument inst)
	{
		instrument = inst;
		// 音を作り直す
		for (int32 i = 0; i < NumKeys; ++i)
		{
			sounds[i] = Audio{ instrument, static_cast<uint8>(PianoKey::C4 + i), // C4 から 20 音分
				0.5s }; // ノートオン時間
		}
	}

	// キーボード入力をチェックし、
   // 押されたキーに対応する音を鳴らす
	Optional<int> update()
	{
		for (int32 i = 0; i < NumKeys; ++i)
		{
			if (Keys[i].down())
			{
				sounds[i].playOneShot();
				return (int)PianoKey::C4 + i;
			}
		}
		return none;
	}
	void playNote(int midiNote)
	{
		int index = midiNote - (int)PianoKey::C4;
		if (InRange(index, 0, NumKeys - 1))
		{
			sounds[index].playOneShot();
		}
	}

	
private:
	//  使用する楽器（デフォルトはピアノ）
	//static constexpr GMInstrument Instrument = GMInstrument::Piano1;
	GMInstrument instrument = GMInstrument::Piano1;
	// 鍵盤の数（20音分）
	static constexpr int32 NumKeys = 20;

	// 音階ごとの Audio オブジェクト
	std::array<Audio, NumKeys> sounds;

	
	//  対応するキーと音階
	const std::array<Input, NumKeys> Keys =
	{
		
		//KeyF,  // ド（C4）
		//KeyG,  // レ（D4）
		//KeyH,  // ミ（E4）
		//KeyJ,  // ファ（F4）
		//KeyK,  // ソ（G4）
		//KeyL,  // ラ（A4）
		//KeyP, // シ（B4）
		//KeyZ,  // ド'（C5）
		//KeyX,  // レ'（D5）
		//KeyC,  // ミ'（E5）
		//KeyV,  // ファ'（F5）
		//KeyB,  // ソ'（G5）
		//KeyN,  // ラ'（A5）
		//KeyM,  // シ'（B5）
		//KeyQ,  // ド''（C6）
		//KeyW,  // レ''（D6）
		//KeyE,  // ミ''（E6）
		//KeyR,  // ファ''（F6）
		//KeyT,  // ソ''（G6）
		//KeyY,  // ラ''（A6）
		 KeyZ, // C
	KeyS, // C#
	KeyX, // D
	KeyD, // D#
	KeyC, // E
	KeyV, // F
	KeyG, // F#
	KeyB, // G
	KeyH, // G#
	KeyN, // A
	KeyJ, // A#
	KeyM  // B
	};
};
