#pragma once
#include"TDNLIB.h"
#include "Number\Number.h"
#include "pie_graph\pie_graph.h"

#include "Effect\BaseEffect\BaseEffect.h"
#include "Number\Number.h"

// ResultUI
class ResultUIManager
{
public:
	// �X�e�b�v(�����̗���)
	enum class STEP
	{
		START,EXE,SUM_CALC,RANK,RANKING_START, RANKING,END
	};
	STEP m_step;


	// ������
	static ResultUIManager& GetInstance();
	static void Release();
	virtual ~ResultUIManager();

	// �X�V�E�`��
	void Update();
	void Render();
	void ResultRender();
	void CircleRender();

	// �u���C���h
	void MaskRender();
	tdn2DObj* m_blindPic;
	int m_blindSplit;
	float m_blindAngle;
	tdn2DObj* m_maskScreen;
	float m_blindRete;

	// �n���E�I��
	void Action();
	void Stop();
	void StopUpdate();
	int m_screenAlpha;
	bool isStopFlag;

	void Debug();

	// �Q�b�^�[
	bool IsResultScreen() { return isResultScreen; }


	// �T�[�N���\����
	struct Circle
	{
		float scale;
		tdn2DObj* pic;
		int x, y;
		int flame;

		int startX, startY;
		int endX, endY;
		int endFlame;
	};
	Circle m_invCircle;
	Circle m_circle;
	bool CircleUpdate();

	// �t�H���g�B
	enum FONT
	{
		RESULT, SCORE, COMBO, WORF, FIRE, SUM, END

	};

	struct Font
	{
		tdn2DAnim* pic;
		int x, y;

	};
	Font m_font[END];

	// ���̑�
	tdn2DAnim* m_senPic;// ��
	tdn2DAnim* m_helpPic;// �w���v

	//�@�����B
	Number* m_number[FONT::END];
	int m_numX[FONT::END], m_numY[FONT::END];
	int m_numNum[FONT::END];
	int m_MaxSumNum;
	
	int m_WaitFlame;// ��΂�

	// �����N����
	enum RANK_TYPE
	{
		S,A,B,C
	};
	int m_rankType;
	tdn2DAnim* m_RankPic;
	tdn2DAnim* m_RankRipPic;// �g��G�t�F�N�g
	int m_RankPicX, m_RankPicY;

	void RankUpdate();

private:
	static ResultUIManager* inst;

	// ����
	ResultUIManager();
	ResultUIManager(const ResultUIManager&) {}
	ResultUIManager& operator=(const ResultUIManager&) {}
private: //�ϐ�

	// �A���t�@�l���̃����_�[�^�[�Q�b�g
	tdn2DObj* m_resultScreen;
	tdn2DObj* m_circleScreen;
	tdn2DObj* m_screen;
	Surface* pBackBuffer;

	bool isResultScreen;

	// �����L���O��
	tdn2DObj* m_rankingScreen;
	tdn2DObj* m_rankingBG;
	tdn2DObj* m_rankingFont;

	void RankingRender();
};


// �C���X�^���X��
#define RESULT_UIMNG (ResultUIManager::GetInstance())
