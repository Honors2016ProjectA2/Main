#include "TDNLIB.h"

#include "UI.h"

// sceneMainのグローバル変数から
extern int RippleCount;



/*************************/
//	吹き出しのセット
/*************************/
void UIManager::PushHukidashi(const Vector3 &pos, HUKIDASHI_TYPE type)
{
	Vector2 srcXY;
	if (tdnRandom::Get(0, 1))
	{
		srcXY.x = 0;
		srcXY.y = 1;
	}
	else
	{
		srcXY.x = (float)tdnRandom::Get(0, 3);
		srcXY.y = 0;
	}
	srcXY.x *= 256;
	srcXY.y *= 128;

	tdn2DObj *Image;
	switch (type)
	{
	case HUKIDASHI_TYPE::ORIGIN:
		Image = m_Datas[UI_ID::HUKIDASHI_MINE]->lpImage;
		srcXY = Vector2(0, 0);
		break;
	case HUKIDASHI_TYPE::SUCCESS:
		Image = m_Datas[UI_ID::HUKIDASHI_CORRECT]->lpImage;
		break;
	case HUKIDASHI_TYPE::FAILED:
		Image = m_Datas[UI_ID::HUKIDASHI_INCORRECT]->lpImage;
		break;
	}
	Hukidashi *data = new Hukidashi(Image, srcXY, Hukidashi::DEFAULT_APP_TIME, pos);
	m_HukidashiList.push_back(data);
}

/*************************/
//	静的メンバ変数の宣言
/*************************/
UIManager *UIManager::pInstance = nullptr;


/***********/
//	実体取得
/***********/
UIManager &UIManager::GetInstance()
{
	if (!pInstance)
	{
		pInstance = new UIManager();
	}

	// TODO: return ステートメントをここに挿入します
	return *pInstance;
}

/***********/
//	初期化
/***********/
void UIManager::Initialize()
{
	std::ifstream ifs("DATA/Text/UI/main.txt");
	assert(ifs);	// ファイル入ってないかファイル名間違えてるよ！

	int count(0);
	while (!ifs.eof())
	{
		UIData *data = new UIData;

		char skip[64];
		ifs >> skip;
		ifs >> skip;
		int ID;
		ifs >> ID;

		assert(ID == count);	// テキストのIDを0から順番に並べてください！！
		count++;

		// 画像のファイルパス
		ifs >> skip;
		char filename[128];
		ifs >> filename;
		data->lpImage = new tdn2DObj(filename);

		// 画像座標	※この値は無視して手打ちで配置しても良い
		ifs >> skip;
		ifs >> data->pos.x;
		ifs >> data->pos.y;

		m_Datas.push_back(data);
	}
}

/***********/
//	解放
/***********/
UIManager::~UIManager()
{
	// リストの開放
	for (auto it : m_Datas)
	{
		delete it;
	}
	for (auto it : m_HukidashiList)
	{
		delete it;
	}
}

void UIManager::Release()
{
	// 静的実体の解放↑のデストラクタを呼び出す
	SAFE_DELETE(pInstance);
}


/***********/
//	更新
/***********/
void UIManager::Update()
{
	// 吹き出しリストの更新
	for (auto it = m_HukidashiList.begin(); it != m_HukidashiList.end();)
	{
		(*it)->Update();
		if ((*it)->EraseOK())
		{ 
			delete (*it);
			it = m_HukidashiList.erase(it);
		}
		else it++;
	}
}

/***********/
//	描画
/***********/
void UIManager::Render()
{
	// 残り
	ID_Render(UI_ID::NOKORI);

	// 残りの数字
	m_Datas[UI_ID::NOKORI_NUMBER]->lpImage->Render(
		(int)m_Datas[UI_ID::NOKORI_NUMBER]->pos.x,
		(int)m_Datas[UI_ID::NOKORI_NUMBER]->pos.y, 
		256, 128, 256*RippleCount, 0, 256, 128);

	// 吹き出しリストの描画
	for (auto it : m_HukidashiList) it->Render();
}




void Hukidashi::Render()
{
	// α
	m_pImage->SetARGB((BYTE)(255 * ((float)m_AppTime / DEFAULT_APP_TIME)), (BYTE)255, (BYTE)255, (BYTE)255);

	//m_pImage->Render3D(m_pos, 256, 128, (int)m_SrcXY.x, (int)m_SrcXY.y, 256, 128);
	m_pImage->Render((int)Math::WorldToScreen(m_pos).x - 20, (int)Math::WorldToScreen(m_pos).y - 160, 256, 128, (int)m_SrcXY.x, (int)m_SrcXY.y, 256, 128);
}



// ずぼら
void UIManager::ID_Render(UI_ID id)
{
	m_Datas[id]->lpImage->Render((int)m_Datas[id]->pos.x, (int)m_Datas[id]->pos.y);
}