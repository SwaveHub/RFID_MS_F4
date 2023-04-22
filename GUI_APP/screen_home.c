/**
  ******************************************************************************
  * @file    screen_home.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2023-03-23
  * @brief   lvgl screen_home create
  *	    修改记录 :
  *		版本号  日期              作者             说明
  *         V1.0    2023-03-23  SwaveYu     正式发布
  ******************************************************************************
  * @attention
  *
  *
  *
  *
  *
  ******************************************************************************
  */

/******************************** 头文件 *************************************/
#include "demo.h"
#include "screen_home.h"
#include "screen_instorage.h"
#include "screen_outstorage.h"
#include "./flash/bsp_internal_flash.h"

/****************************** 私有宏定义 ***********************************/

/******************************* 全局变量 ************************************/

/***************************** 私有全局变量 **********************************/
static lv_style_t my_style_btn_release;//按钮释放状态下的样式
static lv_style_t my_style_btn_press;//按钮按下时的样式
    
static lv_obj_t *label_home;
static lv_obj_t *label_time;
static lv_obj_t *btn_home, *btn_home_label;
static lv_obj_t *btn_upload, *btn_upload_label;
static lv_obj_t *btn_reset, *btn_reset_label;

//图片申明
LV_IMG_DECLARE(btn_img_instorage);
LV_IMG_DECLARE(btn_img_outstorage);

static lv_style_t pr_style;

static lv_obj_t *mbox_upload, *mbox_reset;
/******************************* 私有函数 ************************************/
static lv_obj_t * mbox_create(lv_obj_t * parent, char* msg, void (*mbox_event_cb)(lv_obj_t * , lv_event_t));//函数申明
static void mbox_upload_event_cb(lv_obj_t * obj, lv_event_t event);
static void mbox_reset_event_cb(lv_obj_t * obj, lv_event_t event);

//事件回调函数
static void btn_upload_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
         //创建一个消息对话框
        mbox_upload = mbox_create(lv_scr_act(), "Confirm Upload", mbox_upload_event_cb);          
	}        
	default:
		break;
	}
}

static void btn_reset_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
         //创建一个消息对话框
        mbox_reset = mbox_create(lv_scr_act(), "Confirm Reset", mbox_reset_event_cb);          
	}        
	default:
		break;
	}
}

static void btn_home_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
//        lv_obj_clean(lv_scr_act());
//        screen_home_create();
	}
		break;
	default:
		break;
	}
}

static void imgbtn_to_instorage_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_RELEASED:
	{
		lv_obj_clean(lv_scr_act());
        screen_instorage_create();
	}
		break;
	default:
		break;
	}
}

static void imgbtn_to_outstorage_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_RELEASED:
	{
		lv_obj_clean(lv_scr_act());
        screen_outstorage_create();
	}
		break;
	default:
		break;
	}
}

static void mbox_upload_event_cb(lv_obj_t * obj, lv_event_t event)
{
    uint16_t btn_id;
    if (obj == mbox_upload) {
        if(event == LV_EVENT_VALUE_CHANGED) {
            //获取按钮 id
            btn_id = lv_mbox_get_active_btn(obj);
            if (btn_id == 0) { //OK 按钮
                printf("upload:\r\n");
                InternalFlash_ReadRecord(ITEM_INFO_SIZE / 4);
            }
            lv_mbox_start_auto_close(obj,0);
        }
    }   
}

static void mbox_reset_event_cb(lv_obj_t * obj, lv_event_t event)
{
    uint16_t btn_id;
    if (obj == mbox_reset) {
        if(event == LV_EVENT_VALUE_CHANGED) {
            //获取按钮 id
            btn_id = lv_mbox_get_active_btn(obj);
            if (btn_id == 0) { //OK 按钮
                InternalFlash_ResetRecord(FLASH_USER_START_ADDR, FLASH_USER_END_ADDR);
                printf("reset\r\n");
            }
            lv_mbox_start_auto_close(obj,0);
        }
    }   
}

