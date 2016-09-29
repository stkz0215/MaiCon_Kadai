/***********************************************************************/
/*                                                                     */
/*  FILE        :TZ02.c                                                */
/*  DATE        :Tue, May 31, 2016                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :H8/3687                                               */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.16).    */
/*                                                                     */
/***********************************************************************/
                  
#include "iodefine.h"
#include "machine.h"

#define CW  1 // ���v���iC�o�́j
#define CCW 0 // �����v���iD�o�́j
#define PERIOD 10 // �p���X����[ms]

char rotate_dir = CW; // ���݂̉�]�����i�f�t�H�͎��v���j

// �֐��v���g�^�C�v
void SW1_OFF(void);
void SW2_SwitchRotateDir(void);
void SW34_ChangeRotateSpeed(char innum);
void Motor_ON(void);

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

	
void main(void)
{
	set_imask_ccr(1);
	// ���荞�ݐݒ�
	IO.PMR1.BYTE = 0xF0; // SW1,3
	IEGR1.BYTE = 0x7F;	 // �����オ��
	IENR1.BYTE = 0x1F;	 // ��������
	
	// �^�C�}�ݒ�
	TZ0.TCR.BYTE = 0x23; 
	TZ0.POCR.BYTE = 0xF8;
	TZ.TPMR.BYTE = 0x8E;
	TZ.TOCR.BYTE = 0x0c;
	TZ0.GRA = PERIOD * 2500;
	TZ0.GRC = 0;
	TZ0.GRD = 0;
	
	set_imask_ccr(0);
	
	while(1);

}

void SW1_OFF(void)
{
	TZ.TOER.BIT.EC0 = 1; // �������Ƃ��o�͋֎~
	TZ.TOER.BIT.ED0 = 1;
	
	TZ0.GRC = 0; // Duty��F0
	TZ0.GRD = 0;
	
	TZ.TSTR.BIT.STR0 = 0; // �J�E���g�����~
	TZ.TSTR.BIT.STR1 = 0;
	
	IRR1.BIT.IRRI0 = 0; // �����t���O�N���A	
}

void SW2_SwitchRotateDir(void)
{
	if(rotate_dir == CW) {
		rotate_dir = CCW;
	}
	else {
		rotate_dir = CW;
	}	
	
	IRR1.BIT.IRRI1 = 0;
}

void SW34_ChangeRotateSpeed(char innum)
{
	unsigned int rotate_speed = TZ0.GRA; // �x�[�X�i100%�o�́j
	
	// ��]���x�̌���
	if(innum == 3) {
		rotate_speed *= 0.25; // 25%
	}
	else {
		rotate_speed *= 0.75; // 75%
	}
	
	// �o�̓|�[�g�w��		
	if(rotate_dir == CW) {
		TZ0.GRC = rotate_speed;
	}
	else {
		TZ0.GRD = rotate_speed;
	}
	
	Motor_ON();
	
	IRR1.BIT.IRRI2 = 0;
	IRR1.BIT.IRRI3 = 0;
}

void Motor_ON(void)
{
	// ���[�^����
	if(rotate_dir == CW) {
		TZ0.TCNT = 0;
		TZ.TOER.BIT.EC0 = 0;
		TZ.TSTR.BIT.STR0 = 1;
	}
	else {
		TZ1.TCNT = 0;
		TZ.TOER.BIT.ED0 = 0;
		TZ.TSTR.BIT.STR0 = 1;
	}
}		

#ifdef __cplusplus
void abort(void)
{

}
#endif