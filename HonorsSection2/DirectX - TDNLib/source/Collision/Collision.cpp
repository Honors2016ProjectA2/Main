#include "Collision.h"
#include "../Data/DataMNG.h"
#include "../CurvePoint/CurvePoint.h"
#include "../Stage/StageMNG.h"
#include "../Sound/SoundManager.h"
#include "../UI/UIManager.h"
#include "../Bokusou/Bokusou.h"
#include "../Number/Number.h"
#include "../Shake/Shake.h"
#include "Effect\EffectManager.h"
#include "../Niku/Niku.h"
#include "../PostEffect/PostEffect.h"

namespace{
	int GOAL_X = 0;	// �r�̃S�[�����W(�e�L�X�g�œǂ�)

	const int DOG_SIZE = 64;	// ���̃T�C�Y
	const int CENTER = 1280/2;
}

CollisionManager::CollisionManager()
{
	// �e�L�X�g����p�����[�^�擾
	std::ifstream ifs("DATA/Text/Param/collision.txt");

	char skip[64];	// �ǂݔ�΂��p�ϐ�

	// �r�S�[�����W�ǂݍ���
	ifs >> skip;
	ifs >> GOAL_X;
}

void CollisionManager::Update(SheepManager* sinnMNG, DataManager* dataMNG, StageManager* stageMNG)
{
	for (auto &sinIterator : *sinnMNG->Get_list()){

		Vector2 sPos = sinIterator->GetCenterPos();

		// �v���C���[�ɒ͂܂�Ă���
		if (sinIterator->isCaught())
		{
			// ���܂�����ĂȂ�������
			if (!NikuMgr->GetYakiniku() && !NikuMgr->GetNiku())
			{
				// �r�Ɖ΂̔���
				if ((sPos - (YAKINIKU_AREA + Vector2(128, 128))).LengthSq() < 60 * 60)
				{
					// �ē��J�n
					NikuMgr->StartYakiniku(sinIterator->GetType());

					// �΃G�t�F�N�g
					EffectMgr.AddEffect((int)YAKINIKU_AREA.x + 128, (int)YAKINIKU_AREA.y +128, EFFECT_TYPE::BURN);

					// �r����
					sinIterator->Erase();
				}
			}
			continue;
		}

		// �r�Ɩq���Ԃ̔���
		for (auto &kusaIterator : *BokusouMgr->GetList())
		{
			if (kusaIterator->GetFloor() != sinIterator->Get_floor()) continue;	// ���[���Ⴄ

			Vector2 kPos = kusaIterator->GetCenterPos();

			// ���͈͓�
			if (sPos.x < kPos.x && sPos.x > kPos.x - 128)
			{
				// ����������ő������r�쐬
				if (kusaIterator->GetMode() == BOKUSOU_MODE::BORN){
					sinnMNG->CreateFatSheep(sinIterator);
					EffectMgr.AddEffect((int)sPos.x + 50, (int)sPos.y - 50, EFFECT_TYPE::ClEAR);
					EffectMgr.AddEffect((int)sPos.x - 100, (int)sPos.y, EFFECT_TYPE::PUSH);
					EffectMgr.AddEffect((int)sPos.x + 50, (int)sPos.y - 50, EFFECT_TYPE::EAT);
					// SE�̍Đ�
					se->Play("�ӂƂ�", kusaIterator->GetCenterPos());
				}

				// ������
				kusaIterator->Erase();
			}
		}

		//����ɂ傤�E���i���_&Time�㏸����
		if (EscapedShinnnyo(sinIterator)){
			if (sinIterator->col_check) continue;

			sinIterator->col_check = true;
			dataMNG->AddTime(sinIterator->Get_floor(),
				stageMNG->GetPopupPos(sinIterator->Get_floor(),
				sinIterator->Get_pos()->x > 640 ? true : false));		//���_�ATime�㏸

			//A��� �r���S�[�������u��
			EffectMgr.AddEffect((int)sinIterator->Get_pos()->x, (int)sinIterator->Get_pos()->y+64, EFFECT_TYPE::INEFFECT_MINI);
			//// �v���W�F�N�V�����̃|�W�V���������
			//Vector2 projPos = Math::GetProjPos(Vector2((float)sinIterator->Get_pos()->x + 220,
			//	(float)sinIterator->Get_pos()->y + 64));
			//// ���˃u���\�����I
			//PostEffectMgr.SetRadialBlur(projPos, 15);

			// �X�R�A���Z����
			SetScore(dataMNG, sinIterator->Get_floor(), sinIterator->TokutenBairitsu());

			continue;
		}

		// �r�ƌ�
		for (auto& dogIterator : *stageMNG->GetDogList(sinIterator->Get_floor())){
			if (ExclamationPointAndCurvePoint(sinIterator, dogIterator)){
				sinIterator->SetCurve(dogIterator->GetDir());	// �r�ɋȂ��ꖽ�߂��o���I
				//if (manIterator.col_check)continue;
				//manIterator.col_check = true;
				//if (manIterator.Get_type() != 1)// �I����m�[�}���Ȃ�
				//stageMNG->CurvePointLock(manIterator.Get_floor());
				//uiMNG->SetPopup(dataMNG->GetDiffTime(), dataMNG->GetPos());
			}
		}

		/*
		// �r�Ɖ΂̃_���[�W����
		for (auto &fireIt : *stageMNG->GetFireList(sinIterator->Get_floor()))
		{
			// �܂������ĂȂ�������through
			if (!fireIt->IsOpening()) continue;

			Vector2 fPos;
			Vector2 sPos2;
			sinIterator->Get_pos2(sPos2);
			sPos.x += sinIterator->Get_size();
			fireIt->Get_pos2(fPos);


			// �΂ɂ���������̏��
			if (fireIt->GetMode() == CurvePoint::FIRE_MODE::HITED)
			{
				// �r���W��X�����̍��W�Ƃ��̑O���͈͓�
				if (sPos.x > fPos.x - DOG_SIZE && sPos.x < fPos.x)
				{
					sinIterator->SetCurve(fireIt->GetDir());	// �r�ɋȂ��ꖽ�߂��o���I
				}
				break;
			}

			// ����
			if ((sPos - fireIt->GetCenterPos()).LengthSq() < 64 * 64)
			{
				// �΂̍ŏI�i�K�Ȃ�
				if (fireIt->GetMode() == CurvePoint::FIRE_MODE::ENABLE && !sinIterator->col_check)
				{

					sinIterator->Be_crushed();		// ����
					sinIterator->col_check = true;

					se->Play("�Ă���", *sinIterator->Get_pos());
					se->Play("�ߖ�", sinIterator->GetCenterPos());

					NikuMgr->CreateNiku(sinIterator->GetCenterPos().x, sinIterator->Get_floor());

					// �q�b�g
					fireIt->Hit();
				}
				// ����ȊO
				//else fireIt->Change();// ������
			}

		}
		*/

		//�rVS�T�i�_���[�W����
		for (auto& manIterator : *EnemyMgr->GetList())
		{
			// ��΂�
			if (manIterator->GetMode() != Enemy::Wolf::MODE::RUN || manIterator->GetFloor() != sinIterator->Get_floor()) continue;

			if (ShinnnyoAndExclamationPoint(sinIterator, manIterator))
			{
				// ���A���r����Ȃ�������
				if (sinIterator->GetType() != SHEEP_TYPE::REAL)
				{
					if (!sinIterator->col_check)
					{
						sinIterator->Be_crushed();		//����ɂ傤�ɕ߂܂�������������

						//A��� �T�Ɨr�����������u��
						EffectMgr.AddEffect((int)sinIterator->Get_pos()->x + 96, (int)sinIterator->Get_pos()->y + 64, EFFECT_TYPE::HIT);

						sinIterator->col_check = true;

						// UI�ɗr�����񂾐�
						UIMNG.AddSheepKillCount();

						UIMNG.AddFlame(-20);// ���_�ɂ��Ē��_

						// SE�̍Đ�
						se->Play("DAMAGE", sinIterator->GetCenterPos());
					}
				}
				else
				{
					if (!manIterator->col_check)
					{
						// �T������
						manIterator->Kill();

						//A��� �T�Ɨr�����������u��
						EffectMgr.AddEffect((int)sinIterator->Get_pos()->x + 96, (int)sinIterator->Get_pos()->y + 64, EFFECT_TYPE::HIT);

						// 241(�悢)�_
						UIMNG.AddScore(241);
						// �i���o�[�G�t�F�N�g����
						NumberEffect.AddNumber((int)manIterator->GetCenterPos().x, STAGE_POS_Y[manIterator->GetFloor()] + 32, 241, Number_Effect::COLOR_TYPE::WHITE);

						manIterator->col_check = true;
						se->Play("���A��HIT", sinIterator->GetCenterPos());

						// UI�ɘT�������
						UIMNG.AddWorfHappyCount();

						// �G�Ǘ��ɘT�J�E���g
						EnemyMgr->CheckChangeSpeed(UIMNG.GetWorfHappyCount());
					}
				}
			}
		}


	}	// �r��for��

	// �������r���[�v
	for (auto &fatIt : *sinnMNG->GetFatList())
	{
		////�rVS�T�i�_���[�W����
		//for (auto& manIterator : *EnemyMgr->GetList())
		//{
		//	// ��΂�
		//	if (manIterator->GetMode() != Enemy::Wolf::MODE::RUN || manIterator->GetFloor() != fatIt->GetFloor()) continue;
		//	Vector2 fPos = fatIt->GetCenterPos(), wolfPos = manIterator->GetCenterPos();
		//	Vector2 diff = fPos - wolfPos;
		//	int size = (120 + 240) / 2;
		//	if (diff.x*diff.x < size*size){
		//		//A��� �T�Ɨr�����������u��
		//		EffectMgr.AddEffect((int)fatIt->GetCenterPos().x + 96, (int)fatIt->GetCenterPos().y + 64, EFFECT_TYPE::HIT);
		//		fatIt->Erase();
		//		se->Play("DAMAGE", manIterator->GetCenterPos());
		//	}
		//}

		// �E�o����
		if (EscapedFatSheep(fatIt))
		{
			//A��� �������r���S�[�������u��
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x-96, (int)fatIt->GetCenterPos().y , EFFECT_TYPE::PLUS);
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x-256, (int)fatIt->GetCenterPos().y , EFFECT_TYPE::INEFFECT);
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x - 306, (int)fatIt->GetCenterPos().y+40, EFFECT_TYPE::DON);

			// �v���W�F�N�V�����̃|�W�V���������
			Vector2 projPos = Math::GetProjPos(Vector2(
				(float)fatIt->GetCenterPos().x - 96,
				(float)fatIt->GetCenterPos().y + 64));
			// ���˃u���\�����I
			PostEffectMgr.SetRadialBlur(projPos, 18);


			// �r�̃^�C�v�ɉ����ĕ���
			float Bairitsu;
			switch (fatIt->GetType())
			{
			case SHEEP_TYPE::NOMAL:
				Bairitsu = 1000.0f;
				break;
			case SHEEP_TYPE::GOLD:
				Bairitsu = 5000.0f;
				break;
			case SHEEP_TYPE::REAL:
				Bairitsu = 10000.0f;
				break;
			}
			SetScore(dataMNG, fatIt->GetFloor(), Bairitsu);	// 10000�{
			se->Play("�������rIN");

			// ��炷�I�I
			ShakeMgr->Set();

			// ����
			fatIt->Erase();
			continue;
		}

		int HitCount(0);	// �������r�ɂ������Ă鐔
		for (auto &sheepIt : *sinnMNG->Get_list())
		{
			if (fatIt->GetFloor() != sheepIt->Get_floor() || !sheepIt->isPushOK()) continue;	// ���[���Ⴄ

			Vector2 sPos = sheepIt->GetCenterPos();
			Vector2 fPos = fatIt->GetCenterPos();

			// �����Ă�r�͈͓�
			if (sPos.x < fPos.x && sPos.x > fPos.x - 192)
			{
				// �����������J�E���g
				HitCount++;

				// �����ĂȂ��r�����炷
				if (fPos.x - sPos.x < 128)
				{
					//sheepIt->SetPos(fPos - Vector2(128, 0));
					sheepIt->SetPosX(fPos.x - 128);
				}

				// �����Ă郂�[�h�ɂ���
				sheepIt->Be_Push();
			}
			else sheepIt->Be_Walk();
		}
		// �����������ɉ����đ����Ă�r�̈ړ��ʍ쐬
		fatIt->SetAccel((HitCount * sinnMNG->FatSheepAccel()));
	}

	// �������T�Ɨr
	for (auto &fatIt : *EnemyMgr->GetFatList())
	{
		// �E�o����
		if (fatIt->GetCenterPos().x >= 1400)
		{
			//A��� �������r���S�[�������u��
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x - 96, (int)fatIt->GetCenterPos().y, EFFECT_TYPE::PLUS);
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x - 256, (int)fatIt->GetCenterPos().y, EFFECT_TYPE::INEFFECT);

			// �v���W�F�N�V�����̃|�W�V���������
			Vector2 projPos = Math::GetProjPos(Vector2(
				(float)fatIt->GetCenterPos().x - 96,
				(float)fatIt->GetCenterPos().y + 64));
			// ���˃u���\�����I
			PostEffectMgr.SetRadialBlur(projPos, 15);

			// �����Ă�^�C�v�ɉ����Ď��Ԃ�ݒ�
			int AddTime;

			switch (fatIt->GetSheepType())
			{
			case SHEEP_TYPE::NOMAL:
				switch (fatIt->GetType())
				{
				case FAT_WOLF_TYPE::SMALL:
					AddTime = 1;
					break;
				case FAT_WOLF_TYPE::MIDDLE:
					AddTime = 5;
					break;
				case FAT_WOLF_TYPE::LARGE:
					AddTime = 10;
					break;
				}
				break;
			case SHEEP_TYPE::GOLD:
				switch (fatIt->GetType())
				{
				case FAT_WOLF_TYPE::SMALL:
					AddTime = 3;
					break;
				case FAT_WOLF_TYPE::MIDDLE:
					AddTime = 8;
					break;
				case FAT_WOLF_TYPE::LARGE:
					AddTime = 15;
					break;
				}
				break;
			case SHEEP_TYPE::REAL:
				switch (fatIt->GetType())
				{
				case FAT_WOLF_TYPE::SMALL:
					AddTime = 5;
					break;
				case FAT_WOLF_TYPE::MIDDLE:
					AddTime = 10;
					break;
				case FAT_WOLF_TYPE::LARGE:
					AddTime = 20;
					break;
				}
				break;
			}

			// UI�Ɏ��Ԃ𑫂�
			//UIMNG.AddTimer(AddTime);
			Vector3 startPos =
				Vector3(fatIt->GetCenterPos().x-128, fatIt->GetCenterPos().y, 0.0f);
			Vector3 centerPos = Vector3(400, 600, 0);
			Vector3 center2Pos = Vector3(100, 300, 0);
			Vector3 endPos =	Vector3(720, 40, 0);

			UIMNG.AddSendPower("Data/power.png", startPos, centerPos, center2Pos, endPos, 48, AddTime);

			// UI�ɘT�������
			UIMNG.AddWorfHappyCount();

			// �G�Ǘ��ɘT�J�E���g
			EnemyMgr->CheckChangeSpeed(UIMNG.GetWorfHappyCount());

			se->Play("�������rIN");

			// ��炷�I�I
			ShakeMgr->Set();

			// ����
			fatIt->Erase();
			continue;
		}

		int HitCount(0);	// �������r�ɂ������Ă鐔
		for (auto &sheepIt : *sinnMNG->Get_list())
		{
			if (fatIt->GetFloor() != sheepIt->Get_floor() || !sheepIt->isPushOK()) continue;	// ���[���Ⴄ

			Vector2 sPos = sheepIt->GetCenterPos();
			Vector2 fPos = fatIt->GetCenterPos();

			// �����Ă�r�͈͓�
			if (sPos.x < fPos.x && sPos.x > fPos.x - (fatIt->GetRadius() + 64))
			{
				// �����������J�E���g
				HitCount++;

				float vx = fPos.x - sPos.x;

				// �����ĂȂ��r�����炷
				if (vx < fatIt->GetRadius())
				{
					//sheepIt->SetPos(fPos - Vector2(128, 0));
					sheepIt->SetPosX(fPos.x - fatIt->GetRadius());
				}

				// �����Ă郂�[�h�ɂ���
				sheepIt->Be_Push();
			}
			else sheepIt->Be_Walk();
		}
		// �����������ɉ����đ����Ă�r�̈ړ��ʍ쐬
		fatIt->SetAccel((HitCount * sinnMNG->FatSheepAccel()));
	}

	//���ƘT
	for(auto& manIterator : *EnemyMgr->GetList()){
		Vector2 wPos = manIterator->GetCenterPos();
		for (auto& dogIterator : *stageMNG->GetDogList(manIterator->GetFloor()))
		{
			if (ExclamationDogAndWolf(dogIterator, manIterator)){

			}
		}

		// ��
		Niku *pNiku = NikuMgr->GetNiku();
		if (pNiku)
		{
			// �܂��ݒu���ĂȂ�������łĂ������I�I
			if (!pNiku->isSeted()) continue;

			Vector2 nPos = pNiku->GetCenterPos();

			if ((nPos - wPos).LengthSq() < 128 * 128)
			{
				// ���H���Ă郂�[�h�ɂ���
				manIterator->ChangeMode(Enemy::Wolf::MODE::NIKU);

				// ���̏�Ԃɉ����đ���T�̃^�C�v�ݒ�
				switch (pNiku->GetType())
				{
				case YAKINIKU_MODE::NAMA:
				case YAKINIKU_MODE::KOGETA:
					manIterator->SetFatType(FAT_WOLF_TYPE::SMALL);
					break;
				case YAKINIKU_MODE::RARE:
					manIterator->SetFatType(FAT_WOLF_TYPE::MIDDLE);
					break;
				case YAKINIKU_MODE::MEDIAM:
					manIterator->SetFatType(FAT_WOLF_TYPE::LARGE);
					break;
				}

				// ���̗r�̓����ŕ���
				manIterator->SetSheepType(pNiku->GetSheepType());

				// Delicious�I�I�I
				EffectMgr.AddEffect((int)pNiku->GetCenterPos().x + 8, (int)pNiku->GetCenterPos().y + 32, EFFECT_TYPE::DELICIOUS);
				EffectMgr.AddEffect((int)pNiku->GetCenterPos().x, (int)pNiku->GetCenterPos().y , EFFECT_TYPE::HAPPY);
				EffectMgr.AddEffect((int)pNiku->GetCenterPos().x, (int)pNiku->GetCenterPos().y, EFFECT_TYPE::HAPPY);

				// SE�̍Đ�
				se->Play("�p�N���O", manIterator->GetCenterPos());

				manIterator->SetCenterPos(pNiku->GetCenterPos());

				// ������
				pNiku->Erase();
			}
		}
	}
}

