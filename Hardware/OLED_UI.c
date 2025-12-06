#include "OLED_UI.h"
#include "OLED_UI_MenuData.h"
#include  "StopWatch.h"
#include "MPU6050.h"
#include "GeneralKey.h"
#include "Game_Dino.h"
#include "AD.h"
#include "MyRTC.h"
#include "OLED_UI_MenuData.h"
#include "GeneralKey.h"
#include "OLED_driver.h"
#ifdef OLED_UI

/*OLED_UIÈ«¾Ö±äÁ¿¶¨Òå */
extern int startX;
extern int buttonY;
extern int buttonWidth;
extern int buttonHeight;
/*******º¯ÊıÉùÃ÷******/
void OLED_UI_ResetAllKeys(void) ;
void  Sleep_Init(void) ;

OLED_UI_Counter OLED_FPS = {0,0,0};									//ÓÃÓÚ´æ´¢Ö¡ÂÊµÄ½á¹¹Ìå
OLED_Key OLED_UI_Key = {1,1,1,1};   								//ÓÃÓÚ´æ´¢°´¼ü×´Ì¬µÄ½á¹¹Ìå,Ä¬ÈÏÃ»ÓĞ°´ÏÂ£¬¶¼Îª1
OLED_Key OLED_UI_LastKey = {1,1,1,1};								//ÓÃÓÚ´æ´¢ÉÏÒ»ÂÖ°´¼ü×´Ì¬µÄ½á¹¹Ìå,Ä¬ÈÏÃ»ÓĞ°´ÏÂ£¬¶¼Îª1
MenuPage*  CurrentMenuPage = NULL;									//È«¾Ö½á¹¹ÌåÖ¸Õë£¬µ±Ç°Ò³ÃæµÄÖ¸Õë
MenuWindow *CurrentWindow = NULL;									//È«¾Ö½á¹¹ÌåÖ¸Õë£¬µ±Ç°´°¿ÚµÄÖ¸Õë
MutexFlag KeyEnterFlag = FLAGEND;										//È«¾Öenter°´¼üµÄ»¥³âËø£¬»¥³âËøÎªFLAGSTARTÊ±±íÊ¾ÕıÔÚÖ´ĞĞ»Øµ÷º¯Êı
MutexFlag FadeOutFlag = FLAGEND;										//½¥ÒşĞ§¹ûµÄ»¥³âËø£¬»¥³âËøÎªFLAGSTARTÊ±±íÊ¾ÕıÔÚÖ´ĞĞ½¥ÒşĞ§¹û
MutexFlag WindowFlag = FLAGEND;										//´°¿Ú»¥³âËø£¬»¥³âËøÎªFLAGSTARTÊ±±íÊ¾ÕıÔÚÖ´ĞĞ´°¿Ú¶¯»­
bool ColorMode = DARKMODE;											//È«¾Ö²¼¶ûĞÍÊı¾İ£¬´æ´¢µ±Ç°ÏÔÊ¾Ä£Ê½£¬trueÎªÉîÉ«Ä£Ê½£¬falseÎªÇ³É«Ä£Ê½
bool OLED_UI_ShowFps = false;										//È«¾Ö²¼¶ûĞÍÊı¾İ£¬ÓÃÓÚ¿ØÖÆÊÇ·ñÏÔÊ¾Ö¡ÂÊ
int16_t OLED_UI_Brightness = 100;									//È«¾Ö±äÁ¿£¬´æ´¢µ±Ç°ÆÁÄ»ÁÁ¶È
OLED_UI_WindowSustainCounter OLED_SustainCounter = {0,false};			//ÓÃÓÚ´æ´¢´°¿Ú³ÖĞøÊ±¼äµÄ½á¹¹Ìå
int16_t WindowProbDeltaData = 0;											//´°¿Ú½ø¶ÈÌõÊı¾İµÄÔöÁ¿Êı¾İ
/***********************************************************************************************/
/***************************ÕâĞ©±äÁ¿ÓÃÓÚ´æ´¢ĞèÒª°ó¶¨¶¯»­µÄ¿Ø¼şµÄ²ÎÊı*******************************/
OLED_ChangeArea OLED_StopWatch_Cursor = {0};
// OLED_UI_CursorÊÇÓÃÓÚ´æ´¢¹â±êµÄ½á¹¹Ìå£¬¿ØÖÆ¹â±êµÄÒÆ¶¯
OLED_ChangeArea OLED_UI_Cursor;
// OLED_UI_MenuFrameÊÇÓÃÓÚ´æ´¢²Ëµ¥±ß¿òµÄ½á¹¹Ìå£¬¿ØÖÆ²Ëµ¥±ß¿òÓëÄÚ²¿×é¼şµÄÒÆ¶¯
OLED_ChangeArea OLED_UI_MenuFrame;
// OLED_UI_WindowÊÇÓÃÓÚ´æ´¢´°¿ÚµÄ½á¹¹Ìå£¬¿ØÖÆ´°¿ÚÓëÄÚ²¿×é¼şµÄÒÆ¶¯
OLED_ChangeArea OLED_UI_Window;
// OLED_UI_ScrollBarHeightÊÇÓÃÓÚ´æ´¢¹ö¶¯Ìõ¸ß¶ÈµÄ½á¹¹Ìå£¬¿ØÖÆ¹ö¶¯Ìõ¸ß¶ÈµÄ±ä»¯
OLED_ChangeDistance OLED_UI_ScrollBarHeight;
// OLED_UI_ProbWidthÊÇÓÃÓÚ´æ´¢½ø¶ÈÌõ¿í¶ÈµÄ½á¹¹Ìå£¬¿ØÖÆ½ø¶ÈÌõ¿í¶ÈµÄ±ä»¯
OLED_ChangeDistance OLED_UI_ProbWidth;
// OLED_UI_PageStartPointÊÇÓÃÓÚ´æ´¢Ò³ÃæÆğÊ¼µãµÄ½á¹¹Ìå£¬¿ØÖÆÒ³ÃæÕûÌåµÄÒÆ¶¯
OLED_ChangePoint OLED_UI_PageStartPoint ;
// OLED_UI_LineStepÊÇÓÃÓÚ´æ´¢ĞĞ¼ä¾àµÄ½á¹¹Ìå£¬¿ØÖÆĞĞ¼ä¾àµÄ±ä»¯
OLED_ChangeDistance OLED_UI_LineStep;



/**
 * @brief »ñÈ¡µ±Ç°ÆÁÄ»Ë¢ĞÂÂÊ£¬½á¹û´æ´¢ÔÚÈ«¾Ö±äÁ¿OLED_FPS.valueÖĞ
 * @param ÎŞ
 * @note ¸Ãº¯ÊıĞèÒª·ÅÔÚ20msÖÜÆÚÄÚµ÷ÓÃ£¬·ñÔò»áµ¼ÖÂ¼ÆÊı´íÎó
 * @return ÎŞ
 */
void GetFPS(void){
	if(OLED_FPS.step<49){
		OLED_FPS.step++;
	}else{
		OLED_FPS.step=0;
		OLED_FPS.value = OLED_FPS.count;
		OLED_FPS.count=0;
	}
}

/**
 * @brief ÏÔÊ¾µ±Ç°ÆÁÄ»Ë¢ĞÂÂÊ
 * @param ÎŞ
 * @note Ğè½«´Ëº¯Êı·ÅÔÚÖ÷Ñ­»·µ±ÖĞ£¬Ã¿Ñ­»·Ò»´Î¼ÇÎªÒ»´ÎË¢ĞÂ¡£
 * @return ÎŞ
 */
void OLED_UI_ShowFPS(void){
    OLED_FPS.count ++;
	
	if (OLED_UI_ShowFps){
		OLED_Printf(OLED_WIDTH - CalcStringWidth(OLED_8X8_FULL, OLED_6X8_HALF, "%d",OLED_FPS.value),0,OLED_6X8_HALF,"%d",OLED_FPS.value);
	}
}
/**
 * @brief »ñÈ¡µ±Ç°Ò³ÃæµÄ×ÖÌå¿í¶È
 * @param style CHINESE¡¾ÖĞÎÄ¡¿»ò ASCII¡¾ASCII¡¿
 * @return µ±Ç°Ò³ÃæµÄ×ÖÌå¿í¶È
 */
OLED_Font GetOLED_Font(OLED_Font fontsize,bool style){
	//¸ù¾İµ±Ç°Ò³ÃæµÄ×ÖÌå´óĞ¡ÉèÖÃ×ÖÌå
	OLED_Font ChineseFont,ASCIIFont;
	switch(fontsize){
		case OLED_UI_FONT_8:
			ChineseFont = OLED_8X8_FULL,ASCIIFont = OLED_6X8_HALF;break;
		case OLED_UI_FONT_12:
			ChineseFont = OLED_12X12_FULL,ASCIIFont = OLED_7X12_HALF;break;
		case OLED_UI_FONT_16:
			ChineseFont = OLED_16X16_FULL,ASCIIFont = OLED_8X16_HALF;break;
		case OLED_UI_FONT_20:
			ChineseFont = OLED_20X20_FULL,ASCIIFont = OLED_10X20_HALF;break;
		default:
			ChineseFont = OLED_12X12_FULL,ASCIIFont = OLED_7X12_HALF;break;
	}
	if(style == CHINESE){
		return ChineseFont;
	}else{
		return ASCIIFont;
	}

}
/**
 * @brief ·´×ªÏÔÊ¾¹â±ê
 * @param X ¹â±êX×ø±ê
 * @param Y ¹â±êY×ø±ê
 * @param Width ¹â±ê¿í¶È
 * @param Height ¹â±ê¸ß¶È
 * @param Style ¹â±êÑùÊ½
 * @note ¸Ãº¯ÊıÓÃÓÚ·´×ªÏÔÊ¾¹â±ê£¬Ê¹ÆäÏÔÊ¾Îª·´É«
 * @return ÎŞ
 */
void ReverseCoordinate(int16_t X, int16_t Y, int16_t Width, int16_t Height,uint8_t Style){
	switch(Style){
		
		case REVERSE_RECTANGLE://¾ØĞÎ·´É«
			OLED_ReverseArea(X, Y, Width, Height);
			break;
		case REVERSE_ROUNDRECTANGLE://Ô²½Ç¾ØĞÎ·´É«
			if(Width >= 3 && Height >= 3){
				OLED_ReverseArea(X, Y, 1, 1);
				OLED_ReverseArea(X+Width-1, Y, 1, 1);
				OLED_ReverseArea(X, Y+Height-1, 1, 1);
				OLED_ReverseArea(X+Width-1, Y+Height-1, 1, 1);
			}
			OLED_ReverseArea(X, Y, Width, Height);
			break;

		case HOLLOW_RECTANGLE://¿ÕĞÄ¾ØĞÎ
			OLED_ReverseArea(X, Y, Width, Height);
			OLED_ReverseArea(X+1, Y+1, Width-2, Height-2);
			break;
		case HOLLOW_ROUNDRECTANGLE://¿ÕĞÄ¾ØĞÎ
			OLED_ReverseArea(X, Y, Width, Height);
			OLED_ReverseArea(X+1, Y+1, Width-2, Height-2);
			if(Width >= 3){
				OLED_ReverseArea(X, Y, 1, 1);
				OLED_ReverseArea(X+Width-1, Y, 1, 1);
			}
			if(Height >= 3){
				OLED_ReverseArea(X, Y+Height-1, 1, 1);
				OLED_ReverseArea(X+Width-1, Y+Height-1, 1, 1);
			}
			break;
		case REVERSE_BLOCK://Ğ¡·½¿é
			
			OLED_ReverseArea(X, Y + 1, GetOLED_Font(CurrentMenuPage->General_FontSize,ASCII), Height);
			
			break;
		case NOT_SHOW:
			break;
		default:
			OLED_ReverseArea(X, Y, Width, Height);
		break;
	}
}

/** 
 * @brief£ºÔÚÖ¸¶¨ÇøÓòÓ¦ÓÃÄ£Ê½»¯½¥ÒşĞ§¹û(ÃÉ°æ¿ÅÁ£»¯)
 * @param x0 ÇøÓòÆğÊ¼X×ø±ê
 * @param y0 ÇøÓòÆğÊ¼Y×ø±ê
 * @param width ÇøÓò¿í¶È
 * @param height ÇøÓò¸ß¶È
 * @param fadeLevel ½¥ÒşµµÎ»£¬1µ½5Ö®¼äµÄÖµ£¬±íÊ¾²»Í¬µÄÏñËØÏ¨ÃğÄ£Ê½
 * @note ¸Ãº¯ÊıÓÃÓÚÔÚÖ¸¶¨ÇøÓòÓ¦ÓÃÄ£Ê½»¯½¥ÒşĞ§¹û£¬Ê¹µÃ¸ÃÇøÓòµÄÏñËØÖğ½¥±ä°µ£¬Ğ§¹ûÀàËÆÃÉ°æ¿ÅÁ£»¯¡£
*/
void OLED_UI_FadeOut_Masking(int16_t x0, int16_t y0, int16_t width, int16_t height, int8_t fadeLevel) {
    // ¼ì²é²¢µ÷ÕûÇøÓò·¶Î§
    if (x0 < 0) {
        width += x0;
        x0 = 0;
    }
    if (y0 < 0) {
        height += y0;
        y0 = 0;
    }
    if (x0 + width > OLED_WIDTH) {
        width = OLED_WIDTH - x0;
    }
    if (y0 + height > OLED_HEIGHT) {
        height = OLED_HEIGHT - y0;
    }
    if (width <= 0 || height <= 0) {
        return;
    }

    // È·±£½¥ÒşµµÎ»ÔÚÓĞĞ§·¶Î§ÄÚ
    if (fadeLevel < 1 || fadeLevel > 5) {
        return;
    }

    // ¶¨Òå2x2Íø¸ñµÄ½¥ÒşÄ£Ê½
    // Ã¿¸öÊı×é±íÊ¾Ò»¸ö2x2Íø¸ñÖĞÄÄĞ©ÏñËØĞèÒªÏ¨Ãğ
    // 0±íÊ¾±£³ÖÁÁ£¬1±íÊ¾Ï¨Ãğ
    const uint8_t patterns[5][2][2] = {
        {{0, 0},  // Level 1: È«ÁÁ
         {0, 0}},
        
        {{1, 0},  // Level 2: ×óÉÏ½ÇÏ¨Ãğ
         {0, 0}},
        
        {{1, 0},  // Level 3: ×óÉÏ½ÇºÍÓÒÏÂ½ÇÏ¨Ãğ
         {0, 1}},
        
        {{1, 0},  // Level 4: Ö»±£ÁôÓÒÉÏ½Ç
         {1, 1}},
        
        {{1, 1},  // Level 5: È«°µ
         {1, 1}}
    };

    // ¼ÆËã±ß½ç
    int16_t xEnd = x0 + width;
    int16_t yEnd = y0 + height;

    const uint8_t (*pattern)[2] = patterns[fadeLevel - 1];  // é¢„å…ˆè·å–å½“å‰æ¨¡å¼ï¼Œé¿å…é‡å¤ç´¢å¼•
    // Ó¦ÓÃ½¥ÒşĞ§¹û
    for (int16_t y = y0; y < yEnd; y++) {
        int page = y >> 3;           // ï¿½Å»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê§ï¿½ï¿½ï¿½ï¿½
        int bit_pos = y & 7;         // ï¿½Å»ï¿½ï¿½ï¿½Î»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä£
        uint8_t pixel_mask = 1 << bit_pos;
        int grid_y = (y - y0) & 1;   // ï¿½Å»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ú¼ï¿½Ñ­ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Î»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä£2
        
        for (int16_t x = x0; x < xEnd; x++) {
            // ¼ÆËãÔÚ2x2Íø¸ñÖĞµÄÏà¶ÔÎ»ÖÃ
            int grid_x = (x - x0) & 1;   // ï¿½Å»ï¿½ï¿½ï¿½Î»ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä£2
            
            // ¸ù¾İ½¥Òş¼¶±ğºÍÍø¸ñÎ»ÖÃ¾ö¶¨ÊÇ·ñÏ¨ÃğÏñËØ
            if (pattern[grid_y][grid_x]) {
                OLED_DisplayBuf[page][x] &= ~pixel_mask;
            }
        }
    }
}





/**
 * @brief ½«µ±Ç°²Ëµ¥Ò³ÃæÇøÓòµÄÔªËØ½¥Òş
 * @param ÎŞ
 * @note ¸Ãº¯ÊıÓÃÓÚ½«µ±Ç°²Ëµ¥Ò³ÃæÇøÓòµÄÔªËØÖğ½¥±ä°µ£¬Ğ§¹ûÀàËÆÃÉ°æ¿ÅÁ£»¯¡£
 * @return ÎŞ
 */
