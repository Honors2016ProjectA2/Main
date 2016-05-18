#pragma once

//********************************************************************************************************************
//
//		クラスの宣言
//
//********************************************************************************************************************
//		サウンドマネージャー(グローバル実体で関数呼び出し)
//****************************************************************************************************************

namespace SoundManager
{
	void Initialize();
	void Release();
	void Update();
}

class iexSoundBase;
class tdnSoundSE;
class tdnSoundBGM;

class SE_Manager
{
private:
	std::map<LPSTR, int> ID;	// char*型で番号を振り分ける

	tdnSoundSE *play_manager;	// iexSound

	// データ検索
	//int Find_data_no(char *_ID);

	int Play_in(int data_num, bool loop);
	int Play_in(int data_num, float volume, bool loop);
	int Play_in(int data_num, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop);

public:

	//===============================================
	//	音データ
	//===============================================
	struct	DATA{
		LPSTR id;					// 呼び出す際に使用する識別コード
		LPSTR file_name;			// ファイルパス
		int play_simultaneously;	// 同一の音を複数再生する最大値
		bool b3D;					// 3Dサウンドフラグ
	};


	//===============================================
	//	初期化と解放
	//===============================================
	SE_Manager(){}
	~SE_Manager();
	void Initialize();

	//===============================================
	//	更		新
	//===============================================
	void Update();


	//===============================================
	//	処		理
	//===============================================
	int Play(LPSTR _ID, bool loop = false);																											// 簡易版
	int Play(LPSTR _ID, float volume, bool loop);																									// ボリューム設定版
	int Play(LPSTR _ID, const Vector3 &pos, const Vector3 &front = Vector3(0, 0, -1), const Vector3 &move = Vector3(0, 0, 0), bool loop = false);	// 3D設定版
	void Stop(LPSTR _ID, int no);																													// Playで返ってきた数値を入れる
	void Stop_all();																																// 全部止める
	
	void Set_listener(const Vector3 &pos, const Vector3 &front, const Vector3 &up, const Vector3 &move);											// リスナー情報
};

class BGM_Manager
{
private:
	std::map<LPSTR, int> ID;	// char*型で番号を振り分ける

	tdnSoundBGM *play_manager;	// iexSound

	// データ検索
	//int Find_data_no(char *_ID);

	void Play_in(int data_num, bool loop);
	void Play_in(int data_num, float volume, bool loop);
	void Play_in(int data_num, const Vector3 &pos, const Vector3 &front, const Vector3 &move, bool loop);

public:

	int effect_no = 0;

	//===============================================
	//	音データ
	//===============================================
	struct	DATA{
		LPSTR id;					// 呼び出す際に使用する識別コード
		LPSTR file_name;			// ファイルパス
		bool b3D;
	};


	//===============================================
	//	初期化と解放
	//===============================================
	void Initialize();
	~BGM_Manager();

	//===============================================
	//	更		新
	//===============================================
	void Update();


	//===============================================
	//	処		理
	//===============================================
	void Play(LPSTR _ID, bool loop = true);																											// 簡易版
	void Play(LPSTR _ID, float volume, bool loop);																									// ボリューム設定版
	void Play(LPSTR _ID, const Vector3 &pos, const Vector3 &front = Vector3(0, 0, -1), const Vector3 &move = Vector3(0, 0, 0), bool loop = true);	// 3D設定版
	void Stop(LPSTR _ID);																															// Playで返ってきた数値を入れる
	void Stop_all();																																// 全部止める

	void Fade_in(LPSTR _ID, float fade_speed, bool loop = true);
	void Fade_out(LPSTR _ID, float fade_speed);
	void Cross_fade(LPSTR inID, LPSTR outID, float fade_speed = .0075f, bool loop = true);
	void Cross_fade(LPSTR inID, LPSTR outID, float in_speed, float out_speed, bool loop);

	void Set_listener(const Vector3 &pos, const Vector3 &front, const Vector3 &up, const Vector3 &move);											// リスナー情報
};

//===============================================
//	実体(関数呼び出しはこれらを使う)
//===============================================
extern SE_Manager *se;
extern BGM_Manager *bgm;