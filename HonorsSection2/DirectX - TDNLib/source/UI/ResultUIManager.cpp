#include "ResultUIManager.h"
#include "particle_2d\particle_2d.h"
#include "system\System.h"
#include "Effect\EffectManager.h"
#include "UIManager.h"
#include "../Sound/SoundManager.h"

ResultUIManager* ResultUIManager::inst = nullptr;

ResultUIManager & ResultUIManager::GetInstance()
{
	if (!inst)
	{
		inst = new ResultUIManager();
	}
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return *inst;
}

void ResultUIManager::Release()
{
	SAFE_DELETE(inst);
}



/***************************/
//	�������E���
/***************************/
ResultUIManager::ResultUIManager()
{
	// �A���t�@�X�N���[��
	m_resultScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::RENDERTARGET);
	m_screen = new tdn2DObj(tdnSystem::GetScreenSize().right, tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA);
	m_circleScreen = new tdn2DObj(tdnSystem::GetScreenSize().right, 
		tdnSystem::GetScreenSize().bottom, TDN2D::USEALPHA);

	tdnSystem::GetDevice()->GetRenderTarget(0, &pBackBuffer);

	m_step = STEP::START;

	isResultScreen = false;

	// �T�[�N��
	m_invCircle.pic = new tdn2DObj("Data/Result/invCircle.png");
	m_invCircle.scale = 1.0f;
	m_invCircle.x= 1057;
	m_invCircle.y= 427;
	m_invCircle.flame = 0;

	m_invCircle.startX = 1057;
	m_invCircle.startY = 427;
	m_invCircle.endX = 620;
	m_invCircle.endY = 130;
	m_invCircle.endFlame = 8;

	m_circle.pic = new tdn2DObj("Data/Result/Circle.png");
	m_circle.scale = 1.0f;
	m_circle.x = -874;
	m_circle.y = 474;
	m_circle.flame = 0;

	m_circle.startX = -874;
	m_circle.startY = 474;
	m_circle.endX = -283;
	m_circle.endY = 43;
	m_circle.endFlame = 6;

	// �t�H���g
	//m_font[RESULT].pic = new tdn2DAnim("DATA/Result/F_result.png");	
	//m_font[SCORE].pic = new tdn2DAnim("DATA/Result/F_score.png");
	//m_font[COMBO].pic = new tdn2DAnim("DATA/Result/F_combo.png");
	//m_font[WORF].pic = new tdn2DAnim("DATA/Result/F_worf.png");
	//m_font[FIRE].pic = new tdn2DAnim("DATA/Result/F_fire.png");
	//m_font[SUM].pic = new tdn2DAnim("DATA/Result/F_sum.png");
	m_sumFont.pic = new tdn2DAnim("DATA/Result/F_sum.png");

	//for (int i = 0; i < FONT::END; i++)
	//{	
	//	m_font[i].pic->OrderMoveAppeared(14, -360, i * 96);
	//	m_font[i].x = 40;
	//	m_font[i].y = (i * 92)+6;
	//	m_font[i].isSE = false;
	//}

	//// �����̌��ʏ�������
	//m_font[FONT::RESULT].pic->OrderShrink(6, 1.0f, 1.1f);
	//m_font[FONT::SUM].pic->OrderMoveAppeared(8, 0, 700);
	//m_font[FONT::SUM].x = 0;
	//m_font[FONT::SUM].y = 530;

	m_sumFont.x = 500;
	m_sumFont.y = 560;
	m_sumFont.pic->OrderMoveAppeared(8, m_sumFont.x, 700);
	m_sumFont.isSE = false;


	// ��
	m_senPic = new tdn2DAnim("DATA/Result/sen.png");
	m_senPic->OrderShrink(6, 1.0f, 1.5f);

	// �ւ���
	m_helpPic= new tdn2DAnim("DATA/Result/help.png");
	m_helpPic->OrderMoveAppeared(8, 0, 700);

	// ����
	for (int i = 0; i < FONT::END-1; i++)
	{
		m_number[i] = new Number("DATA/Number/Number.png",64);
		m_number[i]->GetAnim()->OrderMoveAppeared(14, -40, (i * 96) + 48);

		m_numX[i] = 650;
		m_numY[i] = (i * 94)+48;
		m_numNum[i] = 114514;
	}
	//m_number[FONT::SUM]->GetAnim()->OrderShrink(6, 1.0f, 1.5f);
	m_number[FONT::SUM] = new Number("DATA/Number/Number4.png", 96);
	m_numX[FONT::SUM] = 1150;
	m_numY[FONT::SUM] = 580;
	m_number[FONT::SUM]->GetAnim()->OrderMoveAppeared(8, m_numX[FONT::SUM], 700);
	m_numNum[FONT::SUM] = 0;
	//m_number[FONT::SUM]->GetAnim()->SetScale()

	m_WaitFlame = 0;

	// �����N ABCD
	m_RankPic = new tdn2DAnim("DATA/Result/rank.png");
	//m_RankPic->OrderMoveAppeared(12, 1500, 400);
	m_RankPic->OrderShrink(12, 1.0f, 2.5f);
	m_RankPicX = 100, m_RankPicY = 350;
	m_RankRipPic = new tdn2DAnim("DATA/Result/rank.png");
	m_RankRipPic->OrderRipple(12, 1.0f, 0.1f);

	isStopFlag = false;

	m_rankType = S;

	/**************************************/
	// �u���C���h
	m_blindPic = new tdn2DObj("DATA/UI/blind.png");
	m_blindSplit = 24;
	m_blindAngle = -1.57f;
	m_blindAngle = 0;
	m_maskScreen = new tdn2DObj(512, 512, TDN2D::RENDERTARGET);

	m_blindRete = 0.0f;
	shader2D->SetValue("blindRate", m_blindRete);
	
	m_rankingBG = new tdn2DObj("DATA/Result/RankingBG.png");
	m_rankingScreen = new tdn2DObj(512, 720, TDN2D::RENDERTARGET);

	m_rankingFont = new tdn2DObj("Data/Result/Ranking.png");

	/*****************************************/
	//
	for (int i = 0; i < RANKING_MAX; i++)
	{
		m_rankingNumber[i] = new Number();
		m_rankingNumber[i]->GetAnim()->OrderShake(60 * 120, 0, 16, 12);
	}


	Load();
	m_RankingPic = new tdn2DObj("Data/ranking.png");


	m_RankINPic = new tdn2DAnim("Data/Result/rankin.png");
	m_RankINPic->OrderMoveAppeared(24, 600, 100);

	m_resultBG = new tdn2DObj("Data/result/rizaruto.png");

	//
	m_SheepMGN = new ResultSheepManager(100, 250, -1000, 24);

	// 
	//m_boadScore = new tdn2DAnim("Data/result/boadScore.png");
	//m_boadScore->OrderMoveAppeared(14, -40, 300);

	//m_boadEx    = new tdn2DAnim("Data/result/boadEx.png");
	//m_boadEx->OrderMoveAppeared(14, -40, 800);

}

