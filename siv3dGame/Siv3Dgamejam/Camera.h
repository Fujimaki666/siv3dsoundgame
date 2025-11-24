#pragma once
#include <Siv3D.hpp>

class Camera
{
public:
	Camera(double width, double height)
		: m_sceneSize{ width, height } {
	}

	//  カメラをプレイヤーに追従させる
	void follow(const Vec2& targetPos)
	{
		// プレイヤー中心にカメラを合わせる
		double targetX = targetPos.x - Scene::Width() / 2;
		double targetY = targetPos.y - Scene::Height() / 2;

		// シーン範囲を超えないように制限
		targetX = Clamp(targetX, 0.0, m_sceneSize.x - Scene::Width());
		targetY = Clamp(targetY, 0.0, m_sceneSize.y - Scene::Height());

		// スムーズに追従
		m_cameraPos = Math::Lerp(m_cameraPos, Vec2{ targetX, targetY }, 0.1);
	}

	//このカメラを有効にする
	Mat3x2 transform() const { return Mat3x2::Translate(-m_cameraPos); }

	// 現在のカメラ位置を取得
	Vec2 pos() const { return m_cameraPos; }

private:
	Vec2 m_cameraPos{ 0, 0 };
	Vec2 m_sceneSize; // ステージ全体のサイズ
	//mutable Transformer2D m_transform; // 描画スコープ用
};
#pragma once
