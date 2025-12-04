#include "OLED_UI.h"
#include "OLED_UI_MenuData.h"

#include "StopWatch.h"
#include "Game_Dino.h"

extern uint32_t StopWatchToltalSec;
extern bool StopWatch_Flag ;
extern bool Dino_Flag;
extern uint16_t StopCounter;
/*目前例程为测试版，代码仍在优化当中*/
/*接线方式：
scl pb8 
sda pb9
res pb5
dc  pb6
cs  pb7
确认按键 pb12
取消按键 pb13
上按键   pb14
下按键   pb15
 编码器a pa8
 编码器b pa9
 编码器按键 pb12
*/


int main(){
	Delay_ms(50);
	OLED_UI_Init(&ColckMenuPage);
	
	
	
	while(1){

		OLED_UI_MainLoop();
	
	}
}

//中断函数
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
	{
		OLED_UI_InterruptHandler();
		
		if(StopWatch_Flag)
	  {
			static uint8_t StopWatchCount = 0;
			StopWatchCount ++;
			if(StopWatchCount == 50)
			{
				StopWatchToltalSec ++ ;
				StopWatchCount = 0 ;
			}
		}
		
		if(Dino_Flag){
			Dino_Tick();
		}
		
		StopCounter ++ ;
		if(StopCounter >= 3000 ){
		
			Enter_Stop_Mode();
		
		}
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
	
}
