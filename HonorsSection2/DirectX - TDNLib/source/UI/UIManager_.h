#pragma once
#include"TDNLIB.h"
#include "Number\Number.h"

// UI

class UIManager
{
public:

	// いつもの
	static UIManager& GetInstance();
	static void Release();
	virtual ~UIManager();
	
	// 更新・描画7
	void Update();
	void Render();	


private:
	static UIManager* inst;
	

	// 封印
	UIManager();
	UIManager(const UIManager&){}
	UIManager& operator=(const UIManager&){}

private: //変数

	int m_score;
	int m_combo;

	Number* m_socrePic;
	Number* m_comboPic;


};


// インスタンス化
#define UIMNG (UIManager::GetInstance())

