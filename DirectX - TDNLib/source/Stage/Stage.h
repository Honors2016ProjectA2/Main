#pragma once

//*****************************************************************************************************************************
//
//		�l�Ԃ̔z�u���Ƃ��ǂݍ��ރN���X(�X�^�e�B�b�J�[)
//
//*****************************************************************************************************************************

class Stage
{
public:
	static int LoadPerson();
	static void SetStageNo(int no) { m_StageNo = no; }
	static int GetStageNo(){ return m_StageNo; }
private:
	static int m_StageNo;

};