void OLED_UI_FadeoutCurrentArea(int16_t x, int16_t y, int16_t width, int16_t height) {
	if(CurrentMenuPage->General_MovingSpeed <= 0){
		OLED_Clear();
		return;
	}
    for(uint8_t i=1;i<=5;i++){
		// Ó¦ÓÃ½¥ÒşĞ§¹û
			
		OLED_UI_FadeOut_Masking(x,y,width,height,i);
		// ÏÔÊ¾¸üĞÂ
        OLED_Update();
		Delay_ms(FADEOUT_TIME);
    }
	// Delay_ms(20);
}
/**
 * @brief È«ÆÁ½¥Òş
 * @param ÎŞ
 * @note ¸Ãº¯ÊıÓÃÓÚ½«µ±Ç°²Ëµ¥Ò³ÃæÇøÓòµÄÔªËØÖğ½¥±ä°µ£¬Ğ§¹ûÀàËÆÃÉ°æ¿ÅÁ£»¯¡£
 * @return ÎŞ
 */
void OLED_UI_FadeoutAllArea(void) {
	if(CurrentMenuPage->General_MovingSpeed <= 0){
		OLED_Clear();
		return;
	}
    for(uint8_t i=1;i<=5;i++){
		// Ó¦ÓÃ½¥ÒşĞ§¹û
			
		OLED_UI_FadeOut_Masking(0 ,0 , OLED_WIDTH, OLED_HEIGHT,i);
		// ÏÔÊ¾¸üĞÂ
        OLED_Update();
		Delay_ms(FADEOUT_TIME);
    }
	// Delay_ms(20);
}

/**
 * @brief »ñÈ¡²Ëµ¥Ïî½á¹¹ÌåÊı×éµÄ×îºóÒ»¸öÔªËØµÄID
 * @param items ½á¹¹ÌåÊı×éMenuItemµÄÖ¸Õë
 * @return ²Ëµ¥Ïî½á¹¹ÌåÊı×éµÄÔªËØÊıÁ¿
 */
MenuID GetMenuItemNum(MenuItem * items){
	MenuID num = 0;
	while(items[num].General_item_text != NULL){
		num+=1;
	}
	return num;
}


/**
 * @brief ¸ù¾İµ±Ç°ËùÑ¡µÄ¶¯»­·½Ê½£¬¸Ä±ä¸¡µãÊı²ÎÊı
 * @param CurrentNum µ±Ç°ÖµµÄÖ¸Õë
 * @param TargetNum Ä¿±êÖµÖ¸Õë
 * @param StartNum ÆğÊ¼ÖµÖ¸Õë
 * @param CurrentStepNum µ±Ç°²½ÊıÖ¸Õë
 * @return ÎŞ
 */
void ChangeFloatNum(float *CurrentNum, float *TargetNum, float *ErrorNum, float *LastErrorNum, float * IntegralNum, float *DerivativeNum)  {
	if(CurrentMenuPage->General_MoveStyle ==  UNLINEAR){
		if(*CurrentNum == *TargetNum){
			return;
		}
		//Èç¹ûÓÃ»§½«ËÙ¶ÈÉèÖÃÎª0£¬ÄÇÃ´µ±Ç°ÖµÖ±½ÓµÈÓÚÄ¿±êÖµ
		if(CurrentMenuPage->General_MovingSpeed <= 0){

			*ErrorNum = 0;
			*LastErrorNum = 0;
		    *CurrentNum = *TargetNum;
			return;
		}
		*LastErrorNum = *ErrorNum;
		//¼ÆËã±¾ÂÖÎó²îÖµ
		*ErrorNum = *TargetNum - *CurrentNum; 
		//¼ÆËãµ±Ç°Öµ
		*CurrentNum += 0.02*CurrentMenuPage->General_MovingSpeed * (*ErrorNum);
		//µ±Ä¿±êÖµÓëµ±Ç°Öµ²î¾àĞ¡ÓÚËÙ¶ÈÖµµÄ1/20Ê±£¬ÈÏÎªÒÑ¾­µ½´ïÄ¿±êÖµ
		if(fabs(*CurrentNum - *TargetNum) < CurrentMenuPage->General_MovingSpeed/20.0f){

			*ErrorNum = 0;
			*LastErrorNum = 0;
		    *CurrentNum = *TargetNum;
			return;
		}
	}
	//Èç¹ûµ±Ç°µÄ¶¯»­·½Ê½ÊÇPID_CURVE·½Ê½
	if(CurrentMenuPage->General_MoveStyle ==  PID_CURVE){
		/*ÕâÊÇÒ»ÖÖÆæÌØµÄ·½·¨£¬ÒòÎªµ±µ±Ç°ÖµµÈÓÚÄ¿±êÖµµÄÊ±ºò£¬ÆäËûÏîÖÃÁãÁË£¬µ«ÊÇ»ı·ÖÏî²¢Ã»ÓĞ±»ÖÃÁã¡£¸ù¾İÊµ¼ÊÏÖÏó£¬ÕâÑùµÄĞ§¹ûÊÇ×îºÃµÄ¡£ */
		//Èç¹ûÓÃ»§½«ËÙ¶ÈÉèÖÃÎª0£¬ÄÇÃ´µ±Ç°ÖµÖ±½ÓµÈÓÚÄ¿±êÖµ£¬ÆäËûËùÓĞÖĞ¼äÖµÖÃÁã
		if(CurrentMenuPage->General_MovingSpeed <= 0){

			*ErrorNum = 0;
			*LastErrorNum = 0;
			*DerivativeNum = 0;
			*IntegralNum = 0;
		    *CurrentNum = *TargetNum;
			return;
		}
		// ¶¨ÒåPID²ÎÊı
		//¶¨ÒåPID²ÎÊı
		float Kp = 0.02f * CurrentMenuPage->General_MovingSpeed;
		float Ki = 0.005f * CurrentMenuPage->General_MovingSpeed;
		float Kd = 0.002f ;

		//¼ÇÂ¼ÉÏÒ»ÂÖÎó²îÖµ
		*LastErrorNum = *ErrorNum;
		//¼ÆËã±¾ÂÖÎó²îÖµ
		*ErrorNum = *TargetNum - *CurrentNum; 
		//¼ÆËã»ı·ÖÖµ
		*IntegralNum += *ErrorNum;
		//¼ÆËãÎ¢·ÖÖµ£¬²¢¼ÙÉèÊ±¼ä¼ä¸ôÎª0.1s
		*DerivativeNum = (*ErrorNum - *LastErrorNum) / 0.1;
		//¼ÆËãµ±Ç°Öµ
		*CurrentNum += Kp * (*ErrorNum) + Ki *  (*IntegralNum) + Kd *(*DerivativeNum);
		//µ±Ä¿±êÖµÓëµ±Ç°Öµ²î¾àĞ¡ÓÚ0.5Ê±£¬½«Ä¿±êÖµÇ¿ÖÆµÈÓÚµ±Ç°Öµ£¬³ıÁË»ı·ÖÏî²»ÖÃÁã£¬ÆäËûËùÓĞÖĞ¼äÖµÖÃÁã
		if(fabs(*TargetNum - *CurrentNum) < 0.5f){

			*ErrorNum = 0;
			*LastErrorNum = 0;
			*DerivativeNum = 0;
		    *CurrentNum = *TargetNum;
			return;
		}	
	}
	
}

/**
 * @brief ·ÇÏßĞÔ¸Ä±ä¾àÀë²ÎÊı
 * @param CurrentNum µ±Ç°ÖµµÄÖ¸Õë
 * @param TargetNum Ä¿±êÖµÖ¸Õë
 * @param StepNum ²½³¤Ö¸Õë
 */
void ChangeDistance(OLED_ChangeDistance *distance){
	ChangeFloatNum(&distance->CurrentDistance,&distance->TargetDistance,&distance->Error,&distance->LastError,&distance->Integral,&distance->Derivative);
}

/**
 * @brief ·ÇÏßĞÔ¸Ä±äµã×ø±ê²ÎÊı
 * @param OLED_MovingArea ½á¹¹ÌåÊı×éµÄÖ¸Õë
 * @return ÎŞ
 */
void ChangePoint(OLED_ChangePoint *point){
	ChangeFloatNum(&point->CurrentPoint.X,&point->TargetPoint.X,&point->Error.X,&point->LastError.X,&point->Integral.X,&point->Derivative.X);
	ChangeFloatNum(&point->CurrentPoint.Y,&point->TargetPoint.Y,&point->Error.Y,&point->LastError.Y,&point->Integral.Y,&point->Derivative.Y);
}
/**
 * @brief ·ÇÏßĞÔ¸Ä±äÇøÓò²ÎÊı
 * @param OLED_MovingArea ½á¹¹ÌåÊı×éµÄÖ¸Õë
 * @return ÎŞ
 */
void ChangeArea(OLED_ChangeArea *area)	{
	ChangeFloatNum(&area->CurrentArea.X,&area->TargetArea.X,&area->Error.X,&area->LastError.X,&area->Integral.X,&area->Derivative.X);
	ChangeFloatNum(&area->CurrentArea.Y,&area->TargetArea.Y,&area->Error.Y,&area->LastError.Y,&area->Integral.Y,&area->Derivative.Y);
	ChangeFloatNum(&area->CurrentArea.Width,&area->TargetArea.Width,&area->Error.Width,&area->LastError.Width,&area->Integral.Width,&area->Derivative.Width);
	ChangeFloatNum(&area->CurrentArea.Height,&area->TargetArea.Height,&area->Error.Height,&area->LastError.Height,&area->Integral.Height,&area->Derivative.Height);
}




/**
 * @brief ¶Ôµ±Ç°µÄ²Ëµ¥Ò³ÃæµÄ²ÎÊı½øĞĞ¼ì²éÓë³õÊ¼»¯
 * @param ÎŞ
 * @note ÓÃÓÚÈ·ÈÏÊÂ¼şÏÂ½øÈë×Ó²Ëµ¥Ê±µÄ²Ù×÷
 * @return ÎŞ
 */
void CurrentMenuPageInit(void){
	// Èç¹ûÊÇÃë±íÒ³Ãæ£¬³õÊ¼»¯×Ô¶¨Òå¹â±ê
    if (CurrentMenuPage == &StopWatchMenuPage) {
   
		OLED_StopWatch_Cursor.CurrentArea.X = 0;
    OLED_StopWatch_Cursor.CurrentArea.Y = buttonY;
    OLED_StopWatch_Cursor.CurrentArea.Width = buttonWidth;
    OLED_StopWatch_Cursor.CurrentArea.Height = buttonHeight;
    // ÉèÖÃÄ¿±êÎ»ÖÃ
    OLED_StopWatch_Cursor.TargetArea.X = 0;
    OLED_StopWatch_Cursor.TargetArea.Y = buttonY;
    OLED_StopWatch_Cursor.TargetArea.Width = buttonWidth;
    OLED_StopWatch_Cursor.TargetArea.Height = buttonHeight;
    	
    }
	//Èç¹ûµ±Ç°µÄ²Ëµ¥ÀàĞÍÎªLIST
	if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
		//ÉèÖÃÈ«¾ÖÒ³Ãæµ±Ç°ÆğÊ¼µãÎª²Ëµ¥½á¹¹ÌåµÄ¿ªÊ¼µã+Ïà¶ÔÎ»ÖÃÆğÊ¼µã
		OLED_UI_PageStartPoint.CurrentPoint.X = OLED_UI_MenuFrame.CurrentArea.X+CurrentMenuPage->List_StartPointX + OLED_WIDTH;
		OLED_UI_PageStartPoint.CurrentPoint.Y = OLED_UI_MenuFrame.CurrentArea.Y+CurrentMenuPage->List_StartPointY;
		//ÉèÖÃÈ«¾ÖÒ³ÃæÄ¿±êÆğÊ¼µãÎª²Ëµ¥½á¹¹ÌåµÄ¿ªÊ¼µã+Ïà¶ÔÎ»ÖÃÆğÊ¼µã
		OLED_UI_PageStartPoint.TargetPoint.X = CurrentMenuPage->List_MenuArea.X+CurrentMenuPage->List_StartPointX;
		OLED_UI_PageStartPoint.TargetPoint.Y = CurrentMenuPage->List_MenuArea.Y+CurrentMenuPage->List_StartPointY;
		//ÉèÖÃÈ«¾ÖÒ³ÃæÄ¿±êÇøÓòÎªµ±Ç°²Ëµ¥½á¹¹ÌåµÄÇøÓò
		OLED_UI_MenuFrame.TargetArea.X = CurrentMenuPage->List_MenuArea.X;
		OLED_UI_MenuFrame.TargetArea.Y = CurrentMenuPage->List_MenuArea.Y;
		OLED_UI_MenuFrame.TargetArea.Width = CurrentMenuPage->List_MenuArea.Width;
		OLED_UI_MenuFrame.TargetArea.Height = CurrentMenuPage->List_MenuArea.Height;

		//ÉèÖÃµ±Ç°ĞĞ¼ä¾àÎª-×ÖÌå´óĞ¡ÒÔÖÆÔì²Ëµ¥ÏîÕ¹¿ªµÄ¶¯»­Ğ§¹û
		OLED_UI_LineStep.CurrentDistance =-3;
		//ÉèÖÃÄ¿±êĞĞ¼ä¾àÎªÓÃ»§ÉèÖÃµÄĞĞ¾à
		OLED_UI_LineStep.TargetDistance = CurrentMenuPage->General_LineSpace;
		//ÉèÖÃµ±Ç°¹â±êÎªµ±Ç°²Ëµ¥ÏîµÄµÚÒ»¸ö
		CurrentMenuPage->_ActiveMenuID = 0;
		//ÉèÖÃµ±Ç°²ÛÎ»Îª0
		CurrentMenuPage->_Slot = 0;
	}else
	//Èç¹ûµ±Ç°µÄ²Ëµ¥ÀàĞÍÎªTILES
	if (CurrentMenuPage->General_MenuType == MENU_TYPE_TILES)
	{
		//ÉèÖÃÈ«¾ÖÒ³ÃæÄ¿±êÆğÊ¼µãÎªÆÁÄ»ÖĞÑëÏò×óÆ«ÒÆ°ë¸ö´ÅÌù¿í¶È£¬Ê¹µÃµ±Ç°²Ëµ¥Ïî¾ÓÖĞ
		OLED_UI_PageStartPoint.TargetPoint.X = CurrentMenuPage->Tiles_ScreenWidth/2-CurrentMenuPage->Tiles_TileWidth/2;
		//ÉèÖÃÈ«¾ÖÒ³ÃæÄ¿±êÆğÊ¼µãÎªÔ¤ÉèµÄÆğÊ¼µã
		OLED_UI_PageStartPoint.TargetPoint.Y = TILES_STARTPOINT_Y;
		//ÉèÖÃÈ«¾ÖÒ³Ãæµ±Ç°ÆğÊ¼µãÎªÒ»¸ö¸ºµÄÎ»ÖÃ£¬Ê¹µÃ¿ªÊ¼Ê±ÓĞ¶¯»­
		OLED_UI_PageStartPoint.CurrentPoint.X = -50;
		OLED_UI_PageStartPoint.CurrentPoint.Y = -CurrentMenuPage->Tiles_TileWidth;
		//ÉèÖÃµ±Ç°´ÅÌù¼ä¾àÎª1ÒÔ±ã²úÉú¶¯»­Ğ§¹û
		OLED_UI_LineStep.CurrentDistance = 1;
		//ÉèÖÃÄ¿±ê´ÅÌù¼ä¾àÎªÓÃ»§ÉèÖÃµÄĞĞ¾à
		OLED_UI_LineStep.TargetDistance = CurrentMenuPage->General_LineSpace;
		//ÉèÖÃµ±Ç°¹â±êÎªµ±Ç°²Ëµ¥ÏîµÄµÚÒ»¸ö
		CurrentMenuPage->_ActiveMenuID = 0;
	}
	//½«¹ö¶¯µÄ¿ªÊ¼µã¹éÁã£¬È·±£µ±È·ÈÏ»òÊÇ·µ»Ø²Ù×÷Ê±£¬¹ö¶¯µÄ²Ëµ¥Ïî¶¼´ÓĞÂ¿ªÊ¼
	SetLineSplitZero();

		
}
/**
 * @brief »¹Ô­²Ëµ¥²ÎÊıµ½ÉÏÒ»´ÎµÄ×´Ì¬
 * @param ÎŞ
 * @note ÓÃÓÚ·µ»ØÉÏÒ»¼¶²Ëµ¥Ê±µÄ²Ù×÷
 * @return ÎŞ
 */
