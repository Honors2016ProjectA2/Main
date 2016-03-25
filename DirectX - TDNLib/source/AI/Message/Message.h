#pragma once
#include <iostream>
#include <math.h>
#include <string>


// メッセージタイプ
enum message_type
{
	A,
	B,
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
	int          Sender;

	// このTelegramを受け取るエンティティ
	int          Receiver;

	// メッセージの内容。　（列挙型に設定されているもの）
	int          Msg;

	// メッセージの追加情報 (キャラクターが動く→ [追加情報] Xに２移動 )
	// 追加情報を事前に知ることが不可能なのでVoid*で提供
	void*        ExtraInfo;

	// メッセージの遅延時間
	double       DispatchTime;

	Message() :DispatchTime(-1),
		Sender(-1),
		Receiver(-1),
		Msg(-1)
	{}

	Message(double time,
		int    sender,
		int    receiver,
		int    msg,
		void*  info = NULL) : DispatchTime(time),
		Sender(sender),
		Receiver(receiver),
		Msg(msg),
		ExtraInfo(info)
	{}

};