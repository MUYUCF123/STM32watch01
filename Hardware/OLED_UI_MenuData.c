#include "OLED_UI_MenuData.h"
#include "OLED_UI.h"
#include "MyRTC.h"
#include "StopWatch.h"
#include "MPU6050.h"
#include "GeneralKey.h"
#include "Game_Dino.h"
#include "AD.h"
/*此文件用于存放菜单数据。实际上菜单数据可以存放在任何地方，存放于此处是为了规范与代码模块化*/
//函数声明
void MPUAuxFunc(void);
void DinoGameAuxFunc(void);
void DinoGameStart(void);
//秒表相关数据
extern uint32_t StopWatchToltalSec;
bool StopWatch_Flag = false;
extern	bool Dino_Flag  ;
extern	int Score ;
extern struct Object_Position barrier;
extern struct Object_Position dino;
// ColorMode 是一个在OLED_UI当中定义的bool类型变量，用于控制OLED显示的颜色模式， DARKMODE 为深色模式， LIGHTMOOD 为浅色模式。这里将其引出是为了创建单选框菜单项。
extern bool ColorMode;
extern bool OLED_UI_ShowFps;
// OLED_UI_Brightness 是一个在OLED_UI当中定义的int16_t类型变量，用于控制OLED显示的亮度。这里将其引出是为了创建调整亮度的滑动条窗口，范围0-255。
extern int16_t OLED_UI_Brightness;
float testfloatnum = 0.5;
int32_t testintnum = 1;
#define SPEED 10
//游戏的回调
void DinoGameOver(){
	Dino_Flag = false ;
	Score = 0;
	BackEventMenuItem();
 
}
//窗口的关闭回调
void TimeWindowClose(void){
	MyRTC_SetTime();
}

