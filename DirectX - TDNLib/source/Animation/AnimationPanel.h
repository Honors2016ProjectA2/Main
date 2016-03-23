#pragma once
#include "iextreme.h"

class AnimationPanel
{
public:
	AnimationPanel(char* _fileName, int _sizeX, int _sizeY, int _endCount, int _upflame, int _upCount, bool loop = false);
	~AnimationPanel();


	void Update();
	void Render(int x, int y, DWORD dwFlag = RS_COPY);

	void Action();
	bool GetisAction();

private:
	iex2DObj* m_image;

	int m_sizeX;
	int m_sizeY;

	// アニメ用
	int m_flame;
	int m_upflame;
	int m_count;
	int m_upCount;
	int m_endCount;


	// スイッチ
	bool isAction;

	// ループフラグ
	bool m_loop;
};
