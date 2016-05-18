#include "Number.h"

/***********************************************/
// ���l�\���p�N���X
/***********************************************/

/***********************************************/
// �C���X�^���X��
/***********************************************/
//Number* Number::m_pInstance = nullptr;
//
//Number & Number::GetInstance()
//{
//	if (!m_pInstance)
//	{
//		m_pInstance = new Number();
//	}
//
//	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
//	return *m_pInstance;
//}
//
//void Number::Release()
//{
//	SAFE_DELETE(m_pInstance);
//}


/***********************************************/
// �������E���
/***********************************************/
Number::Number()
{
	m_pic = new tdn2DAnim("Data/Number/Number.png");
	m_pic->OrderJump(4, 1.0f, 1.3f);
	//m_pic->OrderShake(7, 10, 20, 2);
	//m_pic->OrderRipple(12, 1.0, 0.1f);
	//m_pic->OrderShrink(6, 1.0f, 3.0f);

	m_picSize = 64;
	m_picScale = 1;
}

Number::Number(char * name, int picSize)
{
	m_pic = new tdn2DAnim(name);
	m_pic->OrderJump(4, 1.0f, 1.3f);
	//m_pic->OrderShake(7, 10, 20, 2);
	//m_pic->OrderRipple(12, 1.0, 0.1f);
	//m_pic->OrderShrink(6, 1.0f, 3.0f);
	m_picSize = picSize;
	m_picScale = 1;

}

Number::~Number()
{
	SAFE_DELETE(m_pic);
}


/***********************************************/
// �X�V
/***********************************************/
void Number::Update()
{
	m_pic->Update();
}


/***********************************************/
// �`��
/***********************************************/
void Number::Render()
{

}

void Number::Render(int x, int y, int num, NUM_KIND kind)
{
	int number = num;

	//int DIGIT_MAX = 0;
	//int calcNum = num;
	//for (int i = 0;; i++)
	//{	
	//	calcNum = (int)(calcNum / 10);// ���l�̈�ԏ�������������
	//	if (calcNum <= 0)break;
	//	DIGIT_MAX++;
	//}

	// �����̎�ނɂ�蕪��
	switch (kind)
	{
	case Number::NUM_KIND::NORMAL:
		for (int count = 0;; ++count)
		{
			int digitNum = number % 10;	// ��ԏ������������
			number = (int)(number / 10);// ���l�̈�ԏ�������������

			m_pic->Render(x - ((count * (m_picSize /2))*m_picScale), y, m_picSize*m_picScale, m_picSize*m_picScale, digitNum*m_picSize, 0, m_picSize, m_picSize);// �����`��

			if (number <= 0)break;// ���l���O�ȉ��ɂȂ����炳��Ȃ�
		}
		break;
	case Number::NUM_KIND::HEAL:
		for (int count = 0;; ++count)
		{
			int digitNum = number % 10;	// ��ԏ������������
			number = (int)(number / 10);// ���l�̈�ԏ�������������

			m_pic->Render(x - ((count *32)*m_picScale), y, m_picSize*m_picScale, m_picSize*m_picScale, digitNum*m_picSize, 0, m_picSize, m_picSize);// �����`��

			if (number <= 0)break;// ���l���O�ȉ��ɂȂ����炳��Ȃ�
		}
		break;
	default:
		//for (int count = 0;; ++count)
		//{
		//	int digitNum = number % 10;	// ��ԏ������������
		//	number = (int)(number / 10);// ���l�̈�ԏ�������������

		//	m_pic->Render(x + (((DIGIT_MAX - count) * 32)*m_picScale), y, m_picSize*m_picScale, m_picSize*m_picScale, digitNum*m_picSize, 0, m_picSize, m_picSize);// �����`��

		//	if (number <= 0)break;// ���l���O�ȉ��ɂȂ����炳��Ȃ�
		//}
		break;
	}

}

