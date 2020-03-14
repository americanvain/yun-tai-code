#include "sys.h"
#include "adc.h"
#include "oled.h"
#include "OLED_task.h"
#include "stdlib.h"
#include "string.h"


uint8_t OLED_Button(void);

//-----------------------Button_Data------------------------//
static uint8_t Button_Val=0;
static uint8_t Button_Val_last=0;
static bool	Button_Change=0;
static portTickType KeyTime=0;
//-----------------------Menu_Data------------------------//
static Menu_option root_menu={"root",NULL,NULL,NULL};
static Menu_option *current_menu=NULL;

static int8_t cursor_pos=0;
static uint8_t cursor_pos_max=0;
//-----------------------APP_Data------------------------//
static uint8_t APP_ID=0;
static bool goto_submenu=false;
static bool goto_parentmenu=false;
static bool option_operat=false;
/**
 * @brief OLED���̺���
 * 
 */
void OLED_Prc(void){
	
	OLED_Button();
	
	switch(APP_ID)
	{
		case 0:{
			menu_switch();
			menu_display();
			cursor_display();
		}break;
		case 1:{
			oled_LOGO();
			if(Button_Change&&Button_Val==5)APP_ID=0;
		}break;
		case 2:{
			show_time();
			if(Button_Change&&Button_Val==5)APP_ID=0;
		}break;
	}
	
	
	oled_refresh_gram();
	oled_clear(Pen_Clear);
}
/**
 * @brief ����������
 * 
 * @return uint8_t ������ֵ
 */
uint8_t OLED_Button(void)
{
	static uint8_t _Button_Val=0;
	static uint8_t _Button_Val_last=0;
	static bool	_Button_Change=0;
	static portTickType currentTime;
	static portTickType KeyChangeTime;
	uint16_t ad1;
	
	_Button_Change=0;
	_Button_Val_last=_Button_Val;
	Button_Change=0;
	Button_Val_last=Button_Val;
	
	ad1 = get_ADC(ADC_Channel_6);
	if(ad1>3200)//�ް�������
	{
//		printf("NONE\n");������������
		_Button_Val = 0;
		KeyTime=0;
	}
	else if(ad1>800&&ad1<1000)//�󰴼�����
	{
//		printf("LEFT\n");
		_Button_Val = 1;
	}
	else if(ad1>1600&&ad1<1800)//�Ұ�������
	{
//		printf("RIGHT\n");
		_Button_Val = 2;
	}
	else if(ad1>2800&&ad1<3100)//�°�������
	{
//		printf("DOWN\n");
		_Button_Val = 3;
	}
	else if(ad1>2200&&ad1<2400)//�ϰ�������
	{
//		printf("UP\n");
		_Button_Val = 4;
	}
	else if(ad1<10)//��������
	{
//		printf("HIGH\n");
		_Button_Val = 5;
	}
	
	if(_Button_Val!=_Button_Val_last)
	{
		KeyChangeTime=xTaskGetTickCount();
		_Button_Change=1;
	}
	KeyTime=xTaskGetTickCount()-KeyChangeTime;
	if(KeyTime>50)Button_Val=_Button_Val;
	
	if(Button_Val!=Button_Val_last)
	{
		KeyChangeTime=xTaskGetTickCount();
		Button_Change=1;
	}
	
	return Button_Val;
}

//-----------------------Menu_Functions------------------------//
/**
 * @brief ���ڴ����˵������в˵��Ӹ��˵���ʼ��������һ���˵�Ϊ���˵����Ӳ˵���
 * 
 */
void menu_init(void){
	current_menu=&root_menu;
	
	Menu_option *_IDEA=submenu_creat("IDEA",&root_menu,NULL);
	Menu_option *_time=submenu_creat("time",_IDEA,&show_time_hook);
	Menu_option *_print=menu_option_creat("print",_IDEA,NULL);
	
	Menu_option *_logo=menu_option_creat("logo",_time,&show_logo_hook);
}
/**
 * @brief ���ڴ����µĲ˵�ѡ�������ѡ���ԭѡ��ĺ���
 * 
 * @param title ѡ�����
 * @param previous_option ��һ��ѡ�� 
 * @param hook ���Ӻ�������������ʱִ��
 * @return Menu_option* �˵�ѡ���ָ��
 */
