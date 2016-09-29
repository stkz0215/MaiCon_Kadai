/***********************************************************************/
/*                                                                     */
/*  FILE        :TIMER.c                                               */
/*  DATE        :Tue, May 24, 2016                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :H8/3687                                               */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.16).    */
/*                                                                     */
/***********************************************************************/
                  
#include "iodefine.h"
#include <machine.h>
#include "lcd_func.h"

// グローバル変数
unsigned int min = 0, sec = 0;

// 関数プロトタイプ
void wait(unsigned int count);
void setTime(void);
void SW(char flag_SW);
void startTimer(void);
void printTimer(void);
void TIMER_B1(void);
void soundBuzzer(void);

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

// メイン関数
void main(void)
{	
	// 初期設定
	IO.PCR3 = 0xFF;	// LEDを出力に
	IO.PCR2 = 0x01;	// ブザーを出力に
	
	// SWの割り込み設定
	set_imask_ccr(1);
	IO.PMR1.BYTE = 0x70;  // SW1~3の割り込みに使用 
	
	IENR1.BYTE = 0x07;	// SW1~3の割り込みを許可
	IEGR1.BYTE = 0x07;	// 立ち上がりを検出
	set_imask_ccr(0);
	
	setTime();
	lcd_init();
	startTimer();

	while(1);
}

// 待機関数
void wait(unsigned int count)
{
	unsigned int i, j;
	
	for(i=0; i<=count; i++) {
		for(j=0; j<=2000; j++) {
		} 
	}
}

// 時間をセットする
void setTime(void) {
	min = 1;	// デフォルトは1分
	
	// LCDの初期画面の出力
	lcd_init();
	lcd_xy(1,1);
	lcd_puts(" <1> 2  3 [min] ");
	lcd_xy(1,2);
	lcd_puts("   SW4 : SET    ");
	
	/*SW1~3の割込動作は関数SWへ*/
	
	while(IO.PDR1.BIT.B7); // SW4が押されるまで待機
	
	lcd_clear();		//画面クリア
	IENR1.BYTE = 0x00;	// SW1~3の割り込みを禁止

}

// SW1~3が押されたときの画面表示に変更
// 引数flag_SWの数字がSWの番号に対応している
void SW(char flag_SW) {
	lcd_xy(1,1);
	switch(flag_SW) {
		case 1:
			min = 1;
			lcd_puts(" <1> 2  3 [min] ");
			IRR1.BIT.IRRI0 = 0;
			break;
		case 2:
			min = 2;
			lcd_puts("  1 <2> 3 [min] ");
			IRR1.BIT.IRRI1 = 0;
			break;
		case 3:
			min = 3;
			lcd_puts("  1  2 <3>[min] ");
			IRR1.BIT.IRRI2 = 0;
			break;
	}
}

// タイマーをスタートさせる
void startTimer(void) {
	sec = min*60;
	
	printTimer();
	
	lcd_xy(1,2);
	lcd_puts("  SW4 : START   ");
	
	wait(1000);
	while(IO.PDR1.BIT.B7); 
	
	// タイマの設定
	set_imask_ccr(1);
	TB1.TMB1.BIT.RLD = 0; // インターバルタイマ
	TB1.TMB1.BIT.CKS = 1; // プリスケーラφ/2048
	
	IRR2.BIT.IRRTB1 = 0;  // 割込フラグクリア
	IENR2.BIT.IENTB1 = 1; // タイマ割込許可
	TB1.TCB1 = 0; // タイマカウンタクリア
	set_imask_ccr(0);
	
	lcd_xy(1,2);
	lcd_puts("                ");
	
}

// タイマーの出力 LED出力も行う
void printTimer(void) {
	lcd_xy(7,1);
	lcd_dataout(sec/60);
	lcd_puts(":");
	if(sec%60 < 10) {
		lcd_puts("0");
	}
	lcd_dataout(sec%60);
	
	// LED0~5の点灯
	IO.PDR3.BYTE = 0x00;
	switch(sec/30) {
	case 6:
	case 5:
		IO.PDR3.BIT.B5 = 1;
	case 4:
		IO.PDR3.BIT.B4 = 1;
	case 3:
		IO.PDR3.BIT.B3 = 1;
	case 2:
		IO.PDR3.BIT.B2 = 1;
	case 1:
		IO.PDR3.BIT.B1 = 1;
	default:
		IO.PDR3.BIT.B0 = 1;
	}
}

// タイマの割込時動作
void TIMER_B1(void) {
	static int cont = 0;
	
	cont++;
	
	if(cont == 38) { // 38カウント=およそ1秒
		cont = 0;
		sec--;
		printTimer();
		
		if(sec == 0) {
			IENR2.BIT.IENTB1 = 0; // タイマ割込禁止
			soundBuzzer(); // ブザーを鳴らす
		}
	}
	
	// LED7とついでにタイマの":"も点滅
	if(cont <= 19) {
		IO.PDR3.BIT.B7 = 1;
		lcd_xy(8,1);
		lcd_puts(":");
	}
	else {
		IO.PDR3.BIT.B7 = 0;
		lcd_xy(8,1);
		lcd_puts(" ");
	}
	
	IRR2.BIT.IRRTB1 = 0; // 割り込みフラグクリア
}

// ブザーを鳴らす
void soundBuzzer(void) {
	IO.PDR2.BIT.B0 = 1; // 鳴らす
	
	lcd_xy(1,2);
	lcd_puts("   SW4 : STOP   ");
	
	while(IO.PDR1.BIT.B7);
	
	IO.PDR2.BIT.B0 = 0; // 止める
	lcd_clear();
	lcd_xy(1,1);
	lcd_puts(" PRESS 'RESET'. ");
	while(1);
}

#ifdef __cplusplus
void abort(void)
{

}
#endif
