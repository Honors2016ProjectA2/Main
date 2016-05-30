#pragma once
#include"TDNLIB.h"
#include "Number\Number.h"
#include "pie_graph\pie_graph.h"

#include "Effect\BaseEffect\BaseEffect.h"

// UI

class UIManager
{
public:

	// ������
	static UIManager& GetInstance();
	static void Release();
	virtual ~UIManager();

	// �X�V�E�`��7
	void Update();
	void Render();
		
	// �㏸
	void AddScore(int score);
	void ConboCount();

	// �O���t�̐ݒ�
	void SetGraph(float graph) {
		m_percent = graph;
	}
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
	tdn2DObj* m_timerPic;
	//float timer;

	// ���Q�[�W
	int m_graphX, m_graphY;
	tdn2DObj* m_graphBack;
	tdn2DObj* m_graphFlont;
	Pie_graph* m_graph;
	tdn2DAnim* m_graphRip;
	float m_percent;


};


// �C���X�^���X��
#define UIMNG (UIManager::GetInstance())

