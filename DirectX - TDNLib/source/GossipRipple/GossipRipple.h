#pragma once
#include "TDNLIB.h"
#include "system\ItDebug.h"
#include "AI\Message\Message.h"

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
	bool IsAction() { return isAction; };
	bool IsEnd() { return isEnd; };

	Vector3 GetPos() { return m_Pos; }
	void SetPos(Vector3 pos) { m_Pos = pos; }

private:

	// �g��̎��
	RIPPLE_TYPE m_Type;

	iexMesh* obj;	//	3DObj	
	Vector3 m_Pos;	// �|�W�V����
	float m_Size;

	// �t���[��
	int m_Flame;
	static const int MAX_FLAME = 180;	// 3�b���������

	// �X�C�b�`
	bool isAction;
	bool isEnd;

};
