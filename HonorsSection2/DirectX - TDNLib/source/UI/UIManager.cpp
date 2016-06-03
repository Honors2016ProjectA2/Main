#include "UIManager.h"
#include "particle_2d\particle_2d.h"

//Particle_2d* particle;

UIManager* UIManager::inst = nullptr;


UIManager & UIManager::GetInstance()
{
	if (!inst)
	{
		inst = new UIManager();
	}
	// TODO: return ステートメントをここに挿入します
	return *inst;
}

void UIManager::Release()
{
	SAFE_DELETE(inst);
}

/***************************/
//	初期化・解放
/***************************/
UIManager::UIManager()
{
	
	// スコア
	m_score = 0;
	m_scorePoint = 0;
	m_scoreNum = new Number();
	m_scorePic = new tdn2DObj("DATA/UI/score.png");

	// コンボ
	m_combo = 0;
	m_comboNum = new Number("DATA/Number/Number2.png", 64, Number::NUM_KIND::COMBO);
	m_conboBG = new tdn2DObj("DATA/UI/UIcombo.png");
	//m_conboBG->OrderMoveAppeared()
	m_conboGage = new tdn2DObj("DATA/UI/UIgage.png");
	m_conboGageEnergy= new tdn2DObj("DATA/UI/UIenergy.png");
	m_energyRate = 1.0f;

	// タイマー
	m_timerNum = new Number("DATA/Number/Number.png", 64);
	m_timerPic = new tdn2DObj("DATA/Number/Number.png");
	m_timer = 60 * 2;
	//m_timer = 0;
	m_flame = 0;

	// 草ゲージ
	m_graphX = 10, m_graphY = 0;
	m_graphBack = new tdn2DObj("DATA/UI/Circle/SpiritCircle_UNDER.png");
	m_graphFlont= new tdn2DObj("DATA/UI/Circle/SpiritCircle_ON.png");
	m_graph = new Pie_graph("DATA/UI/Circle/SpiritCircle_gage.png");
	m_graphRip = new tdn2DAnim("DATA/UI/Circle/SpiritCircle_FULL.png");
	m_graphRip->OrderRipple(12, 1.0f, 0.1f);
	m_percent = 0.0f;

	// 
	//particle = new Particle_2d();
	//particle->Initialize("DATA/Effect/star.png", 256, 1, 1);

	NumberEffect;
}

UIManager::~UIManager()
{
	SAFE_DELETE(m_scoreNum);
	SAFE_DELETE(m_scorePic);
	SAFE_DELETE(m_comboNum);
	SAFE_DELETE(m_conboBG);
	SAFE_DELETE(m_conboGage);
	SAFE_DELETE(m_conboGageEnergy);
	SAFE_DELETE(m_timerNum);
	SAFE_DELETE(m_timerPic);
	SAFE_DELETE(m_graph);
	SAFE_DELETE(m_graphBack);
	SAFE_DELETE(m_graphFlont);
	SAFE_DELETE(m_graphRip);

	NumberEffect.Release();

}

/***************************/
//	更新・描画
/***************************/
void UIManager::Update()
{
	m_scoreNum->Update();
	//m_conboBG->Update();
	m_comboNum->Update();

	// 色々な処理
	ScoreUpdate();
	ConboUpdate();
	TimerUpdate();
	PointUpdate();
	GraphUpdate();
	NumberEffect.Update();

	//仮
	Debug();


	//particle->Set(
	//	1, 0, 0xFFffffff,
	//	128, 0x00ffffff,
	//	0, 0xFFffffff,
	//	300.0f, 300.0f,
	//	1.0f, -3.0f,
	//	0.0f, 0.1f ,
	//	0.0f,1.0f, 32.0f, RS::ADD);

	//particle->Update();


}
void UIManager::Render()
{
	
	/****************************************/
	// スコア
	ScoreRender();

	/****************************************/
	// コンボ
	ConboRender();

	/****************************************/
	// タイマー
	TimerRender();
	
	/****************************************/
	// ポイント
	PointRender();

	/****************************************/
	// グラフゲージ
	GraphRender();

	//particle->Render();

}




