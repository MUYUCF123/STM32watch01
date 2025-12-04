#include "stm32f10x.h"                  // Device header
#include "GeneralKey.h"

//UI作者的按键处理更专注于处理菜单ID，调用起来不太方便，这个库专门处理按键，每个按键都有按下，松开，长按（1s触发）
//这样在写游戏等功能时可以有更方便的调用和更多的输入
//用不上随时删了，不影响按键功能，单开的一个文件

#define LONG_PRESS_THRESHOLD 50
extern OLED_Key OLED_UI_Key ;   								//用于存储按键状态的结构体,默认没有按下，都为1
extern OLED_Key OLED_UI_LastKey ;
extern uint16_t StopCounter; //休眠的计数器，在下方函数里清零，就可以实现有按键操作就清零
uint8_t HoldCounter[KEY_COUNT] = {0};

void Key_Update(){
	uint8_t i = 0;
	for(i = 0 ; i<KEY_COUNT ; i++)
	{
		bool CurrentPress = false ;
		switch(i){
			case KEY_UP :
				CurrentPress = !OLED_UI_Key.Up;
			  break ;
			case KEY_DOWN :
				CurrentPress = !OLED_UI_Key.Down;
				break ;
			case KEY_ENTER :
				CurrentPress = !OLED_UI_Key.Enter;	
				break ;
		}
		
		if(CurrentPress){
			HoldCounter[i] ++ ; //每20ms执行一次该函数(定时器中断)，执行一次说明按住20ms，等于1000ms（50次）时判定为长按
		}else{
			HoldCounter[i] = 0 ;//不按的时候清空
		}
	}
}

KeyEvent Key_GetEvent(KeyType key){

		bool CurrentPress = false ;
		bool LastPress = false ; //默认两者都不按下
		
		switch(key){
			case KEY_UP :
				CurrentPress = !OLED_UI_Key.Up;
				LastPress = !OLED_UI_LastKey.Up;
			  break ;
			case KEY_DOWN :
				CurrentPress = !OLED_UI_Key.Down;
				LastPress = !OLED_UI_LastKey.Down;
				break ;
			case KEY_ENTER :
				CurrentPress = !OLED_UI_Key.Enter;
				LastPress = !OLED_UI_LastKey.Enter;	
				break ;
			default :
				break;
		}
		
    // 按下事件
    if(CurrentPress && !LastPress) {
			  StopCounter = 0;
        return KEY_PRESSED;  
    }
    
    // 释放事件
    if(!CurrentPress && LastPress) {
			  StopCounter = 0;			
        return KEY_RELEASED;
    }
    
    // 长按事件
    if(CurrentPress && HoldCounter[key] >= LONG_PRESS_THRESHOLD) {
				StopCounter = 0;
        return KEY_LONG_PRESS;
    }
    		
		return KEY_NO;
}

