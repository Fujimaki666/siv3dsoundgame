#pragma once
#include <Siv3D.hpp>

// マイク入力を扱うクラス
class MikeAudio
{
public:
	//マイクを起動
	MikeAudio()
		: mic(StartImmediately::No){ }

	// 録音中かどうか
	bool isRecording() const
	{
		return mic.isRecording();
	}

	// マイクの録音開始
	void start()
	{
		if (!mic.isRecording())
		{
			mic.start(); 
		}
	}

	
	void update()
	{
		
		if (mic.isRecording()) // 録音中になったら
		{
			mic.fft(fft);

			//  音量の計算
			double sum = 0.0;
			for (auto& f : fft.buffer) sum += f;

			// 生の音量値
			volume = sum / fft.buffer.size();
			const double volumeThreshold = 0.04;
			if (volume < volumeThreshold)
			{
				pitchHz = 0.0;
				return;
			}

			//  ピーク周波数（最も強い成分）を求める
			pitchHz = getPitchHz(fft);
			if (pitchHz < 80.0 || pitchHz > 1200.0)
			{
				pitchHz = 0.0;
			}
		}
		
	}

	//  音量を返す
	double getVolume() const { return Clamp(volume * 30.0, 0.0, 1.0); }

	// 周波数(Hz)を返す
	double getPitchHz() const { return pitchHz; }

	//  音名を返す
	String getNoteNameStr() const { return noteName; }

private:
	Microphone mic;
	FFTResult fft; // FFT解析結果
	double volume = 0.0;
	double pitchHz = 0.0;
	String noteName = U"-";
	bool micStarted = false;
	//  FFTデータからピーク周波数を求める
	static double getPitchHz(const FFTResult& fft)
	{
		double maxAmp = 0.0;// 最大振幅
		int peakIndex = 0;// 周波数のインデックス

		for (int i = 0; i < fft.buffer.size(); ++i)
		{
			if (fft.buffer[i] > maxAmp)
			{
				maxAmp = fft.buffer[i];
				peakIndex = i;
			}
		}

		return peakIndex * fft.resolution;
	}

	///// 周波数から音名を推定
	//static String getNoteName(double freq)
	//{
	//	if (freq <= 0.0) return U"-";

	//	static const Array<String> notes = {
	//		U"ラ", U"ラ#", U"シ", U"ド", U"ド#", U"レ",
	//		U"レ#", U"ミ", U"ファ", U"ファ#", U"ソ", U"ソ#"
	//	};

	//	double n = 12.0 * Math::Log2(freq / 440.0); // A4=440Hz基準
	//	int noteIndex = static_cast<int>(Math::Round(n)) % 12;
	//	if (noteIndex < 0) noteIndex += 12;

	//	return notes[noteIndex];
	//}
};
#pragma once