//关于窗口的结构体
MenuWindow SetBrightnessWindow = {
	.General_Width = 80,								//窗口宽度
	.General_Height = 28, 							//窗口高度
	.Text_String = "屏幕亮度屏幕亮度",					//窗口标题
	.Text_FontSize = OLED_UI_FONT_12,				//字高
	.Text_FontSideDistance = 4,							//字体距离左侧的距离
	.Text_FontTopDistance = 3,							//字体距离顶部的距离
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//窗口类型
	.General_ContinueTime = 4.0,						//窗口持续时间

	.Prob_Data_Int_16 = &OLED_UI_Brightness,				//显示的变量地址
	.Prob_DataStep = 5,								//步长
	.Prob_MinData = 5,									//最小值
	.Prob_MaxData = 255, 								//最大值
	.Prob_BottomDistance = 3,							//底部间距
	.Prob_LineHeight = 8,								//进度条高度
	.Prob_SideDistance = 4,								//边距
	.General_WindowCloseCallback =NULL
};
MenuWindow SetYearWindow = {
	.General_Width = 80,								//窗口宽度
	.General_Height = 28, 							//窗口高度
	.Text_String = "年",					//窗口标题
	.Text_FontSize = OLED_UI_FONT_12,				//字高
	.Text_FontSideDistance = 4,							//字体距离左侧的距离
	.Text_FontTopDistance = 3,							//字体距离顶部的距离
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//窗口类型
	.General_ContinueTime = 4.0,						//窗口持续时间

	.Prob_Data_Int_32 = &MyRTC_Time[0],				//显示的变量地址
	.Prob_DataStep = 1,								//步长
	.Prob_MinData = 1024,									//最小值
	.Prob_MaxData = 4096, 								//最大值
	.Prob_BottomDistance = 3,							//底部间距
	.Prob_LineHeight = 8,								//进度条高度
	.Prob_SideDistance = 4,								//边距
	.General_WindowCloseCallback = TimeWindowClose
};
MenuWindow SetMonWindow = {
	.General_Width = 80,								//窗口宽度
	.General_Height = 28, 							//窗口高度
	.Text_String = "月",					//窗口标题
	.Text_FontSize = OLED_UI_FONT_12,				//字高
	.Text_FontSideDistance = 4,							//字体距离左侧的距离
	.Text_FontTopDistance = 3,							//字体距离顶部的距离
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//窗口类型
	.General_ContinueTime = 4.0,						//窗口持续时间

	.Prob_Data_Int_32 = &MyRTC_Time[1],				//显示的变量地址
	.Prob_DataStep = 1,								//步长
	.Prob_MinData = 1,									//最小值
	.Prob_MaxData = 12, 								//最大值
	.Prob_BottomDistance = 3,							//底部间距
	.Prob_LineHeight = 8,								//进度条高度
	.Prob_SideDistance = 4,								//边距
	.General_WindowCloseCallback = TimeWindowClose
};
MenuWindow SetDayWindow = {
	.General_Width = 80,								//窗口宽度
	.General_Height = 28, 							//窗口高度
	.Text_String = "日",					//窗口标题
	.Text_FontSize = OLED_UI_FONT_12,				//字高
	.Text_FontSideDistance = 4,							//字体距离左侧的距离
	.Text_FontTopDistance = 3,							//字体距离顶部的距离
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//窗口类型
	.General_ContinueTime = 4.0,						//窗口持续时间

	.Prob_Data_Int_32 = &MyRTC_Time[2],				//显示的变量地址
	.Prob_DataStep = 1,								//步长
	.Prob_MinData = 1,									//最小值
	.Prob_MaxData = 31, 								//最大值
	.Prob_BottomDistance = 3,							//底部间距
	.Prob_LineHeight = 8,								//进度条高度
	.Prob_SideDistance = 4,								//边距
	.General_WindowCloseCallback = TimeWindowClose
};
MenuWindow SetHourWindow = {
	.General_Width = 80,								//窗口宽度
	.General_Height = 28, 							//窗口高度
	.Text_String = "时",					//窗口标题
	.Text_FontSize = OLED_UI_FONT_12,				//字高
	.Text_FontSideDistance = 4,							//字体距离左侧的距离
	.Text_FontTopDistance = 3,							//字体距离顶部的距离
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//窗口类型
	.General_ContinueTime = 4.0,						//窗口持续时间

	.Prob_Data_Int_32 = &MyRTC_Time[3],				//显示的变量地址
	.Prob_DataStep = 1,								//步长
	.Prob_MinData = 1,									//最小值
	.Prob_MaxData = 24, 								//最大值
	.Prob_BottomDistance = 3,							//底部间距
	.Prob_LineHeight = 8,								//进度条高度
	.Prob_SideDistance = 4,								//边距
	.General_WindowCloseCallback = TimeWindowClose
};
MenuWindow SetMinWindow = {
	.General_Width = 80,								//窗口宽度
	.General_Height = 28, 							//窗口高度
	.Text_String = "分",					//窗口标题
	.Text_FontSize = OLED_UI_FONT_12,				//字高
	.Text_FontSideDistance = 4,							//字体距离左侧的距离
	.Text_FontTopDistance = 3,							//字体距离顶部的距离
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//窗口类型
	.General_ContinueTime = 4.0,						//窗口持续时间

	.Prob_Data_Int_32 = &MyRTC_Time[4],				//显示的变量地址
	.Prob_DataStep = 1,								//步长
	.Prob_MinData = 0,									//最小值
	.Prob_MaxData = 60, 								//最大值
	.Prob_BottomDistance = 3,							//底部间距
	.Prob_LineHeight = 8,								//进度条高度
	.Prob_SideDistance = 4,								//边距
	.General_WindowCloseCallback = TimeWindowClose
};
MenuWindow SetSecWindow = {
	.General_Width = 80,								//窗口宽度
	.General_Height = 28, 							//窗口高度
	.Text_String = "年",					//窗口标题
	.Text_FontSize = OLED_UI_FONT_12,				//字高
	.Text_FontSideDistance = 4,							//字体距离左侧的距离
	.Text_FontTopDistance = 3,							//字体距离顶部的距离
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//窗口类型
	.General_ContinueTime = 4.0,						//窗口持续时间

	.Prob_Data_Int_32 = &MyRTC_Time[5],				//显示的变量地址
	.Prob_DataStep = 1,								//步长
	.Prob_MinData = 0,									//最小值
	.Prob_MaxData = 60, 								//最大值
	.Prob_BottomDistance = 3,							//底部间距
	.Prob_LineHeight = 8,								//进度条高度
	.Prob_SideDistance = 4,								//边距
	.General_WindowCloseCallback = TimeWindowClose
};
/**
 * @brief 创建显示亮度窗口
 */
void BrightnessWindow(void){
	OLED_UI_CreateWindow(&SetBrightnessWindow);
}
void YearWindow(void){
	OLED_UI_CreateWindow(&SetYearWindow);
}
void MonWindow(void){
	OLED_UI_CreateWindow(&SetMonWindow);
}
void DayWindow(void){
	OLED_UI_CreateWindow(&SetDayWindow);
}
void HourWindow(void){
	OLED_UI_CreateWindow(&SetHourWindow);
}
void MinWindow(void){
	OLED_UI_CreateWindow(&SetMinWindow);
}
void SecWindow(void){
	OLED_UI_CreateWindow(&SetSecWindow);
}
//关于窗口的结构体
MenuWindow NullWindow = {
	.General_Width = 80,								//窗口宽度
	.General_Height = 28, 							//窗口高度
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//窗口类型
	.General_ContinueTime = 4.0,						//窗口持续时间
	.General_WindowCloseCallback =NULL
};
/**
 * @brief 创建显示亮度窗口
 */
