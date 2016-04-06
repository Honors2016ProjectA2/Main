#include "TDNLIB.h"

#include "Camera.h"
#include "CameraState.h"
#include "../AI/Message/Message.h"
#include "../AI/Entity/BaseGameEntity.h"
#include "../AI/Entity/EntityManager.h"
#include "../AI/State/state.h"
#include "../AI/State/StateMachine.h"



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
void CameraGlobalState::Enter(Camera *pCamera)
{

}

// 実行中
void CameraGlobalState::Execute(Camera *pCamera)
{

}

// 出口
void CameraGlobalState::Exit(Camera *pCamera)
{

}

void CameraGlobalState::Render(Camera * pCamera)
{
}

//
bool CameraGlobalState::OnMessage(Camera *pCamera, const Message &msg)
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
void FixCameraState::Enter(Camera *pCamera)
{
	std::ifstream ifs("DATA/Camera/Fix.txt");
	assert(ifs);	// ファイル入ってない！

	char skip[32];	// 読み飛ばし用

	ifs >> skip;

	// 座標読み込み
	Vector3 pos;
	ifs >> pos.x, ifs >> pos.y, ifs >> pos.z;

	// 注視点読み込み
	Vector3 target;
	ifs >> target.x, ifs >> target.y, ifs >> target.z;
}

// 実行中
void FixCameraState::Execute(Camera *pCamera)
{
	// 固定カメラなので基本的に何もしない
}

// 出口
void FixCameraState::Exit(Camera *pCamera)
{

}

void FixCameraState::Render(Camera * pCamera)
{
}

bool FixCameraState::OnMessage(Camera *pCamera, const Message &msg)
{
	// 出ていけ！
	return false;
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
void EffectCameraState::Enter(Camera *pCamera)
{
	// スクリプトIN
	textLoader::Open("DATA/Text/Camera/Effect.txt");
	time_line_data = nullptr;
}

// 実行中
void EffectCameraState::Execute(Camera *pCamera)
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
void EffectCameraState::Exit(Camera *pCamera)
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

void EffectCameraState::Render(Camera * pCamera)
{
}

bool EffectCameraState::OnMessage(Camera *pCamera, const Message &msg)
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