Menu_option *menu_option_creat(char title[TITLE_LEN],Menu_option *previous_option,void (*hook)(void* param))
{
	Menu_option *p=NULL;
	p=(Menu_option*)malloc(sizeof(Menu_option));
	
	memcpy(&p->title,title,sizeof(char)*TITLE_LEN);
	p->hook=hook;
	
	p->parentmenu=previous_option->parentmenu;
	
	p->submenu=NULL;
	
	p->nextoption=previous_option->nextoption;
	previous_option->nextoption=p;
	return p;
}
/**
 * @brief ���ڴ����µ��Ӳ˵�
 * 
 * @param title �Ӳ˵�ѡ��ı���
 * @param parentmenu �����˵�ѡ��
 * @param hook ���Ӻ�������������ʱִ��
 * @return Menu_option* �˵�ѡ���ָ��
 */
Menu_option *submenu_creat(char title[TITLE_LEN],Menu_option *parentmenu,void (*hook)(void* param))
{
	Menu_option *p=NULL;
	p=(Menu_option*)malloc(sizeof(Menu_option));
	
	memcpy(&p->title,title,sizeof(char)*TITLE_LEN);
	p->hook=hook;
	
	p->submenu=NULL;
	
	p->nextoption=NULL;
	
	p->parentmenu=parentmenu;

	parentmenu->submenu=p;
	return p;
}
/**
 * @brief �˵���������
 * 
 */
void menu_switch(void)
{
	if(Button_Change){
		switch (Button_Val)
		{
			case 0:;break; 
			case 1:goto_parentmenu=true;break; 									////////                    
			case 2:goto_submenu=true;break; 									////////
			case 3:cursor_pos++;break;		//��
			case 4:cursor_pos--;break;		//��
			case 5:option_operat=true;
			default:break;
		}
		VAL_LIMIT(cursor_pos,0,cursor_pos_max);
	}
}
/**
 * @brief �˵�����ʾ
 * 
 */
void menu_display(void)
{
	int n=0;
	Menu_option *p=NULL;
	p=current_menu->submenu; 									////////
	for(n=0;p!=NULL;n++){
		if(cursor_pos==n){
			if(goto_submenu==true&&p->submenu!=NULL){
				current_menu=p;
				goto_submenu=false;
			}
			if(goto_parentmenu==true&&p->parentmenu->parentmenu!=NULL){
				current_menu=p->parentmenu->parentmenu;
				goto_parentmenu=false;
			}
			if(option_operat==true&&p->hook!=NULL){
				p->hook(NULL);
				option_operat=false;
			}
		}
		if(p->submenu!=NULL)oled_showstring(n,19,(uint8_t*)"->");
		oled_showstring(n,2,(uint8_t*)p->title);
		p=p->nextoption;
	}
	cursor_pos_max=n-1;
	goto_submenu=false;
	goto_parentmenu=false;
	option_operat=false;
}
/**
 * @brief ָ�����ʾ
 * 
 */
void cursor_display(void)
{
	oled_showchar(cursor_pos,0,'>');
}

//-----------------------Hook_Fuctions,User_Fuctions------------------------//
void show_logo_hook(void* param)
{
	APP_ID=1;
}

void show_time_hook(void* param)
{
	APP_ID=2;
}
void show_time(void)
{
	uint8_t x=2;
	uint8_t y=3;
	portTickType time=xTaskGetTickCount();
	uint8_t second=(uint8_t)(time/1000);
	uint8_t minute=(uint8_t)(second/60);
	uint16_t hours=(uint8_t)(minute/60);
	oled_shownum(x,y+10,time%1000,1,3);
	oled_shownum(x,y+7,second%60,1,2);
	oled_shownum(x,y+4,minute%60,1,2);
	oled_shownum(x,y,hours%60,0,3);
	oled_showchar(x,y+3,':');
	oled_showchar(x,y+6,':');
	oled_showchar(x,y+9,'.');
}
void show_motor_state_hook(void* param)
{
	APP_ID=3;
}