ResultUIManager::~ResultUIManager()
{
	// �Ō�Z�[�u
	Save();

	SAFE_DELETE(m_resultScreen);

	SAFE_DELETE(m_invCircle.pic);
	SAFE_DELETE(m_circle.pic);

	for (int i = 0; i < FONT::END; i++)
	{
		//SAFE_DELETE(m_font[i].pic);
		SAFE_DELETE(m_number[i]);
	}
	SAFE_DELETE(m_senPic);
	SAFE_DELETE(m_helpPic);
	SAFE_DELETE(m_RankPic);
	SAFE_DELETE(m_RankRipPic);

	SAFE_DELETE(m_blindPic);
	SAFE_DELETE(m_maskScreen);
	SAFE_DELETE(m_screen);
	SAFE_DELETE(m_rankingBG);
	SAFE_DELETE(m_rankingScreen);
	SAFE_DELETE(m_rankingFont);
	SAFE_DELETE(m_circleScreen);

	for (int i = 0; i < RANKING_MAX; i++)
	{
		SAFE_DELETE(m_rankingNumber[i]);
	}

	SAFE_DELETE(m_RankingPic);

	SAFE_DELETE(m_RankINPic);

	SAFE_DELETE(m_resultBG);

	SAFE_DELETE(m_SheepMGN);

	//SAFE_DELETE(m_boadScore);
	//SAFE_DELETE(m_boadEx);
	
	SAFE_DELETE(m_sumFont.pic);
	
}

