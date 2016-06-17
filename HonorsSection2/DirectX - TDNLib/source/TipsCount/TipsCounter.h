#pragma once

// Tips�Ɏg�������ȏ����Ǘ�����N���X<Singleton>
class TipsCounter
{
public:
	// ���̂̎擾
	static TipsCounter *GetInstance(){ static TipsCounter i; return &i; }

	// �f�X�g���N�^
	~TipsCounter();

	// UI�̐��l���Q��
	struct
	{
		int *pScore;		// �X�R�A
		int *pWolfKill;		// ����ӂ͂��ҁ[
		int *pSheepKill;	// �r���񂾐�
	}m_ReferenceUI;


	int *m_NikuSheepTypes;	// ��������̎��(�T�ɉ��̓���H�킹����) :�m�[�}�����΂�����Ȃ悤�ł���΁A�ق��̓����Ă��邱�Ƃ������Ă�����
	int *m_YakinikuSteps;	// �Ă������̎��(���`�ł�)
	int m_DogUse;			// ���g������
	int m_LaneChange;		// ���[���ς�����

	// �J�E���g���Z�b�g
	void Reset();
private:

	// Singleton�̍�@
	TipsCounter();
	TipsCounter(const TipsCounter&){}
	TipsCounter &operator=(const TipsCounter&){}
};


#define TipsCountMgr (TipsCounter::GetInstance())