void CurrentMenuPageBackUp(void){
	//Èç¹ûµ±Ç°µÄ²Ëµ¥ÀàĞÍÎªLIST
	if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
		//ÉèÖÃÈ«¾ÖÒ³Ãæµ±Ç°ÆğÊ¼µãÎª²Ëµ¥½á¹¹ÌåµÄ¿ªÊ¼µã+2
		OLED_UI_PageStartPoint.CurrentPoint.X = OLED_UI_MenuFrame.CurrentArea.X + CurrentMenuPage->List_StartPointX - OLED_WIDTH;
		OLED_UI_PageStartPoint.CurrentPoint.Y = CurrentMenuPage->_StartPoint.Y;
		//ÉèÖÃÈ«¾ÖÒ³ÃæÄ¿±êÆğÊ¼µãÎª²Ëµ¥½á¹¹ÌåµÄ¿ªÊ¼µã
		OLED_UI_PageStartPoint.TargetPoint.X = CurrentMenuPage->_StartPoint.X;
		OLED_UI_PageStartPoint.TargetPoint.Y = CurrentMenuPage->_StartPoint.Y;
		//ÉèÖÃµ±Ç°ĞĞ¼ä¾àÎª-×ÖÌå´óĞ¡ÒÔÖÆÔì¶¯»­Ğ§¹û
		OLED_UI_LineStep.CurrentDistance = CurrentMenuPage->General_LineSpace;
		//ÉèÖÃÄ¿±êĞĞ¼ä¾à
		OLED_UI_LineStep.TargetDistance = CurrentMenuPage->General_LineSpace;
	}else
	//Èç¹ûµ±Ç°µÄ²Ëµ¥ÀàĞÍÎªTILES
	if (CurrentMenuPage->General_MenuType == MENU_TYPE_TILES)
	{
		//ÉèÖÃÈ«¾ÖÒ³Ãæµ±Ç°ÆğÊ¼µãÎª²Ëµ¥½á¹¹ÌåµÄ¿ªÊ¼µã+2
		OLED_UI_PageStartPoint.CurrentPoint.X = CurrentMenuPage->_StartPoint.X + CurrentMenuPage->Tiles_TileWidth;
		OLED_UI_PageStartPoint.CurrentPoint.Y = -CurrentMenuPage->Tiles_TileHeight-1;
		//ÉèÖÃÈ«¾ÖÒ³ÃæÄ¿±êÆğÊ¼µãÎª²Ëµ¥½á¹¹ÌåµÄ¿ªÊ¼µã
		OLED_UI_PageStartPoint.TargetPoint.X = CurrentMenuPage->_StartPoint.X;
		OLED_UI_PageStartPoint.TargetPoint.Y = CurrentMenuPage->_StartPoint.Y;
		//ÉèÖÃµ±Ç°ĞĞ¼ä¾àÎª-×ÖÌå´óĞ¡ÒÔÖÆÔì¶¯»­Ğ§¹û
		OLED_UI_LineStep.CurrentDistance = CurrentMenuPage->General_LineSpace;
		//ÉèÖÃÄ¿±êĞĞ¼ä¾à
		OLED_UI_LineStep.TargetDistance = CurrentMenuPage->General_LineSpace;
	}
	
	//½«¹ö¶¯µÄ¿ªÊ¼µã¹éÁã£¬È·±£µ±È·ÈÏ»òÊÇ·µ»Ø²Ù×÷Ê±£¬¹ö¶¯µÄ²Ëµ¥Ïî¶¼´ÓĞÂ¿ªÊ¼
	SetLineSplitZero();
}

/**
 * @brief ³õÊ¼»¯OLED_UI£¬ÉèÖÃµ±Ç°Ò³ÃæµÄ½á¹¹ÌåÖ¸Õë£¬²¢³õÊ¼»¯OLEDÏÔÊ¾ÆÁ
 * @param Page ²Ëµ¥Ò³Ãæ½á¹¹Ìå
 * @return ÎŞ
 */
void OLED_UI_Init(MenuPage* Page){
	//³õÊ¼»¯OLEDÏÔÊ¾ÆÁ
	OLED_Init();
	Timer_Init();
	Key_Init();
	Encoder_Init();
	LED_Init();
	MPU6050_Init();
	AD_Init();
	MyRTC_Init();
	Sleep_Init();
	//ÉèÖÃµ±Ç°Ò³ÃæµÄ½á¹¹ÌåÖ¸Õë
	CurrentMenuPage = Page;	//ÉèÖÃµ±Ç°Ò³ÃæµÄ½á¹¹ÌåÖ¸Õë
	//³õÊ¼»¯²Ëµ¥Ò³Ãæ²ÎÊı
	CurrentMenuPageInit();
	
}



/**
 * @brief »ñÈ¡enterÊÂ¼ş×´Ì¬£¬ÓÃÓÚÅĞ¶ÏÊÇ·ñÕıÔÚÖ´ĞĞ»Øµ÷º¯Êı
 * @param void
 * @return ÎŞ
 */
bool GetEnterFlag(void){
	if(KeyEnterFlag == FLAGEND){
		return true;
	}else{
		return false;
	}
}

/**
 * @brief »ñÈ¡enterÊÂ¼ş×´Ì¬£¬ÓÃÓÚÅĞ¶ÏÊÇ·ñÕıÔÚÖ´ĞĞ»Øµ÷º¯Êı
 * @param void
 * @return ÎŞ
 */
bool GetFadeoutFlag(void){
	if(FadeOutFlag == FLAGEND){
		return true;
	}else{
		return false;
	}
}

/**
 * @brief ¼ÆËã×Ö·û´®µÄ¿í¶È£¨¿ÉÒÔÊÇÖĞÓ¢ÎÄ×Ö·û´®£©
 * @param String ×Ö·û´®Ö¸Õë
 * @param ChineseFont ÖĞÎÄ×ÖÌå¿í¶È
 * @param ASCIIFont ASCII×ÖÌå¿í¶È
 * @return ×Ö·û´®µÄ¿í¶È
 */
int16_t CalcStringWidth(int16_t ChineseFont, int16_t ASCIIFont, const char *format, ...) {
    int16_t StringLength = 0;
    char String[MAX_STRING_LENGTH];

    va_list args;
    va_start(args, format);
    vsnprintf(String, sizeof(String), format, args); // Ê¹ÓÃvsnprintf
    va_end(args);

    char *ptr = String;
    while (*ptr != '\0') {
        if ((unsigned char)*ptr & 0x80) { // ´¦ÀíÖĞÎÄ×Ö·û
            StringLength += ChineseFont;
            ptr += 2;
        } else {
            StringLength += ASCIIFont;
            ptr++;
        }
    }

    return StringLength;
}
/**
 * @brief »ñÈ¡´°¿ÚÊı¾İÇé¿ö
 * @param window
 * @return ¸¡µãÖµÎª·µ»Ø WINDOW_DATA_STYLE_FLOAT £¬int16_tÖµÎª·µ»Ø WINDOW_DATA_STYLE_INT £¬¿ÕÖ¸Õë·µ»Ø WINDOW_DATA_STYLE_NONE
 */
int8_t GetWindowProbDataStyle(MenuWindow* window){

	//±£»¤±ÜÃâ·ÃÎÊ·Ç·¨ÄÚ´æ
	if(window == NULL){
		return WINDOW_DATA_STYLE_NONE;
	}
	// Èç¹û´°¿ÚÊı¾İÀàĞÍÎªint8_t
	if(window->Prob_Data_Int_8 != NULL){
		return WINDOW_DATA_STYLE_INT8;
		// Èç¹û´°¿ÚÊı¾İÀàĞÍÎªint16_t
	}else if(window->Prob_Data_Int_16 != NULL){
		return WINDOW_DATA_STYLE_INT16;
		// Èç¹û´°¿ÚÊı¾İÀàĞÍÎªint32_t
	}else if(window->Prob_Data_Int_32 != NULL){
		return WINDOW_DATA_STYLE_INT32;
		// Èç¹û´°¿ÚÊı¾İÀàĞÍÎªint64_t
	}else if(window->Prob_Data_Int_64 != NULL){
		return WINDOW_DATA_STYLE_INT64;
		// Èç¹û´°¿ÚÊı¾İÀàĞÍÎªfloat
	}else if(window->Prob_Data_Float != NULL){
		return WINDOW_DATA_STYLE_FLOAT;
	}else{
		return WINDOW_DATA_STYLE_NONE;
	}
}

/**
 * @brief »æÖÆ´°¿Ú
 * @param ÎŞ
 * @return ÎŞ
 */