void ResultUIManager::Init()
{
}

/***************************/
//	�X�V�E�`��
/***************************/
bool ResultUIManager::Update()
{
	if (isResultScreen == false)return false;// ���s����Ă��Ȃ������烊�^�[��

	// ���l���X�V
	m_numNum[FONT::SCORE] = UIMNG.GetScore();
	m_numNum[FONT::COMBO] = UIMNG.GetMaxCombo();
	m_numNum[FONT::WORF] = UIMNG.GetWorfHappyCount();
	m_numNum[FONT::FIRE] = UIMNG.GetSheepKillCount();

	//���ł̍��v�|�C���g
	m_MaxSumNum =
		m_numNum[FONT::SCORE] + (m_numNum[FONT::COMBO] * 1000) + m_numNum[FONT::WORF] * 10000 + m_numNum[FONT::FIRE] * 1;


	//m_MaxSumNum = 114514810;

	Debug();

	switch (m_step)
	{
	case ResultUIManager::STEP::START:
		// �~�̏������I�����Ă���
		if (CircleUpdate())
		{
			// ���������̓����̍X�V
			//for (int i = 0; i < FONT::END; i++)
			//{
			//	m_font[i].pic->Action((i * 38));
			//	m_number[i]->GetAnim()->Action((i * 38));
			//}
			////�@��
			//m_senPic->Action();
			//// �w���v
			//m_helpPic->Action(12);

			//// ������������
			//m_font[FONT::RESULT].pic->Action();
			//m_font[FONT::SUM].pic->Action(6);
			m_sumFont.pic->Action(6);

			//// ������������
			//m_number[FONT::RESULT]->GetAnim()->Stop();
			//m_number[FONT::SUM]->GetAnim()->Stop();
			
			//m_boadScore->Action();
			//m_boadEx->Action();

			m_SheepMGN->StartScoreSheep();

			// ���̃X�e�b�v��
			m_step = STEP::SCORE;
		}
		
		break;
	case ResultUIManager::STEP::SCORE:	// �X�R�A

		// �{�^��
		if (KeyBoardTRG(KB_Y))
		{
			// ���̃X�e�b�v��
			m_step = STEP::SONOTA;
		}
		if (m_SheepMGN->isScoreEnd())
		{
			// ���̃X�e�b�v��
			m_step = STEP::SONOTA;

			// ���̔���
			m_SheepMGN->StartOtherSheep();
		}
		

		break;
	case ResultUIManager::STEP::SONOTA:	// ���̑�

		// �{�^��
		if (KeyBoardTRG(KB_Y))
		{
			// ���̃X�e�b�v��
			m_step = STEP::EXE;
		}

		if (m_SheepMGN->isOtherEnd())
		{
			// ���̃X�e�b�v��
			m_step = STEP::EXE;

		}

		break;
	case ResultUIManager::STEP::EXE:
		// ���̃��[�h�̊ԂɃf�B���C���|����ꂽ���ڂ��o�Ă���

		m_WaitFlame++;
		if (m_WaitFlame >= 40)
		{
			m_WaitFlame = 0;
			// ���̃X�e�b�v��
			m_step = STEP::SUM_CALC;
			m_number[FONT::SUM]->GetAnim()->Action();
			m_numNum[FONT::SUM] = 0;

			// �e������SE�Đ�
			//se->Play("���U���g�X�R�A", true);
		}

		break;
	case ResultUIManager::STEP::SUM_CALC:

		if (tdnMouse::GetLeft()==3)
		{
			m_numNum[FONT::SUM] = m_MaxSumNum;
		}

		// �e�������������Ċ�����
		if (m_numNum[FONT::SUM] < m_MaxSumNum)
		{
			int len = abs(m_numNum[FONT::SUM] - m_MaxSumNum);
			if (m_numNum[FONT::SUM] < 10)
			{
				m_numNum[FONT::SUM] += 1;
			}
			else if (m_numNum[FONT::SUM] < 100)
			{
				m_numNum[FONT::SUM] += 12;
			}
			else if (m_numNum[FONT::SUM] < 1000)
			{
				m_numNum[FONT::SUM] += 123;
			}
			else if (m_numNum[FONT::SUM] < 10000)
			{
				m_numNum[FONT::SUM] += 1234;
			}
			else if (m_numNum[FONT::SUM] < 100000)
			{
				m_numNum[FONT::SUM] += 12345;
			}
			else if (m_numNum[FONT::SUM] < 1000000)
			{
				m_numNum[FONT::SUM] += 123456;
			}
			else
			{
				m_numNum[FONT::SUM] += 1234567;
			}

			// SE�P�� �Ă��ꂒ
			static int waitSE = 0;
			if (++waitSE >= 4) {
				waitSE = 0;
				se->Play("���U���g�X�R�A");
			}
			
		}
		else
		{
			m_numNum[FONT::SUM] = m_MaxSumNum;

			m_WaitFlame++;
			if (m_WaitFlame >= 24)
			{
				m_WaitFlame = 0;

				//���@���̃X�e�b�v��
				m_step = STEP::RANK;
				m_RankPic->Action();

				// �e������SE��~
				//se->Stop("���U���g�X�R�A", 0);

				RankUpdate();				// �����N�̍X�V
			}

		}

		break;
		case ResultUIManager::STEP::RANK:
			m_WaitFlame++;
			if (m_WaitFlame >= 12)
			{
				m_WaitFlame = 0;
				m_RankPic->OrderShake(8, 23.0f, 35.0f, 2);
				m_RankPic->Action();
				m_RankRipPic->Action();
				EffectMgr.AddEffect(m_RankPicX + 180, m_RankPicY + 180, EFFECT_TYPE::ClEAR);

				/*
				// �����N�\��SE�Đ�
				static const char *RANK_SE_ID[] = { "RANK_S", "RANK_A", "RANK_B", "RANK_C" };
				assert(_countof(RANK_SE_ID) >= m_rankType);	// �z��Q�ƊO
				se->Play((char*)RANK_SE_ID[m_rankType]);
				*/

				// �h�h���I
				se->Play("�h�h��");

				m_step = STEP::RANKING_START;
			}

		break;
	case ResultUIManager::STEP::RANKING_START:
		if (tdnMouse::GetLeft() == 3)
		{
			m_step = STEP::RANKING;

			// kokok�Ń\�[�g����
			Sort();

			
		}

		m_WaitFlame++;
		//if (m_WaitFlame >= 180)
		//{
		//	m_WaitFlame = 0;
		//	m_step = STEP::RANKING;
		//}

		break;
	case ResultUIManager::STEP::RANKING:

			m_blindRete += 0.1f;
			if (m_blindRete >= 1.0f)m_blindRete = 1.0f;
			shader2D->SetValue("blindRate", m_blindRete);

			if (m_blindRete == 1.0f)
			{
				m_step = STEP::END;
			}

		break;
	case ResultUIManager::STEP::END:
	{
		int a = 0;
		a++;

		// �I��B����񂿂��@�ӂ��������������������������������I�I�I�I�I
		if (tdnMouse::GetLeft() == 3)
		{
			return true;
		}

	}		break;
	default:
		break;

	}

	// ���������̓����̍X�V
	//for (int i = 0; i < FONT::END; i++)
	//{
	//	m_font[i].pic->Update();
	//}
	m_sumFont.pic->Update();

	// ��
	m_senPic->Update();
	// �w���v
	m_helpPic->Update();

	// ����
	for (int i = 0; i < FONT::END; i++)
	{
		m_number[i]->Update();
	}

	m_RankPic->Update();
	m_RankRipPic->Update();

	//  �����L���O�\��
	for (int i = 0; i < RANKING_MAX; i++)
	{
		m_rankingNumber[i]->Update();
	}

	StopUpdate();

	m_RankINPic->Update();


	/***************************/
	//	���Ȃ炷�|��
	/**************************/
	//for (int i = 0; i < FONT::END; i++)
	//{
	//	if (m_font[i].pic->GetAction()->IsEnd() == true)
	//	{
	//		// ����
	//		if (m_font[i].isSE == false)
	//		{
	//			se->Play("���U���g�_��");
	//			m_font[i].isSE = true;
	//		}
	//	}	
	//}

		//if (m_sumFont.pic->GetAction()->IsEnd() == true)
		//{
		//	// ����
		//	if (m_sumFont.isSE == false)
		//	{
		//		se->Play("���U���g�_��");
		//		m_sumFont.isSE = true;
		//	}
		//}	
	
	

	//if (KeyBoardTRG(KB_R))
	//{
	//	m_SheepMGN->StartScoreSheep();
	//}
	if (KeyBoardTRG(KB_ENTER))
	{
		m_SheepMGN->Reset();
	}
	m_SheepMGN->Update();

	// �{�[�h
	//m_boadScore->Update();
	//m_boadEx->Update();

	return false;
}

