#pragma once

class sceneLoading : public BaseScene
{
private:
	BaseScene*	m_newScene;

	//------------------------------------------------------
	//	ロードの炎
	//------------------------------------------------------
	int			m_AnimeCount;
	int			m_AnimeFrame;
	tdn2DObj*	m_pImage;
	const static int m_AnimeSpeed = 8;

public:

	//------------------------------------------------------
	//	スレッド関係
	//------------------------------------------------------
	static void Thread_funk(void *arg);
	static bool isThread;


	//------------------------------------------------------
	//	初期化と開放
	//------------------------------------------------------
	bool Initialize(BaseScene *newScene);
	~sceneLoading();


	//------------------------------------------------------
	//	更新と描画
	//------------------------------------------------------
	bool Update();
	void Render();

};
