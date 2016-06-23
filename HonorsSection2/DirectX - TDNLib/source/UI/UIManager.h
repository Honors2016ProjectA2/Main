#pragma once
#include"TDNLIB.h"
#include "Number\Number.h"
#include "pie_graph\pie_graph.h"

#include "Effect\BaseEffect\BaseEffect.h"
#include "UI\SendPower\SendPower.h"


// UI

class UIManager
{
public:

	// ������
	static UIManager& GetInstance();
	static void Release();
	virtual ~UIManager();

	void Init();

	// �X�V�E�`��7
	void Update();
	void Render();
		
	// �㏸
	void AddScore(int score);
	void ConboCount();

	// ���ԏ㏸
	void AddTimer(int timer);
	void AddFlame(int flame);

	// ������
	void AddSendPower(char* filename, Vector3 startPos, Vector3 centerPos,
		Vector3 center2Pos, Vector3 endPos, int endFlame, int power, bool bokusouFlag = false);

	// �O���t�̐ݒ�
	void SetGraph(float graph) {
		m_percent = graph;
	}

	// �^�C�}�[�̃Q�b�^�[
	int GetTimer() { return m_timer; }
	void SetTimer(int timer) { m_timer = timer; }

	int GetFlame() { return m_flame; }

	// 
	int GetWorfHappyCount() { return m_worfHappyCount; }
	void AddWorfHappyCount(int add = 1) { m_worfHappyCount += add; }
	int *GetWolfHappyCountAddress(){ return &m_worfHappyCount; }

	int GetSheepKillCount() { return m_sheepKillCount; }
	void AddSheepKillCount(int add = 1) { m_sheepKillCount += add; }
	int *GetSheepKillCountAddress(){ return &m_sheepKillCount; }

	int GetScore() { return m_score; }
	int *GetScoreAddress(){ return &m_score; }
	int GetCombo() { return m_combo; }
	int GetMaxCombo() { return m_maxCombo; }

	void GraphAction();
private:
	static UIManager* inst;

	void ScoreUpdate();
	void ScoreRender();
	void ConboUpdate();
	void ConboRender();
	void TimerUpdate();
	void TimerRender();
	void PointUpdate();
	void PointRender();
	void GraphUpdate();
	void GraphRender();


	void Debug();


	// ����
	UIManager();
	UIManager(const UIManager&) {}
	UIManager& operator=(const UIManager&) {}
private: //�ϐ�

	int m_score;
	int m_scorePoint;

	int m_combo;
	int m_maxCombo;

	// �X�R�A
	Number* m_scoreNum;
	tdn2DObj* m_scorePic;

	// �R���{
	Number* m_comboNum;
	tdn2DObj* m_conboBG;
	tdn2DObj* m_conboGage;
	tdn2DObj* m_conboGageEnergy;
	float m_energyRate;
	

	// �^�C�}�[
	int m_timer;
	int m_flame;
	Number* m_timerNum;
	Number* m_flameNum;
	tdn2DObj* m_flamePic;
	tdn2DObj* m_timerPic;
	tdn2DObj* m_tttPic;
	float m_timeColR, m_timeColG, m_timeColB;
	//float timer;

	// ���Q�[�W
	int m_graphX, m_graphY;
	tdn2DObj* m_graphBack;
	tdn2DObj* m_graphFlont;
	Pie_graph* m_graph;
	tdn2DAnim* m_graphRip;
	float m_percent;

	// 
	int m_worfHappyCount;
	int m_sheepKillCount;

	tdn2DObj* m_UIBoad;

	/****************/
	// �x�W�F�̌�
	/****************/
	//SendPower;
	// �����̃f�[�^
	std::list<SendPower*> m_SendPowerData;
	std::list<SendPower*>::iterator m_SendPowerIt;

	void SendPowerUpdate();
	void SendPowerRender();

};


// �C���X�^���X��
#define UIMNG (UIManager::GetInstance())

