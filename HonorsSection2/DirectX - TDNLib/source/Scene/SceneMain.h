#pragma once




class sceneMain : public BaseScene
{
public:
	//	�������E���
	bool Initialize();
	~sceneMain();
	//	�X�V�E�`��
	bool Update();
	void Render();

private:
	// BG
	tdn2DObj* BG;

	// �L�����N�^�[
	iex3DObj* player;

	// �X�e�[�W
	iexMesh* stage;

};


