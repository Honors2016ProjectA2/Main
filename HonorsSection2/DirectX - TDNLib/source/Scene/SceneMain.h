#pragma once




class sceneMain : public BaseScene
{
public:
	//	初期化・解放
	bool Initialize();
	~sceneMain();
	//	更新・描画
	bool Update();
	void Render();

private:
	// BG
	tdn2DObj* BG;

	// キャラクター
	iex3DObj* player;

	// ステージ
	iexMesh* stage;

};


