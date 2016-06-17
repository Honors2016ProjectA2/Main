#pragma once

class sceneLoading : public BaseScene
{
private:
	BaseScene*	m_newScene;

	//------------------------------------------------------
	//	���[�h�̉�
	//------------------------------------------------------
	int			m_AnimeCount;
	int			m_AnimeFrame;
	tdn2DObj*	m_pImage;
	const static int m_AnimeSpeed = 8;

public:

	//------------------------------------------------------
	//	�X���b�h�֌W
	//------------------------------------------------------
	static void Thread_funk(void *arg);
	static bool isThread;


	//------------------------------------------------------
	//	�������ƊJ��
	//------------------------------------------------------
	bool Initialize(BaseScene *newScene);
	~sceneLoading();


	//------------------------------------------------------
	//	�X�V�ƕ`��
	//------------------------------------------------------
	bool Update();
	void Render();

};
