#ifndef __GENERALKEY_H
#define __GENERALKEY_H
#include "OLED_UI.h"
typedef enum {
    KEY_UP = 0,
    KEY_DOWN, 
    KEY_ENTER,
    KEY_COUNT
} KeyType;

// 简化的按键事件
typedef enum {
    KEY_PRESSED = 0,    // 按下瞬间
    KEY_RELEASED,       // 释放瞬间  
    KEY_LONG_PRESS,  	// 长按
	  KEY_NO
} KeyEvent;

extern void Key_Update(void);
extern KeyEvent Key_GetEvent(KeyType key);

#endif
