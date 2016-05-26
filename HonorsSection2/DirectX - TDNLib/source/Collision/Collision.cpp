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

		// �r�Ɩq���Ԃ̔���
		for (auto &kusaIterator : *BokusouMgr->GetList())
		{
			if (kusaIterator->GetFloor() != sinIterator->Get_floor()) continue;	// ���[���Ⴄ

			Vector2 kPos = kusaIterator->GetCenterPos();

			// ���͈͓�
			if (sPos.x < kPos.x && sPos.x > kPos.x - 128)
			{
				// ����������ő������r�쐬
				if (kusaIterator->GetMode() == BOKUSOU_MODE::BORN)
					sinnMNG->CreateFatSheep(sinIterator);

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
			EffectMgr.AddEffect((int)sinIterator->Get_pos()->x+220, (int)sinIterator->Get_pos()->y+64, EFFECT_TYPE::INEFFECT_MINI);


			// �X�R�A���Z����
			SetScore(dataMNG, sinIterator->Get_floor(), sinIterator->TokutenBairitsu());

			continue;
		}

		// �r�ƌ�
		for (auto& dogIterator : *stageMNG->GetCurvePointList(sinIterator->Get_floor())){
			if (ExclamationPointAndCurvePoint(sinIterator, dogIterator)){
				sinIterator->SetCurve(dogIterator->GetDir());	// �r�ɋȂ��ꖽ�߂��o���I
				//if (manIterator.col_check)continue;
				//manIterator.col_check = true;
				//if (manIterator.Get_type() != 1)// �I����m�[�}���Ȃ�
				//stageMNG->CurvePointLock(manIterator.Get_floor());
				//uiMNG->SetPopup(dataMNG->GetDiffTime(), dataMNG->GetPos());
			}
		}

		//����ɂ傤�@�ƁI�i�_���[�W����
		for (auto& manIterator : *EnemyMgr->GetList()){
			if (ShinnnyoAndExclamationPoint(sinIterator, manIterator))
			{
				if (sinIterator->col_check) continue;

				//A��� �T�Ɨr�����������u��
				EffectMgr.AddEffect((int)sinIterator->Get_pos()->x + 96, (int)sinIterator->Get_pos()->y+64, EFFECT_TYPE::HIT);

				sinIterator->col_check = true;
				dataMNG->SubTime_Kill(sinIterator->Get_floor(), *sinIterator->Get_pos());		//���Ԃ�����������
				se->Play("DAMAGE",sinIterator->GetCenterPos());
			}

		}

	}	// �r��for��

	// �������r���[�v
	int FatSheepIndex(0);
	for (auto &fatIt : *sinnMNG->GetFatList())
	{
		// �E�o����
		if (EscapedFatSheep(fatIt))
		{
			//A��� �������r���S�[�������u��
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x-96, (int)fatIt->GetCenterPos().y , EFFECT_TYPE::PLUS);
			EffectMgr.AddEffect((int)fatIt->GetCenterPos().x+96, (int)fatIt->GetCenterPos().y , EFFECT_TYPE::INEFFECT);


			SetScore(dataMNG, fatIt->GetFloor(), 10000);	// 10000�{
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
			if (fatIt->GetFloor() != sheepIt->Get_floor()) continue;	// ���[���Ⴄ

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
					sheepIt->SetPos(fPos - Vector2(128, 0));
					//sheepIt->SetPosX(fPos.x - 128);
				}
			}
		}
		// �����������ɉ����đ����Ă�r�̈ړ��ʍ쐬
		fatIt->SetMove(HitCount * sinnMNG->FatSheepAccel());

		FatSheepIndex++;
	}

	//���ƘT
	for(auto& manIterator : *EnemyMgr->GetList()){
		for (auto& dogIterator : *stageMNG->GetCurvePointList(manIterator->GetFloor()))
		{
			if (ExclamationDogAndWolf(dogIterator, manIterator)){

			}
		}
	}
}

bool CollisionManager::ShinnnyoAndExclamationPoint(Sheep::Base* sinn, Enemy::Base* enemy)
{
	Vector2 sinnPos, wolfPos = enemy->GetCenterPos();
	sinn->Get_pos2(sinnPos);
	Vector2 diff = sinnPos - wolfPos;
	if( enemy->GetFloor() != sinn->Get_floor() ) return false;	//�K�w���Ⴄ
	int size = (sinn->Get_size() + enemy->GetWidth())/2;
	if( diff.x*diff.x < size*size ){
		sinn->Be_crushed();		//����ɂ傤�ɕ߂܂�������������
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

bool CollisionManager::ExclamationPointAndCurvePoint(Sheep::Base* sheep, CurvePoint* cp)
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

bool CollisionManager::ExclamationDogAndWolf(CurvePoint *cp, Enemy::Base* enemy)
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
		for (auto it : *stageMNG->GetCurvePointList(i))
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