// �~�̏���
bool ResultUIManager::CircleUpdate()
{

	int moveX,moveY;
	moveX = abs(m_invCircle.startX - m_invCircle.endX);
	moveY = abs(m_invCircle.startY - m_invCircle.endY);
	moveX /= m_invCircle.endFlame;// ����
	moveY /= m_invCircle.endFlame;
	

	// +��-�ŕ���
	if (m_invCircle.startX > m_invCircle.endX)
	{
		// �Ō�̒l�̕����������@�|
		moveX = -moveX;
	}
	if (m_invCircle.startY > m_invCircle.endY)
	{
		// �Ō�̒l�̕����������@�|
		moveY = -moveY;
	}


	// ����Move�l��G��[�ޖ��ɉ��Z�@�{ �ŏ��̈ʒu
	m_invCircle.x = (moveX*m_invCircle.flame)+ m_invCircle.startX;
	m_invCircle.y = (moveY*m_invCircle.flame)+ m_invCircle.startY;


	// ���t���[�����Z
	m_invCircle.flame++;
	// �s���߂��h�~
	if (m_invCircle.flame >= m_invCircle.endFlame)
	{
		m_invCircle.flame = m_invCircle.endFlame;
	}

	// �K���Ɋg��I�I
	m_invCircle.scale += 0.15f;
	// �s���߂��h�~!!
	if (m_invCircle.scale >= 3.0f)
	{
		m_invCircle.scale = 3.0f;
	}
	m_invCircle.pic->SetScale(m_invCircle.scale);

	/**********************************************/
	// �����΂̕�
	/**********************************************/

	if (m_invCircle.scale >= 2.7f) // �������@�����������\�[�X�R�[�h�����A�����ǂ��Ȃ��Ă�����
	{
		moveX = abs(m_circle.startX - m_circle.endX);
		moveY = abs(m_circle.startY - m_circle.endY);
		moveX /= m_circle.endFlame;// ����
		moveY /= m_circle.endFlame;


		// +��-�ŕ���
		if (m_circle.startX > m_circle.endX)
		{
			// �Ō�̒l�̕����������@�|
			moveX = -moveX;
		}
		if (m_circle.startY > m_circle.endY)
		{
			// �Ō�̒l�̕����������@�|
			moveY = -moveY;
		}


		// ����Move�l��G��[�ޖ��ɉ��Z�@�{ �ŏ��̈ʒu
		m_circle.x = (moveX*m_circle.flame) + m_circle.startX;
		m_circle.y = (moveY*m_circle.flame) + m_circle.startY;


		// ���t���[�����Z
		m_circle.flame++;
		// �s���߂��h�~
		if (m_circle.flame >= m_circle.endFlame)
		{
			m_circle.flame = m_circle.endFlame;
		}

		// ����
		// �K���Ɋg��I�I
		// �s���߂��h�~!!

		m_circle.scale += 0.20f;
		if (m_circle.scale >= 3.0f)
		{
			m_circle.scale = 3.0f;
			return true;// �I��
		}

		m_circle.pic->SetScale(m_circle.scale);

	}// �S���N���������S���S����if��


	return false;
}

