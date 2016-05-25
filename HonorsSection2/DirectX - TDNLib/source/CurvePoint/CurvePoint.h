#pragma once

//--------- CurvePointLock class -----------
//--------- CurvePoint class ---------
class CurvePoint
{
public:
	enum CurvePointState { OPEN, CLOSE };
	//--------- field ---------
private:
	tdn2DObj* image;
	const int W;
	const int H;
	const int SW;
	const int SH;
	Vector2 pos;
	CurvePointState state;
	int m_AnimFrame, m_AnimPanel;
	DIR m_dir;	// �r���Ȃ������
public:
	const int LOCK_TIME;
	int lockTimer;

	//------- method ---------
public:
	CurvePoint(tdn2DObj* dogImage, Vector2 pos, DIR dir);
	~CurvePoint();
	void Update();
	void Render();
	void SetPos(int y);
	void Change(){ state = IsOpening() ? CurvePointState::CLOSE : CurvePointState::OPEN; }		// ON_OFF�̐؂�ւ�
	bool IsOpening();
	int GetWidth();
	DIR GetDir(){ return m_dir; }		// �J�[�u�̕����擾
	Vector2 &GetPos(){ return pos; }
	void Get_pos2(Vector2 &out){ out = pos; }

	bool m_bCursorIn;	// �}�E�X�J�[�\�����͈͓��ɓ����Ă邩�ǂ���
};