/***********************/
//	スコア系
/***********************/
void UIManager::ScoreUpdate()
{
	// テレレレレンって感じに
	if (m_scorePoint < m_score)
	{
		int len = abs(m_scorePoint - m_score);
		if (len <= 10)
		{
			m_scorePoint += 1;
		}
		else if (len <= 100)
		{
			m_scorePoint += 11;
		}
		else if (len <= 1000)
		{
			m_scorePoint += 111;
		}
		else if (len <= 10000)
		{
			m_scorePoint += 1111;
		}
		else 
		{
			m_scorePoint += 11111;
		}

		
	}
	else
	{
		m_scorePoint = m_score;
	}
	

}

void UIManager::ScoreRender()
{
	m_scoreNum->Render(300, 10, m_scorePoint);
	m_scorePic->Render(350, 20);
}

void UIManager::AddScore(int score)
{
	m_score += score;
	//m_scoreNum->Action();
}

/***********************/
//	コンボ系
/***********************/
void UIManager::ConboUpdate()
{
	// エネルギー
	m_energyRate -= 0.03f;

}

void UIManager::ConboRender()
{
	m_conboBG->Render(1000, 0);
	m_conboGage->Render(1000, 90);	// ゲージ	  
									//m_conboGageEnergy->SetTurnOver(true);
									//m_conboGageEnergy->SetAngle(1.57);
	m_conboGageEnergy->Render((1000+128)-(int)(128 * m_energyRate), 90, (int)(128 * m_energyRate) , 128, 0, 0, 128, 128);  // ゲージエネルギー
	m_comboNum->Render(1080, 30, m_combo);// 数値

}

void UIManager::ConboCount()
{
	m_energyRate = 1.0f;
	m_comboNum->Action();
	m_combo++;
}

// 時間上昇！
void UIManager::AddTimer(int timer)
{
	m_timer += timer;
	NumberEffect.AddNumber(780, 40, timer, Number_Effect::COLOR_TYPE::YELLOW_GREEN);
}

/***********************/
//	タイマー系
/***********************/
void UIManager::TimerUpdate()
{
	
	// カウントダウン
	m_flame++;
	if (m_flame >= 60)
	{
		m_flame = 0;
		m_timer--;	
	}

}

void UIManager::TimerRender()
{
	const int second = m_timer % 60,
		minutes = m_timer / 60;
	const int TimerX = 600;
	m_timerPic->Render(TimerX, 16, 64, 64, minutes * 64, 0, 64, 64);			// 分
	m_timerPic->Render(TimerX + 36, 16, 64, 64, 13 * 64, 0, 64, 64);			// :
	m_timerPic->Render(TimerX + 72, 16, 64, 64, second / 10 * 64, 0, 64, 64);	// 秒(10の位)
	m_timerPic->Render(TimerX + 108, 16, 64, 64, second % 10 * 64, 0, 64, 64);	// 秒(1の位)
																				//m_timerNum->Render(600, 10, m_timer);

}

/***********************/
//	ポイント系
/***********************/
void UIManager::PointUpdate()
{
	
}

void UIManager::PointRender()
{
	NumberEffect.Render();
}


/***********************/
//	グラフ系
/***********************/
void UIManager::GraphUpdate()
{	
	// %
	m_graph->Set_percent(m_percent);

	// Rip
	m_graphRip->Update();
}

void UIManager::GraphRender()
{
	m_graphBack->Render(m_graphX, m_graphY);
	m_graph->Render(m_graphX, m_graphY, 128, 128, 0, 0, 128, 128, RS::COPY, 0xFFFFFFFF, .0f);
	m_graphFlont->Render(m_graphX, m_graphY);
	m_graphRip->Render(m_graphX, m_graphY, RS::ADD);

}

// 
void UIManager::GraphAction()
{
	m_graphRip->Action();
	m_percent = 0.0f;
}


void UIManager::Debug()
{
	if (KeyBoard(KB_R) == 3)
	{
		AddScore(1200);
	}

	if (KeyBoard(KB_E) == 3)
	{
		ConboCount();
	}

	if (KeyBoard(KB_T) == 3)
	{		
		NumberEffect.AddNumber(1200, 360, 1000);
	}

	if (KeyBoard(KB_Y))
	{
		m_percent += 0.01f;
	}
	if (KeyBoard(KB_U)==3)
	{
		m_graphRip->Action();
		m_percent = 0.0f;
	}
}



