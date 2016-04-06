#pragma once

//*****************************************************************************************************************************
//
//		�l�Ԃ̔z�u���Ƃ��ǂݍ��ރN���X<Singleton>
//
//*****************************************************************************************************************************

class Stage
{
public:

	// ���̎擾
	static Stage &GetInstance();

	// �e�L�X�g����l�Ԃ̔z�u���ǂݍ���
	int LoadPerson();

	// �X�e�[�W�ԍ��̃Z�b�^�[�ƃQ�b�^�[
	void SetStageNo(int no) { m_StageNo = no; }
	int GetStageNo(){ return m_StageNo; }

private:

	// �X�e�[�W�ԍ�
	int m_StageNo;

	// �K�v�B����
	int m_ClearCount;

	// Singleton�̍�@
	Stage(){}
	Stage(const Stage&){}
	Stage &operator=(const Stage&){}
};

#define StageMgr (Stage::GetInstance())