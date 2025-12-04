#include "stm32f10x.h"                  // Device header
#include "string.h"
#include "OLED.h"
#include "OLED_UI.h"
#include "OLED_UI_MenuData.h"
#include "StopWatch.h"

//按钮全局变量(在OLED_UI.c中调用)
int16_t buttonWidth = 28;
int16_t buttonHeight = 16;
int16_t buttonSpacing = 5;
int16_t startX = 0;
int16_t buttonY = 40;
int16_t buttonX ;

//秒表相关参数
uint32_t StopWatchToltalSec = 0;
extern MenuPage*  CurrentMenuPage ;
/**
 * @brief 打印秒表界面的光标
 * @param 无
 * @return 无
 */
void DrawStopWatchCursor(void) {
    
    int16_t currentX = OLED_StopWatch_Cursor.CurrentArea.X;
    int16_t currentY = OLED_StopWatch_Cursor.CurrentArea.Y;
    int16_t currentWidth = OLED_StopWatch_Cursor.CurrentArea.Width;
    int16_t currentHeight = OLED_StopWatch_Cursor.CurrentArea.Height;	
	
    // 绘制反色矩形作为选中效果
    OLED_ReverseArea(currentX, currentY, currentWidth, currentHeight);
    
}
/**
 * @brief 计算秒表值并打印
 * @param 无
 * @return 无
 */
void DrawStopWatch(void){	
    uint32_t hours = StopWatchToltalSec / 3600;
    uint32_t minutes = (StopWatchToltalSec % 3600) / 60;
    uint32_t seconds = StopWatchToltalSec % 60;
		
	  char timeStr[10] ;
		sprintf(timeStr, "%02u:%02u:%02u", hours, minutes, seconds);
		
		int16_t textWidth = CalcStringWidth(OLED_16X16_FULL, OLED_8X16_HALF, timeStr);
	  OLED_Printf(64 - textWidth/2, 15, OLED_8X16_HALF, "%s", timeStr);
		OLED_DrawRoundedRectangle(10, 10, 108, 25, 3, OLED_UNFILLED);
}
/**
 * @brief 打印秒表节目按钮
 * @param 无
 * @return 无
 */
void DrawStopwatchButtons(void) {
    char* buttonTexts[] = {"开始", "暂停", "清零", "返回"};
    
    for (int i = 0; i < 4; i++) {
        buttonX = startX + i * (buttonWidth + buttonSpacing);
        OLED_DrawRoundedRectangle(buttonX, buttonY, buttonWidth, buttonHeight, 2, OLED_UNFILLED);
        // 绘制按钮文字
        int16_t textWidth = CalcStringWidth(OLED_12X12_FULL, OLED_12X12_FULL, buttonTexts[i]);
        OLED_ShowChinese(buttonX + (buttonWidth - textWidth)/2, buttonY + 2,  buttonTexts[i] ,OLED_12X12_FULL);

    }
}

/**
 * @brief 设定初始光标位置
 * @param 无
 * @return 无
 */
void SetTargetCursor_StopWatch(void) {
    
        OLED_StopWatch_Cursor.TargetArea.X = 0 + CurrentMenuPage->_ActiveMenuID*(buttonWidth + buttonSpacing);
        OLED_StopWatch_Cursor.TargetArea.Y = buttonY;
        OLED_StopWatch_Cursor.TargetArea.Height = buttonHeight;
        OLED_StopWatch_Cursor.TargetArea.Width = buttonWidth;           
}

