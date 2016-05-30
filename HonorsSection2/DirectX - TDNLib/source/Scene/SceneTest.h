#pragma once




class sceneTest : public BaseScene
{
public:
	//	初期化・解放
	bool Initialize();
	~sceneTest();
	//	更新・描画
	bool Update();
	void Render();

	void MaskRender();
	tdn2DObj* maskScreen;
	int m_blindSplit;
	float m_blindAngle;

private:
	// BG
	tdn2DObj* BG;

	// キャラクター
	iex3DObj* player;

	// ステージ
	iexMesh* stage;

};