// �A�N�V����
void Number::Action(int derayTime)
{
	m_pic->Action(derayTime);
}





/*****************************************************************/
//
//				�i���o�[�G�t�F�N�g
//
/*****************************************************************/

/***********************************************/
// �C���X�^���X��
/***********************************************/
Number_Effect* Number_Effect::m_pInstance = nullptr;

Number_Effect& Number_Effect::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new Number_Effect();
	}

	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return *m_pInstance;
}

void Number_Effect::Release()
{
	SAFE_DELETE(m_pInstance);
}


/***********************************************/
// �������E���
/***********************************************/
Number_Effect::Number_Effect()
{

}

Number_Effect::~Number_Effect()
{
	// �S��������
	for (auto it : m_NumberData)
	{
		SAFE_DELETE(it->number);
		SAFE_DELETE(it);
	}

	//�@�f�[�^�����
	m_NumberData.clear();
}


/***********************************************/
// �X�V
/***********************************************/
void Number_Effect::Update()
{
	// List
	for (auto it = m_NumberData.begin(); it != m_NumberData.end();)
	{

		//it->Update();

		//it->alpha += 21;
		//if (it->alpha > 255)
		//{
		//	it->alpha = 255;
		//}

		// �X�e�[�g�œ����𕪊�
		switch ((*it)->state)
		{
		case NumberData::STATE::START:

			// y���W�����ɂ��炷
			(*it)->y -= 4;

			// �A���t�@���グ��
			(*it)->alpha +=50;
			if ((*it)->alpha >= 255)(*it)->alpha = 255;

			// �t���[���Œ���
			(*it)->flame++;
			
			//if ((*it)->alpha >= 255)
			if ((*it)->flame >= 12)
			{
				//�@�A���t�@���}�b�N�X�ɂȂ�����I��
				(*it)->state = NumberData::STATE::ARRIVAL;

				// �t���[����������
				(*it)->flame = 0;

				
			}
			
				break;
		case NumberData::STATE::ARRIVAL:

			// y���W�����ɂ��炷
			(*it)->y -= 1;

			// �t���[���Ŏ��ԉ҂�
			(*it)->flame++;

			if ((*it)->flame > 12)
			{
				(*it)->state = NumberData::STATE::END;
			}		

			break;
		case NumberData::STATE::END:
			(*it)->y += 1;
			// �A���t�@��������
			(*it)->alpha -= 40;

			// �Ȃ񂩏I���t���O�𗧂Ă�
			if ((*it)->alpha <= 0)
			{
				(*it)->alpha = 0;
				// �I���t���OOn 
				(*it)->isEnd = true;
			}

			break;
		default:
			break;
		}	

		// �A���t�@�X�V
		(*it)->number->GetAnim()->SetARGB((*it)->alpha, 255, 255, 255);

		// �i���o�[�X�V
		(*it)->number->Update();

		if ((*it)->isEnd == true)
		{
			// ��ɏ���
			SAFE_DELETE((*it)->number);
			SAFE_DELETE((*it));
			// ����ɍX�V�����
			it = m_NumberData.erase(it);
		}
		else
		{
			// �����ōX�V
			it++;
		}
	}
}


/***********************************************/
// �`��
/***********************************************/
void Number_Effect::Render()
{
	// List
	for (auto it : m_NumberData)
	{
		it->number->Render(it->x, it->y, it->score);
	}
}

/************************************************/
//	�����ǉ�
/************************************************/
void Number_Effect::AddNumber(int x, int y, int score)
{
	NumberData* data;
	
	data = new NumberData();
	data->number = new Number();	
	//data->number->Action();
	data->x = x;
	data->y = y;
	data->score = score;

	data->flame = 0;
	data->endFlame = 100;
	data->alpha = 0;
	data->state = NumberData::STATE::START;
	
	data->isEnd = false;

	// �v�f�ǉ�
	m_NumberData.push_back(data);


}

