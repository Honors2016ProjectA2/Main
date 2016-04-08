#include "TDNLIB.h"

#include "Camera.h"
#include "CameraState.h"
#include "../AI/Message/Message.h"
#include "../AI/Entity/BaseGameEntity.h"
#include "../AI/Entity/EntityManager.h"
#include "../AI/State/state.h"
#include "../AI/State/StateMachine.h"
#include "../AI/Message/MessageDispatcher.h"
#include "../AI/NPC/PersonManager.h"

//*****************************************************************************************************************************
//			グローバルステート
//*****************************************************************************************************************************
CameraGlobalState* CameraGlobalState::GetInstance()
{
	// ここに変数を作る
	static CameraGlobalState instance;
	return &instance;
}

// 入り口
void CameraGlobalState::Enter(CameraManager *pCamera)
{

}

// 実行中
void CameraGlobalState::Execute(CameraManager *pCamera)
{

}

// 出口
void CameraGlobalState::Exit(CameraManager *pCamera)
{

}

void CameraGlobalState::Render(CameraManager * pCamera)
{
}

//
bool CameraGlobalState::OnMessage(CameraManager *pCamera, const Message &msg)
{
	// メッセージタイプ
	//switch (msg.Msg)
	//{
	////case MESSAGE_TYPE::
	//
	//	break;
	//default:
	//
	//	break;
	//}//end switch

	// Flaseで返すとグローバルステートなので正真正銘の終り
	return false;
}


//*****************************************************************************************************************************
//			固定カメラ
//*****************************************************************************************************************************
FixCameraState* FixCameraState::GetInstance()
{
	// ここに変数を作る
	static FixCameraState instance;
	return &instance;
}

// 入り口
void FixCameraState::Enter(CameraManager *pCamera)
{
	/* テキストからデータ読み込み */
	std::ifstream ifs("DATA/Text/Camera/Fix.txt");
	assert(ifs);	// ファイル入ってない！

	char skip[32];	// 読み飛ばし用

	ifs >> skip;

	// 座標読み込み
	Vector3 pos;
	ifs >> pos.x, ifs >> pos.y, ifs >> pos.z;

	// 注視点読み込み
	Vector3 target;
	ifs >> target.x, ifs >> target.y, ifs >> target.z;

	// 本体にセット
	pCamera->SetPos(pos);
	pCamera->SetTarget(target);
}

// 実行中
void FixCameraState::Execute(CameraManager *pCamera)
{
	// 固定カメラなので基本的に何もしない
}

// 出口
void FixCameraState::Exit(CameraManager *pCamera)
{

}

void FixCameraState::Render(CameraManager * pCamera)
{
}

bool FixCameraState::OnMessage(CameraManager *pCamera, const Message &msg)
{
	// 出ていけ！
	return false;
}


//*****************************************************************************************************************************
//			イントロカメラ
//*****************************************************************************************************************************
IntroCameraState* IntroCameraState::GetInstance()
{
	// ここに変数を作る
	static IntroCameraState instance;
	return &instance;
}

// 入り口
void IntroCameraState::Enter(CameraManager *pCamera)
{
	// 働きたくないでござる
	m_bAction = false;

	// ステップ初期化
	m_step = 0;

	// タイマー初期化
	m_timer = 0;

	// 補間の力を初期化
	m_SlerpPow = .1f;

	// テキストから最初の座標をとってくる
	std::ifstream ifs("DATA/Text/Camera/Intro.txt");

	char skip[32];	// 読み飛ばし用変数

	ifs >> skip;
	ifs >> skip;
	ifs >> skip;
	ifs >> skip;

	// 座標読み込み
	ifs >> m_StartPos.x, ifs >> m_StartPos.y, ifs >> m_StartPos.z;

	// 注視点読み込み
	Vector3 target;
	ifs >> m_StartTarget.x, ifs >> m_StartTarget.y, ifs >> m_StartTarget.z;

	// 目標座標を設定
	m_NextPos = m_StartPos, m_NextTarget = m_StartTarget;
}

// 実行中
void IntroCameraState::Execute(CameraManager *pCamera)
{
	// 左クリックでイントロスキップ
	if (tdnMouse::GetLeft() == 3)
	{
		// 通常の固定カメラに切り替え(今はイントロのステートじゃないよというのがsceneMainStateに対するメッセージ)
		pCamera->GetFSM()->ChangeState(FixCameraState::GetInstance());
	}

	// まだ仕事命令来ていないから出ていけぇ！
	if (!m_bAction) return;

	// カメラの情報とってくる
	Vector3 cPos = pCamera->GetPos();
	Vector3 cTarget = pCamera->GetTarget();

	// 補間処理
	cPos = cPos * (1 - m_SlerpPow) + m_NextPos * m_SlerpPow;
	cTarget = cTarget * (1 - m_SlerpPow) + m_NextTarget * m_SlerpPow;


	switch (m_step)
	{
	case 0:
		/* 一定時間ステージを見下ろすだけの仕事 */
		if (++m_timer > 120)		// 時間過ぎたら
		{
			// 次のステップへ！
			NextStep();

			// 次の目標座標を設定(噂をスタートさせる人)
			m_NextTarget = m_StartPersonPos + Vector3(0, 10, 0);
			m_NextPos = m_StartPersonPos + Vector3(0, 30, -30);
		}
		break;
	case 1:
		/* 一定時間うわさを流す人を見てるだけの仕事 */
		if (++m_timer > 120)
		{
			// 次のステップへ！
			NextStep();

			// 効果のないTDN波紋を出してもらう！
			PersonMgr.ProductRipple();

		}
		break;
	case 2:
		/* 一定時間TDN波紋を見てるだけの仕事 */
		if (++m_timer > 180)
		{
			// 次のステップへ！
			NextStep();

			// 次の目標座標を設定(最初の位置)
			m_NextPos = m_StartPos, m_NextTarget = m_StartTarget;
		}
		break;
	case 3:
		/* スタート地点に帰るだけの仕事 */
		if (++m_timer > 120)
		{
			// カメラの仕事は終わり！
			//MsgMgr->Dispatch(
			//	MSG_NO_DELAY,
			//	ENTITY_ID::CAMERA_MNG,
			//	ENTITY_ID::,
			//	MESSAGE_TYPE::END_INTRO_CAMRA,
			//	nullptr	// 追記なし
			//	);

			// 通常の固定カメラに切り替え(今はイントロのステートじゃないよというのがsceneMainStateに対するメッセージ)
			pCamera->GetFSM()->ChangeState(FixCameraState::GetInstance());
		}
		break;
	}

	// カメラの情報更新
	pCamera->SetPos(cPos);
	pCamera->SetTarget(cTarget);
}

