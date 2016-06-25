#include "UIManager.h"
#include "particle_2d\particle_2d.h"
#include "Effect\EffectManager.h"
#include "Bokusou\Bokusou.h"
#include "../Sound/SoundManager.h"

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
	m_maxCombo = 0;

	m_comboNum = new Number("DATA/Number/Number2.png", 64, Number::NUM_KIND::COMBO);
	m_conboBG = new tdn2DObj("DATA/UI/UIcombo.png");
	//m_conboBG->OrderMoveAppeared()
	m_conboGage = new tdn2DObj("DATA/UI/UIgage.png");
	m_conboGageEnergy= new tdn2DObj("DATA/UI/UIenergy.png");
	m_energyRate = 0.0f;

	// タイマー
	m_timerNum = new Number("DATA/Number/Number.png", 64);
	m_flameNum = new Number("DATA/Number/Number.png", 64);
	m_timerPic = new tdn2DObj("DATA/Number/Number.png");
	m_flamePic = new tdn2DObj("DATA/Number/Number.png");
	m_flamePic->SetScale(0.8f);
	m_tttPic = new tdn2DObj("DATA/UI/Time.png");
	m_timeColR = 255; m_timeColG = 255; m_timeColB = 255;

	//m_timerPic->SetRGB(55, 255, 255);
	m_timer = 60 * 2;
	//m_timer = 0;
	m_flame = 60;

	// 草ゲージ
	m_graphX = 10, m_graphY = -10;
	m_graphBack = new tdn2DObj("DATA/UI/Circle/SpiritCircle_UNDER.png");
	m_graphFlont= new tdn2DObj("DATA/UI/Circle/SpiritCircle_ON.png");
	m_graph = new Pie_graph("DATA/UI/Circle/SpiritCircle_gage.png");
	m_graphRip = new tdn2DAnim("DATA/UI/Circle/SpiritCircle_FULL.png");
	m_graphRip->OrderRipple(12, 1.0f, 0.1f);
	m_percent = 0.0f;

	// 
	//particle = new Particle_2d();
	//particle->Initialize("DATA/Effect/star.png", 256, 1, 1);

	// 初期化
	m_worfHappyCount = 0;
	m_sheepKillCount = 0;

	m_UIBoad = new tdn2DObj("DATA/UI/ue.png");


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
	SAFE_DELETE(m_flameNum);
	SAFE_DELETE(m_flamePic);
	SAFE_DELETE(m_timerPic);
	SAFE_DELETE(m_graph);
	SAFE_DELETE(m_graphBack);
	SAFE_DELETE(m_graphFlont);
	SAFE_DELETE(m_graphRip);

	SAFE_DELETE(m_tttPic);

	SAFE_DELETE(m_UIBoad);

	NumberEffect.Release();

	// 全部消して
	for (auto it : m_SendPowerData)
	{
		SAFE_DELETE(it);
	}
	//　データを空に
	m_SendPowerData.clear();
}

// 初期化
void UIManager::Init()
{
	// スコア
	m_score = 0;
	m_scorePoint = 0;
	// コンボ
	m_combo = 0;
	m_maxCombo = 0;
	m_energyRate = 0.0f;

	m_timer = 60 * 2;
	//m_timer = 0;
	m_flame = 60;

	// 初期化
	m_worfHappyCount = 0;
	m_sheepKillCount = 0;

}

/***************************/
//	更新・描画
/***************************/
void UIManager::Update()
{
	m_scoreNum->Update();
	//m_conboBG->Update();
	m_comboNum->Update();

	m_timerNum->Update();

	// 色々な処理
	ScoreUpdate();
	ConboUpdate();
	TimerUpdate();
	PointUpdate();
	GraphUpdate();
	NumberEffect.Update();
	SendPowerUpdate();

	//仮
	//Debug();


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
	
	m_UIBoad->Render(0, 0);

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

	/****************************************/
	// ベジェ
	SendPowerRender();

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
	m_scoreNum->Render(350, 14, m_scorePoint);
	//m_scorePic->Render(400, 30);
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
	//m_energyRate -= 0.03f;

	// 時間によって速さを変えようと思います
	if (m_combo <= 100)
	{
		m_energyRate -= 0.002f;
	}
	else if (m_combo <= 200)
	{
		m_energyRate -= 0.003f;
	}
	else
	{
		m_energyRate -= 0.005f;
	}


	// 真面目にコンボの計算
	if (m_energyRate <= 0.0f)
	{
		m_energyRate = 0.0f;
		m_combo = 0;
	}
}

void UIManager::ConboRender()
{
	//m_conboBG->Render(1000, -10);
	m_conboGage->Render(900, 55);	// ゲージ	  
									//m_conboGageEnergy->SetTurnOver(true);
									//m_conboGageEnergy->SetAngle(1.57);
	m_conboGageEnergy->Render((900+128)-(int)(128 * m_energyRate), 55, (int)(128 * m_energyRate) , 128, 0, 0, 128, 128);  // ゲージエネルギー
	m_comboNum->Render(980, 10, m_combo);// 数値

}

