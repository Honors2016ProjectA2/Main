#pragma once
#include "TDNLIB.h"

/*********************/
//	SendPower
// ベジェ使って動かすよ
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

	// アクション
	bool IsAction() { return m_bAction; }
	bool IsEnd() { return m_bEnd; };

	int GetPower() { return m_power; };

	bool IsBokusou() { return m_bBokusou; }

protected:
	tdn2DObj* m_pic;

	// 自分の位置
	Vector3  m_pos;

	// 始点・中点・終点とか動く場所
	enum class MOVE_POS
	{
		START, CENTER, CENTER2, END, MAX
	};

	int m_flame;
	int m_endFlame;		//　終りのフレーム
	
	float m_percentRate;// レート
	float m_percent;

	bool m_bAction;
	bool m_bEnd;

	int m_power;

	bool m_bBokusou;
public:
	Vector3 m_movePos[(int)MOVE_POS::MAX];



};
