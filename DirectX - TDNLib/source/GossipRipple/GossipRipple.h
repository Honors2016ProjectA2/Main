#pragma once
#include "TDNLIB.h"
#include "system\ItDebug.h"
#include "AI\Message\Message.h"

/*
	噂の波紋
*/

enum class RIPPLE_TYPE
{
	WHITE,	// 白
	RED,	// 赤
	BLUE,	// 青
};

class GossipRipple
{
public:
	GossipRipple(RIPPLE_TYPE type, Vector3 pos);
	~GossipRipple();

	void Action();
	void Update();
	void Render();

	// アクセサ
	bool IsAction() { return isAction; };
	bool IsEnd() { return isEnd; };

	Vector3 GetPos() { return m_Pos; }
	void SetPos(Vector3 pos) { m_Pos = pos; }

private:

	// 波紋の種類
	RIPPLE_TYPE m_Type;

	iexMesh* obj;	//	3DObj	
	Vector3 m_Pos;	// ポジション
	float m_Size;

	// フレーム
	int m_Flame;
	static const int MAX_FLAME = 180;	// 3秒後消し去る

	// スイッチ
	bool isAction;
	bool isEnd;

};