void OLED_DrawWindow(void){
	//Èç¹û´°¿Ú±êÖ¾Î»Ã»ÓĞ±»ÖÃÎ»£¬Ôò²»½øĞĞ´°¿ÚĞ§¹û£¬½«´°¿ÚµÄÄ¿±êÎ»ÖÃÖÃÎ»ÆÁÄ»Íâ²¿
	if(OLED_SustainCounter.SustainFlag == false){
		OLED_UI_Window.TargetArea.Height = 30;
		OLED_UI_Window.TargetArea.Width = 60;
		OLED_UI_Window.TargetArea.X = (OLED_WIDTH - 60)/2;	
		OLED_UI_Window.TargetArea.Y = -40;
	}
	if(CurrentWindow == NULL){
		return;
	}
	OLED_Font ChineseFont = GetOLED_Font(CurrentWindow->Text_FontSize,CHINESE),ASCIIFont = GetOLED_Font(CurrentWindow->Text_FontSize,ASCII);

	

	//Èç¹û´°¿Ú¶¯»­»¹Ã»ÓĞÍË³öÆÁÄ»£¬Ôò»æÖÆ´°¿Ú
	if(OLED_SustainCounter.SustainFlag == true || (OLED_UI_Window.CurrentArea.Height != 30 && OLED_UI_Window.CurrentArea.Width != 60 && OLED_UI_Window.CurrentArea.Y != -40)){
		//Í¨¹ıºêÅĞ¶ÏÊÇ·ñĞèÒª»æÖÆÔ²½Ç¾ØĞÎ
		if(CurrentWindow->General_WindowType == WINDOW_RECTANGLE){
			OLED_DrawRectangle(OLED_UI_Window.CurrentArea.X - 1,OLED_UI_Window.CurrentArea.Y - 1,OLED_UI_Window.CurrentArea.Width + 2,OLED_UI_Window.CurrentArea.Height + 2,OLED_UNFILLED);
			OLED_ClearArea(OLED_UI_Window.CurrentArea.X,OLED_UI_Window.CurrentArea.Y,OLED_UI_Window.CurrentArea.Width,OLED_UI_Window.CurrentArea.Height);
		}else{
			OLED_DrawRoundedRectangle(OLED_UI_Window.CurrentArea.X - 1,OLED_UI_Window.CurrentArea.Y - 1,OLED_UI_Window.CurrentArea.Width + 2,OLED_UI_Window.CurrentArea.Height + 2,2,OLED_UNFILLED);
			OLED_ClearArea(OLED_UI_Window.CurrentArea.X,OLED_UI_Window.CurrentArea.Y,OLED_UI_Window.CurrentArea.Width,OLED_UI_Window.CurrentArea.Height);
		}
		//ÏÔÊ¾ÎÄ×Ö
		/**
		 * ÏŞÖÆÏÔÊ¾µÄÇøÓò£º
		 * X£ºµ±Ç°´°¿ÚÇøÓòµÄÆğÊ¼µãX + ´Ë´°¿ÚÉèÖÃµÄ×ÖÌå×ó²à¼ä¾à
		 * Y£º µ±Ç°´°¿ÚÇøÓòµÄÆğÊ¼µãY + ×ÖÌå¶¥²¿¼ä¾à
		 * Width£ºµ±Ç°´°¿Ú¿í¶È¼õÈ¥ 2*ÎÄ×Ö±ß¾à ¼õÈ¥ Êı¾İµÄ¿í¶È ¼õÈ¥Êı¾İºÍÎÄ×ÖµÄ¾àÀëWINDOW_DATA_TEXT_DISTANCE
		 * Height£º×Ö·û´®¸ß¶È
		 * ÏÔÊ¾ÎÄ×ÖµÄÆğÊ¼µãÊÇ£º
		 * X£º µ±Ç°´°¿ÚÇøÓòµÄÆğÊ¼µãX + ´Ë´°¿ÚÉèÖÃµÄ×ÖÌå×ó²à¼ä¾à
		 * Y£º µ±Ç°´°¿ÚÇøÓòµÄÆğÊ¼µãY + ×ÖÌå¶¥²¿¼ä¾à
		 * Width£º×Ö·û´®¿í¶È
		 * Height£º×Ö·û´®¸ß¶È
		 * */
        // ÏÈ¼ÆËã²»°üº¬Êı¾İ¿í¶ÈµÄ×Ö·û´®¿í¶È×÷ÎªÄ¬ÈÏ¿í¶È
		int16_t MaxLength  = OLED_UI_Window.CurrentArea.Width - 2*CurrentWindow->Text_FontSideDistance;
		int16_t DataLength = 0;
		//¼ÆËãÊı¾İÀàĞÍ
		int8_t DataStyle = GetWindowProbDataStyle(CurrentWindow);
		//Èç¹û½ø¶ÈÌõÊı¾İ²»Îª¿Õ£¬ÔòËµÃ÷ÓÃ»§ÉèÖÃÁË½ø¶ÈÌõ£¬ÔòÏÔÊ¾½ø¶ÈÌõ
		//Èç¹ûÊı¾İÖ¸Õë²»Îª¿Õ
		if(DataStyle != WINDOW_DATA_STYLE_NONE ){
			//Èç¹ûÊı¾İÀàĞÍÎª¸¡µã
			if (DataStyle == WINDOW_DATA_STYLE_FLOAT)
			{
				//¼ÆËã×Ö·û´®±»ÏŞÖÆµÄ×î´ó¿í¶È
				MaxLength = OLED_UI_Window.CurrentArea.Width - 2*CurrentWindow->Text_FontSideDistance - CalcStringWidth(ChineseFont,	ASCIIFont,	"%.*f",FLOAT_POINT_LENGTH,*CurrentWindow->Prob_Data_Float) - WINDOW_DATA_TEXT_DISTANCE;
				//¼ÆËãÊı¾İËùÕ¼µÄ¿í¶È
				DataLength = CalcStringWidth(ChineseFont,ASCIIFont,"%.*f",FLOAT_POINT_LENGTH,*CurrentWindow->Prob_Data_Float);
				//ÏÔÊ¾Êı¾İ
				OLED_PrintfMixArea(
					//ÇøÓòÏŞÖÆ
					OLED_UI_Window.CurrentArea.X,
					OLED_UI_Window.CurrentArea.Y,
					OLED_UI_Window.CurrentArea.Width,
					OLED_UI_Window.CurrentArea.Height,
					OLED_UI_Window.CurrentArea.X + CurrentWindow->General_Width - 1 - CurrentWindow->Text_FontSideDistance - DataLength,
					OLED_UI_Window.CurrentArea.Y + CurrentWindow->Text_FontTopDistance,
					ChineseFont,ASCIIFont,
					"%.*f",FLOAT_POINT_LENGTH,*CurrentWindow->Prob_Data_Float);

			}else{
				
				

				switch (DataStyle) {
				    case WINDOW_DATA_STYLE_INT8:

						//¼ÆËã×Ö·û´®±»ÏŞÖÆµÄ×î´ó¿í¶È
						MaxLength = OLED_UI_Window.CurrentArea.Width - 2*CurrentWindow->Text_FontSideDistance - CalcStringWidth(ChineseFont,ASCIIFont,"%d",*CurrentWindow->Prob_Data_Int_8)- 		WINDOW_DATA_TEXT_DISTANCE;
						//¼ÆËãÊı¾İËùÕ¼µÄ¿í¶È
						DataLength = CalcStringWidth(ChineseFont,ASCIIFont,"%d",*CurrentWindow->Prob_Data_Int_8);
						//ÏÔÊ¾Êı¾İ
						OLED_PrintfMixArea(
							//ÇøÓòÏŞÖÆ
							OLED_UI_Window.CurrentArea.X,
							OLED_UI_Window.CurrentArea.Y,
							OLED_UI_Window.CurrentArea.Width,
							OLED_UI_Window.CurrentArea.Height,
							OLED_UI_Window.CurrentArea.X + CurrentWindow->General_Width - 1 - CurrentWindow->Text_FontSideDistance - DataLength,
							OLED_UI_Window.CurrentArea.Y + CurrentWindow->Text_FontTopDistance,
							ChineseFont,ASCIIFont,
							"%d",*CurrentWindow->Prob_Data_Int_8);

				        break;
				    case WINDOW_DATA_STYLE_INT16:
							//¼ÆËã×Ö·û´®±»ÏŞÖÆµÄ×î´ó¿í¶È
							MaxLength = OLED_UI_Window.CurrentArea.Width - 2*CurrentWindow->Text_FontSideDistance - CalcStringWidth(ChineseFont,ASCIIFont,"%d",*CurrentWindow->Prob_Data_Int_16)- 			WINDOW_DATA_TEXT_DISTANCE;
							//¼ÆËãÊı¾İËùÕ¼µÄ¿í¶È
							DataLength = CalcStringWidth(ChineseFont,ASCIIFont,"%d",*CurrentWindow->Prob_Data_Int_16);
							//ÏÔÊ¾Êı¾İ
							OLED_PrintfMixArea(
								//ÇøÓòÏŞÖÆ
								OLED_UI_Window.CurrentArea.X,
								OLED_UI_Window.CurrentArea.Y,
								OLED_UI_Window.CurrentArea.Width,
								OLED_UI_Window.CurrentArea.Height,
								OLED_UI_Window.CurrentArea.X + CurrentWindow->General_Width - 1 - CurrentWindow->Text_FontSideDistance - DataLength,
								OLED_UI_Window.CurrentArea.Y + CurrentWindow->Text_FontTopDistance,
								ChineseFont,ASCIIFont,
								"%d",*CurrentWindow->Prob_Data_Int_16);

				        break;
				    case WINDOW_DATA_STYLE_INT32:


							//¼ÆËã×Ö·û´®±»ÏŞÖÆµÄ×î´ó¿í¶È
							MaxLength = OLED_UI_Window.CurrentArea.Width - 2*CurrentWindow->Text_FontSideDistance - CalcStringWidth(ChineseFont,ASCIIFont,"%d",*CurrentWindow->Prob_Data_Int_32)- WINDOW_DATA_TEXT_DISTANCE;
							//¼ÆËãÊı¾İËùÕ¼µÄ¿í¶È
							DataLength = CalcStringWidth(ChineseFont,ASCIIFont,"%d",*CurrentWindow->Prob_Data_Int_32);
							//ÏÔÊ¾Êı¾İ
							OLED_PrintfMixArea(
								//ÇøÓòÏŞÖÆ
								OLED_UI_Window.CurrentArea.X,
								OLED_UI_Window.CurrentArea.Y,
								OLED_UI_Window.CurrentArea.Width,
								OLED_UI_Window.CurrentArea.Height,
								OLED_UI_Window.CurrentArea.X + CurrentWindow->General_Width - 1 - CurrentWindow->Text_FontSideDistance - DataLength,
								OLED_UI_Window.CurrentArea.Y + CurrentWindow->Text_FontTopDistance,
								ChineseFont,ASCIIFont,
								"%d",*CurrentWindow->Prob_Data_Int_32);

				        break;
				    case WINDOW_DATA_STYLE_INT64:

							//¼ÆËã×Ö·û´®±»ÏŞÖÆµÄ×î´ó¿í¶È
							MaxLength = OLED_UI_Window.CurrentArea.Width - 2*CurrentWindow->Text_FontSideDistance - CalcStringWidth(ChineseFont,ASCIIFont,"%d",*CurrentWindow->Prob_Data_Int_64)- 			WINDOW_DATA_TEXT_DISTANCE;
							//¼ÆËãÊı¾İËùÕ¼µÄ¿í¶È
							DataLength = CalcStringWidth(ChineseFont,ASCIIFont,"%d",*CurrentWindow->Prob_Data_Int_64);
							//ÏÔÊ¾Êı¾İ
							OLED_PrintfMixArea(
								//ÇøÓòÏŞÖÆ
								OLED_UI_Window.CurrentArea.X,
								OLED_UI_Window.CurrentArea.Y,
								OLED_UI_Window.CurrentArea.Width,
								OLED_UI_Window.CurrentArea.Height,
								OLED_UI_Window.CurrentArea.X + CurrentWindow->General_Width - 1 - CurrentWindow->Text_FontSideDistance - DataLength,
								OLED_UI_Window.CurrentArea.Y + CurrentWindow->Text_FontTopDistance,
								ChineseFont,ASCIIFont,
								"%d",*CurrentWindow->Prob_Data_Int_64);

				        break;

				}

				
				
				
				
				
				
			}
			

			//ÏÔÊ¾½ø¶ÈÌõ£¨Íâ¿ò£©
			OLED_DrawRoundedRectangle(OLED_UI_Window.CurrentArea.X +CurrentWindow->Prob_SideDistance,
			OLED_UI_Window.CurrentArea.Y + OLED_UI_Window.CurrentArea.Height  - CurrentWindow->Prob_LineHeight - CurrentWindow->Prob_BottomDistance,
			OLED_UI_Window.CurrentArea.Width- 2*CurrentWindow->Prob_SideDistance,CurrentWindow->Prob_LineHeight,2,OLED_UNFILLED);
			//ÏÔÊ¾½ø¶ÈÌõ£¨ÄÚ²¿£©
			OLED_DrawRectangle(OLED_UI_Window.CurrentArea.X +CurrentWindow->Prob_SideDistance + 2,
							OLED_UI_Window.CurrentArea.Y + OLED_UI_Window.CurrentArea.Height  - CurrentWindow->Prob_LineHeight - CurrentWindow->Prob_BottomDistance + 2,
							OLED_UI_ProbWidth.CurrentDistance>=OLED_UI_Window.CurrentArea.Width- 2*CurrentWindow->Prob_SideDistance - 4  ?OLED_UI_Window.CurrentArea.Width- 2*CurrentWindow->Prob_SideDistance - 4: OLED_UI_ProbWidth.CurrentDistance  ,CurrentWindow->Prob_LineHeight-4,OLED_FILLED);
		}
		if(CurrentWindow->Text_String != NULL){
			int16_t WindowTextStringLength = CalcStringWidth(ChineseFont,ASCIIFont,CurrentWindow->Text_String);
			//Èç¹û×Ö·û´®µÄ¿í¶È³¬¹ıÁË×î´óÏŞ¶¨¿í¶È
			if(WindowTextStringLength > MaxLength){
#if IF_WAIT_ANIMATION_FINISH
                if( OLED_UI_Window.CurrentArea.X == OLED_UI_Window.TargetArea.X &&
                        OLED_UI_Window.CurrentArea.Y == OLED_UI_Window.TargetArea.Y &&
                        OLED_UI_Window.CurrentArea.Width == OLED_UI_Window.TargetArea.Width &&
                        OLED_UI_Window.CurrentArea.Height == OLED_UI_Window.TargetArea.Height){
#endif
		    	CurrentWindow->_LineSlip-=LINE_SLIP_SPEED;
#if IF_WAIT_ANIMATION_FINISH
                }
#endif
		}
			if(CurrentWindow->_LineSlip < -WindowTextStringLength){
		   	 	CurrentWindow->_LineSlip =  MaxLength + 1;
			}

			OLED_PrintfMixArea(
			        OLED_UI_Window.CurrentArea.X + CurrentWindow->Text_FontSideDistance,
			        OLED_UI_Window.CurrentArea.Y + CurrentWindow->Text_FontTopDistance,
			        MaxLength,
			        OLED_UI_Window.CurrentArea.Height,
					OLED_UI_Window.CurrentArea.X + CurrentWindow->Text_FontSideDistance + CurrentWindow->_LineSlip,
					OLED_UI_Window.CurrentArea.Y + CurrentWindow->Text_FontTopDistance,
					ChineseFont,ASCIIFont,CurrentWindow->Text_String);
			}

	}else{
		
		if(CurrentWindow != NULL && CurrentWindow->General_WindowCloseCallback != NULL){
            CurrentWindow->General_WindowCloseCallback();  // Ö´ĞĞ¹Ø±Õ»Øµ÷
        }
		// Èç¹û´°¿Ú¶¯»­ÒÑ¾­ÍË³öÆÁÄ»£¬Ôò½«µ±Ç°´°¿ÚÖ¸ÕëÖ¸ÏòNULL
		OLED_UI_ProbWidth.CurrentDistance = 0;
		OLED_UI_ProbWidth.Derivative = 0;
		OLED_UI_ProbWidth.Error = 0;
		OLED_UI_ProbWidth.Integral = 0;
		OLED_UI_ProbWidth.LastError = 0;
		OLED_UI_ProbWidth.TargetDistance = 0;
		CurrentWindow = NULL;
	}
	
}







/**
 * @brief ½«²Ëµ¥ÕûÌåÏòÉÏÒÆ¶¯Ò»ĞĞ
 * @param ÎŞ
 * @return ÎŞ
 */
void MenuItemsMoveUp(void){
	OLED_UI_PageStartPoint.TargetPoint.Y -= (OLED_UI_LineStep.TargetDistance + CurrentMenuPage->General_FontSize);
}
/**
 * @brief ½«²Ëµ¥ÕûÌåÏòÏÂÒÆ¶¯Ò»ĞĞ
 * @param ÎŞ
 * @return ÎŞ
 */
void MenuItemsMoveDown(void){
	OLED_UI_PageStartPoint.TargetPoint.Y += (OLED_UI_LineStep.TargetDistance + CurrentMenuPage->General_FontSize);
}
/**
 * @brief ½«²Ëµ¥ÕûÌåÏò×óÒÆ¶¯Ò»ÁĞ
 * @param ÎŞ
 * @return ÎŞ
 */
void MenuItemsMoveLeft(void){
	OLED_UI_PageStartPoint.TargetPoint.X -= (OLED_UI_LineStep.TargetDistance + CurrentMenuPage->Tiles_TileWidth);
}
/**
 * @brief ½«²Ëµ¥ÕûÌåÏòÓÒÒÆ¶¯Ò»ÁĞ
 * @param ÎŞ
 * @return ÎŞ
 */
void MenuItemsMoveRight(void){
	OLED_UI_PageStartPoint.TargetPoint.X += (OLED_UI_LineStep.TargetDistance + CurrentMenuPage->Tiles_TileWidth);
}
/**
 * @brief »ñÈ¡µ±Ç°²Ëµ¥Ò³ÃæµÄ²ÛÎ»ÊıÁ¿
 * @param ÎŞ
 * @return µ±Ç°²Ëµ¥Ò³ÃæµÄ²ÛÎ»ÊıÁ¿
 */
int16_t GetCurrentMenuPageMaxSlotNum(void){
	return (int16_t)(CurrentMenuPage->List_MenuArea.Height - CurrentMenuPage->List_StartPointY+OLED_UI_LineStep.TargetDistance-1) / (OLED_UI_LineStep.TargetDistance + CurrentMenuPage->General_FontSize );
}



/**
 * @brief ÉèÖÃÄ¿±ê¹â±êÇøÓò
 * @param ÎŞ
 * @return ÎŞ
 */
void SetTargetCursor(void){
	//Èç¹ûµ±Ç°Ò³ÃæµÄÀàĞÍÎªListÀà
	if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
		//Ä¿±ê¹â±êx×ø±êµÈÓÚ ¿ªÊ¼´òÓ¡Ò³ÃæÆğÊ¼µãµÄx×ø±ê¼õ1¡¾¼õÈ¥1ÊÇÎªÁËÈ·±£¹â±ê¸²¸Çµ½²Ëµ¥ÏîµÄÎÄ×Ö¡¿
		OLED_UI_Cursor.TargetArea.X = OLED_UI_PageStartPoint.TargetPoint.X -1;

		//Ä¿±ê¹â±êy×ø±êµÈÓÚ ¿ªÊ¼´òÓ¡Ò³ÃæÆğÊ¼µãµÄy×ø±ê  ¼ÓÉÏ  (×ÖÌå¸ß¶È ¼Ó ĞĞ¾à)³Ë µ±Ç°Ò³ÃæµÄIDºÅ ¼õ1¡¾¼õÈ¥1ÊÇÎªÁËÈ·±£¹â±ê¸²¸Çµ½²Ëµ¥ÏîµÄÎÄ×Ö¡¿
		OLED_UI_Cursor.TargetArea.Y = OLED_UI_PageStartPoint.TargetPoint.Y + 
		CurrentMenuPage->_ActiveMenuID * ( CurrentMenuPage->General_LineSpace + CurrentMenuPage->General_FontSize) -1 ;
		//Ä¿±ê¹â±ê¸ß¶ÈµÈÓÚ ×ÖÌå¸ß¶È ¼Ó2¡¾¼Ó2ÊÇÎªÁËÈ·±£¹â±ê¸²¸Çµ½²Ëµ¥ÏîµÄÎÄ×Ö¡¿
		OLED_UI_Cursor.TargetArea.Height = CurrentMenuPage->General_FontSize + 2;
		//Ä¿±ê¹â±ê¿í¶ÈµÈÓÚ µ±Ç°×Ö·û´®µÄ¿í¶È	¡¾¼Ó2ÊÇÎªÁËÈ·±£¹â±ê¸²¸Çµ½²Ëµ¥ÏîµÄÎÄ×Ö¡¿
		//Èç¹ûÓĞĞĞÇ°×º
		int8_t LinePerfixWidth = 0;
		if(CurrentMenuPage->List_IfDrawLinePerfix == true){
			LinePerfixWidth = GetOLED_Font(CurrentMenuPage->General_FontSize,ASCII) + LINEPERFIX_DISTANCE;
		}
		////µ¥Ñ¡¿ò¿í¶È
		int8_t RadioCompensationWidth = 0;
		//Èç¹ûĞèÒª»æÖÆµ¥Ñ¡¿ò(¼´BoolRadioBox²»Îª¿Õ)
		if(CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].List_BoolRadioBox != NULL){
			RadioCompensationWidth = (GetOLED_Font(CurrentMenuPage->General_FontSize,CHINESE) + 2);
		}else{
			RadioCompensationWidth = 0;
		}
		OLED_UI_Cursor.TargetArea.Width = 
		fmin((float)CalcStringWidth(
			//×Ö·û´®³¤¶È
			GetOLED_Font(CurrentMenuPage->General_FontSize,CHINESE),GetOLED_Font(CurrentMenuPage->General_FontSize,ASCII),CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_item_text) + 2 + LinePerfixWidth ,
			//µ±Ç°Ò³ÃæµÄ¿í¶È¼Óµ±Ç°Ò³ÃæµÄÆğÊ¼×ø±ê¼õÈ¥¿ªÊ¼´òÓ¡Ò³ÃæÆğÊ¼µãµÄ×ø±ê¼õÈ¥6£¨ÊÇ¹ö¶¯Ìõ¿í¶È¼ÓÒ»£©¼ÓÉÏĞĞÇ°×ºµÄ¿í¶È
			OLED_UI_MenuFrame.CurrentArea.Width + OLED_UI_MenuFrame.CurrentArea.X - OLED_UI_PageStartPoint.CurrentPoint.X - 6 - LinePerfixWidth + LinePerfixWidth - RadioCompensationWidth) ;
	}
	//Èç¹ûµ±Ç°Ò³ÃæµÄÀàĞÍÎªTilesÀà
	if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
		//´ÅÌùÀà²»ĞèÒª¹â±êµÄÏÔÊ¾£¬ËùÒÔÉèÖÃÎª0.
		// SetCursorZero();
		OLED_UI_Cursor.TargetArea.X = CurrentMenuPage->Tiles_ScreenWidth/2 - CalcStringWidth(GetOLED_Font(CurrentMenuPage->General_FontSize,CHINESE),GetOLED_Font(CurrentMenuPage->General_FontSize,ASCII),CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_item_text)/2 - 1;
		OLED_UI_Cursor.TargetArea.Y = CurrentMenuPage->Tiles_ScreenHeight - CurrentMenuPage->General_FontSize - TILES_BOTTOM_DISTANCE - 1;
		OLED_UI_Cursor.TargetArea.Height = CurrentMenuPage->General_FontSize + 2;
		OLED_UI_Cursor.TargetArea.Width = CalcStringWidth(GetOLED_Font(CurrentMenuPage->General_FontSize,CHINESE),GetOLED_Font(CurrentMenuPage->General_FontSize,ASCII),CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_item_text) + 2;

	}
}
/**
 * @brief ÉèÖÃÄ¿±ê²Ëµ¥±ß¿ò
 * @param ÎŞ
 * @return ÎŞ
 */