// �����N
void ResultUIManager::RankUpdate()
{
	// �_�����ƂɃ����N��t����
	if (m_MaxSumNum < 1000000)
	{
		m_rankType = C;
	}
	else if (m_MaxSumNum < 3000000)
	{
		m_rankType = B;
	}
	else if (m_MaxSumNum < 10000000)
	{
		m_rankType = A;
	}
	else
	{
		m_rankType = S;
	}
	

}

/***********************/
// �`��
void ResultUIManager::Render()
{

	if (isResultScreen == false)return;// ���s����Ă��Ȃ������烊�^�[��

	// ���U���gscreen
	ResultRender();
	// �����L���O�V�[��
	RankingRender();
	// �}�X�N�p
	MaskRender();
	// �T�C�N��
	CircleRender();

	m_screen->RenderTarget();
	tdnView::Clear();

	// �ŏ��̓A���t�@�l��
	if (m_step==STEP::START)
	{
		m_circleScreen->Render(0, 0);
		m_resultBG->Render(0, 0,shader2D,"mask");
	}
	else
	{
		// ���̎��̓A���t�@�l���Ȃ��̃����_�[�^�[�Q�b�g��
		//m_circleScreen->Render(0, 0);
		m_resultScreen->Render(0, 0);
		m_rankingScreen->Render(0, 0, shader2D, "blind");
		m_SheepMGN->Render();
		// �{�[�h
		//m_boadScore->Render(500,300);
		//m_boadEx->Render(500, 300);

	}
	//m_resultScreen->Render(0, 0);
	//m_rankingScreen->Render(780, 0, shader2D, "blind");

	// ����������`��
	tdnSystem::GetDevice()->SetRenderTarget(0, pBackBuffer);
	m_screen->Render(0, 0);

	// m_circleScreen->Render(0, 0, 1280/4, 720 / 4, 0, 0, 1280 , 720 );
	// m_maskScreen->Render(0, 0);

	//tdnText::Draw(100, 200, 0xff555555, "x=%d:y=%d", m_invCircle.x, m_invCircle.y);
	//tdnText::Draw(100, 300, 0xff555555, "x=%d:y=%d", m_circle.x, m_circle.y);
}


