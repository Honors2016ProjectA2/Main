#include	"TDNLIB.h"
#include	"system.h"

//*****************************************************************************************************************************
//
//
//
//*****************************************************************************************************************************

tdnShader*	shader;
tdnShader*	shader2D;

//*****************************************************************************************************************************
//
//*****************************************************************************************************************************
void	SYSTEM_Initialize()
{
	//	�W���L�[�ݒ�
	//                 ���@ ���@ ���@ ���@ ��   �~   ��   ��    L1   L2   L3   R1   R2   R3   Sta        Sel
	//KEYSET	keyset = { 'W', 'S', 'A', 'D', 'Z', 'X', 'C', 'V',  'A', 'Q', '1', 'S', 'W', '2', VK_RETURN, VK_SPACE  };
	
	// �g�C���p
	//for (int i = 0; i < 4; i++)
	//{
	//	KEY_Asign(keyset, i);
	//}
	//KEY_Asign(keyset);

	//	�R���g���[���[�Z�b�e�B���O(DUALSHOCK2-ELECOM JC-PS101USV)
	//                 ��X��   ��Y��   �EX��   �EY��   �� �~ �� ��   L1 L2 L3   R1 R2 R3   Sta Sel
	//PADSET	padset = { AXIS_X, AXIS_Y, AXIS_Z, AXIS_RZ, 2, 3, 1, 4,   7, 5,11,   8, 6,12,   9, 10 };

	// �g�C���p
	//for (int i = 0; i < 4; i++)
	//{
	//	KEY_PadAsign(padset, i);
	//}
	//KEY_PadAsign( padset );

	//KEY_PadAsign( iexInput::ps101 );		// ELECOM JC-PS101USV
	//KEY_PadAsign( iexInput::sixaxis );	// PS3 SIXAXIS DUALSHOCK3
	//KEY_PadAsign( iexInput::xbox360 );	// XBOX360 Controller

	//	�V�F�[�_�[�ǂݍ���
	shader = new tdnShader("DATA\\SHADER\\3D.fx");
	shader2D = new tdnShader("DATA\\SHADER\\2D.fx");
	//shaderD = new iexShader("DATA\\SHADER\\Deferred.fx");
	//	�p�[�e�B�N���V�X�e��������
//	iexParticle::Initialize("DATA\\PARTICLE.PNG", 10000 );
}

void	SYSTEM_Release()
{
	delete	shader;
	delete	shader2D;
	//delete	shaderD;

	//IEX_Release();
}


