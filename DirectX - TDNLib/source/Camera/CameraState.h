#pragma once


//*****************************************************************************************************************************
//
//			カメラステート担当
//
//*****************************************************************************************************************************

// テキストの読み込み用
#include	"../textLoader\textLoader.h"

// 前方宣言
class Camera;
class EffectCamera;

//--------------------グローバルステート
class CameraGlobalState:public State<Camera>
{
public:

	//this is a シングルトン
	static CameraGlobalState* GetInstance();

	// 入る
	virtual void Enter(Camera* pCamera);

	// 実行します
	virtual void Execute(Camera* pCamera);

	// 帰る
	virtual void Exit(Camera* pCamera);

	// 描画
	virtual void Render(Camera* pCamera);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(Camera* pCamera, const Message& msg);

private:
	CameraGlobalState() {};
	~CameraGlobalState() {};
	CameraGlobalState(const CameraGlobalState&);
	CameraGlobalState& operator=(const CameraGlobalState&);
};


//--------------------固定カメラ
class FixCameraState :public State<Camera>
{
public:

	//this is a シングルトン
	static FixCameraState* GetInstance();

	// 入る
	virtual void Enter(Camera* pCamera);

	// 実行します
	virtual void Execute(Camera* pCamera);

	// 帰る
	virtual void Exit(Camera* pCamera);

	// 描画
	virtual void Render(Camera* pCamera);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(Camera* pCamera, const Message& msg);

private:
	FixCameraState() {};
	~FixCameraState() {};

	FixCameraState(const FixCameraState&);
	FixCameraState& operator=(const FixCameraState&);

};

//--------------------エフェクトカメラ用データ
class TimeLineCameraPattern
{
public:
	int no;			// 自分のパターン番号
	int data_count;	// データの持ってる数
	struct Data
	{
		int start_frame;		// タイムライン上での開始フレーム
		int end_frame;			// タイムライン上での終了フレーム
		int frame_len;			// 1パターンあたりのフレーム数(バーの長さ)
		Vector3 *pos_array;		// 座標(開始と終わりだけでなく、中間とかも作るので可変長)
		Vector3 *target_array;	// 注視点(同じく可変長)
		int num_elements;		// 上記可変長の要素数
		bool fix;
	}*data;
};

//--------------------エフェクトカメラ(スクリプト)
class EffectCameraState :public State<Camera>,textLoader
{
public:

	//this is a シングルトン
	static EffectCameraState* GetInstance();

	// 入る
	virtual void Enter(Camera* pCamera);

	// 実行します
	virtual void Execute(Camera* pCamera);

	// 帰る
	virtual void Exit(Camera* pCamera);

	// 描画
	virtual void Render(Camera* pCamera);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(Camera* pCamera, const Message& msg);

private:
	EffectCameraState() {};
	~EffectCameraState() {};

	EffectCameraState(const EffectCamera&);
	EffectCameraState& operator=(const EffectCamera&);

	int pattern_no;
	int data_cursor;
	char effect_path[256];
	DWORD ptr;	// 
	int current_frame;	// 現在のフレーム数
	TimeLineCameraPattern *time_line_data = new TimeLineCameraPattern;

	void GetTimeLineCameraPos(Vector3 *out_pos, Vector3 *out_target);
};