// �����_�[�^�[�Q�b�g
void ResultUIManager::ResultRender()
{
	//�@�����_�[���Q�b�g�؂�ւ�
	m_resultScreen->RenderTarget();
	tdnView::Clear();

	// �T�[�N������
	m_invCircle.pic->Render(m_invCircle.x, m_invCircle.y);
	m_circle.pic->Render(m_circle.x, m_circle.y);

	// �ǉ��i���������j
	m_resultBG->Render(0, 0);

	//// BG
	//m_rankingBG->Render(780, 0, shader2D, "blind");
	//// �����L���O�t�H���g
	//m_rankingFont->Render(780, 0, shader2D, "blind");

	// ��
	//m_senPic->Render(0, 0);
	// help
	//m_helpPic->Render(0, 650);
	// ��������
	//for (int i = 0; i < FONT::END; i++)
	//{
	//	m_font[i].pic->Render(m_font[i].x, m_font[i].y);
	//}
	m_sumFont.pic->Render(m_sumFont.x, m_sumFont.y);

	// ����
	for (int i = 0; i < FONT::END; i++)
	{
		m_number[i]->Render(m_numX[i], m_numY[i], m_numNum[i]);
	}

	m_RankPic->Render(m_RankPicX, m_RankPicY, 360, 360, 360* m_rankType, 0, 360, 360,RS::COPY_NOZ);
	m_RankRipPic->Render(m_RankPicX, m_RankPicY, 360, 360, 360 * m_rankType, 0, 360, 360,RS::ADD);

	// �X�C�b�`
	switch (m_step)
	{
	case ResultUIManager::STEP::START:
		break;
	case ResultUIManager::STEP::SCORE:
		tdnText::Draw(0, 90, 0xfff00fff, "Score");

		break;
	case ResultUIManager::STEP::SONOTA:
		tdnText::Draw(0, 90, 0xfff00fff, "EX");

		break;
	case ResultUIManager::STEP::EXE:
		break;
	case ResultUIManager::STEP::END:
		break;
	default:
		break;

	}


	//tdnText::Draw(500, 200, 0xff555555, "������傢");

}

