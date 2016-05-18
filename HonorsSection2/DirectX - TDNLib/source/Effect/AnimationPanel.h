#pragma once
#include "tdnlib.h"

class AnimationPanel
{
public:
	AnimationPanel(char* _fileName, int _sizeX, int _sizeY, int _endCount, int _upflame, int _upCount, bool loop = false);
	~AnimationPanel();


	void Update();
	void Render(int x, int y, DWORD dwFlag = RS::COPY);

	void Action();
	bool GetisAction();

private:
	tdn2DObj* m_image;

	int m_sizeX;
	int m_sizeY;

	// �A�j���p
	int m_flame;
	int m_upflame;
	int m_count;
	int m_upCount;
	int m_endCount;


	// �X�C�b�`
	bool isAction;

	// ���[�v�t���O
	bool m_loop;
};