// 出口
void IntroCameraState::Exit(CameraManager *pCamera)
{

}

void IntroCameraState::Render(CameraManager * pCamera)
{
}

bool IntroCameraState::OnMessage(CameraManager *pCamera, const Message &msg)
{
	if (msg.Msg == MESSAGE_TYPE::START_INTRO_CAMERA)
	{
		// 命令が来たので働きます！
		m_bAction = true;

		// オリジナル構造体のポインタ―型で受け取るぜ！
		INTRO_CAMERA_INFO* exInfo = (INTRO_CAMERA_INFO*)msg.ExtraInfo;

		// 噂をスタートさせる人の座標を受け取った
		m_StartPersonPos = exInfo->start_person_pos;
		return true;
	}

	else
	{
		MyAssert(0,"例外: 意図しないメッセージが送信されました　byイントロカメラ")
		return false;
	}
}


//*****************************************************************************************************************************
//			エフェクトカメラ
//*****************************************************************************************************************************
EffectCameraState* EffectCameraState::GetInstance()
{
	// ここに変数を作る
	static EffectCameraState instance;
	return &instance;
}

// 入り口
void EffectCameraState::Enter(CameraManager *pCamera)
{
	// スクリプトIN
	textLoader::Open("DATA/Text/Camera/Effect.txt");
	time_line_data = nullptr;
}

// 実行中
void EffectCameraState::Execute(CameraManager *pCamera)
{
	data_cursor = -1;

	if (time_line_data)
	{
		for (int i = 0; i < time_line_data->data_count; i++)
		{
			if (current_frame >= time_line_data->data[i].start_frame &&
				current_frame <= time_line_data->data[i].end_frame)
			{
				data_cursor = i;
				Vector3 pos, target;
				GetTimeLineCameraPos(&pos, &target);

				// カメラのアングルに応じて座標回転
				pos.z = (pos.z * cosf(pCamera->GetOrgAngle())) - (pos.x * sinf(pCamera->GetOrgAngle()));
				pos.x = (pos.z * sinf(pCamera->GetOrgAngle())) + (pos.x * cosf(pCamera->GetOrgAngle()));

				// カメラが設定した原点を0とする
				pos += pCamera->GetOrgPos();
				target += pCamera->GetOrgPos();

				// カメラに情報セット
				pCamera->SetPos(pos);
				pCamera->SetTarget(target);
				break;
			}
		}

		// 再生終了したら
		if (++current_frame > time_line_data->data[time_line_data->data_count - 1].end_frame)
		{
			// 再生フレームを0にする
			current_frame = 0;

			// ステートを前のステートに戻す
			pCamera->GetFSM()->ChangeState(pCamera->GetFSM()->GetPreviousState());

			// 解放処理
			for (int i = 0; i < time_line_data->data_count; i++)
			{
				delete time_line_data->data[i].pos_array;
				delete time_line_data->data[i].target_array;
			}
			delete time_line_data->data;
			delete time_line_data, time_line_data = nullptr;
		}
	}
}

// 出口
void EffectCameraState::Exit(CameraManager *pCamera)
{
	if (time_line_data)
	{
		for (int i = 0; i < time_line_data->data_count; i++)
		{
			delete time_line_data->data[i].pos_array;
			delete time_line_data->data[i].target_array;
		}
		delete time_line_data->data;
		delete time_line_data;
	}
}

void EffectCameraState::Render(CameraManager * pCamera)
{
}

bool EffectCameraState::OnMessage(CameraManager *pCamera, const Message &msg)
{
	// 出ていけ！
	return false;
}


void EffectCameraState::GetTimeLineCameraPos(Vector3 *out_pos, Vector3 *out_target)
{
	if (data_cursor == -1) return;
	if (time_line_data->data[data_cursor].fix)
	{
		*out_pos = time_line_data->data[data_cursor].pos_array[0];
		*out_target = time_line_data->data[data_cursor].target_array[0];
	}

	// ベジエ計算関数に丸投げ
	Math::Bezier(
		out_pos,																	// 受け皿
		time_line_data->data[data_cursor].pos_array,								// 始点、中間、終点が入ってる座標配列
		time_line_data->data[data_cursor].num_elements,								// の要素数
		(float)(current_frame - time_line_data->data[data_cursor].start_frame) / time_line_data->data[data_cursor].frame_len
		);

	Math::Bezier(
		out_target,
		time_line_data->data[data_cursor].target_array,								// 始点、中間、終点が入ってる座標配列
		time_line_data->data[data_cursor].num_elements,								// の要素数
		(float)(current_frame - time_line_data->data[data_cursor].start_frame) / time_line_data->data[data_cursor].frame_len
		);
}