void EmptyWindow(void){
	OLED_UI_CreateWindow(&NullWindow);
}
//关于窗口的结构体
MenuWindow TextWindow = {
	.General_Width = 75,								//窗口宽度
	.General_Height = 18, 							//窗口高度
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//窗口类型
	.General_ContinueTime = 4.0,						//窗口持续时间

	.Text_String = "文字text",					//窗口标题
	.Text_FontSize = OLED_UI_FONT_12,				//字高
	.Text_FontSideDistance = 8,							//字体距离左侧的距离
	.Text_FontTopDistance = 2,							//字体距离顶部的距离
	.General_WindowCloseCallback =NULL

};

//主LOGO移动的结构体
OLED_ChangePoint LogoMove;
//主LOGO文字移动的结构体
OLED_ChangePoint LogoTextMove;
//welcome文字移动的结构体
OLED_ChangePoint WelcomeTextMove;

extern OLED_ChangePoint OLED_UI_PageStartPoint ;

void ClockAuxFunc(void){
  MyRTC_ReadTime();
	OLED_Printf(0,0,OLED_6X8_HALF ,"%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
	OLED_Printf(0,16,OLED_12x24_HALF,"%02d:%02d",MyRTC_Time[3],MyRTC_Time[4]);
	OLED_Printf(60,24,OLED_8X16_HALF,":%02d",MyRTC_Time[5]);
	Show_Battery();
	
}

//设置菜单项的辅助显示函数
void SettingAuxFunc(void){
	//在规定位置显示LOGO
	if(fabs(OLED_UI_PageStartPoint.CurrentPoint.X - OLED_UI_PageStartPoint.TargetPoint.X) < 4){
		LogoMove.TargetPoint.X = 0;
		LogoMove.TargetPoint.Y = 0;
	}
	//将LOGOTEXT移动到屏幕右侧看不见的地方
	LogoTextMove.TargetPoint.X = 129;
	LogoTextMove.TargetPoint.Y = 0;
	//将Welcome文字移动到屏幕底部看不见的地方
	WelcomeTextMove.TargetPoint.X = 128;
	WelcomeTextMove.TargetPoint.Y = 0;
	ChangePoint(&LogoMove);
	OLED_ShowImageArea(LogoMove.CurrentPoint.X,LogoMove.CurrentPoint.Y,32,64,0,0,128,128,OLED_UI_SettingsLogo);
	ChangePoint(&LogoTextMove);
	OLED_ShowImageArea(LogoTextMove.CurrentPoint.X,LogoTextMove.CurrentPoint.Y,26,64,0,0,128,128,OLED_UI_LOGOTEXT64);
	ChangePoint(&WelcomeTextMove);
	OLED_ShowImageArea(WelcomeTextMove.CurrentPoint.X,WelcomeTextMove.CurrentPoint.Y,16,64,0,0,128,128,OLED_UI_LOGOGithub);
}
//关于时间的辅助显示函数
void SettingTimeAuxFunc(void){
	//将LOGO移动到屏幕上方看不见的地方
	LogoMove.TargetPoint.X = 0;
	LogoMove.TargetPoint.Y = -80;
	ChangePoint(&LogoMove);
    OLED_ShowImageArea(LogoMove.CurrentPoint.X,LogoMove.CurrentPoint.Y,32,64,0,0,128,128,OLED_UI_SettingsLogo);
	//在屏幕右侧显示LOGO文字
	if(fabs(OLED_UI_PageStartPoint.CurrentPoint.X - OLED_UI_PageStartPoint.TargetPoint.X) < 4){
		LogoTextMove.TargetPoint.X = 102;
		LogoTextMove.TargetPoint.Y = 0;
	}
	ChangePoint(&LogoTextMove);
	OLED_ShowImageArea(LogoTextMove.CurrentPoint.X,LogoTextMove.CurrentPoint.Y,26,64,0,0,128,128,OLED_UI_LOGOTIME);
}
//关于菜单的辅助显示函数
void AboutThisDeviceAuxFunc(void){
	//将LOGO移动到屏幕上方看不见的地方
	LogoMove.TargetPoint.X = 0;
	LogoMove.TargetPoint.Y = -80;
	ChangePoint(&LogoMove);
    OLED_ShowImageArea(LogoMove.CurrentPoint.X,LogoMove.CurrentPoint.Y,32,64,0,0,128,128,OLED_UI_SettingsLogo);
	//在屏幕右侧显示LOGO文字
	if(fabs(OLED_UI_PageStartPoint.CurrentPoint.X - OLED_UI_PageStartPoint.TargetPoint.X) < 4){
		LogoTextMove.TargetPoint.X = 102;
		LogoTextMove.TargetPoint.Y = 0;
	}
	ChangePoint(&LogoTextMove);
	OLED_ShowImageArea(LogoTextMove.CurrentPoint.X,LogoTextMove.CurrentPoint.Y,26,64,0,0,128,128,OLED_UI_LOGOTEXT64);
}
//关于OLED UI的辅助显示函数
void AboutOLED_UIAuxFunc(void){
	//将LOGO移动到屏幕上方看不见的地方
	LogoMove.TargetPoint.X = 0;
	LogoMove.TargetPoint.Y = -80;
	ChangePoint(&LogoMove);
	OLED_ShowImageArea(LogoMove.CurrentPoint.X,LogoMove.CurrentPoint.Y,32,64,0,0,128,128,OLED_UI_SettingsLogo);
	//在屏幕右测显示Welcome文字
	if(fabs(OLED_UI_PageStartPoint.CurrentPoint.X - OLED_UI_PageStartPoint.TargetPoint.X) < 4){
		WelcomeTextMove.TargetPoint.X = 110;
		WelcomeTextMove.TargetPoint.Y = 0;
	}
	ChangePoint(&WelcomeTextMove);
	OLED_ShowImageArea(WelcomeTextMove.CurrentPoint.X,WelcomeTextMove.CurrentPoint.Y,16,64,0,0,128,128,OLED_UI_LOGOGithub);

}

//秒表的辅助显示函数
void StopWatchAuxFunc(void){

	DrawStopWatch();
	DrawStopwatchButtons();
	
}

//秒表的回调函数
void StopWatch_Start(){
	StopWatch_Flag = true ;

}
void StopWatch_Paused(){
	StopWatch_Flag = false ;
}
void StopWatch_Del(){
	StopWatch_Flag = false ;
	StopWatchToltalSec = 0;
}
//手电的回调函数
void LED_ON(){
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}
void LED_OFF(){
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
}
//主菜单的辅助显示函数
void MainAuxFunc(void){
	//不显示
	LogoMove.TargetPoint.X = -200;
	LogoMove.TargetPoint.Y = 0;
	LogoMove.CurrentPoint.X = -200;
	LogoMove.CurrentPoint.Y = 0;

	LogoTextMove.TargetPoint.X = 129;
	LogoTextMove.TargetPoint.Y = 0;
	LogoTextMove.CurrentPoint.X = 129;
	LogoTextMove.CurrentPoint.Y = 0;
	
	WelcomeTextMove.TargetPoint.X = 128;
	WelcomeTextMove.TargetPoint.Y = 0;
	WelcomeTextMove.CurrentPoint.X = 128;
	WelcomeTextMove.CurrentPoint.Y = 0;
}
//首页时钟页面的菜单项
MenuItem ColckMenuItems[] = {
	//全部设置为NULL，只保留父子级

	{.General_item_text = NULL ,.General_callback = NULL,.General_SubMenuPage = &MainMenuPage,.Tiles_Icon = NULL,.List_BoolRadioBox = NULL},
};

//秒表的菜单项
MenuItem StopWatchMenuItems[] = {
	//全部设置为NULL，只保留父子级
		{.General_item_text = "开始",.General_callback = StopWatch_Start},  // ID 0
    {.General_item_text = "暂停",.General_callback = StopWatch_Paused},  // ID 1  
    {.General_item_text = "清零",.General_callback = StopWatch_Del},  // ID 2
    {.General_item_text = "返回",.General_callback = OLED_UI_Back},  // ID 3
    {.General_item_text = NULL}

};
//手电筒菜单项
MenuItem LEDMenuItems[] = {
	//全部设置为NULL，只保留父子级
		{.General_item_text = "ON",.General_callback = LED_ON},  // ID 0
    {.General_item_text = "OFF",.General_callback = LED_OFF},  // ID 1  
    {.General_item_text = "返回",.General_callback = OLED_UI_Back},  // ID 3
    {.General_item_text = NULL}

};
//手电筒菜单项
MenuItem MPUMenuItems[] = {
	//全部设置为NULL，只保留父子级
    {.General_item_text = "返回",.General_callback = OLED_UI_Back},  
    {.General_item_text = NULL}

};
//游戏菜单项
MenuItem GameMenuItems[] = {
	{.General_item_text = "谷歌小恐龙",.General_callback = NULL,.General_SubMenuPage = &DinoGameMenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "返回",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = NULL},/*最后一项的General_item_text置为NULL，表示该项为分割线*/
};

//主菜单的菜单项
MenuItem MainMenuItems[] = {

	{.General_item_text = "Settings",.General_callback = NULL,.General_SubMenuPage = &SettingsMenuPage,.Tiles_Icon = Image_setings},
	{.General_item_text = "WeChat",.General_callback = NULL,.General_SubMenuPage = NULL,.Tiles_Icon = Image_wechat},
	{.General_item_text = "Alipay",.General_callback = NULL,.General_SubMenuPage = NULL,.Tiles_Icon = Image_alipay},
	{.General_item_text = "计算器",.General_callback = NULL,.General_SubMenuPage = NULL,.Tiles_Icon = Image_calc},
	{.General_item_text = "秒表",.General_callback = NULL,.General_SubMenuPage = &StopWatchMenuPage,.Tiles_Icon = Image_stopwatch},
	{.General_item_text = "游戏",.General_callback = NULL,.General_SubMenuPage = &GameMenuPage,.Tiles_Icon = Image_game},
	{.General_item_text = "手电",.General_callback = NULL,.General_SubMenuPage = &LEDMenuPage,.Tiles_Icon = Image_led},
	{.General_item_text = "MPU",.General_callback = NULL,.General_SubMenuPage = &MPUMenuPage,.Tiles_Icon = Image_mpu},
	{.General_item_text = "Return",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.Tiles_Icon = Image_return},
	{.General_item_text = NULL},/*最后一项的General_item_text置为NULL，表示该项为分割线*/
//void BackEventMenuItem(void)
};
//设置菜单项内容数组
MenuItem SettingsMenuItems[] = {
	{.General_item_text = "亮度",.General_callback = BrightnessWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "黑暗模式",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = &ColorMode},
	{.General_item_text = "显示帧率",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = &OLED_UI_ShowFps},
	{.General_item_text = "此设备",.General_callback = NULL,.General_SubMenuPage = &AboutThisDeviceMenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "设置时间",.General_callback = NULL,.General_SubMenuPage = &SettingTimeMenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "[返回]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = NULL},/*最后一项的General_item_text置为NULL，表示该项为分割线*/
};

MenuItem SettingTimeItems[] = {
	{.General_item_text = "设置年",.General_callback = YearWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "设置月",.General_callback = MonWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "设置日",.General_callback = DayWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "设置时",.General_callback = HourWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "设置分",.General_callback = MinWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "设置秒",.General_callback = SecWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "[返回]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = NULL},/*最后一项的General_item_text置为NULL，表示该项为分割线*/
};

MenuItem AboutThisDeviceMenuItems[] = {
	{.General_item_text = "-[MCU:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " STM32F103",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " RAM:20KB",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " ROM:64KB",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "-[Screen:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " SSD1306 128x64 OLED",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "-[CP:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " SoftWare SPI",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "[返回]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},

	{.General_item_text = NULL},/*最后一项的General_item_text置为NULL，表示该项为分割线*/
};


MenuPage ColckMenuPage = {
	//通用属性，必填
	.General_MenuType = MENU_TYPE_LIST,  		 //菜单类型为列表类型
	.General_CursorStyle = NOT_SHOW,	 //光标类型为线型
	.General_FontSize = OLED_UI_FONT_12,			//字高
	.General_ParentMenuPage = NULL,		 //由于这是根菜单，所以父菜单为NULL
	.General_LineSpace = 0,						//行间距 单位：像素
	.General_MoveStyle = UNLINEAR,				//移动方式为非线性曲线动画
	.General_MovingSpeed = SPEED,					//动画移动速度(此值根据实际效果调整)
	.General_ShowAuxiliaryFunction = ClockAuxFunc,		 //显示辅助函数
	.General_MenuItems = ColckMenuItems,		 //菜单项内容数组

	//特殊属性，根据.General_MenuType的类型选择
	.List_MenuArea = {0, 0, 0, 0},			 //列表显示区域
	.List_IfDrawFrame = false,					 //是否显示边框
	.List_IfDrawLinePerfix = false,				 //是否显示行前缀
	.List_StartPointX = 0,                        //列表起始点X坐标
	.List_StartPointY = 0,                        //列表起始点Y坐标
};

MenuPage MainMenuPage = {
	//通用属性，必填
	.General_MenuType = MENU_TYPE_TILES,  		 //菜单类型为磁贴类型
	.General_CursorStyle = NOT_SHOW,			 //光标类型
	.General_FontSize = OLED_UI_FONT_16,			//字高
	.General_ParentMenuPage = &ColckMenuPage,		//父菜单为时间页面		
	.General_LineSpace = 5,						//磁贴间距 单位：像素（对于磁贴类型菜单，此值表示每个磁贴之间的间距，对于列表类型菜单，此值表示行间距）
	.General_MoveStyle = UNLINEAR,				//移动方式
	.General_MovingSpeed = SPEED,					//动画移动速度(此值根据实际效果调整)
	.General_ShowAuxiliaryFunction = MainAuxFunc,		 //显示辅助函数
	.General_MenuItems = MainMenuItems,			//菜单项内容数组

	//特殊属性，根据.General_MenuType的类型选择
	.Tiles_ScreenHeight = 64,					//屏幕高度
	.Tiles_ScreenWidth = 128,						//屏幕宽度
	.Tiles_TileWidth = 32,						 //磁贴宽度
	.Tiles_TileHeight = 32,						 //磁贴高度
};
MenuPage StopWatchMenuPage = {
	//通用属性，必填
	.General_MenuType = MENU_TYPE_LIST,  		 //菜单类型为列表类型
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //光标类型为线型
	.General_FontSize = OLED_UI_FONT_12,			//字高
	.General_ParentMenuPage = &MainMenuPage,		 //父级菜单给到主菜单
	.General_LineSpace = 0,						//行间距 单位：像素
	.General_MoveStyle = UNLINEAR,				//移动方式为非线性曲线动画
	.General_MovingSpeed = 15,					//动画移动速度(此值根据实际效果调整)
	.General_ShowAuxiliaryFunction = StopWatchAuxFunc,		 //显示辅助函数
	.General_MenuItems = StopWatchMenuItems,		 //菜单项内容数组
	//特殊属性，根据.General_MenuType的类型选择
	.List_MenuArea = {0, 0, 0, 0},			 //列表显示区域
	.List_IfDrawFrame = false,					 //是否显示边框
	.List_IfDrawLinePerfix = false,				 //是否显示行前缀
	.List_StartPointX = 0,                        //列表起始点X坐标
	.List_StartPointY = 0,                        //列表起始点Y坐标
};
MenuPage LEDMenuPage = {
	//通用属性，必填
	.General_MenuType = MENU_TYPE_LIST,  		 //菜单类型为列表类型
	.General_CursorStyle = HOLLOW_ROUNDRECTANGLE,	 //光标类型为线型
	.General_FontSize = OLED_UI_FONT_12,			//字高
	.General_ParentMenuPage = &MainMenuPage,		 //父级菜单给到主菜单
	.General_LineSpace = 0,						//行间距 单位：像素
	.General_MoveStyle = PID_CURVE,				//移动方式为非线性曲线动画
	.General_MovingSpeed = SPEED,					//动画移动速度(此值根据实际效果调整)
	.General_ShowAuxiliaryFunction = NULL,		 //显示辅助函数
	.General_MenuItems = LEDMenuItems,		 //菜单项内容数组
	//特殊属性，根据.General_MenuType的类型选择
	.List_MenuArea = {2, 2, 128, 64},			 //列表显示区域
	.List_IfDrawFrame = false,					 //是否显示边框
	.List_IfDrawLinePerfix = false,				 //是否显示行前缀
	.List_StartPointX = 0,                        //列表起始点X坐标
	.List_StartPointY = 0,                        //列表起始点Y坐标
};

MenuPage MPUMenuPage = {
	//通用属性，必填
	.General_MenuType = MENU_TYPE_LIST,  		 //菜单类型为列表类型
	.General_CursorStyle = NOT_SHOW,	 //光标类型为线型
	.General_FontSize = OLED_UI_FONT_16,			//字高
	.General_ParentMenuPage = &MainMenuPage,		 //父级菜单给到主菜单
	.General_LineSpace = 0,						//行间距 单位：像素
	.General_MoveStyle = PID_CURVE,				//移动方式为非线性曲线动画
	.General_MovingSpeed = SPEED,					//动画移动速度(此值根据实际效果调整)
	.General_ShowAuxiliaryFunction = MPUAuxFunc,		 //显示辅助函数
	.General_MenuItems = MPUMenuItems,		 //菜单项内容数组
	//特殊属性，根据.General_MenuType的类型选择
	.List_MenuArea = {0, 0, 0, 0},			 //列表显示区域
	.List_IfDrawFrame = false,					 //是否显示边框
	.List_IfDrawLinePerfix = false,				 //是否显示行前缀
	.List_StartPointX = 0,                        //列表起始点X坐标
	.List_StartPointY = 0,                        //列表起始点Y坐标
};

MenuPage SettingsMenuPage = {
	//通用属性，必填
	.General_MenuType = MENU_TYPE_LIST,  		 //菜单类型为列表类型
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //光标类型为线型
	.General_FontSize = OLED_UI_FONT_12,			//字高
	.General_ParentMenuPage = &MainMenuPage,		 //父菜单为主菜单
	.General_LineSpace = 4,						//行间距 单位：像素
	.General_MoveStyle = UNLINEAR,				//移动方式为非线性曲线动画
	.General_MovingSpeed = SPEED,					//动画移动速度(此值根据实际效果调整)
	.General_ShowAuxiliaryFunction = SettingAuxFunc,		 //显示辅助函数
	.General_MenuItems = SettingsMenuItems,		 //菜单项内容数组

	//特殊属性，根据.General_MenuType的类型选择
	.List_MenuArea = {32, 0, 95, 64},			 //列表显示区域
	.List_IfDrawFrame = false,					 //是否显示边框
	.List_IfDrawLinePerfix = true,				 //是否显示行前缀
	.List_StartPointX = 4,                        //列表起始点X坐标
	.List_StartPointY = 2,                        //列表起始点Y坐标
};
MenuPage SettingTimeMenuPage = {
	//通用属性，必填
	.General_MenuType = MENU_TYPE_LIST,  		 //菜单类型为列表类型
	.General_CursorStyle = REVERSE_BLOCK,	 //光标类型为圆角矩形
	.General_FontSize = OLED_UI_FONT_12,			//字高
	.General_ParentMenuPage = &SettingsMenuPage,		 //父菜单为主菜单
	.General_LineSpace = 4,						//行间距 单位：像素
	.General_MoveStyle = UNLINEAR,				//移动方式为非线性曲线动画
	.General_MovingSpeed = SPEED,					//动画移动速度(此值根据实际效果调整)
	.General_ShowAuxiliaryFunction = SettingTimeAuxFunc,		 //显示辅助函数
	.General_MenuItems = SettingTimeItems,		 //菜单项内容数组

	//特殊属性，根据.General_MenuType的类型选择
	.List_MenuArea = {0, 0, 100, 64},			 //列表显示区域
	.List_IfDrawFrame = false,					 //是否显示边框
	.List_IfDrawLinePerfix = true,				 //是否显示行前缀
	.List_StartPointX = 4,                        //列表起始点X坐标
	.List_StartPointY = 2,                        //列表起始点Y坐标

};
MenuPage AboutThisDeviceMenuPage = {
	//通用属性，必填
	.General_MenuType = MENU_TYPE_LIST,  		 //菜单类型为列表类型
	.General_CursorStyle = REVERSE_BLOCK,	 //光标类型为圆角矩形
	.General_FontSize = OLED_UI_FONT_12,			//字高
	.General_ParentMenuPage = &SettingsMenuPage,		 //父菜单为主菜单
	.General_LineSpace = 4,						//行间距 单位：像素
	.General_MoveStyle = UNLINEAR,				//移动方式为非线性曲线动画
	.General_MovingSpeed = SPEED,					//动画移动速度(此值根据实际效果调整)
	.General_ShowAuxiliaryFunction = AboutThisDeviceAuxFunc,		 //显示辅助函数
	.General_MenuItems = AboutThisDeviceMenuItems,		 //菜单项内容数组

	//特殊属性，根据.General_MenuType的类型选择
	.List_MenuArea = {0, 0, 100, 64},			 //列表显示区域
	.List_IfDrawFrame = false,					 //是否显示边框
	.List_IfDrawLinePerfix = false,				 //是否显示行前缀
	.List_StartPointX = 4,                        //列表起始点X坐标
	.List_StartPointY = 2,                        //列表起始点Y坐标

};

/*----------------------------------MPU6050-------------------------------------*/

int16_t ax,ay,az,gx,gy,gz;//MPU6050测得的三轴加速度和角速度
float roll_g,pitch_g,yaw_g;//陀螺仪解算的欧拉角
float roll_a,pitch_a;//加速度计解算的欧拉角
float Roll,Pitch,Yaw;//互补滤波后的欧拉角
float a=0.9;//互补滤波器系数
float Delta_t=0.005;//采样周期
double pi=3.1415927;

//通过MPU6050的数据进行姿态解算的函数
void MPU6050_Calculation(void)
{
	Delay_ms(5);
	MPU6050_GetData(&ax,&ay,&az,&gx,&gy,&gz);
	
	//通过陀螺仪解算欧拉角
	roll_g=Roll+(float)gx*Delta_t;
	pitch_g=Pitch+(float)gy*Delta_t;
	yaw_g=Yaw+(float)gz*Delta_t;
	
	//通过加速度计解算欧拉角
	pitch_a=atan2((-1)*ax,az)*180/pi;
	roll_a=atan2(ay,az)*180/pi;
	
	//通过互补滤波器进行数据融合
	Roll=a*roll_g+(1-a)*roll_a;
	Pitch=a*pitch_g+(1-a)*pitch_a;
	Yaw=a*yaw_g;
	
}
//MPU的辅助显示函数
void MPUAuxFunc(void){
	MPU6050_Calculation();
	
	OLED_Printf(0 ,0,OLED_8X16_HALF, "Roll:");
	OLED_Printf(0 ,16,OLED_8X16_HALF, "Pitch:");
	OLED_Printf(0 ,32,OLED_8X16_HALF, "Yaw:");
	
	OLED_Printf(40 ,0,OLED_8X16_HALF, "%.2f",Roll);
	OLED_Printf(48 ,16,OLED_8X16_HALF, "%.2f",Pitch);
	OLED_Printf(32 ,32,OLED_8X16_HALF, "%.2f",Yaw);
	
}
/*----------------------------------游戏-------------------------------------*/
MenuPage GameMenuPage = {
	//通用属性，必填
	.General_MenuType = MENU_TYPE_LIST,  		 //菜单类型为列表类型
	.General_CursorStyle = REVERSE_BLOCK,	 //光标类型为圆角矩形
	.General_FontSize = OLED_UI_FONT_12,			//字高
	.General_ParentMenuPage = &MainMenuPage,		 //父菜单为主菜单
	.General_LineSpace = 4,						//行间距 单位：像素
	.General_MoveStyle = UNLINEAR,				//移动方式为非线性曲线动画
	.General_MovingSpeed = SPEED,					//动画移动速度(此值根据实际效果调整)
	.General_ShowAuxiliaryFunction = NULL,		 //显示辅助函数
	.General_MenuItems = GameMenuItems,		 //菜单项内容数组

	//特殊属性，根据.General_MenuType的类型选择
	.List_MenuArea = {0, 0, 128, 64},			 //列表显示区域
	.List_IfDrawFrame = false,					 //是否显示边框
	.List_IfDrawLinePerfix = false,				 //是否显示行前缀
	.List_StartPointX = 4,                        //列表起始点X坐标
	.List_StartPointY = 2,                        //列表起始点Y坐标

};
MenuItem DinoGameMenuItems[] = {
	{.General_item_text = "返回",.General_callback = DinoGameOver},
	{.General_item_text = NULL},
};
MenuPage DinoGameMenuPage = {
	//通用属性，必填
	.General_MenuType = MENU_TYPE_LIST,  		 //菜单类型为列表类型
	.General_CursorStyle = NOT_SHOW,	 //光标类型为线型
	.General_FontSize = OLED_UI_FONT_16,			//字高
	.General_ParentMenuPage = &GameMenuPage,		 //父级菜单给到游戏菜单
	.General_LineSpace = 0,						//行间距 单位：像素
	.General_MoveStyle = PID_CURVE,				//移动方式为非线性曲线动画
	.General_MovingSpeed = SPEED,					//动画移动速度(此值根据实际效果调整)
	.General_ShowAuxiliaryFunction = DinoGameAuxFunc,		 //显示辅助函数
	.General_MenuItems = DinoGameMenuItems,		 //菜单项内容数组
	//特殊属性，根据.General_MenuType的类型选择
	.List_MenuArea = {0, 0, 0, 0},			 //列表显示区域
	.List_IfDrawFrame = false,					 //是否显示边框
	.List_IfDrawLinePerfix = false,				 //是否显示行前缀
	.List_StartPointX = 0,                        //列表起始点X坐标
	.List_StartPointY = 0,                        //列表起始点Y坐标
};

void DinoGameAuxFunc(){ 
	Dino_Flag = true ;
	DinoGameLoop();

}
/*----------------------------------ADC-------------------------------------*/
uint16_t ADValue;
float VBAT;
int Battery_Capacity;
void Show_Battery(void)
{
	int sum;
	for(int i=0;i<3000;i++)
	{
		ADValue=AD_GetValue();
		sum+=ADValue;
		
	}
	ADValue=sum/3000;
	VBAT=(float)ADValue/4095*3.3;
	Battery_Capacity=(ADValue-3276)*100/819;
	if(Battery_Capacity<0)Battery_Capacity=0;
	
	OLED_ShowNum(85,4,Battery_Capacity,3,OLED_6X8_HALF);
	OLED_ShowChar(103,4,'%',OLED_6X8_HALF);
	
	if(Battery_Capacity==100)OLED_ShowImage(110,0,16,16,Battery);
	else if(Battery_Capacity>=10&&Battery_Capacity<100)
	{
		OLED_ShowImage(110,0,16,16,Battery);
		OLED_ClearArea((112+Battery_Capacity/10),5,(10-Battery_Capacity/10),6);
		OLED_ClearArea(85,4,6,8);
	}
	
	else
	{
		OLED_ShowImage(110,0,16,16,Battery);
		OLED_ClearArea(112,5,10,6);
		OLED_ClearArea(85,4,12,8);
	}
};