void ResultUIManager::CircleRender()
{
	//�@�����_�[���Q�b�g�؂�ւ�
	m_circleScreen->RenderTarget();
	tdnView::Clear();

	// �T�[�N������
	m_invCircle.pic->Render(m_invCircle.x, m_invCircle.y);
	m_circle.pic->Render(m_circle.x, m_circle.y);

	// ���ŕ`�悵���G���}�X�N����	
	shader2D->SetValue("MaskTex", m_circleScreen);
}



void ResultUIManager::MaskRender()
{

	if (KeyBoard(KB_T) == 3)
	{
		m_blindSplit++;
	}
	if (KeyBoard(KB_R) == 3)
	{
		m_blindSplit--;
	}
	if (KeyBoard(KB_Y) == 3)
	{
		m_blindAngle -= 0.1f;
	}
	if (KeyBoard(KB_U) == 3)
	{
		m_blindAngle += 0.1f;
	}

	
	if (KeyBoard(KB_Q))
	{
		m_blindRete -= 0.1f;
	}
	if (KeyBoard(KB_E))
	{
		m_blindRete += 0.1f;
	}

	m_blindRete = Math::Clamp(m_blindRete, 0.0f, 1.0f);
	shader2D->SetValue("blindRate", m_blindRete);


	m_maskScreen->RenderTarget();
	tdnView::Clear();

	// �ۊǂ��Ȃ�
	tdnRenderState::Filter(false);

	// �u���C���h
	m_blindPic->SetScale(1.5f);
	m_blindPic->SetAngle(m_blindAngle);
	m_blindPic->Render(0, 0, 512, 512, 0, 0, 256 * m_blindSplit, 256 * m_blindSplit);


	tdnRenderState::Filter(true);

	// ���ŕ`�悵���G���}�X�N����	
	shader2D->SetValue("BlindTex", m_maskScreen);



}

/*********************************/
// �������L���O�V�[��
void ResultUIManager::RankingRender()
{
	m_rankingScreen->RenderTarget();
	tdnView::Clear();

	// BG
	m_rankingBG->Render(0, 0);

	// �����L���O�t�H���g
	m_rankingFont->Render(0, 0);

	//  �����L���O�\��
	for (int i = 0; i < RANKING_MAX; i++)
	{
		m_RankingPic->Render(10, (112 * i) + 205, 128, 128, 0, i * 128, 128, 128);
		m_rankingNumber[i]->Render(400, (112 * i) + 230, m_RankingNum[i]);

		//tdnText::Draw(0, 0, 0xffffffff, "������������������");
	}

	// �����N�C��
	m_RankINPic->Render(200, 120);

}

// ���[�h
void ResultUIManager::Load()
{
	std::string fileName = "DATA/Ranking.txt";
	// Load
	std::ifstream in(fileName.c_str());

	assert(in);
	if (in)
	{
		// �����L���O�擾
		for (int i = 0; i < RANKING_MAX; i++)
		{
			in >> m_RankingNum[i];		
		}	

	}


}

