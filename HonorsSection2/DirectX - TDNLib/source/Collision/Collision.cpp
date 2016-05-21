#include	"Collision.h"
#include	"../Data/DataMNG.h"
#include	"../Enemy/watchman.h"
#include	"../Sheep/Sheep.h"
#include	"../CurvePoint/CurvePoint.h"
#include	"../Stage/StageMNG.h"
#include	"../CurvePoint/CurvePoint.h"
#include	"../Sound/SoundManager.h"
#include	"../UI/UIManager.h"

namespace{
	int GOAL_X = 0;	// �r�̃S�[�����W(�e�L�X�g�œǂ�)

	const int DOG_SIZE = 128;	// ���̃T�C�Y
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

void CollisionManager::Update(SheepManager* sinnMNG, Watchman_mng* manMNG, DataManager* dataMNG, StageManager* stageMNG)
{
	for(auto &sinIterator : *sinnMNG->Get_list()){

		//����ɂ傤�E���i���_&Time�㏸����
		if( EscapedShinnnyo(sinIterator) ){
			if (sinIterator->col_check) continue;

			sinIterator->col_check = true;
			dataMNG->AddTime(sinIterator->Get_floor(),
							 stageMNG->GetPopupPos(sinIterator->Get_floor(),
							 sinIterator->Get_pos()->x > 640 ? true : false));		//���_�ATime�㏸

			// ��UI����ɒm�点��
			UIMNG.AddScore(dataMNG->AddScore(sinIterator->Get_floor()));	// �f�[�^�}�l�[�W���[�̃X�R�A���Z�֐��ŉ��Z�����X�R�A��Ԃ�����A�����UI�ɓn��
			UIMNG.ConboCount();
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
		for(auto& manIterator : *manMNG->Get_list()){
			if (ShinnnyoAndExclamationPoint(sinIterator, &manIterator))
{
			if (sinIterator->col_check) continue;

			sinIterator->col_check = true;
			dataMNG->SubTime_Kill(sinIterator->Get_floor(), *sinIterator->Get_pos());		//���Ԃ�����������
			se->Play("DAMAGE");
		}

	}

		
	}

	//���ƘT
	for(auto& manIterator : *manMNG->Get_list()){
		for (auto& dogIterator : *stageMNG->GetCurvePointList(manIterator.Get_floor()))
		{
			if (ExclamationDogAndWolf(dogIterator, &manIterator)){
				if (manIterator.col_check)continue;
				manIterator.col_check = true;
			}
		}
	}
}

bool CollisionManager::ShinnnyoAndExclamationPoint(Sheep* sinn, Watchman* man)
{
	Vector2 sinnPos, manPos;
	sinn->Get_pos2(sinnPos);
	man->Get_pos2(manPos);
	Vector2 diff = sinnPos - manPos;
	if( man->Get_floor() != sinn->Get_floor() ) return false;	//�K�w���Ⴄ
	int size = (sinn->Get_size() + man->Get_size())/2;
	if( diff.x*diff.x < size*size ){
		sinn->Be_caught(man->Get_type());		//����ɂ傤�ɕ߂܂�������������
		return true;
	}
	return false;
}

bool CollisionManager::EscapedShinnnyo(Sheep* sinn)
{
	Vector2 pos;
	sinn->Get_pos2(pos);

	return (pos.x >= GOAL_X);
}

bool CollisionManager::ExclamationPointAndCurvePoint(Sheep* sheep, CurvePoint* cp)
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

bool CollisionManager::ExclamationDogAndWolf(CurvePoint *cp, Watchman* man)
{
	Vector2 WolfPos;
	Vector2 DogPos;
	man->Get_pos2(WolfPos);
	cp->Get_pos2(DogPos);
	if ((DogPos.x - WolfPos.x)*(DogPos.x - WolfPos.x) < cp->GetWidth() * cp->GetWidth()){
		if (cp->IsOpening()){
			man->Enter();
			return true;
		}
	}
	return false;
}


void CollisionManager::DebugRender(SheepManager* sinnMNG, Watchman_mng* manMNG, DataManager* dataMNG, StageManager* stageMNG)
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