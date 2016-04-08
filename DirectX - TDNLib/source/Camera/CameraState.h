#pragma once


//*****************************************************************************************************************************
//
//			カメラステート担当
//
//*****************************************************************************************************************************

// テキストの読み込み用
#include	"../textLoader\textLoader.h"

// 前方宣言
class CameraManager;
class EffectCamera;

//--------------------グローバルステート
class CameraGlobalState :public State<CameraManager>
{
public:

	//this is a シングルトン
	static CameraGlobalState* GetInstance();

	// 入る
	virtual void Enter(CameraManager* pCamera);

	// 実行します
	virtual void Execute(CameraManager* pCamera);

	// 帰る
	virtual void Exit(CameraManager* pCamera);

	// 描画
	virtual void Render(CameraManager* pCamera);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(CameraManager* pCamera, const Message& msg);

private:
	CameraGlobalState() {};
	~CameraGlobalState() {};
	CameraGlobalState(const CameraGlobalState&);
	CameraGlobalState& operator=(const CameraGlobalState&);
};


//--------------------固定カメラ
class FixCameraState :public State<CameraManager>
{
public:

	//this is a シングルトン
	static FixCameraState* GetInstance();

	// 入る
	virtual void Enter(CameraManager* pCamera);

	// 実行します
	virtual void Execute(CameraManager* pCamera);

	// 帰る
	virtual void Exit(CameraManager* pCamera);

	// 描画
	virtual void Render(CameraManager* pCamera);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(CameraManager* pCamera, const Message& msg);

private:
	FixCameraState() {};
	~FixCameraState() {};

	FixCameraState(const FixCameraState&);
	FixCameraState& operator=(const FixCameraState&);

};

//--------------------イントロ演出用カメラ
class IntroCameraState :public State<CameraManager>
{
public:

	//this is a シングルトン
	static IntroCameraState* GetInstance();

	// 入る
	virtual void Enter(CameraManager* pCamera);

	// 実行します
	virtual void Execute(CameraManager* pCamera);

	// 帰る
	virtual void Exit(CameraManager* pCamera);

	// 描画
	virtual void Render(CameraManager* pCamera);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(CameraManager* pCamera, const Message& msg);

private:
	int m_step;		// 演出のステップ
	int m_timer;	// 演出での時間を計る時に

	Vector3 m_StartPos, m_StartTarget;	// 開始時の座標(移動して、最後に元の位置に戻る際に必要になる)

	Vector3 m_NextPos, m_NextTarget;	// 目標位置(補間で使う)
	float m_SlerpPow;					// 補間にかける力(0 ～ 1.0f)

	Vector3 m_StartPersonPos;		// 噂をスタートする人の座標(メッセージから受け取る)

	bool m_bAction;	// 最初はfalseでtrueになったら稼働する

	// ステップを進めるプチ関数
	void NextStep()
	{
		m_step++;
		m_timer = 0;
	}

	IntroCameraState() {};
	~IntroCameraState() {};

	IntroCameraState(const IntroCameraState&);
	IntroCameraState& operator=(const IntroCameraState&);

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
class EffectCameraState :public State<CameraManager>, textLoader
{
public:

	//this is a シングルトン
	static EffectCameraState* GetInstance();

	// 入る
	virtual void Enter(CameraManager* pCamera);

	// 実行します
	virtual void Execute(CameraManager* pCamera);

	// 帰る
	virtual void Exit(CameraManager* pCamera);

	// 描画
	virtual void Render(CameraManager* pCamera);

	// エージェントからのメッセージを受信した場合、これが実行される
	virtual bool OnMessage(CameraManager* pCamera, const Message& msg);

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