#pragma once
#include "TDNLIB.h"
#include <iostream>
#include <math.h>
#include <string>
#include "AI\Entity\EntityEnum.h"

// メッセージタイプ
enum MESSAGE_TYPE
{
	RIPPLE_VS_PERSON,
	Riple_Red,
	START_GOSSIP,
	GOAL_GOSSIP,

	SHED_GOSSIP,		// うわさを流したとき、ジャッジマネージャに送信する。(プレイヤーが設置した人間だったら送信しないようにしたい)
	GAMEOVER,			// うわさを流してはいけないヤツに噂を流したら、ジャッジマネージャに送信する

	START_INTRO_CAMERA,	// ？→カメラ　ゲームが始まってイントロカメラ発動してくださいメッセージ
	//END_INTRO_CAMRA		// カメラ→sceneMainState カメラ演出が終わったというメッセージ(シーンメインにBaseEntityを継承させるとUpdateの箇所等で面倒なことになるので、廃止)
};

enum class PERSON_TYPE;	// 前方宣言
// EX_INFO用の構造体
struct RIPPLE_INFO
{
	PERSON_TYPE type;
	Vector3 pos;
	float size;
};

/* カメラマネージャにデータを送る時の構造体 */

// イントロ発動メッセージ送信のときにいれる構造体
struct INTRO_CAMERA_INFO
{
	Vector3 start_person_pos;	// うわさを流す人間の座標
};

/*
	メッセージはシンプルな列挙型。
	内容はなんでもあり。
	
	・誰が
	・誰に
	・メッセージの列挙
	・メッセージの追加情報(Void*)
	・ディレイ用
*/

/*電報構造体*/
struct Message
{
	// このTelegramを送ったエンティティ
	ENTITY_ID          Sender;

	// このTelegramを受け取るエンティティ
	ENTITY_ID          Receiver;

	// メッセージの内容。　（列挙型に設定されているもの）
	int          Msg;

	// メッセージの追加情報 (キャラクターが動く→ [追加情報] Xに２移動 )
	// 追加情報を事前に知ることが不可能なのでVoid*で提供
	void*        ExtraInfo;

	// メッセージの遅延時間
	double       DispatchTime;

	Message() :DispatchTime(-1),
		Sender(ENTITY_ID::id_error),
		Receiver(ENTITY_ID::id_error),
		Msg(-1)
	{}

	Message(double time,
		ENTITY_ID    sender,
		ENTITY_ID    receiver,
		int    msg,
		void*  info = NULL) : DispatchTime(time),
		Sender(sender),
		Receiver(receiver),
		Msg(msg),
		ExtraInfo(info)
	{}

};