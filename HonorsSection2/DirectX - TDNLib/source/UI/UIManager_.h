#pragma once
#include"TDNLIB.h"
#include "Number\Number.h"

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


private:
	static UIManager* inst;
	

	// ����
	UIManager();
	UIManager(const UIManager&){}
	UIManager& operator=(const UIManager&){}

private: //�ϐ�

	int m_score;
	int m_combo;

	Number* m_socrePic;
	Number* m_comboPic;


};


// �C���X�^���X��
#define UIMNG (UIManager::GetInstance())

