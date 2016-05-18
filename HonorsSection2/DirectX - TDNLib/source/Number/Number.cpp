#include "Number.h"

/***********************************************/
// 数値表示用クラス
/***********************************************/

/***********************************************/
// インスタンス化
/***********************************************/
//Number* Number::m_pInstance = nullptr;
//
//Number & Number::GetInstance()
//{
//	if (!m_pInstance)
//	{
//		m_pInstance = new Number();
//	}
//
//	// TODO: return ステートメントをここに挿入します
//	return *m_pInstance;
//}
//
//void Number::Release()
//{
//	SAFE_DELETE(m_pInstance);
//}


/***********************************************/
// 初期化・解放
/***********************************************/
Number::Number()
{
	m_pic = new tdn2DAnim("Data/Number/Number.png");
	m_pic->OrderJump(4, 1.0f, 1.3f);
	//m_pic->OrderShake(7, 10, 20, 2);
	//m_pic->OrderRipple(12, 1.0, 0.1f);
	//m_pic->OrderShrink(6, 1.0f, 3.0f);

	m_picSize = 64;
	m_picScale = 1;
}

Number::Number(char * name, int picSize)
{
	m_pic = new tdn2DAnim(name);
	m_pic->OrderJump(4, 1.0f, 1.3f);
	//m_pic->OrderShake(7, 10, 20, 2);
	//m_pic->OrderRipple(12, 1.0, 0.1f);
	//m_pic->OrderShrink(6, 1.0f, 3.0f);
	m_picSize = picSize;
	m_picScale = 1;

}

Number::~Number()
{
	SAFE_DELETE(m_pic);
}


/***********************************************/
// 更新
/***********************************************/
void Number::Update()
{
	m_pic->Update();
}


/***********************************************/
// 描画
/***********************************************/
void Number::Render()
{

}

void Number::Render(int x, int y, int num, NUM_KIND kind)
{
	int number = num;

	//int DIGIT_MAX = 0;
	//int calcNum = num;
	//for (int i = 0;; i++)
	//{	
	//	calcNum = (int)(calcNum / 10);// 数値の一番小さい桁を消す
	//	if (calcNum <= 0)break;
	//	DIGIT_MAX++;
	//}

	// 数字の種類により分岐
	switch (kind)
	{
	case Number::NUM_KIND::NORMAL:
		for (int count = 0;; ++count)
		{
			int digitNum = number % 10;	// 一番小さい桁を入手
			number = (int)(number / 10);// 数値の一番小さい桁を消す

			m_pic->Render(x - ((count * (m_picSize /2))*m_picScale), y, m_picSize*m_picScale, m_picSize*m_picScale, digitNum*m_picSize, 0, m_picSize, m_picSize);// 数字描画

			if (number <= 0)break;// 数値が０以下になったらさよなら
		}
		break;
	case Number::NUM_KIND::HEAL:
		for (int count = 0;; ++count)
		{
			int digitNum = number % 10;	// 一番小さい桁を入手
			number = (int)(number / 10);// 数値の一番小さい桁を消す

			m_pic->Render(x - ((count *32)*m_picScale), y, m_picSize*m_picScale, m_picSize*m_picScale, digitNum*m_picSize, 0, m_picSize, m_picSize);// 数字描画

			if (number <= 0)break;// 数値が０以下になったらさよなら
		}
		break;
	default:
		//for (int count = 0;; ++count)
		//{
		//	int digitNum = number % 10;	// 一番小さい桁を入手
		//	number = (int)(number / 10);// 数値の一番小さい桁を消す

		//	m_pic->Render(x + (((DIGIT_MAX - count) * 32)*m_picScale), y, m_picSize*m_picScale, m_picSize*m_picScale, digitNum*m_picSize, 0, m_picSize, m_picSize);// 数字描画

		//	if (number <= 0)break;// 数値が０以下になったらさよなら
		//}
		break;
	}

}

// アクション
void Number::Action(int derayTime)
{
	m_pic->Action(derayTime);
}





/*****************************************************************/
//
//				ナンバーエフェクト
//
/*****************************************************************/

/***********************************************/
// インスタンス化
/***********************************************/
Number_Effect* Number_Effect::m_pInstance = nullptr;

Number_Effect& Number_Effect::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new Number_Effect();
	}

	// TODO: return ステートメントをここに挿入します
	return *m_pInstance;
}

void Number_Effect::Release()
{
	SAFE_DELETE(m_pInstance);
}


/***********************************************/
// 初期化・解放
/***********************************************/
Number_Effect::Number_Effect()
{

}

Number_Effect::~Number_Effect()
{
	// 全部消して
	for (auto it : m_NumberData)
	{
		SAFE_DELETE(it->number);
		SAFE_DELETE(it);
	}

	//　データを空に
	m_NumberData.clear();
}


/***********************************************/
// 更新
/***********************************************/
void Number_Effect::Update()
{
	// List
	for (auto it = m_NumberData.begin(); it != m_NumberData.end();)
	{

		//it->Update();

		//it->alpha += 21;
		//if (it->alpha > 255)
		//{
		//	it->alpha = 255;
		//}

		// ステートで動きを分岐
		switch ((*it)->state)
		{
		case NumberData::STATE::START:

			// y座標を↑にずらす
			(*it)->y -= 4;

			// アルファを上げる
			(*it)->alpha +=50;
			if ((*it)->alpha >= 255)(*it)->alpha = 255;

			// フレームで調整
			(*it)->flame++;
			
			//if ((*it)->alpha >= 255)
			if ((*it)->flame >= 12)
			{
				//　アルファがマックスになったら終了
				(*it)->state = NumberData::STATE::ARRIVAL;

				// フレームを初期化
				(*it)->flame = 0;

				
			}
			
				break;
		case NumberData::STATE::ARRIVAL:

			// y座標を↑にずらす
			(*it)->y -= 1;

			// フレームで時間稼ぎ
			(*it)->flame++;

			if ((*it)->flame > 12)
			{
				(*it)->state = NumberData::STATE::END;
			}		

			break;
		case NumberData::STATE::END:
			(*it)->y += 1;
			// アルファを下げる
			(*it)->alpha -= 40;

			// なんか終わるフラグを立てる
			if ((*it)->alpha <= 0)
			{
				(*it)->alpha = 0;
				// 終了フラグOn 
				(*it)->isEnd = true;
			}

			break;
		default:
			break;
		}	

		// アルファ更新
		(*it)->number->GetAnim()->SetARGB((*it)->alpha, 255, 255, 255);

		// ナンバー更新
		(*it)->number->Update();

		if ((*it)->isEnd == true)
		{
			// 先に消す
			SAFE_DELETE((*it)->number);
			SAFE_DELETE((*it));
			// 勝手に更新される
			it = m_NumberData.erase(it);
		}
		else
		{
			// 自分で更新
			it++;
		}
	}
}


/***********************************************/
// 描画
/***********************************************/
void Number_Effect::Render()
{
	// List
	for (auto it : m_NumberData)
	{
		it->number->Render(it->x, it->y, it->score);
	}
}

/************************************************/
//	数字追加
/************************************************/
void Number_Effect::AddNumber(int x, int y, int score)
{
	NumberData* data;
	
	data = new NumberData();
	data->number = new Number();	
	//data->number->Action();
	data->x = x;
	data->y = y;
	data->score = score;

	data->flame = 0;
	data->endFlame = 100;
	data->alpha = 0;
	data->state = NumberData::STATE::START;
	
	data->isEnd = false;

	// 要素追加
	m_NumberData.push_back(data);


}

