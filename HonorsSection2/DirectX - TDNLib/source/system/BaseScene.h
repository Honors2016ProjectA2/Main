#pragma once

/*****************************/
//		ベースシーン
/*****************************/
class BaseScene
{
public:
	//	生成・解放
	BaseScene(){}
	virtual ~BaseScene(){}
	//	初期化
	virtual bool Initialize(){ return true; }
	//	更新・描画
	virtual bool Update(){ return false; }
	virtual void Render(){}
};