//例程入口
void lv_screen_home_start()
{
    lv_obj_t* scr = lv_scr_act();//获取当前活跃的屏幕对象
    
    //创建 label_home 标签,用来显示 label_time 标签的长文本模式
    label_home = lv_label_create(scr,NULL);//创建标签
    lv_label_set_long_mode(label_home,LV_LABEL_LONG_BREAK);//设置长文本模式
    //设置宽度,一定得放在 lv_label_set_long_mode 的后面,否则不起作用的
    lv_obj_set_width(label_home,150);
    lv_label_set_align(label_home,LV_LABEL_ALIGN_CENTER);//文本居中对齐
    lv_label_set_text(label_home,"Home");//设置文本    
    lv_obj_set_pos(label_home, 165, 10);
    
    //创建 label_time 标签
    static lv_style_t label_text_style; 
    lv_style_copy(&label_text_style,&lv_style_plain);
    label_text_style.text.font = &lv_font_roboto_16;
    label_time = lv_label_create(scr,NULL);//创建标签
    lv_label_set_long_mode(label_time,LV_LABEL_LONG_BREAK);//设置长文本模式    
    lv_obj_set_width(label_time,100);
    lv_label_set_align(label_time,LV_LABEL_ALIGN_CENTER);//文本居中对齐    
    lv_label_set_style(label_time, LV_LABEL_STYLE_MAIN, &label_text_style);
    uint8_t buf[30];
    RTC_Get(&system_time.year, &system_time.month, &system_time.day, &system_time.hour, &system_time.minute, &system_time.second);// 获取最新RTC时间
    sprintf(buf, "%u:%02u\n%u/%u/%u", system_time.hour, system_time.minute, 
                                                        system_time.year, system_time.month, system_time.day);
    lv_label_set_text(label_time, buf);//设置文本
    lv_obj_set_pos(label_time, 375, 5);

    //1.先创建 2 种状态下按钮样式
    //1.1 释放状态下的样式
    lv_style_copy(&my_style_btn_release,&lv_style_plain_color);
    //设置纯色的背景
    my_style_btn_release.body.main_color = LV_COLOR_MAKE(0x1E,0x9F,0xFF);
    my_style_btn_release.body.grad_color = my_style_btn_release.body.main_color;
    my_style_btn_release.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    my_style_btn_release.body.radius = LV_RADIUS_CIRCLE;//绘制圆角按钮
    my_style_btn_release.body.shadow.color = LV_COLOR_MAKE(0x1E,0x9F,0xFF);
    my_style_btn_release.body.shadow.type = LV_SHADOW_FULL;//设置四边都有阴影
    my_style_btn_release.body.shadow.width = 3;//设置阴影的宽度
    my_style_btn_release.text.color = LV_COLOR_WHITE;
    my_style_btn_release.body.padding.left = 10;//设置左内边距
    my_style_btn_release.body.padding.right = 10;//设置右内边距
    //1.2 按下状态下的样式
    lv_style_copy(&my_style_btn_press,&lv_style_plain_color);
    my_style_btn_press.body.opa = LV_OPA_0;//设置背景色透明
    my_style_btn_press.body.radius = LV_RADIUS_CIRCLE;//绘制圆角按钮
    //设置边框的颜色
    my_style_btn_press.body.border.color = LV_COLOR_MAKE(0xC9,0xC9,0xC9);
    my_style_btn_press.body.border.part = LV_BORDER_FULL;//四条边框都绘制
    my_style_btn_press.body.border.width = 2;//设置边框的宽度
    my_style_btn_press.body.border.opa = LV_OPA_COVER;//设置边框完全不透明
    my_style_btn_press.text.color = LV_COLOR_BLACK;
    my_style_btn_press.body.padding.left = 10;//设置左内边距
    my_style_btn_press.body.padding.right = 10;//设置右内边距

    //创建一个home按钮   
    btn_home = lv_btn_create(scr, NULL);
    //设置按钮正常态下释放状态样式
    lv_btn_set_style(btn_home,LV_BTN_STYLE_REL,&my_style_btn_release);
    //设置按钮正常态下按下状态样式
    lv_btn_set_style(btn_home,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_home, 100, 50);//设置大小
    lv_obj_set_pos(btn_home,190,750);//设置坐标 
    btn_home_label = lv_label_create(btn_home,NULL);//给 btn_home 添加 label 子对象
    lv_label_set_text(btn_home_label,"Home");
    lv_obj_set_event_cb(btn_home,btn_home_event_cb);//设置事件回调函数
    
    //创建一个upload按钮   
    btn_upload = lv_btn_create(scr, NULL);
    //设置按钮正常态下释放状态样式
    lv_btn_set_style(btn_upload,LV_BTN_STYLE_REL,&my_style_btn_release);
    //设置按钮正常态下按下状态样式
    lv_btn_set_style(btn_upload,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_upload, 100, 35);//设置大小
    lv_obj_set_pos(btn_upload,5,5);//设置坐标 
    btn_upload_label = lv_label_create(btn_upload,NULL);//给 btn_upload 添加 label 子对象
    lv_label_set_text(btn_upload_label,"Upload");
    lv_obj_set_event_cb(btn_upload,btn_upload_event_cb);//设置事件回调函数

    //创建一个reset按钮   
    btn_reset = lv_btn_create(scr, NULL);
    //设置按钮正常态下释放状态样式
    static lv_style_t my_style_resetbtn_release;//按钮释放状态下的样式
    lv_style_copy(&my_style_resetbtn_release,&my_style_btn_release);
    my_style_resetbtn_release.body.main_color = LV_COLOR_MAKE(0xFF,0x00,0x00);
    my_style_resetbtn_release.body.grad_color = my_style_resetbtn_release.body.main_color;
    my_style_resetbtn_release.body.shadow.color = my_style_resetbtn_release.body.main_color;
    lv_btn_set_style(btn_reset,LV_BTN_STYLE_REL,&my_style_resetbtn_release);
    //设置按钮正常态下按下状态样式
    lv_btn_set_style(btn_reset,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_reset, 100, 50);//设置大小
    lv_obj_set_pos(btn_reset, 30, 750);//设置坐标 
    btn_reset_label = lv_label_create(btn_reset,NULL);//给 btn_reset 添加 label 子对象
    lv_label_set_text(btn_reset_label,"RESET");
    lv_obj_set_event_cb(btn_reset,btn_reset_event_cb);//设置事件回调函数
    
    //1.创建按下时的样式
    lv_style_copy(&pr_style,&lv_style_plain);
    pr_style.image.color = LV_COLOR_BLACK;//图片重绘色时的混合色为黑色,这样看上去有按下的效果
    pr_style.image.intense = LV_OPA_50;//混合强度
    pr_style.text.color = LV_COLOR_MAKE(0xAA,0xAA,0xAA);//按下时的文本色

    //2.创建imgbtn_to_instorage图片按钮对象
    lv_obj_t * imgbtn_to_instorage = lv_imgbtn_create(scr,NULL);
    lv_imgbtn_set_src(imgbtn_to_instorage,LV_BTN_STATE_REL,&btn_img_instorage);//设置正常态松手时的图片
    lv_imgbtn_set_src(imgbtn_to_instorage,LV_BTN_STATE_PR,&btn_img_instorage);//设置正常态按下时的图片
    lv_imgbtn_set_style(imgbtn_to_instorage,LV_BTN_STATE_PR,&pr_style);//设置正常态按下时的样式
    lv_obj_set_size(imgbtn_to_instorage, 120, 80);//设置大小
    lv_obj_set_pos(imgbtn_to_instorage,30,100);//设置坐标 
    lv_obj_set_event_cb(imgbtn_to_instorage,imgbtn_to_instorage_event_cb);//设置事件回调函数
    lv_obj_t * imgbtn_to_instorage_label = lv_label_create(scr,NULL);//给图片按钮添加标题
    lv_obj_align(imgbtn_to_instorage_label, imgbtn_to_instorage , LV_ALIGN_OUT_BOTTOM_MID, -25, 0);//文本居中对齐    
    lv_label_set_text(imgbtn_to_instorage_label, "InStorage");    

    //2.创建imgbtn_to_outstorage图片按钮对象
    lv_obj_t * imgbtn_to_outstorage = lv_imgbtn_create(scr,NULL);
    lv_imgbtn_set_src(imgbtn_to_outstorage,LV_BTN_STATE_REL,&btn_img_outstorage);//设置正常态松手时的图片
    lv_imgbtn_set_src(imgbtn_to_outstorage,LV_BTN_STATE_PR,&btn_img_outstorage);//设置正常态按下时的图片
    lv_imgbtn_set_style(imgbtn_to_outstorage,LV_BTN_STATE_PR,&pr_style);//设置正常态按下时的样式
    lv_obj_set_size(imgbtn_to_outstorage, 120, 80);//设置大小
    lv_obj_set_pos(imgbtn_to_outstorage,180,100);//设置坐标 
    lv_obj_set_event_cb(imgbtn_to_outstorage,imgbtn_to_outstorage_event_cb);//设置事件回调函数
    lv_obj_t * imgbtn_to_outstorage_label = lv_label_create(scr,NULL);//给图片按钮添加标题
    lv_obj_align(imgbtn_to_outstorage_label,  imgbtn_to_outstorage, LV_ALIGN_OUT_BOTTOM_MID, -25, 0);//文本居中对齐    
    lv_label_set_text(imgbtn_to_outstorage_label, "OutStorage");    
}

