#pragma once

class IconButtonManager;

class sceneSelect : public BaseScene
{
public:
	//	�������E���
	bool Initialize();
	~sceneSelect();
	//	�X�V�E�`��
	bool Update();
	void Render();

private:
	IconButtonManager *m_pIconMgr;

};


