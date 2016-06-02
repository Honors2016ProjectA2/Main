#pragma once

class DebuBase
{
public:
	DebuBase(tdn2DObj *image, const Vector2 &pos, float radius = 120);
	virtual ~DebuBase();

	// 更新・描画
	virtual void Update();
	virtual void Render();

	// ゲッター
	Vector2 &GetCenterPos(){ return m_pos + Vector2(96, 96); }
	int GetWidth(){ return m_image->GetWidth(); }
	int GetFloor(){ return m_floor; }
	bool EraseOK(){ return m_bErase; }
	float GetRadius(){ return m_radius; }


	// セッター
	void SetAccel(float val){ m_accel = val; }
	void SetFloor(int val){ m_floor = val; }
	void Erase(){ m_bErase = true; }

protected:
	int m_AnimPanel, m_AnimFrame;	// アニメ
	Vector2 m_pos;		// 座標
	float m_moveX;		// 移動値
	tdn2DObj *m_image;	// 自分の画像
	float m_angle;		// 転がるので、アングル。
	int m_floor;		// 自分のいるレーン
	bool m_bErase;		// 消去フラグ
	int m_ReceiveSE;	// 鳴らしたSEのID
	float m_accel;
	float m_radius;		// 半径
};