// �Z�[�u
void ResultUIManager::Save()
{
	std::string fileName = "DATA/Ranking.txt";
	// Load
	std::ofstream out(fileName.c_str());

	assert(out);
	if (out)
	{
		// �����L���O�擾
		for (int i = 0; i < RANKING_MAX; i++)
		{
			out << m_RankingNum[i] << "\n";
		}
	}

}

void ResultUIManager::Sort()
{
	m_MaxSumNum;
	for (int i = 0; i < RANKING_MAX; i++)
	{
		if (m_RankingNum[i] <= m_MaxSumNum) //���ł����ʂ̕�������������
		{
			int mae = m_RankingNum[i];

			// �J�艺������
			for (int i2 = i+1; i2 < RANKING_MAX; i2++)
			{
				int �O�P = m_RankingNum[i2];
				m_RankingNum[i2] = mae;

				mae = �O�P;
			}
			m_RankingNum[i] = m_MaxSumNum;
			m_rankingNumber[i]->Action();

			m_RankINPic->Action();// �����N�C���̃A�j����
			break;
		}	

	}

}

/******************/
//	�n��
/******************/
void ResultUIManager::Action()
{
	isResultScreen = true;
	m_step = STEP::START;


	// �T�[�N��������
	m_invCircle.scale = 1.0f;
	m_invCircle.x = 1057;
	m_invCircle.y = 427;
	m_invCircle.flame = 0;
	m_invCircle.pic->SetScale(m_invCircle.scale);

	m_circle.scale = 1.0f;
	m_circle.x = -874;
	m_circle.y = 474;
	m_circle.flame = 0;
	m_circle.pic->SetScale(m_circle.scale);

	// ��������
	//for (int i = 0; i < FONT::END; i++)
	//{
	//	m_font[i].pic->Stop();
	//}
	m_sumFont.pic->Stop();

	// ��
	m_senPic->Stop();
	// help
	m_helpPic->Stop();

	// ��������
	for (int i = 0; i < FONT::END; i++)
	{
		m_number[i]->GetAnim()->Stop();
	}

	m_WaitFlame = 0;

	m_RankPic->OrderShrink(12, 1.0f, 2.5f);
	m_RankPic->Stop();
	m_RankRipPic->Stop();

	m_screenAlpha = 255;
	m_screen->SetARGB(m_screenAlpha,255,255,255);

	isStopFlag = false;

	// �u���C���h
	m_blindRete = 0.0f;
	shader2D->SetValue("blindRate", m_blindRete);


	// ��������
	//for (int i = 0; i < FONT::END; i++)
	//{
	//	// ����
	//	m_font[i].isSE = false;
	//}
	m_sumFont.isSE = false;

}

// �I��
void ResultUIManager::Stop()
{
	isStopFlag = true;
}

void ResultUIManager::StopUpdate()
{
	if (isStopFlag)
	{
		m_screenAlpha -= 35;
		if (m_screenAlpha <= 0)
		{
			isResultScreen = false;
		}
		m_screen->SetARGB(m_screenAlpha, 255, 255, 255);
	}

}


// �ł΂�
void ResultUIManager::Debug()
{
	if (KeyBoard(KB_Z) == 3)
	{
		m_invCircle.flame++;
	}


	if (KeyBoard(KB_A))
	{
		m_invCircle.x--;
	}
	if (KeyBoard(KB_D))
	{
		m_invCircle.x++;
	}
	if (KeyBoard(KB_W))
	{
		m_invCircle.y--;
	}
	if (KeyBoard(KB_S))
	{
		m_invCircle.y++;
	}

	if (KeyBoard(KB_F))
	{
		m_circle.x--;
	}
	if (KeyBoard(KB_H))
	{
		m_circle.x++;
	}
	if (KeyBoard(KB_T))
	{
		m_circle.y--;
	}
	if (KeyBoard(KB_G))
	{
		m_circle.y++;
	}


}

