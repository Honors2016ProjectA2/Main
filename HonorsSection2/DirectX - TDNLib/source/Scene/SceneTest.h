#pragma once




class sceneTest : public BaseScene
{
public:
	//	�������E���
	bool Initialize();
	~sceneTest();
	//	�X�V�E�`��
	bool Update();
	void Render();

	void MaskRender();
	tdn2DObj* maskScreen;
	int m_blindSplit;
	float m_blindAngle;

private:
	// BG
	tdn2DObj* BG;

	// �L�����N�^�[
	iex3DObj* player;

	// �X�e�[�W
	iexMesh* stage;

};