void SetTargetMenuFrame(void){
	OLED_UI_MenuFrame.TargetArea.X = CurrentMenuPage->List_MenuArea.X;
	OLED_UI_MenuFrame.TargetArea.Y = CurrentMenuPage->List_MenuArea.Y;
	OLED_UI_MenuFrame.TargetArea.Width = CurrentMenuPage->List_MenuArea.Width;
	OLED_UI_MenuFrame.TargetArea.Height = CurrentMenuPage->List_MenuArea.Height;

}
/**
 * @brief ½«µ±Ç°Ò³ÃæµÄLineSlip ÉèÖÃÎª0
 * @param ÎŞ
 * @return ÎŞ
 */
void SetLineSplitZero(void){
	MenuPage* page = CurrentMenuPage;
	for(MenuID i = 0; i<GetMenuItemNum(page->General_MenuItems);i++){
		page->General_MenuItems[i]._LineSlip = 0;
	}
}

/**
 * @brief ÉèÖÃÄ¿±ê¹ö¶¯Ìõ¸ß¶È
 * @param ÎŞ
 * @return ÎŞ
 */
void SetTargetScrollBarHeight(void){
	if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
		OLED_UI_ScrollBarHeight.TargetDistance = (float)CurrentMenuPage->List_MenuArea.Height*(CurrentMenuPage->_ActiveMenuID + 1)/GetMenuItemNum(CurrentMenuPage->General_MenuItems);
	}else if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
		OLED_UI_ScrollBarHeight.TargetDistance = (float)(CurrentMenuPage->Tiles_ScreenWidth *(CurrentMenuPage->_ActiveMenuID + 1)/GetMenuItemNum(CurrentMenuPage->General_MenuItems));
	}
}
/**
 * @brief ÉèÖÃÄ¿±ê½ø¶ÈÌõ³¤¶È
 * @param ÎŞ
 * @return ÎŞ
 */
void SetTargetProbWidth(void){
	// if(CurrentWindow == NULL)
	// {
	// 	return;
	// }
	//È·ÈÏÊı¾İÀàĞÍ
	MenuWindow* window = CurrentWindow;
	int8_t DataStyle = GetWindowProbDataStyle(window);
	
	if(DataStyle != WINDOW_DATA_STYLE_NONE){
		if(DataStyle == WINDOW_DATA_STYLE_FLOAT){
			OLED_UI_ProbWidth.TargetDistance = ((*window->Prob_Data_Float - window->Prob_MinData)/((float)window->Prob_MaxData - window->Prob_MinData ))*	(OLED_UI_Window.CurrentArea.Width- 2*window->Prob_SideDistance - 4);
		}else{

			switch (DataStyle) {
				case WINDOW_DATA_STYLE_INT8:
				
					OLED_UI_ProbWidth.TargetDistance = ((*CurrentWindow->Prob_Data_Int_8 - window->Prob_MinData)/((float)window->Prob_MaxData - window->Prob_MinData ))*	(OLED_UI_Window.CurrentArea.Width- 2*window->Prob_SideDistance - 4);
					break;
				case WINDOW_DATA_STYLE_INT16:
				
					OLED_UI_ProbWidth.TargetDistance = ((*CurrentWindow->Prob_Data_Int_16 - window->Prob_MinData)/((float)window->Prob_MaxData - window->Prob_MinData ))*	(OLED_UI_Window.CurrentArea.Width- 2*window->Prob_SideDistance - 4);
					break;
				case WINDOW_DATA_STYLE_INT32:
				
					OLED_UI_ProbWidth.TargetDistance = ((*CurrentWindow->Prob_Data_Int_32 - window->Prob_MinData)/((float)window->Prob_MaxData - window->Prob_MinData ))*	(OLED_UI_Window.CurrentArea.Width- 2*window->Prob_SideDistance - 4);
					break;
				case WINDOW_DATA_STYLE_INT64:
				
					OLED_UI_ProbWidth.TargetDistance = ((*CurrentWindow->Prob_Data_Int_64 - window->Prob_MinData)/((float)window->Prob_MaxData - window->Prob_MinData ))*	(OLED_UI_Window.CurrentArea.Width- 2*window->Prob_SideDistance - 4);
					break;

					
			}


			
		}
	}else{
		return;
	}
	
}

/**
 * @brief ¸ù¾İµ±Ç°Ò³ÃæÇé¿ö¾ö¶¨ÊÇ·ñ»æÖÆĞĞÇ°×º
 * @param page ²Ëµ¥Ò³Ãæ½á¹¹ÌåÖ¸Õë
 * @param id ²Ëµ¥ÏîIDºÅ
 * @param CursorPoint ¹â±êµÄ×ø±ê
 * @return ÎŞ
 */
void DrawLinePermix(MenuPage* page,MenuID i,OLED_Point* CursorPoint,OLED_Font ChineseFont,OLED_Font ASCIIFont){
	//Èç¹ûĞèÒª´òÓ¡ĞĞÇ°×º
	if(page->List_IfDrawLinePerfix == true){
		char* LinePerfixSymb = "";
		//Èç¹ûµ±Ç°µÄ²Ëµ¥ÏîÓĞ×Ó²Ëµ¥ÇÒÃ»ÓĞ»Øµ÷º¯Êı£¬ÄÇÃ´´òÓ¡ÏàÓ¦µÄ·ûºÅ
		if(CurrentMenuPage->General_MenuItems[i].General_SubMenuPage != NULL && page->General_MenuItems[i].General_callback == NULL && page->General_MenuItems[i].List_BoolRadioBox == NULL){
			LinePerfixSymb = SUBMENU_PREFIX;
		}else//Èç¹ûÓĞ»Øµ÷º¯Êı
		if(CurrentMenuPage->General_MenuItems[i].General_SubMenuPage == NULL && page->General_MenuItems[i].General_callback != NULL && page->General_MenuItems[i].List_BoolRadioBox == NULL){
			LinePerfixSymb = FUNCTION_PREFIX;
		}else
		if(CurrentMenuPage->General_MenuItems[i].General_SubMenuPage == NULL && page->General_MenuItems[i].General_callback == NULL && page->General_MenuItems[i].List_BoolRadioBox != NULL)
		{
			LinePerfixSymb = RADIO_PREFIX;
		}else{
			LinePerfixSymb = NONE_PREFIX;
		}

		OLED_PrintfMixArea(//ÔÚÏŞÖÆµÄÇøÓòÄÚ´òÓ¡ÎÄ×Ö
					   //¹â±êµÄÆğÊ¼x×ø±ê
				       OLED_UI_MenuFrame.CurrentArea.X ,
					   OLED_UI_MenuFrame.CurrentArea.Y,
					   OLED_UI_MenuFrame.CurrentArea.Width - 6,
					   OLED_UI_MenuFrame.CurrentArea.Height,
					   //´òÓ¡ÎÄ×ÖµÄ×ø±ê
					   CursorPoint->X,
					   CursorPoint->Y,
					   //´òÓ¡ÎÄ×ÖµÄ´óĞ¡
					   ChineseFont,ASCIIFont,
					   //´òÓ¡ÎÄ×ÖµÄÄÚÈİ
					   LinePerfixSymb);
		
	}

}


/**
 * @brief ¸ù¾İ´«ÈëµÄ½á¹¹ÌåÊı×éÖ¸Õë´òÓ¡²Ëµ¥ÔªËØ
 * @param ÎŞ
 * @return ÎŞ
 */
void PrintMenuElements(void){
	//¡¾²È¿Ó¡¿½«µ±Ç°Ò³ÃæµÄ½á¹¹ÌåÖ¸ÕëÈ¡³ö£¬Èç¹û²»ÕâÑù×ö£¬ÈôÊÇÔÚforÑ­»·µ±ÖĞ¸Ä±ä½á¹¹ÌåÖ¸Õë£¬Ôò»áµ¼ÖÂforÑ­»·³ö´í£¬·ÃÎÊµ½NULLÖ¸Õë£¬·¢ÉúÓ²¼ş´íÎóÔì³É¿¨ËÀ
	MenuPage* page = CurrentMenuPage;
	OLED_ChangePoint TempTargetPoint = OLED_UI_PageStartPoint;
	OLED_ChangeArea TempTargetArea = OLED_UI_MenuFrame;
	
	//»ñÈ¡µ±Ç°Ò³ÃæµÄ²Ëµ¥ÏîÊıÁ¿
	MenuID num = GetMenuItemNum(page->General_MenuItems);
	//»ñÈ¡ÎÄ±¾´óĞ¡£¬´Óµ±Ç°Ò³ÃæµÄ×ÖÌå¿í¶ÈÍÆËã³öASCII×ÖÌå¿í¶ÈÓëÖĞÎÄ×ÖÌå¿í¶È
	OLED_Font ChineseFont = GetOLED_Font(page->General_FontSize,CHINESE),ASCIIFont = GetOLED_Font(page->General_FontSize,ASCII);
	//»æÖÆËùĞèµÄ×ø±ê¹â±ê,³õÊ¼»¯Îªµ±Ç°²Ëµ¥½á¹¹ÌåµÄ¿ªÊ¼µã
	OLED_Point CursorPoint = TempTargetPoint.CurrentPoint;
	//¼ì²â´ËÒ³ÃæÊÇ·ñÒÑ¾­³õÊ¼»¯ÁË£¬Èç¹ûÃ»ÓĞ£¬Ôò³õÊ¼»¯
   
	//Èç¹ûµ±Ç°Ò³ÃæµÄÀàĞÍÎªÁĞ±íÀà
	if(page->General_MenuType == MENU_TYPE_LIST){
		//ĞĞÇ°×º¿í¶È
		int16_t LinePerfixWidth = 0;
		////µ¥Ñ¡¿ò¿í¶È
		int16_t RadioCompensationWidth = 0;

		//»æÖÆ±ß¿ò¡¾×ø±ê¼õÒ»¿í¶È¼Ó¶şÊÇÎªÁËÊ¹µÃÍâ¿ò²»»áµ²×¡²Ëµ¥ÇøÓò£¬ËùÒÔÊµ¼ÊÉÏÍâ¿ò²»ÊôÓÚ²Ëµ¥ÇøÓò¡¿
		if(page->List_IfDrawFrame == true){
			OLED_DrawRectangle(TempTargetArea.CurrentArea.X-1,TempTargetArea.CurrentArea.Y-1,	TempTargetArea.CurrentArea.Width+2,TempTargetArea.CurrentArea.Height+2,OLED_UNFILLED);
		}
		//Èç¹ûĞèÒª»æÖÆĞĞÇ°×º
		if(page->List_IfDrawLinePerfix == true){
			//½«ĞĞÇ°×º¿í¶ÈÉèÖÃÎªASCII×ÖÌå¿í¶È¼ÓLINEPERFIX_DISTANCE£¨Óëºó·½×Ö·û´®µÄ¾àÀë£©
			LinePerfixWidth = ASCIIFont + LINEPERFIX_DISTANCE;
		}


		//´òÓ¡²Ëµ¥Ïî
		for(MenuID i = 0; i<num;i++){
			
			if(CursorPoint.Y + ChineseFont <0 || CursorPoint.Y > OLED_HEIGHT){
				CursorPoint.Y += (page->General_FontSize + OLED_UI_LineStep.CurrentDistance);
				continue;
			}

			//¼ÇÂ¼´ËÂÖÑ­»·µÄ×Ö·û´®¿í¶È
			int16_t StringLength = CalcStringWidth(ChineseFont,ASCIIFont,page->General_MenuItems[i].General_item_text);

			//¸ù¾İÇé¿ö»æÖÆĞĞÇ°×º
			DrawLinePermix(page,i,&CursorPoint,ChineseFont,ASCIIFont);

			//Èç¹ûĞèÒª»æÖÆµ¥Ñ¡¿ò(¼´BoolRadioBox²»Îª¿Õ)
			if(page->General_MenuItems[i].List_BoolRadioBox != NULL){
				RadioCompensationWidth = (ChineseFont + 2);
				char* RadioBoxSymb = "";
				if(*page->General_MenuItems[i].List_BoolRadioBox == true){
					RadioBoxSymb = "¡ö";
				}else{
					RadioBoxSymb = "¡õ";
				}


				OLED_PrintfMixArea(//ÔÚÏŞÖÆµÄÇøÓòÄÚ´òÓ¡ÎÄ×Ö
							   //¹â±êµÄÆğÊ¼x×ø±ê¼ÓÈëĞĞÇ°×º¿í¶È£¬ÕâÑù¿ÉÒÔ×Ô¶¯Áô³ö´òÓ¡ĞĞÇ°×ºµÄ¿Õ¼ä
						       TempTargetArea.CurrentArea.X ,
							   TempTargetArea.CurrentArea.Y,
							   TempTargetArea.CurrentArea.Width - 6,
							   TempTargetArea.CurrentArea.Height,

							   //´òÓ¡ÎÄ×ÖµÄ×ø±ê
							   CursorPoint.X + TempTargetArea.CurrentArea.Width - RadioCompensationWidth -9 ,
							   CursorPoint.Y,
							   //´òÓ¡ÎÄ×ÖµÄ´óĞ¡
							   ChineseFont,ASCIIFont,
							   //´òÓ¡ÎÄ×ÖµÄÄÚÈİ
							   RadioBoxSymb);
			}else{
				RadioCompensationWidth = 0;
			}

			//Èç¹û×Ö·û´®µÄ¿í¶È³¬¹ıÁË×î´óÏŞ¶¨¿í¶È
			if(StringLength > (TempTargetArea.TargetArea.X + TempTargetArea.TargetArea.Width - 6 - 2) - TempTargetPoint.TargetPoint.X - LinePerfixWidth - RadioCompensationWidth ){
#if IF_WAIT_ANIMATION_FINISH
			    if(TempTargetArea.TargetArea.Width == TempTargetArea.CurrentArea.Width &&
					TempTargetArea.TargetArea.X == TempTargetArea.CurrentArea.X &&
					TempTargetArea.TargetArea.Height == TempTargetArea.CurrentArea.Height &&
					TempTargetArea.TargetArea.Y == TempTargetArea.CurrentArea.Y){
#endif
						page->General_MenuItems[i]._LineSlip -= LINE_SLIP_SPEED;
#if IF_WAIT_ANIMATION_FINISH
					}
#endif
				
				
				//Èç¹ûpage->MenuItems[i]._LineSlipĞ¡ÓÚ¸ºµÄ×Ö·û´®¿í¶È£¬ËµÃ÷×Ö·û´®ÒÑ¾­×óÒÆµ½¿´²»¼û
				if(page->General_MenuItems[i]._LineSlip < -StringLength){
					//½«Î»ÖÃÉèÔÚ×î´óÏŞ¶¨¿í¶ÈµÄÓÒ²à¸Õ¸ÕºÃ¿´²»¼ûµÄµØ·½
					page->General_MenuItems[i]._LineSlip = (TempTargetArea.TargetArea.X + TempTargetArea.TargetArea.Width - 6) - CursorPoint.X - LinePerfixWidth;
				}


			}

			OLED_PrintfMixArea(TempTargetArea.CurrentArea.X + LinePerfixWidth + CurrentMenuPage->List_StartPointX ,
			 					TempTargetArea.CurrentArea.Y,
			 					TempTargetArea.CurrentArea.Width - 6 - LinePerfixWidth - CurrentMenuPage->List_StartPointX  -2 - RadioCompensationWidth,
			 					TempTargetArea.CurrentArea.Height,
								//×ø±ê¼ÓÉÏLinePerfixWidthÊÇÎªÁË¸øĞĞÇ°×ºÁôÏÂ¿Õ¼ä
							   	CursorPoint.X + LinePerfixWidth + page->General_MenuItems[i]._LineSlip,
							   	CursorPoint.Y,
							   	ChineseFont,ASCIIFont,page->General_MenuItems[i].General_item_text);

			// ´òÓ¡¹â±êÏÂÒÆ
			CursorPoint.Y += (page->General_FontSize + OLED_UI_LineStep.CurrentDistance);

		}

		//»æÖÆ¹ö¶¯ÌõÓëÆäÖĞĞÄÏß
		OLED_DrawRectangle(TempTargetArea.CurrentArea.X + TempTargetArea.CurrentArea.Width - 5,TempTargetArea.CurrentArea.Y,5,(OLED_UI_ScrollBarHeight.CurrentDistance > page->List_MenuArea.Height) ?page->List_MenuArea.Height:OLED_UI_ScrollBarHeight.CurrentDistance,OLED_FILLED);
		OLED_DrawLine(TempTargetArea.CurrentArea.X + TempTargetArea.CurrentArea.Width - 3,TempTargetArea.CurrentArea.Y,TempTargetArea.CurrentArea.X + TempTargetArea.CurrentArea.Width-3,TempTargetArea.CurrentArea.Height + TempTargetArea.CurrentArea.Y-1);
	}
	//Èç¹ûµ±Ç°µÄÒ³ÃæÎª´ÅÌùÀàĞÍ
	if(page->General_MenuType == MENU_TYPE_TILES){
		//´òÓ¡´ÅÌùÏî
		for(MenuID i = 0; i<num;i++){
			if(CursorPoint.X + page->Tiles_TileWidth < 0 || CursorPoint.X > OLED_WIDTH){
				CursorPoint.X += (page->Tiles_TileWidth + OLED_UI_LineStep.CurrentDistance);
				continue;
			}

			//ÏÔÊ¾´ÅÌùÍ¼±ê
			OLED_ShowImageArea(ceil(CursorPoint.X),CursorPoint.Y,page->Tiles_TileWidth,page->Tiles_TileHeight,0,0,page->Tiles_ScreenWidth,page->Tiles_ScreenHeight,page->General_MenuItems[i].Tiles_Icon == NULL?UnKnown:page->General_MenuItems[i].Tiles_Icon);
			// ´òÓ¡¹â±êÓÒÒÆ
			CursorPoint.X += (page->Tiles_TileWidth + OLED_UI_LineStep.CurrentDistance);
			//ÏÔÊ¾²Ëµ¥ÏîÎÄ×Ö
			
		}
		if(IF_DRAW_ARROR == true){
			//»æÖÆÖ¸Ê¾¼ıÍ·
			OLED_ShowImageArea(page->Tiles_ScreenWidth/2 - 3,0,6,5,0,0,page->Tiles_ScreenWidth,page->Tiles_ScreenHeight,Arrow);
		}
		//¼ÇÂ¼´ËÂÖÑ­»·µÄ×Ö·û´®¿í¶È
		int16_t StringLength = CalcStringWidth(ChineseFont,ASCIIFont,page->General_MenuItems[page->_ActiveMenuID].General_item_text);
		//Èç¹û×Ö·û´®µÄ¿í¶È´óÓÚÓÃ»§ËùÉèÖÃµÄÆÁÄ»¿í¶È
		if(StringLength > page->Tiles_ScreenWidth){
#if IF_WAIT_ANIMATION_FINISH
		    if(OLED_UI_PageStartPoint.CurrentPoint.X == OLED_UI_PageStartPoint.TargetPoint.X &&
		        OLED_UI_PageStartPoint.CurrentPoint.Y == OLED_UI_PageStartPoint.TargetPoint.Y ){
#endif
		        page->General_MenuItems[page->_ActiveMenuID]._LineSlip -= LINE_SLIP_SPEED;
#if IF_WAIT_ANIMATION_FINISH
		    }
#endif
		}else{
		    SetLineSplitZero();
		}
		//Èç¹ûpage->MenuItems[i]._LineSlipĞ¡ÓÚ¸ºµÄ×Ö·û´®¿í¶È£¬ËµÃ÷×Ö·û´®ÒÑ¾­×óÒÆµ½¿´²»¼û
		if(page->General_MenuItems[page->_ActiveMenuID]._LineSlip < -StringLength){
		     //½«Î»ÖÃÉèÔÚ×î´óÏŞ¶¨¿í¶ÈµÄÓÒ²à¸Õ¸ÕºÃ¿´²»¼ûµÄµØ·½
		     page->General_MenuItems[page->_ActiveMenuID]._LineSlip = page->Tiles_ScreenWidth + 1;
		}

		OLED_PrintfMixArea(0,0,page->Tiles_ScreenWidth,page->Tiles_ScreenHeight,
		        StringLength > page->Tiles_ScreenWidth? 0 + page->General_MenuItems[page->_ActiveMenuID]._LineSlip : page->Tiles_ScreenWidth/2 - CalcStringWidth(ChineseFont,ASCIIFont,page->General_MenuItems[page->_ActiveMenuID].General_item_text)/2 + page->General_MenuItems[page->_ActiveMenuID]._LineSlip,
							   page->Tiles_ScreenHeight - page->General_FontSize - TILES_BOTTOM_DISTANCE,
							   ChineseFont,ASCIIFont,
							   page->General_MenuItems[page->_ActiveMenuID].General_item_text);
		//»æÖÆ¹ö¶¯ÌõÓëÆäÖĞĞÄÏß
		int16_t ScrollBarHeight = (page->Tiles_ScreenHeight >= 128? 5:3);
		OLED_DrawRectangle(0,TILES_STARTPOINT_Y + page->Tiles_TileHeight + TILES_SCROLLBAR_Y,
						OLED_UI_ScrollBarHeight.CurrentDistance,ScrollBarHeight,OLED_FILLED);
		OLED_DrawLine(0,TILES_STARTPOINT_Y + page->Tiles_TileHeight + TILES_SCROLLBAR_Y + ScrollBarHeight/2,page->Tiles_ScreenWidth-1,TILES_STARTPOINT_Y + page->Tiles_TileHeight + TILES_SCROLLBAR_Y + ScrollBarHeight/2);

		
		
	}
	if(page->General_ShowAuxiliaryFunction != NULL){
		//»æÖÆ¸¨Öú¹¦ÄÜ
		page->General_ShowAuxiliaryFunction();
	}
	
}