bool CollisionManager::ShinnnyoAndExclamationPoint(Sheep::Base* sinn, Enemy::Wolf* enemy)
{
	Vector2 sinnPos, wolfPos = enemy->GetCenterPos();
	sinn->Get_pos2(sinnPos);
	Vector2 diff = sinnPos - wolfPos;
	int size = (sinn->Get_size() + enemy->GetWidth())/2;
	if( diff.x*diff.x < size*size ){
		return true;
	}
	return false;
}

bool CollisionManager::EscapedShinnnyo(Sheep::Base* sinn)
{
	Vector2 pos;
	sinn->Get_pos2(pos);

	return (pos.x >= GOAL_X);
}

bool CollisionManager::EscapedFatSheep(FatSheep *fat)
{
	return (fat->GetCenterPos().x >= 1400);
}

bool CollisionManager::ExclamationPointAndCurvePoint(Sheep::Base* sheep, CurvePoint::Base* cp)
{
	// ���u���Ă���
	if (cp->IsOpening())
	{
		Vector2 SheepPos;
		Vector2 DogPos;
		sheep->Get_pos2(SheepPos);
		SheepPos.x += sheep->Get_size();
		cp->Get_pos2(DogPos);

		// �r���W��X�����̍��W�Ƃ��̑O���͈͓�
		if (SheepPos.x > DogPos.x - DOG_SIZE && SheepPos.x < DogPos.x){
			return true;
		}
	}
	return false;
}

