#pragma once

//*****************************************************************************************************************************
//
//		人間の配置情報とか読み込むクラス(スタティッカー)
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