/**
 * @brief °´¼üÓë±àÂëÆ÷¼ÇÂ¼º¯Êı
 * @param ÎŞ
 * @return ÉÏÒ»ÂÖÓëÕâÒ»ÂÖµÄ_ActiveMenuID²îÖµ
 * @note Êµ¼ÊÉÏ_ActiveMenuIDµÄÖµ²»±ä¡£
  */
MenuID_Type OLED_KeyAndEncoderRecord(void){
    static bool IfUpTapAndHold = false;
    static bool IfDownTapAndHold = false;
	//¼ÇÂ¼ÉÏÒ»ÂÖ°´¼ü×´Ì¬
    OLED_UI_LastKey.Up=OLED_UI_Key.Up;
	OLED_UI_LastKey.Down=OLED_UI_Key.Down;
	OLED_UI_LastKey.Enter=OLED_UI_Key.Enter;
	OLED_UI_LastKey.Back=OLED_UI_Key.Back;
    
    //¼ÇÂ¼µ±Ç°°´¼ü×´Ì¬
	OLED_UI_Key.Enter = Key_GetEnterStatus();
//	OLED_UI_Key.Back = Key_GetBackStatus();
	OLED_UI_Key.Up = Key_GetUpStatus();
	OLED_UI_Key.Down = Key_GetDownStatus();
#if IF_START_UP_AND_DOWN_LONG_PRESS

	static int16_t UpPressCounter = 0;
	static int16_t UpPressedCounter  = 0;
	static int16_t DownPressCounter = 0;
	static int16_t DownPressedCounter = 0;
	//Èç¹ûÉÏÒ»´ÎºÍÕâÒ»´Î¶¼°´ÏÂµÄ×´Ì¬³ÖĞø
	if(OLED_UI_LastKey.Up == 0 && OLED_UI_Key.Up == 0){
	    UpPressCounter++;
	    if(UpPressCounter >= (float)PRESS_TIME * 50){
	        UpPressedCounter ++;
	        if(UpPressedCounter >= (UpPressCounter >= (PRESS_TIME + CONTINUE_PRESS_TIME) * 50? CONTINUE_PRESS_MOVE_SPEED : PRESS_MOVE_SPEED)){
	            IfUpTapAndHold = true;
	            UpPressedCounter = 0;
	            IfUpTapAndHold = true;
	        }else{
	            IfUpTapAndHold = false;
	        }
	    }

	}else{
	    UpPressCounter = 0;
	    UpPressedCounter = 0;
	    IfUpTapAndHold = false;
	}
	//Èç¹ûÉÏÒ»´ÎºÍÕâÒ»´Î¶¼°´ÏÂµÄ×´Ì¬³ÖĞø
	    if(OLED_UI_LastKey.Down == 0 && OLED_UI_Key.Down == 0){
	        DownPressCounter++;

	        if(DownPressCounter >= (float)PRESS_TIME * 50){
	            DownPressedCounter ++;
	            if(DownPressedCounter >= (DownPressCounter >= (PRESS_TIME + CONTINUE_PRESS_TIME) * 50? CONTINUE_PRESS_MOVE_SPEED : PRESS_MOVE_SPEED)){
	                IfDownTapAndHold = true;
	                DownPressedCounter = 0;
	                IfDownTapAndHold = true;
	            }else{
	                IfDownTapAndHold = false;
	            }
	        }

	    }else{
	        DownPressCounter = 0;
	        DownPressedCounter = 0;
	        IfDownTapAndHold = false;
	    }

#endif
	if(OLED_UI_Key.Up == OLED_UI_Key.Down){
	    IfUpTapAndHold = false;
	    IfDownTapAndHold = false;

	}

	//»ñÈ¡µ±Ç°Ò³ÃæµÄ²Ëµ¥ÏîÊıÁ¿
	int16_t MaxID = GetMenuItemNum(CurrentMenuPage->General_MenuItems);
	MenuID_Type IncreaseID = {0,0};
	int16_t ActiveMenuID = CurrentMenuPage->_ActiveMenuID;
	int16_t LastActiveID = ActiveMenuID;
	//»ñÈ¡±àÂëÆ÷×´Ì¬
	ActiveMenuID += Encoder_Get();

	//Èç¹û¼ì²âµ½¡¾ÉÏ¡¿°´¼üµÄÉÏÒ»×´Ì¬ÓëÕâ´ÎµÄ×´Ì¬²»Í¬£¬ÇÒÕâÒ»×´Ì¬ÊÇÌ§Æğ×´Ì¬£¬ËµÃ÷ÓÃ»§°´ÏÂÁË¡¾ÉÏ¡¿°´¼ü£¬²¢ÇÒ¸Õ¸Õ²ÅÌ§Æğ
	if((OLED_UI_Key.Up != OLED_UI_LastKey.Up && OLED_UI_Key.Up == 1) || IfUpTapAndHold){
		ActiveMenuID--;
	}

	//Èç¹û¼ì²âµ½¡¾ÏÂ¡¿°´¼üµÄÉÏÒ»×´Ì¬ÓëÕâ´ÎµÄ×´Ì¬²»Í¬£¬ÇÒÕâÒ»×´Ì¬ÊÇÌ§Æğ×´Ì¬£¬ËµÃ÷ÓÃ»§°´ÏÂÁË¡¾ÏÂ¡¿°´¼ü£¬²¢ÇÒ¸Õ¸Õ²ÅÌ§Æğ
	if((OLED_UI_Key.Down != OLED_UI_LastKey.Down && OLED_UI_Key.Down == 1) || IfDownTapAndHold){
		ActiveMenuID++;
	}
	
	IncreaseID.Unsafe = ActiveMenuID - LastActiveID;
	//Èç¹ûµ±Ç°²Ëµ¥ÏîIDºÅÔ½½ç£¬Ôò½«ÆäÏŞÖÆÔÚ0~MaxID-1Ö®¼ä
	if(ActiveMenuID >= MaxID-1){
		ActiveMenuID = MaxID-1;
	}
	if(ActiveMenuID <= 0){
		ActiveMenuID = 0;
	}

	IncreaseID.Safe = ActiveMenuID - LastActiveID;
	return IncreaseID;
}
/**
 * @brief ½«KeyEnterFlagÖÃFLAGSTART£¬ÓÃÓÚ±ê¼Ç»Øµ÷º¯ÊıÕıÔÚÖ´ĞĞ
 * @param ÎŞ
 * @return ÎŞ
 */
void SetEnterFlag(void){
	KeyEnterFlag = FLAGSTART;
	Encoder_Disable();  // Ê§ÄÜ±àÂëÆ÷
}

/**
 * @brief ½«KeyEnterFlagÖÃFLAGEND£¬ÓÃÓÚ±íÊ¾»Øµ÷º¯ÊıÃ»ÓĞÕıÔÚÖ´ĞĞ
 * @param void
 * @return ÎŞ
 */
void ResetEnterFlag(void){
	KeyEnterFlag = FLAGEND;
}


/**
 * @brief ½«FadeOutFlagÖÃFLAGSTART£¬ÓÃÓÚ±ê¼Ç½¥ÒşĞ§¹ûÕıÔÚÖ´ĞĞ
 * @param ÎŞ
 * @return ÎŞ
 */
void SetFadeOutFlag(MutexFlag action){
	FadeOutFlag = action;
	Encoder_Disable();  // Ê§ÄÜ±àÂëÆ÷
}

/**
 * @brief ½«FadeOutFlagÖÃFLAGEND£¬ÓÃÓÚ±íÊ¾½¥ÒşĞ§¹ûÃ»ÓĞÕıÔÚÖ´ĞĞ
 * @param void
 * @return ÎŞ
 */
void ResetFadeOutFlag(void){
	FadeOutFlag = FLAGEND;
}

/**
 * @brief ´Ëº¯Êı´¦Àí°´ÏÂEnter°´¼üºóµÄÇé¿ö£¬·ÖÎªÒÔÏÂ¼¸ÖÖÇé¿ö£º
 * 		1. µ±Ç°²Ëµ¥ÏîÓĞ»Øµ÷º¯Êı£¬ÄÇÃ´¾Í½«KeyEnterFlagÖÃÎ»¡£
 * 		2. µ±Ç°²Ëµ¥ÏîÃ»ÓĞ»Øµ÷º¯Êı£¬µ«ÊÇÓĞ×Ó²Ëµ¥£¬ÄÇÃ´¾Í½øÈë×Ó²Ëµ¥
 * 		3. µ±Ç°²Ëµ¥ÏîÃ»ÓĞ»Øµ÷º¯Êı£¬Ò²Ã»ÓĞ×Ó²Ëµ¥£¬ÄÇÃ´¾ÍÊ²Ã´¶¼²»×ö
 * 		4. µ±Ç°²Ëµ¥Ïî¼ÈÓĞ»Øµ÷º¯Êı£¬ÓÖÓĞ×Ó²Ëµ¥£¬ÄÇÃ´¾ÍÊ²Ã´Ò²²»×ö
 * @param ÎŞ
 * @return ÎŞ
 */
void EnterEventMenuItem(void){
    //Èç¹ûµ±Ç°Ñ¡ÖĞ²Ëµ¥ÏîÓĞ»Øµ÷º¯Êı²¢ÇÒÃ»ÓĞ×Ó²Ëµ¥£¬ÄÇÃ´¾Í½«KeyEnterFlagÖÃÎ»
    if(CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_callback != NULL && CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_SubMenuPage == NULL){

        SetEnterFlag();
    }
	//Èç¹ûµ±Ç°Ñ¡ÖĞ²Ëµ¥ÏîÃ»ÓĞ»Øµ÷º¯Êı£¬µ«ÊÇÓĞ×Ó²Ëµ¥£¬ÄÇÃ´ÖÃÈ·ÈÏ±êÖ¾Î»
    if(CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_SubMenuPage != NULL 
	&& CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_callback == NULL){
		SetFadeOutFlag(ENTER_FLAGSTART);
    }

}
/**
 * @brief °´ÏÂ¡¾·µ»Ø¡¿°´¼üºóµÄ²Ù×÷
 * @param ÎŞ
 * @return ÎŞ
 */
void BackEventMenuItem(void){
	//Èç¹ûµ±Ç°²Ëµ¥µÄ¸¸²Ëµ¥²»Îª¿Õ
	if(CurrentMenuPage->General_ParentMenuPage != NULL){
		//ÖÃ·µ»Ø±êÖÃÎ»
        SetFadeOutFlag(BACK_FLAGSTART);
    }
	
}
/**
 * @brief ·µ»Øº¯Êı
 * @param ÎŞ
 * @note Ä£Äâ°´ÏÂ¡¾·µ»Ø¡¿°´¼ü
 * @return ÎŞ
 */
void OLED_UI_Back(void){
	BackEventMenuItem();
}

/**
 * @brief ½øÈë²¢ÔËĞĞ»Øµ÷º¯Êı
 * @param void
 * @return ÎŞ
 */