void UIManager::ConboCount()
{
	m_energyRate = 1.0f;
	m_comboNum->Action();
	m_combo++;

	// 最大コンボ数を保存
	if (m_maxCombo <= m_combo)
	{
		m_maxCombo = m_combo;
	}

}

// 時間上昇！
void UIManager::AddTimer(int timer)
{
	m_timer += timer;
	NumberEffect.AddNumber(780, 60, timer, Number_Effect::COLOR_TYPE::YELLOW_GREEN);
	EffectMgr.AddEffect(720, 60, EFFECT_TYPE::PLUS);
}

// 時間上昇！
void UIManager::AddFlame(int flame)
{
	m_flame += flame;
	//NumberEffect.AddNumber(780, 60, timer, Number_Effect::COLOR_TYPE::YELLOW_GREEN);
	//EffectMgr.AddEffect(720, 60, EFFECT_TYPE::PLUS);
}

/***********************/
//	タイマー系
/***********************/
void UIManager::TimerUpdate()
{

	float rate = (float)m_timer / 20.0f;

	rate = Math::Clamp(rate, 0.0f, 1.0f);
	
	m_timeColG = 255 *rate;
	m_timeColB = 255 *rate;

	m_timeColG = Math::Clamp(m_timeColG, 0, 255);
	m_timeColB = Math::Clamp(m_timeColB, 0, 255);

	// 色変えるぞー
	m_timerPic->SetRGB((int)m_timeColR, (int)m_timeColG, (int)m_timeColB);
	m_flamePic->SetRGB((int)m_timeColR, (int)m_timeColG, (int)m_timeColB);
	
	//if (m_timer)
	//{

	//}


	// カウントダウン
	m_flame--;
	if (m_flame < 0)
	{
		m_flame = 60;
		m_timer--;	

	}

	// 0以下にさせない
	if (m_timer <= 0)
	{
		m_timer = 0;
	}

	// 残りタイマーに応じてサウンドの速度を変更
	// 30秒以下
	if (m_timer <= 20)
	{
		const float percentage =  1 - (m_timer / 20.0f);
		const float speed = 1.0f + (percentage * .05f);
		bgm->SetSpeed("MAIN", speed);
	}
	else
	{
		// 元の速度
		bgm->SetSpeed("MAIN", 1.0f);
	}
}

void UIManager::TimerRender()
{

	// タイマー
	

	const int second = m_timer % 60,
		minutes = m_timer / 60;
	const int TimerX = 570;
	m_timerPic->Render(TimerX, 16, 64, 64, minutes * 64, 0, 64, 64);			// 分
	m_timerPic->Render(TimerX + 36, 16, 64, 64, 13 * 64, 0, 64, 64);			// :
	m_timerPic->Render(TimerX + 72, 16, 64, 64, second / 10 * 64, 0, 64, 64);	// 秒(10の位)
	m_timerPic->Render(TimerX + 108, 16, 64, 64, second % 10 * 64, 0, 64, 64);	// 秒(1の位)
				
																//m_timerNum->Render(600, 10, m_timer);

	const int secondflame= m_flame % 60;
	//const int minitflame = m_flame % 10;
	//m_flamePic->Render(TimerX+200, 16, 64, 64, secondflame * 64, 0, 64, 64);
	m_flamePic->Render(TimerX + 136, 24, 64, 64, 13 * 64, 0, 64, 64);			// :
	m_flamePic->Render(TimerX + 162, 24, 64, 64, secondflame / 10 * 64, 0, 64, 64);
	m_flamePic->Render(TimerX + 190, 24, 64, 64, secondflame % 10 * 64, 0, 64, 64);

	//m_tttPic->Render(TimerX - 128, 0);


}

/***********************/
//	ポイント系
/***********************/
void UIManager::PointUpdate()
{
	
}

void UIManager::PointRender()
{

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



void UIManager::SendPowerUpdate()
{

	// List
	for (auto it = m_SendPowerData.begin(); it != m_SendPowerData.end();)
	{
		//■■ここででぃれい

		(*it)->Update();

		if ((*it)->IsEnd() == true)
		{
			if ((*it)->IsBokusou() == true)
			{
				BokusouMgr->CreateByBazier();
			}
			else
			{
				// ★★★　ここでタイマー加算
				AddTimer((*it)->GetPower());
			}

			
			// 先に消す
			SAFE_DELETE((*it));
			// 勝手に更新される
			it = m_SendPowerData.erase(it);
		}
		else
		{
			// 自分で更新
			it++;
		}
	}

}

void UIManager::SendPowerRender()
{
	// List
	for (auto it : m_SendPowerData)
	{
		it->Render();
	}
}

// ベジェ
void UIManager::AddSendPower(char* filename, Vector3 startPos, Vector3 centerPos,
	Vector3 center2Pos, Vector3 endPos, int endFlame, int power, bool bokusouFlag, int waitTimer)
{

	SendPower* data;
	data = new SendPower(filename, startPos, centerPos, center2Pos, endPos, endFlame, power, bokusouFlag, waitTimer);

	data->Action();

	// 要素追加
	m_SendPowerData.push_back(data);
}
