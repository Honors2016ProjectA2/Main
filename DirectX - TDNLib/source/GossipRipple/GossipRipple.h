#pragma once
#include "TDNLIB.h"
#include "system\ItDebug.h"
#include "AI\Message\Message.h"
#include "Animation\AnimationUV.h"	// UVanim
/*
	�\�̔g��
*/

enum class RIPPLE_TYPE
{
	WHITE,	// ��
	RED,	// ��
	BLUE,	// ��
};

class GossipRipple
{
public:
	GossipRipple(RIPPLE_TYPE type, Vector3 pos);
	~GossipRipple();

	void Action();
	void Update();
	void Render();

	// �A�N�Z�T
	//bool IsAction() { return isAction; };
	//bool IsEnd() { return isEnd; };

	bool IsAction() { return m_uvObj->IsAction(); };
	bool IsEnd() { return m_uvObj->IsEnd();; };

	Vector3 GetPos() { return m_Pos; }
	void SetPos(Vector3 pos) { m_Pos = pos; }

private:

	// �g��̎��
	RIPPLE_TYPE m_Type;

	AnimationUV* m_uvObj;	//	3DObj	
	Vector3 m_Pos;	// �|�W�V����
	float m_Size;

	//// �t���[��
	//int m_Flame;
	//static const int MAX_FLAME = 60;	// 3�b���������

	//// �X�C�b�`
	//bool isAction;
	//bool isEnd;

};