void RunCurrentCallBackFunction(void){
	//KeyEnterFlag == FLAGSTARTÕâ¸öÌõ¼ş±íÊ¾enter±»´¥·¢
	if(KeyEnterFlag == FLAGSTART){
		CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_callback();	
		//ÔÚ»Øµ÷º¯ÊıÖ´ĞĞÍê±ÏÖ®ºó£¬½«KeyEnterFlag¸´Î»¡£
		ResetEnterFlag();
		Encoder_Disable();  // Ê¹ÄÜ±àÂëÆ÷
	}
}
/**
 * @brief ½«µ±Ç°¹â±êÇøÓòÓëÄ¿±ê¹â±êÇøÓò¶¼ÉèÖÃÎª0
 * @param ÎŞ
 * @return ÎŞ
 */
void SetCursorZero(void){
	OLED_UI_Cursor.TargetArea.X = 0;
	OLED_UI_Cursor.TargetArea.Y = 0;
	OLED_UI_Cursor.TargetArea.Width = 0;
	OLED_UI_Cursor.TargetArea.Height = 0;
	OLED_UI_Cursor.CurrentArea.X = 0;
	OLED_UI_Cursor.CurrentArea.Y = 0;
	OLED_UI_Cursor.CurrentArea.Width = 0;
	OLED_UI_Cursor.CurrentArea.Height = 0;
}
/**
 * @brief ÉèÖÃ´°¿Ú´óĞ¡
 * @param Width ´°¿Ú¿í¶È
 * @param Height ´°¿Ú¸ß¶È
 * @return ÎŞ
 */
void OLED_UI_CreateWindow(MenuWindow* window){
	//Èç¹ûµ±Ç°´°¿Ú»¹Ã»ÓĞ½áÊøÓÃ»§¾ÍÖØĞÂ´´½¨´°¿Ú£¬Ôò²»ÖØÖÃ½ø¶ÈÌõ³¤¶È£¬±£³Ö¶¯»­Á¬¹áĞÔ
	if(CurrentWindow == NULL){
		//½«µ±Ç°½ø¶ÈÌõ³¤¶ÈÖÃÁã
		OLED_UI_ProbWidth.CurrentDistance = 0;
		window->_LineSlip = 0;
	}
	//½«´°¿Ú±êÖ¾Î»ÉèÖÃÎªtrue£¬±íÊ¾¿ªÊ¼´°¿ÚĞ§¹û
	OLED_SustainCounter.SustainFlag = true;
	
	//ÖØÖÃ´°¿ÚĞ§¹ûÊ±¼ä¼ÆÊıÆ÷
	OLED_SustainCounter.count = 0;
	//Èç¹û´°¿Ú±êÖ¾Î»±»ÖÃÎ»£¬ÔòÉèÖÃÄ¿±ê´°¿ÚµÄÎ»ÖÃµ½ÆÁÄ»ÖĞÑë
	
	//ÉèÖÃÄ¿±ê´°¿ÚµÄÎ»ÖÃÎªÆÁÄ»ÖĞÑë¾ÓÖĞÏÔÊ¾
	OLED_UI_Window.TargetArea.Width = window->General_Width;
	OLED_UI_Window.TargetArea.Height = window->General_Height;
	OLED_UI_Window.TargetArea.X = (OLED_WIDTH - window->General_Width)/2;
	OLED_UI_Window.TargetArea.Y = (OLED_HEIGHT - window->General_Height)/2;
	
	//½«µ±Ç°´°¿ÚÖ¸ÕëÖ¸Ïòwindow
	CurrentWindow = window;
	
}




/**
 * @brief ÔËĞĞ½¥ÒşĞ§¹û
 * @param ÎŞ
 * @return ÎŞ
 */
void RunFadeOut(void){
	/*Èç¹ûµ±Ç°µÄFadeOutFlagÒÑ¾­±»ÖÃÎ»£¬ÔòËµÃ÷ÕıÔÚÔËĞĞ½¥ÒşĞ§¹û¡£
	µ±Ç°ÔÚÔËĞĞ½¥ÒşĞ§¹ûµÄÇ°ÌáÌõ¼şÓĞ2¸ö£º
	1.¡¾ÔÚ°´ÏÂÈ·ÈÏ¼üµÄÇé¿öÏÂ¡¿¡¾Èç¹ûµ±Ç°Ñ¡ÖĞ²Ëµ¥ÏîÃ»ÓĞ»Øµ÷º¯Êı£¬µ«ÊÇÓĞ×Ó²Ëµ¥¡¿£¬´ËÊ± FadeOutFlag == ENTER_FLAGSTART
	2.¡¾ÔÚ°´ÏÂ·µ»Ø¼üµÄÇé¿öÏÂ¡¿¡¾Èç¹ûµ±Ç°²Ëµ¥µÄ¸¸²Ëµ¥²»Îª¿Õ¡¿£¬´ËÊ± FadeOutFlag == BACK_FLAGSTART
	*/
	if(FadeOutFlag != FLAGEND){
		
		//Èç¹ûµ±Ç°²Ëµ¥ÊÇÁĞ±íÀà
		if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
			//µ±Ç°²Ëµ¥ÏîµÄÒ³ÃæÀàĞÍÊÇÁĞ±íÀàµÄÇé¿öÏÂ£¬°´ÏÂÁËÈ·ÈÏ°´¼ü
			if(FadeOutFlag == ENTER_FLAGSTART){
				// ÔÚÓĞÈ·ÈÏ²Ù×÷Ö®ºó£¬µ±Ç°Ò³Ãæ²Å±»È·ÈÏÎªÒÑ¾­³õÊ¼»¯
				CurrentMenuPage->_IfInit = true;
				//£¨ÔÚÓĞ×Ó²Ëµ¥µÄÇé¿öÏÂ£©Èç¹ûµ±Ç°Ò³ÃæµÄµ±Ç°×Ó²Ëµ¥ÏîµÄÒ³ÃæÀàĞÍÊÇÁĞ±íÀà
				if(CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_SubMenuPage->General_MenuType == MENU_TYPE_LIST){
					//Ö»Çå³ıµ±Ç°Ò³ÃæÇøÓò
					OLED_UI_FadeoutCurrentArea(CurrentMenuPage->List_MenuArea.X,CurrentMenuPage->List_MenuArea.Y,CurrentMenuPage->List_MenuArea.Width-5,CurrentMenuPage->List_MenuArea.Height-2);
				}else{
					//Çå³ıÈ«²¿ÇøÓò
					OLED_UI_FadeoutAllArea();
					//½«¹ö¶¯ÌõµÄµ±Ç°¸ß¶ÈÉèÎª0
					OLED_UI_ScrollBarHeight.CurrentDistance = 0;
				}

				//½«µ±Ç°²Ëµ¥µÄÎ»ÖÃ±£´æ£¬ÒÔ±ã·µ»ØÊ±»Ö¸´
				CurrentMenuPage->_StartPoint.X = OLED_UI_PageStartPoint.TargetPoint.X;
				CurrentMenuPage->_StartPoint.Y = OLED_UI_PageStartPoint.TargetPoint.Y;

				//½«µ±Ç°²Ëµ¥µÄÖ¸ÕëÖ¸Ïò×Ó²Ëµ¥
				CurrentMenuPage = CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_SubMenuPage;
				//¶Ôµ±Ç°×Ó²Ëµ¥½øĞĞ³õÊ¼»¯
				CurrentMenuPageInit();
			}
			//µ±Ç°²Ëµ¥ÏîµÄÒ³ÃæÀàĞÍÊÇÁĞ±íÀàµÄÇé¿öÏÂ£¬°´ÏÂÁËÈ¡Ïû°´¼ü
			if(FadeOutFlag == BACK_FLAGSTART) {
				//Èç¹ûµ±Ç°Ò³ÃæµÄ¸¸²Ëµ¥ÏîµÄÒ³ÃæÀàĞÍÊÇÁĞ±íÀà
				if(CurrentMenuPage->General_ParentMenuPage->General_MenuType == MENU_TYPE_LIST){
					//Ö»Çå³ıµ±Ç°Ò³ÃæÇøÓò
					OLED_UI_FadeoutCurrentArea(CurrentMenuPage->List_MenuArea.X,CurrentMenuPage->List_MenuArea.Y,CurrentMenuPage->List_MenuArea.Width-5,CurrentMenuPage->List_MenuArea.Height-2);
				}else{
					//Çå³ıÈ«²¿ÇøÓò
					OLED_UI_FadeoutAllArea();
					//½«¹ö¶¯ÌõµÄµ±Ç°¸ß¶ÈÉèÎª0
					OLED_UI_ScrollBarHeight.CurrentDistance = 0;
				}
				//½«µ±Ç°²Ëµ¥µÄÖ¸ÕëÖ¸Ïò¸¸²Ëµ¥
				CurrentMenuPage = CurrentMenuPage->General_ParentMenuPage;
				//½«µ±Ç°²Ëµ¥µÄÎ»ÖÃµÈ²ÎÊı»Ö¸´
				if(CurrentMenuPage->_IfInit == false){
		
					CurrentMenuPageInit();
				}else{
					//½«µ±Ç°²Ëµ¥µÄÎ»ÖÃµÈ²ÎÊı»Ö¸´
					CurrentMenuPageBackUp();
				}
			}
			
			//½«FadeOutFlag¸´Î»
			ResetFadeOutFlag();
			//½«µ±Ç°¹â±êÇøÓòÓëÄ¿±ê¹â±êÇøÓò¶¼ÉèÖÃÎª0
			SetCursorZero();

		}else //Èç¹ûµ±Ç°²Ëµ¥ÀàĞÍÊÇ´ÅÌùÀà
		if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
			//Çå³ıÈ«²¿ÇøÓò
			OLED_UI_FadeoutAllArea();
			//µ±Ç°²Ëµ¥ÏîµÄÒ³ÃæÀàĞÍÊÇ´ÅÌùÀàµÄÇé¿öÏÂ£¬°´ÏÂÁËÈ·ÈÏ²Ù×÷
			if(FadeOutFlag == ENTER_FLAGSTART){
				// ÔÚÓĞÈ·ÈÏ²Ù×÷Ö®ºó£¬µ±Ç°Ò³Ãæ²Å±»È·ÈÏÎªÒÑ¾­³õÊ¼»¯
				CurrentMenuPage->_IfInit = true;
				//½«µ±Ç°²Ëµ¥µÄÎ»ÖÃ±£´æ£¬ÒÔ±ã·µ»ØÊ±»Ö¸´
				CurrentMenuPage->_StartPoint.X = OLED_UI_PageStartPoint.TargetPoint.X;
				CurrentMenuPage->_StartPoint.Y = OLED_UI_PageStartPoint.TargetPoint.Y;
				//½«µ±Ç°²Ëµ¥µÄÖ¸ÕëÖ¸Ïò×Ó²Ëµ¥
				CurrentMenuPage = CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].General_SubMenuPage;
				//¶Ôµ±Ç°×Ó²Ëµ¥½øĞĞ³õÊ¼»¯
				CurrentMenuPageInit();
				
			}
			//µ±Ç°²Ëµ¥ÏîµÄÒ³ÃæÀàĞÍÊÇ´ÅÌùÀàµÄÇé¿öÏÂ£¬°´ÏÂÁË·µ»Ø²Ù×÷
			if(FadeOutFlag == BACK_FLAGSTART){
				//½«µ±Ç°²Ëµ¥µÄÖ¸ÕëÖ¸Ïò¸¸²Ëµ¥
				CurrentMenuPage = CurrentMenuPage->General_ParentMenuPage;
				if(CurrentMenuPage->_IfInit == false){
		
					CurrentMenuPageInit();
				}else{
					//½«µ±Ç°²Ëµ¥µÄÎ»ÖÃµÈ²ÎÊı»Ö¸´
					CurrentMenuPageBackUp();
				}
				
			}
			//½«¹ö¶¯ÌõµÄµ±Ç°¸ß¶ÈÉèÎª0
			OLED_UI_ScrollBarHeight.CurrentDistance = 0;
			//½«µ±Ç°¹â±êÇøÓòÓëÄ¿±ê¹â±êÇøÓò¶¼ÉèÖÃÎª0
			SetCursorZero();
			//½«FadeOutFlag¸´Î»
			ResetFadeOutFlag();
		}
		// Ê¹ÄÜ±àÂëÆ÷
		Encoder_Disable();
		
	}
}

/**
 * @brief ´°¿ÚÄÚ²¿Êı¾İ´¦Àí
 * @param ÎŞ
 * @return ÎŞ
 */
void OLED_WindowDataDispose(){
	
	if(CurrentWindow != NULL){
		if(OLED_SustainCounter.count >= (int16_t)(CurrentWindow->General_ContinueTime * 50)){
			OLED_SustainCounter.SustainFlag = false;
			OLED_SustainCounter.count = 0;
		}
		
	}
	
	int8_t DataStyle = GetWindowProbDataStyle(CurrentWindow);
	//Èç¹û´°¿ÚÓĞÊı¾İ
	if(DataStyle != WINDOW_DATA_STYLE_NONE){

		//Èç¹ûÊı¾İÀàĞÍÊÇÕûÊı
		if(DataStyle == WINDOW_DATA_STYLE_FLOAT){
			//Èç¹ûÊÇĞ¡Êı
			*CurrentWindow->Prob_Data_Float += (WindowProbDeltaData * CurrentWindow->Prob_DataStep);
			if(*CurrentWindow->Prob_Data_Float <= CurrentWindow->Prob_MinData) {*CurrentWindow->Prob_Data_Float = CurrentWindow->Prob_MinData;}
			if(*CurrentWindow->Prob_Data_Float >= CurrentWindow->Prob_MaxData) {*CurrentWindow->Prob_Data_Float = CurrentWindow->Prob_MaxData;}
		}else{
			

			switch (DataStyle) {
				case WINDOW_DATA_STYLE_INT8:
					*CurrentWindow->Prob_Data_Int_8 += (WindowProbDeltaData * CurrentWindow->Prob_DataStep);
					if(*CurrentWindow->Prob_Data_Int_8 <= CurrentWindow->Prob_MinData) {*CurrentWindow->Prob_Data_Int_8 = CurrentWindow->Prob_MinData;}
					if(*CurrentWindow->Prob_Data_Int_8 >= CurrentWindow->Prob_MaxData) {*CurrentWindow->Prob_Data_Int_8 = CurrentWindow->Prob_MaxData;}
					break;
				case WINDOW_DATA_STYLE_INT16:
					
					*CurrentWindow->Prob_Data_Int_16 += (WindowProbDeltaData * CurrentWindow->Prob_DataStep);
					if(*CurrentWindow->Prob_Data_Int_16 <= CurrentWindow->Prob_MinData) {*CurrentWindow->Prob_Data_Int_16 = CurrentWindow->Prob_MinData;}
					if(*CurrentWindow->Prob_Data_Int_16 >= CurrentWindow->Prob_MaxData) {*CurrentWindow->Prob_Data_Int_16 = CurrentWindow->Prob_MaxData;}
					break;
				case WINDOW_DATA_STYLE_INT32:
					*CurrentWindow->Prob_Data_Int_32 += (WindowProbDeltaData * CurrentWindow->Prob_DataStep);
					if(*CurrentWindow->Prob_Data_Int_32 <= CurrentWindow->Prob_MinData) {*CurrentWindow->Prob_Data_Int_32 = CurrentWindow->Prob_MinData;}
					if(*CurrentWindow->Prob_Data_Int_32 >= CurrentWindow->Prob_MaxData) {*CurrentWindow->Prob_Data_Int_32 = CurrentWindow->Prob_MaxData;}
					break;
				case WINDOW_DATA_STYLE_INT64:
					*CurrentWindow->Prob_Data_Int_64 += (WindowProbDeltaData * CurrentWindow->Prob_DataStep);
					if(*CurrentWindow->Prob_Data_Int_64 <= CurrentWindow->Prob_MinData) {*CurrentWindow->Prob_Data_Int_64 = CurrentWindow->Prob_MinData;}
					if(*CurrentWindow->Prob_Data_Int_64 >= CurrentWindow->Prob_MaxData) {*CurrentWindow->Prob_Data_Int_64 = CurrentWindow->Prob_MaxData;}
					break;

			}
			
		}
	}
	WindowProbDeltaData = 0;
	
}

/**
 * @brief ´òÓ¡²Ëµ¥ÔªËØ²¢¸ù¾İÄ¿±êÖµ¸Ä±äÔªËØµÄ²ÎÊı
 * @param ÎŞ
 * @return ÎŞ
 */