//创建自己封装之后的消息对话框
//parent:父对象,msg显示的消息
//返回值: 返回创建出来的消息对话框对象
lv_obj_t * mbox_create(lv_obj_t * parent, char* msg, void (*mbox_event_cb)(lv_obj_t * , lv_event_t))
{
    #define MBOX_WIDTH 200 //消息对话框的宽度
    #define MBOX_BTN_HEIGHT 30 //其内部每个按钮的高度

    static const char * btns[] ={"OK", "Cancel",""};

    lv_obj_t * mbox = lv_mbox_create(parent, NULL);
    lv_mbox_set_text(mbox, msg);
    lv_mbox_add_btns(mbox, btns);
    lv_obj_set_width(mbox, MBOX_WIDTH);
    lv_obj_set_event_cb(mbox, mbox_event_cb);
    lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the corner*/

    //设置按钮释放状态的样式
    lv_mbox_set_style(mbox,LV_MBOX_STYLE_BTN_REL,&my_style_btn_release);
    //设置按钮按下状态的样式
    lv_mbox_set_style(mbox,LV_MBOX_STYLE_BTN_PR,&my_style_btn_press);
    lv_obj_t * btnm_of_mbox = lv_mbox_get_btnm(mbox);//获取其内部的矩阵按钮对象
    //设置矩阵按钮的大小
    lv_obj_set_size(btnm_of_mbox,MBOX_WIDTH,MBOX_BTN_HEIGHT);
    return mbox;
}

/**
 * @brief   创建用户交互界面
 * @param  无
 * @retval   无
 */
void screen_home_create(void)
{
    printf("screen_home_start\r\n");
    lv_screen_home_start();
}

 /*********************************END OF FILE*********************************/
