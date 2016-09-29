/***********************************************************************/
/*                                                                     */
/*  FILE        :CONT_.c                                               */
/*  DATE        :Mon, Sep 05, 2016                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :H8/3687                                               */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.16).    */
/*                                                                     */
/***********************************************************************/
              
// P50 : �G���R�[�_A��(��)
// P51 : �G���R�[�_B��(��)
// P52 : ���[�^���Z���T�o��
// P53 : �Ǒ��Z���T�o��
// J71 : ���[�^ +������d��(��)
// J72 : ���[�^ -������d��(��)

#include "iodefine.h"
#include "lcd_func.h"
#include <machine.h>

int pulse = 0;
int state = 0;

void WKP_func(void);	// WKP��������
void SW1_moveLeft(void);	// SW1~4��������
void SW2_moveLeft50(void);
void SW3_moveRight50(void);
void SW4_moveRight(void);


void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

void main(void)
{
	lcd_init();          // LCD������

	// �����ݒ�
	IO.PMR1.BYTE = 0xF0; // SW1~4
	IEGR1.BYTE = 0x7F;	 // �����オ��
	IRR1.BYTE = 0x30;    // �����t���O�N���A
	// WKP�����ݒ�
	IO.PCR5 = 0xFD;			// P51(B��)�͓���
	IO.PMR5.BYTE = 0x0D;	// P50(A��),52(Ӱ����ݻ),53(�Ǒ��ݻ)�͊���
	IO.PUCR5.BYTE = 0xFF;	// �v���A�b�v
	IEGR2.BYTE = 0xCF;		// �����オ��
	IWPR.BYTE = 0x00;		// �����t���O�N���A
	// �^�C�}�ݒ�
	TZ0.TCR.BYTE = 0x23;	// �v���X�P�[�����ݒ�
	TZ0.POCR.BYTE = 0xF8;	// ���[�A�N�e�B�u
	TZ.TPMR.BYTE = 0x8E;	// ���[�^�o�͒[�q��PWM����
	TZ.TOCR.BYTE = 0x0C;	// �[�q�����o�͂�0
	TZ0.GRA = 25000;		// �����ݒ�
	TZ0.GRC = 0;
	TZ0.GRD = 0;
	
	IENR1.BYTE = 0x1F;	    // SW1~4��������
	IENR1.BIT.IENWP = 1;	// WKP��������
	
	while(1);
}

void WKP_func(void)
{
	// A���̃p���X����
	if(IWPR.BIT.IWPF0 == 1) {
		// 50mm�ړ��̂Ƃ��݂̂̏���
		if(state == 2 || state == 3) {
			if(pulse >= 618) {
				// 50�~���ړ����Ă����ꍇ�A���[�^��~
				TZ.TOER.BIT.EC0 = 1; 
				TZ.TOER.BIT.ED0 = 1; 
				TZ0.GRC = 0;
				TZ0.GRD = 0;
				TZ.TSTR.BIT.STR0 = 0;
				state = 0;
				pulse = 0;
			}
			else {
				pulse += 1;
			}
		}
	}
// ���[�^���Z���T����
	if(IWPR.BIT.IWPF2 == 1) {
		// ���ړ����Ȃ�΃��[�^��~
		if(state == 1 || state == 2) {
			TZ.TOER.BIT.EC0 = 1; 
			TZ.TOER.BIT.ED0 = 1; 
			TZ0.GRC = 0;
			TZ0.GRD = 0;
			TZ.TSTR.BIT.STR0 = 0;
			state = 0;
			pulse = 0;
		}
	}
	
	// �Ǒ��Z���T����
	if(IWPR.BIT.IWPF3 == 1) {
		// �E�ړ����Ȃ�΃��[�^��~
		if(state == 3 || state == 4) {
			
			TZ.TOER.BIT.EC0 = 1; 
			TZ.TOER.BIT.ED0 = 1; 
			TZ0.GRC = 0;
			TZ0.GRD = 0;
			TZ.TSTR.BIT.STR0 = 0;
			state = 0;
			pulse = 0;
		}
	}
		
	IWPR.BYTE = 0x00;
}

void SW1_moveLeft(void) {
	state = 1;
	pulse = 0;
	TZ0.GRC = 0;
	TZ0.GRD = 15000;
	TZ.TOER.BIT.ED0 = 0;
	TZ0.TCNT = 0;
	TZ.TSTR.BIT.STR0 = 1;
	
	IRR1.BIT.IRRI0 = 0;
}

void SW2_moveLeft50(void)
{
	state = 2;
	pulse = 0;
	TZ0.GRC = 0;
	TZ0.GRD = 15000;
	TZ.TOER.BIT.ED0 = 0;
	TZ0.TCNT = 0;
	TZ.TSTR.BIT.STR0 = 1;
	
	IRR1.BIT.IRRI1 = 0;
}

void SW3_moveRight50(void)
{
	state = 3;
	pulse = 0;
	TZ0.GRC = 15000;
	TZ0.GRD = 0;
	TZ.TOER.BIT.EC0 = 0;
	TZ0.TCNT = 0;
	TZ.TSTR.BIT.STR0 = 1;
	
	IRR1.BIT.IRRI2 = 0;
}

void SW4_moveRight(void) {
	state = 4;
	pulse = 0;
	TZ0.GRC = 15000;
	TZ0.GRD = 0;
	TZ.TOER.BIT.EC0 = 0;
	TZ0.TCNT = 0;
	TZ.TSTR.BIT.STR0 = 1;
	
	IRR1.BIT.IRRI3 = 0;
}

#ifdef __cplusplus
void abort(void)
{

}
#endif