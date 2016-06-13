#pragma once
#include "TDNLIB.h"

/*********************/
//	SendPower
// �x�W�F�g���ē�������
/*********************/


class SendPower
{
public:
	SendPower(char* filename, Vector3 startPos, Vector3 centerPos,
		Vector3 center2Pos, Vector3 endPos, int endFlame, int power = 10, int bBokusou = false);

	~SendPower();

	void Update();
	void Bezier();
	void Move();
	void EndJuge();

	void Render();

	void Action();

	// �A�N�V����
	bool IsAction() { return m_bAction; }
	bool IsEnd() { return m_bEnd; };

	int GetPower() { return m_power; };

	bool IsBokusou() { return m_bBokusou; }

protected:
	tdn2DObj* m_pic;

	// �����̈ʒu
	Vector3  m_pos;

	// �n�_�E���_�E�I�_�Ƃ������ꏊ
	enum class MOVE_POS
	{
		START, CENTER, CENTER2, END, MAX
	};

	int m_flame;
	int m_endFlame;		//�@�I��̃t���[��
	
	float m_percentRate;// ���[�g
	float m_percent;

	bool m_bAction;
	bool m_bEnd;

	int m_power;

	bool m_bBokusou;
public:
	Vector3 m_movePos[(int)MOVE_POS::MAX];



};