bool CollisionManager::ExclamationDogAndWolf(CurvePoint::Base *cp, Enemy::Wolf* enemy)
{
	Vector2 WolfPos;
	Vector2 DogPos;
	WolfPos = enemy->GetCenterPos();
	cp->Get_pos2(DogPos);
	if ((DogPos.x - WolfPos.x)*(DogPos.x - WolfPos.x) < cp->GetWidth() * cp->GetWidth()){
		if (cp->IsOpening()){
			//man->Enter();
			return true;
		}
	}
	return false;
}


void CollisionManager::DebugRender(SheepManager* sinnMNG, DataManager* dataMNG, StageManager* stageMNG)
{
	for (int i = 0; i < STAGE_MAX; i++)
	{
		// �������蔻��
		for (auto it : *stageMNG->GetDogList(i))
		{
			if(it->IsOpening())tdnPolygon::Rect((int)it->GetPos().x - DOG_SIZE, (int)it->GetPos().y, DOG_SIZE, (int)it->GetWidth(), RS::COPY, 0x0fff0000);
		}
	}
}

void CollisionManager::SetScore(DataManager *dataMNG, int floor, float bairitsu)
{
	// ��UI����ɒm�点��
	int add = dataMNG->AddScore(floor, bairitsu);// �f�[�^�}�l�[�W���[�̃X�R�A���Z�֐��ŉ��Z�����X�R�A��Ԃ�����A�����UI�ɓn��
	UIMNG.AddScore(add);
	UIMNG.ConboCount();
	// �i���o�[�G�t�F�N�g����
	NumberEffect.AddNumber(1100, STAGE_POS_Y[floor] + 32, add);
}