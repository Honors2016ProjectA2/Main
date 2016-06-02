#pragma once

class DebuBase
{
public:
	DebuBase(tdn2DObj *image, const Vector2 &pos, float radius = 120);
	virtual ~DebuBase();

	// �X�V�E�`��
	virtual void Update();
	virtual void Render();

	// �Q�b�^�[
	Vector2 &GetCenterPos(){ return m_pos + Vector2(96, 96); }
	int GetWidth(){ return m_image->GetWidth(); }
	int GetFloor(){ return m_floor; }
	bool EraseOK(){ return m_bErase; }
	float GetRadius(){ return m_radius; }


	// �Z�b�^�[
	void SetAccel(float val){ m_accel = val; }
	void SetFloor(int val){ m_floor = val; }
	void Erase(){ m_bErase = true; }

protected:
	int m_AnimPanel, m_AnimFrame;	// �A�j��
	Vector2 m_pos;		// ���W
	float m_moveX;		// �ړ��l
	tdn2DObj *m_image;	// �����̉摜
	float m_angle;		// �]����̂ŁA�A���O���B
	int m_floor;		// �����̂��郌�[��
	bool m_bErase;		// �����t���O
	int m_ReceiveSE;	// �炵��SE��ID
	float m_accel;
	float m_radius;		// ���a
};