void MoveMenuElements(void){

	//ÉèÖÃÄ¿±ê¹â±êÇøÓò
	SetTargetCursor();
	//ÉèÖÃÄ¿±ê²Ëµ¥±ß¿ò
	SetTargetMenuFrame();
	//ÉèÖÃÄ¿±ê½ø¶ÈÌõ³¤¶È
	SetTargetProbWidth();
	// //ÉèÖÃÄ¿±ê¹ö¶¯Ìõ¸ß¶È
	SetTargetScrollBarHeight();


	// ¸Ä±ä²Ëµ¥ÆğÊ¼ÔªËØµÄ×ø±ê
	ChangePoint(&OLED_UI_PageStartPoint);
	// ¸Ä±ä²Ëµ¥ÏîµÄĞĞ¼ä¾à
	ChangeDistance(&OLED_UI_LineStep);
	// ¸Ä±ä¹ö¶¯Ìõ¸ß¶È
	ChangeDistance(&OLED_UI_ScrollBarHeight);
	
//	if (CurrentMenuPage == &StopWatchMenuPage) {
//				SetTargetCursor_StopWatch();  // ÔÚÕâÀïÉèÖÃÄ¿±êÎ»ÖÃ
//				
//    }
	  static uint8_t frameCount = 0;
// Ãë±íÒ³ÃæµÄÌØÊâ´¦Àí
    if (CurrentMenuPage == &StopWatchMenuPage) {
      
        
        SetTargetCursor_StopWatch();
        
        // µÚÒ»Ö¡Ö±½ÓÉèÖÃµ±Ç°Î»ÖÃµÈÓÚÄ¿±êÎ»ÖÃ£¬Ìø¹ı¶¯»­
        if (frameCount == 0) {
            OLED_StopWatch_Cursor.CurrentArea = OLED_StopWatch_Cursor.TargetArea;
        } else {
            ChangeArea(&OLED_StopWatch_Cursor);
        }
        
        frameCount++;
    } else {
        frameCount = 0;  // Àë¿ªÒ³ÃæÊ±ÖØÖÃÖ¡¼ÆÊı
    }
    
    // ... ÆäËû»æÖÆ
	// ¸Ä±ä²Ëµ¥±ß¿ò²ÎÊı
	ChangeArea(&OLED_UI_MenuFrame);

	// ´òÓ¡²Ëµ¥Ò³ÃæµÄÔªËØ
	PrintMenuElements();

	//¸Ä±ä¹â±êµÄ²ÎÊı
	ChangeArea(&OLED_UI_Cursor);
	
	if (CurrentMenuPage == &StopWatchMenuPage) {
//		    ChangeArea(&OLED_StopWatch_Cursor);
        DrawStopWatchCursor();  // ÔÚÕâÀï»æÖÆ¶¯»­¹â±ê
    }
	
	//ÏÔÊ¾¹â±ê
	ReverseCoordinate(OLED_UI_Cursor.CurrentArea.X,OLED_UI_Cursor.CurrentArea.Y,OLED_UI_Cursor.CurrentArea.Width,OLED_UI_Cursor.CurrentArea.Height,CurrentMenuPage->General_CursorStyle);
	//ÉèÖÃÑÕÉ«Ä£Ê½
	OLED_SetColorMode(ColorMode);

	OLED_Brightness(OLED_UI_Brightness);

	ChangeDistance(&OLED_UI_ProbWidth);
	// ¸Ä±ä´°¿Ú²ÎÊı
	ChangeArea(&OLED_UI_Window);

	// ´¦Àí´°¿ÚÊı¾İ
	OLED_WindowDataDispose();

	// »æÖÆ´°¿Ú
	OLED_DrawWindow();


	
	
}


/**
 * @brief OLED_UIµÄÖ÷Ñ­»·º¯Êı
 * @param ÎŞ
 * @note ¸Ãº¯ÊıĞèÒª·ÅÔÚÖ÷Ñ­»·ÖĞµ÷ÓÃ£¬ÒÔ±ãÊµÏÖUIµÄË¢ĞÂ
 * @return ÎŞ
 */
void OLED_UI_MainLoop(void){

	//µ±½¥Òş»¥³âËø±»ÖÃÎ»Ê±£¬ÔËĞĞ½¥ÒşĞ§¹û
	RunFadeOut();
	
	//ÇåÆÁ
	OLED_Clear();

	

	//ÒÆ¶¯²Ëµ¥ÔªËØ
	MoveMenuElements();

	
	//µ±»¥³âËø±»ÖÃÎ»Ê±£¬ÔËĞĞµ±Ç°²Ëµ¥ÏîµÄ»Øµ÷º¯Êı
	RunCurrentCallBackFunction();
	
	//ÏÔÊ¾FPS
	OLED_UI_ShowFPS();
	
	//Ë¢ÆÁ
	OLED_Update();
	
	//¼ì²â¿ª¹Ø»ú
	if(Key_GetEvent(KEY_ENTER) == KEY_LONG_PRESS){
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	
	}
	
	
	
}




/**
 * @brief  OLED_UIµÄÖĞ¶Ïº¯Êı£¬ÄÚ²¿°üº¬ĞèÔÚÖĞ¶ÏÄÚ´¦ÀíµÄÈÎÎñ
 * @param  ÎŞ
 * @note ÓÉÓÚ¸Ãº¯ÊıÔÚÖĞ¶Ïµ±ÖĞµ÷ÓÃ£¬ËùÒÔ¾¡Á¿²»Éæ¼°Ö¸ÕëµÄ²Ù×÷£¬ÒòÎªÎÒ·¢ÏÖÔÚstm32ÉÏÃ»ÓĞÎÊÌâµÄÖ¸Õë»áÔÚesp32ÉÏÔì³ÉÒì³£¡£
 * @return ÎŞ
 */
void OLED_UI_InterruptHandler(void){
	// »ñÈ¡µ±Ç°ÆÁÄ»Ë¢ĞÂÂÊ
    GetFPS();
	
	MenuWindow* window = CurrentWindow;
	// Èç¹ûµ±Ç°ÓĞÕıÔÚÖ´ĞĞµÄ»Øµ÷º¯Êı£¬Ôò²»´¦ÀíÖĞ¶ÏÄÚµÄÈÎÎñ
    if(GetEnterFlag() && GetFadeoutFlag()){
    	

		//»ñÈ¡_ActiveMenuIDµÄ±ä»¯Öµ£¬_ActiveMenuIDµÄÖµ²»±ä£¬²¢¼ÇÂ¼ÁË°´¼üµÄ±ä»¯¡£
		//´Ëº¯Êı·µ»ØÁ½¸öÖµ£¬¼´°²È«ÖµºÍ·Ç°²È«Öµ£¬°²È«ÖµÓÃÓÚÒÆ¶¯ÉÏÏÂ¹â±ê£¬·Ç°²È«ÖµÓÃÓÚ´°¿Ú½ø¶ÈÌõ
		MenuID_Type IncreaseID = OLED_KeyAndEncoderRecord();


		//Èç¹û´°¿ÚÍ£ÁôµÄ±êÖ¾Î»Îªtrue£¬ËµÃ÷µ±Ç°ÕıÔÚÔËĞĞ´°¿Ú£¨´°¿Ú»¹Ã»ÓĞÍêÈ«ÏûÊ§£©
		if(OLED_SustainCounter.SustainFlag == true){
			//Èç¹û±àÂëÆ÷»òÊÇ°´¼üµÄ±ä»¯Öµ²»ÊÇ0
			if(IncreaseID.Unsafe != 0){
				//´°¿Ú¼ÆÊıÖµÇåÁã£¬Ê¹µÃ´°¿Ú¼ÌĞøÍ£ÁôÔ¤ÉèµÄÊ±¼ä
				OLED_SustainCounter.count = 0;
			}
			//½«¸Ä±äµÄÖµ¸³ÓèÈ«¾Ö±äÁ¿WindowProbDeltaData£¬ÓÃÓÚÔÚÖ÷Ñ­»·ÄÚ²¿¸Ä±ä´°¿Ú½ø¶ÈÌõµÄ³¤¶È£¨¸Ä±ä½ø¶ÈÌõ¼°Æäµ±Ç°ÖµÉæ¼°Ö¸Õë²Ù×÷£¬¾­¹ıÊµ²âÔÚÖĞ¶ÏÄÚ²Ù×÷ÓĞ¸ÅÂÊ»áÔì³ÉÒì³££©
			WindowProbDeltaData += IncreaseID.Unsafe;
			//½«°²È«ÖµÇåÁã£¬ÒòÎª´ËÊ±´°¿Ú²¢Ã»ÓĞÏûÊ§£¬¶ø°²È«ÖµÊÇ×÷ÓÃÓÚ²Ëµ¥idµÄ
			IncreaseID.Safe = 0;
		}
		 
		//Èç¹û±ä»¯ÖµĞ¡ÓÚ0£¬ÄÇÃ´Ïàµ±ÓÚ°´ÏÂIncreaseID.Safe´Î¡¾ÉÏ¡¿°´¼ü
		if(IncreaseID.Safe < 0 ){
			for(int i = 0; i < -IncreaseID.Safe; i++){
				//Èç¹ûµ±Ç°²Ëµ¥ÀàĞÍÊÇÁĞ±íÀà
				if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
					/*********************°´ÏÂ¡¾ÉÏ¡¿°´¼ü½øĞĞµÄ²Ù×÷*************************/
					//Èç¹ûµ±Ç°²Ëµ¥Ò³ÃæµÄ¹â±êÒÑ¾­µ½´ï×î¶¥²¿µÄ²ÛÎ»,²¢ÇÒµ±Ç°²Ëµ¥Ïî²»ÊÇµÚÒ»¸ö²Ëµ¥Ïî£¬ÄÇÃ´¾ÍÏòÏÂÒÆ¶¯²Ëµ¥ÏîµÄÄ¿±êÎ»ÖÃ
           			if(CurrentMenuPage->_Slot == 0 && CurrentMenuPage->_ActiveMenuID !=0){
						MenuItemsMoveDown();

					}
					//Èç¹û¹â±ê»¹Ã»ÓĞµ½´ï×î¶¥²¿µÄ²ÛÎ»£¬ÄÇÃ´¾ÍÏòÉÏÒÆ¶¯²ÛÎ»
					if(CurrentMenuPage->_Slot > 0){
						CurrentMenuPage->_Slot--;
					}
					CurrentMenuPage->_ActiveMenuID--;
				}
				//Èç¹ûµ±Ç°²Ëµ¥ÀàĞÍÊÇÁĞ±íÀà
				if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
					CurrentMenuPage->_ActiveMenuID--;
					MenuItemsMoveRight();
				}
			}
		}
		if(IncreaseID.Safe > 0){
			for(int i = 0; i < IncreaseID.Safe; i++){
				/*********************°´ÏÂ¡¾ÏÂ¡¿°´¼ü½øĞĞµÄ²Ù×÷*************************/
					if(CurrentMenuPage->General_MenuType == MENU_TYPE_LIST){
					//Èç¹ûµ±Ç°²Ëµ¥Ò³ÃæµÄ¹â±êÒÑ¾­µ½´ï×îµ×²¿µÄ²ÛÎ»,²¢ÇÒµ±Ç°²Ëµ¥Ïî²»ÊÇ×îºóÒ»¸ö²Ëµ¥Ïî£¬ÄÇÃ´¾ÍÏòÉÏÒÆ¶¯²Ëµ¥ÏîµÄÄ¿±êÎ»ÖÃ
					if(CurrentMenuPage->_Slot == GetCurrentMenuPageMaxSlotNum()-1 && CurrentMenuPage->_ActiveMenuID != GetMenuItemNum(CurrentMenuPage->General_MenuItems)){
						MenuItemsMoveUp();

					}
					//Èç¹û¹â±ê»¹Ã»ÓĞµ½´ï×îµ×²¿µÄ²ÛÎ»£¬ÄÇÃ´¾ÍÏòÏÂÒÆ¶¯²ÛÎ»
					if(CurrentMenuPage->_Slot < GetCurrentMenuPageMaxSlotNum()-1){
						CurrentMenuPage->_Slot++;
					}
					CurrentMenuPage->_ActiveMenuID++;
				}
				//Èç¹ûµ±Ç°²Ëµ¥ÀàĞÍÊÇÁĞ±íÀà
				if(CurrentMenuPage->General_MenuType == MENU_TYPE_TILES){
					CurrentMenuPage->_ActiveMenuID++;
					MenuItemsMoveLeft();
				}

			}
		}
		
	
    	
		//Èç¹û¼ì²âµ½¡¾·µ»Ø¡¿°´¼üµÄÉÏÒ»×´Ì¬ÓëÕâ´ÎµÄ×´Ì¬²»Í¬£¬ÇÒÕâÒ»×´Ì¬ÊÇÌ§Æğ×´Ì¬£¬ËµÃ÷ÓÃ»§°´ÏÂÁË¡¾·µ»Ø¡¿°´¼ü£¬²¢ÇÒ¸Õ¸Õ²ÅÌ§Æğ
		if(OLED_UI_Key.Back != OLED_UI_LastKey.Back && OLED_UI_Key.Back == 1){
			//Èç¹ûµ±Ç°Ã»ÓĞÔËĞĞ´°¿Ú
			if(OLED_SustainCounter.SustainFlag != true){
				BackEventMenuItem();
			}else{
				OLED_SustainCounter.count = (int16_t)(window->General_ContinueTime * 50);
			}
			
		}
		//Èç¹û¼ì²âµ½¡¾È·ÈÏ¡¿°´¼üµÄÉÏÒ»×´Ì¬ÓëÕâ´ÎµÄ×´Ì¬²»Í¬£¬ÇÒÕâÒ»×´Ì¬ÊÇÌ§Æğ×´Ì¬£¬ËµÃ÷ÓÃ»§°´ÏÂÁË¡¾È·ÈÏ¡¿°´¼ü£¬²¢ÇÒ¸Õ¸Õ²ÅÌ§Æğ
		if(OLED_UI_Key.Enter != OLED_UI_LastKey.Enter && OLED_UI_Key.Enter == 1){
			//Èç¹ûµ±Ç°Ã»ÓĞÔËĞĞ´°¿Ú
			if(OLED_SustainCounter.SustainFlag != true){
				EnterEventMenuItem();
				if (CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].List_BoolRadioBox != NULL) {
				    *CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].List_BoolRadioBox = !(*CurrentMenuPage->General_MenuItems[CurrentMenuPage->_ActiveMenuID].List_BoolRadioBox);
				}
			}else{
				//½«°²È«ÖµÇåÁã£¬ÒòÎª´ËÊ±´°¿Ú²¢Ã»ÓĞÏûÊ§£¬¶ø°²È«ÖµÊÇ×÷ÓÃÓÚ²Ëµ¥idµÄ
				OLED_SustainCounter.count = (int16_t)(window->General_ContinueTime * 50);
			}
		}
		
	}
	
    //Èç¹ûµ±Ç°ÕıÔÚÔËĞĞ´°¿Ú£¬ÄÇÃ´¼ÆÊı
	if(OLED_SustainCounter.SustainFlag == true){
		OLED_SustainCounter.count++;
	}
	Key_Update();//Í¨ÓÃ°´¼ü´¦Àí
}
#endif

/***************************************ĞİÃßÏà¹Ø********************************************************/
uint16_t StopCounter = 0 ;
bool isInStopMode = false;


void Sleep_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Event;  
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
       
    StopCounter = 0;
}
void Enter_Stop_Mode(void) {

    OLED_WriteCommand(0xAE);
    
    // ÉèÖÃ»½ĞÑÊÂ¼ş
    EXTI_ClearFlag(EXTI_Line4);
    
    // ½øÈëSTOPÄ£Ê½
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFE);
    
    // »½ĞÑºóÖ´ĞĞµ½ÕâÀï
    StopCounter = 0;
    
    // ÖØĞÂÅäÖÃÏµÍ³Ê±ÖÓ£¨STOPÄ£Ê½»á¹Ø±ÕHSI£©
    SystemInit();
    
    OLED_UI_Init(&ColckMenuPage);   
}
/**************°´¼ü³õÊ¼»¯ÖØÖÃ**********************/
void OLED_UI_ResetAllKeys(void) {
    // ÖØÖÃÈ«¾Ö°´¼ü×´Ì¬
    OLED_UI_Key.Enter = 1;
    OLED_UI_Key.Back = 1;
    OLED_UI_Key.Up = 1;
    OLED_UI_Key.Down = 1;
    
    OLED_UI_LastKey.Enter = 1;
    OLED_UI_LastKey.Back = 1;
    OLED_UI_LastKey.Up = 1;
    OLED_UI_LastKey.Down = 1;
    
}
