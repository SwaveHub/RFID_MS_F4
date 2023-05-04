/**
  ******************************************************************************
  * @file    screen_instorage.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2023-03-23
  * @brief   lvgl screen_instorage create
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
#include "screen_instorage.h"
#include "screen_home.h"

/****************************** 私有宏定义 ***********************************/
//是否将下拉列表框的弹出方向改为向上,默认情况是向下的
#define DDLIST_DIRECTION_UP_EN 0

/******************************* 全局变量 ************************************/

/***************************** 私有全局变量 **********************************/
static lv_style_t my_style_btn_release;//按钮释放状态下的样式
static lv_style_t my_style_btn_press;//按钮按下时的样式
static lv_style_t my_style_btn_toggle;//按钮切换后的样式

static lv_obj_t *label_instorage;
static lv_obj_t *label_time;
static lv_obj_t *btn_home, *btn_home_label;
static lv_obj_t *btn_upload, *btn_upload_label;

static lv_obj_t *label_item, *label_layer, *label_shelf;

static lv_obj_t *label_mode;    
static lv_obj_t *btn_ok, *btn_ok_label;
static lv_obj_t *btn_save, *btn_save_label;

static lv_obj_t *kb;
static lv_obj_t *ta_item_name, *ta_layer_num, *ta_shelf_num;

static lv_style_t bg_style;
static lv_style_t sel_style;
static lv_obj_t * ddlist_mode;

static lv_style_t cell1_style;
static lv_style_t cell2_style;
static lv_obj_t * table_detail;
static bool is_cell_crop = false;//默认是使能文本自动换行功能的
static bool is_cell_merge = false;//是否合并单元格,默认不合并
#define TABLE_COL_CNT 3 //表格的列数
#define TABLE_ROW_CNT 11 //表格的行数
//每一列的宽度
static const uint16_t TABLE_COL_WIDTH[TABLE_COL_CNT] = {40,260,120};

static uint8_t item_name[ITEM_NAME_SIZE];
static uint8_t shelf_num;   
static uint8_t layer_num;

static uint8_t record_no = 0;

static lv_obj_t * mbox_ok, *mbox_save, *mbox_upload;
/******************************* 私有函数 ************************************/
static lv_obj_t * mbox_create(lv_obj_t * parent, char* msg, void (*mbox_event_cb)(lv_obj_t * , lv_event_t));//函数申明
static void mbox_ok_event_cb(lv_obj_t * obj, lv_event_t event);
static void mbox_upload_event_cb(lv_obj_t * obj, lv_event_t event);
static void mbox_save_event_cb(lv_obj_t * obj, lv_event_t event);
static void kb_create(lv_obj_t * parent);

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
		break;
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
        record_no = 0;
        rfid_scan_enable = SCAN_DISABLE;
		lv_obj_clean(lv_scr_act());
        screen_home_create();
	}
		break;
	default:
		break;
	}
}

static void btn_ok_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
        uint8_t len = strlen(lv_ta_get_text(ta_item_name));
        uint32_t val;
        if (len > 0 && len < ITEM_NAME_SIZE) { //输入内容合法
            strcpy(item_name, lv_ta_get_text(ta_item_name));
            if ((val = atoi(lv_ta_get_text(ta_shelf_num))) <= 0xFF) {
                shelf_num = val;
                if ((val = atoi(lv_ta_get_text(ta_layer_num))) <= 10) {
                    layer_num = val;        
                    char buf[32];
                    lv_ddlist_get_selected_str(ddlist_mode,buf,sizeof(buf));//获取选项值的文本内容
                    if (strcmp(buf, "Single") == 0) rfid_scan_mode = SCAN_MODE_INSTORAGE_SINGLE;
                    else if (strcmp(buf, "Multi") == 0) rfid_scan_mode = SCAN_MODE_INSTORAGE_MULTI;                    
                    //创建一个消息对话框
                    mbox_ok = mbox_create(lv_scr_act(), "Configuration Succeeded", mbox_ok_event_cb);
                    return ;
                }
            }
        }
        //输入内容不合法
        mbox_ok = mbox_create(lv_scr_act(), "Configuration Failed", mbox_ok_event_cb);
	}
		break;
	default:
		break;
	}
}

static void btn_save_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
        //创建一个消息对话框
        mbox_save = mbox_create(lv_scr_act(), "Confirm Save", mbox_save_event_cb);
	}
		break;
	default:
		break;
	}
}

//事件回调函数
static void kb_event_handler(lv_obj_t * obj,lv_event_t event)
{
  if(obj==kb)
  {
    const char * key_txt = lv_btnm_get_active_btn_text(kb);//获取按下键的文本标题,放到lv_kb_def_event_cb的前面调用
    lv_kb_def_event_cb(kb,event);//调用键盘的默认事件回调函数,帮我们处理掉一些通用的操作,如果让我们自己写代码来处理,那就太麻烦了
    //添加自己的功能代码
    if(event==LV_EVENT_VALUE_CHANGED)
    {
      //uint16_t key_id = *(uint16_t*)lv_event_get_data();//获取按下键的id,第一个按键的id为0,后面的按键依次增1
      if(key_txt==NULL)
        return;
    }else if(event==LV_EVENT_APPLY)
    {
      /*
        这里可以根据用户输入的文本信息,做相应的业务逻辑处理
      */
      lv_obj_del(kb);//点击√键时把键盘删除掉
      lv_ta_set_cursor_type(ta_item_name,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//先隐藏掉光标
      lv_ta_set_cursor_type(ta_shelf_num,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//先隐藏掉光标
      lv_ta_set_cursor_type(ta_layer_num,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//先隐藏掉光标
      kb = NULL;
    }else if(event==LV_EVENT_CANCEL)
    {
      /*
        这里也可以根据用户输入的文本信息,做相应的业务逻辑处理
      */
      lv_ta_set_cursor_type(ta_item_name,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//先隐藏掉光标
      lv_ta_set_cursor_type(ta_shelf_num,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//先隐藏掉光标
      lv_ta_set_cursor_type(ta_layer_num,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//先隐藏掉光标        
      lv_obj_del(kb);//点击×键时把键盘删除掉        
      kb = NULL;
    }
  }
}

static void ta_event_handler(lv_obj_t * obj,lv_event_t event)
{
  if(obj==ta_item_name||obj==ta_shelf_num||obj==ta_layer_num)
  {
    if(event==LV_EVENT_RELEASED)//松手事件
    {
      if(kb==NULL) {
        kb_create(lv_scr_act());//如果键盘不存在的话,则先创建键盘
        lv_kb_set_ta(kb,obj);//重新绑定文本域对象          
      }
      else
        lv_kb_set_ta(kb,obj);//重新绑定文本域对象
    }
  }
}

//事件回调函数
static void ddlist_event_handler(lv_obj_t * obj,lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
        char buf[32];
        uint16_t selected_index = lv_ddlist_get_selected(obj);//获取选项值的索引
        lv_ddlist_get_selected_str(obj,buf,sizeof(buf));//获取选项值的文本内容
        if (strcmp(buf, "Single") == 0) rfid_scan_mode = SCAN_MODE_INSTORAGE_SINGLE;
        else if (strcmp(buf, "Multi") == 0) rfid_scan_mode = SCAN_MODE_INSTORAGE_MULTI;
    }
}

static void mbox_ok_event_cb(lv_obj_t * obj, lv_event_t event)
{
    uint16_t btn_id;
    if (obj == mbox_ok) {
        if(event == LV_EVENT_VALUE_CHANGED) {
            //获取按钮 id
            btn_id = lv_mbox_get_active_btn(obj);
            if (btn_id == 0) { //OK 按钮
                rfid_scan_enable = SCAN_ENABLE;
                printf("OK\r\n");
            } else if(btn_id == 1) {//Cancel 按钮
                rfid_scan_enable = SCAN_DISABLE;
                printf("Cancel\r\n");
            }
            lv_mbox_start_auto_close(obj,0);
        }
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

static void mbox_save_event_cb(lv_obj_t * obj, lv_event_t event)
{
    uint16_t btn_id;
    if (obj == mbox_save) {
        if(event == LV_EVENT_VALUE_CHANGED) {
            //获取按钮 id
            btn_id = lv_mbox_get_active_btn(obj);
            if (btn_id == 0) { //OK 按钮
                printf("save\r\n");
                InternalFlash_WriteRecord(&item_info_head, ITEM_INFO_SIZE / 4);
                deleteList(&item_info_head);
            }
            lv_mbox_start_auto_close(obj,0);
        }
    }
}

//创建键盘
void kb_create(lv_obj_t * parent)
{
  kb = lv_kb_create(parent,NULL);
  lv_kb_set_cursor_manage(kb,true);//使能对光标的接管
  lv_kb_set_mode(kb,LV_KB_MODE_TEXT);//设置为文本键盘模式,这也是默认值
  lv_kb_set_ta(kb,ta_item_name);//先默认绑定ta_item_name文本域对象,后面可以通过点击某文本域来重新绑定到相应的文本域对象
  lv_obj_set_event_cb(kb,kb_event_handler);//设置自定义的事件回调函数
}

//例程入口
void lv_screen_instorage_start()
{
    lv_obj_t* scr = lv_scr_act();//获取当前活跃的屏幕对象

    //创建 label_instorage 标签,用来显示 label_time 标签的长文本模式
    label_instorage = lv_label_create(scr,NULL);//创建标签
    lv_label_set_long_mode(label_instorage,LV_LABEL_LONG_BREAK);//设置长文本模式
    //设置宽度,一定得放在 lv_label_set_long_mode 的后面,否则不起作用的
    lv_obj_set_width(label_instorage,150);
    lv_label_set_align(label_instorage,LV_LABEL_ALIGN_CENTER);//文本居中对齐
    lv_label_set_text(label_instorage,"InStorage");//设置文本
    lv_obj_set_pos(label_instorage, 165, 10);

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

    //创建 label_item 标签
    label_item = lv_label_create(scr,NULL);//创建标签
    lv_label_set_long_mode(label_item,LV_LABEL_LONG_BREAK);//设置长文本模式
    lv_obj_set_width(label_item,80);
    lv_label_set_align(label_item,LV_LABEL_ALIGN_RIGHT);
    lv_label_set_text(label_item,"Item:");//设置文本
    lv_obj_set_pos(label_item, 10, 60);

    //创建 label_shelf 标签
    label_shelf = lv_label_create(scr,NULL);//创建标签
    lv_label_set_long_mode(label_shelf,LV_LABEL_LONG_BREAK);//设置长文本模式
    lv_obj_set_width(label_shelf,80);
    lv_label_set_align(label_shelf,LV_LABEL_ALIGN_RIGHT);
    lv_label_set_text(label_shelf,"Shelf:");//设置文本
    lv_obj_set_pos(label_shelf, 10, 100);
    
    //创建 label_layer 标签
    label_layer = lv_label_create(scr,NULL);//创建标签
    lv_label_set_long_mode(label_layer,LV_LABEL_LONG_BREAK);//设置长文本模式
    lv_obj_set_width(label_layer,80);
    lv_label_set_align(label_layer,LV_LABEL_ALIGN_RIGHT);
    lv_label_set_text(label_layer,"Layer:");//设置文本
    lv_obj_set_pos(label_layer, 10, 140);

    //创建 label_mode 标签
    label_mode = lv_label_create(scr,NULL);//创建标签
    lv_label_set_long_mode(label_mode,LV_LABEL_LONG_BREAK);//设置长文本模式
    lv_obj_set_width(label_mode,80);
    lv_label_set_align(label_mode,LV_LABEL_ALIGN_RIGHT);
    lv_label_set_text(label_mode,"Mode:");//设置文本
    lv_obj_set_pos(label_mode, 260, 60);

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
    //1.3 切换状态下的样式
    lv_style_copy(&my_style_btn_toggle,&my_style_btn_press);
    my_style_btn_toggle.body.main_color = LV_COLOR_MAKE(0x04,0xBE,0x02);
    my_style_btn_toggle.body.grad_color = my_style_btn_toggle.body.main_color;
    my_style_btn_toggle.body.border.color = my_style_btn_toggle.body.main_color;
    my_style_btn_toggle.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    my_style_btn_toggle.text.color = LV_COLOR_WHITE;
    
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

    //创建一个ok按钮
    btn_ok = lv_btn_create(scr, NULL);
//    //设置按钮正常态下释放状态样式
//    lv_btn_set_style(btn_ok,LV_BTN_STYLE_REL,&my_style_btn_toggle);
//    //设置按钮正常态下按下状态样式
//    lv_btn_set_style(btn_ok,LV_BTN_STYLE_PR,&my_style_btn_toggle);
    //设置按钮正常态下释放状态样式
    lv_btn_set_style(btn_ok,LV_BTN_STYLE_REL,&my_style_btn_release);
    //设置按钮正常态下按下状态样式
    lv_btn_set_style(btn_ok,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_ok, 60, 30);//设置大小
    lv_obj_set_pos(btn_ok,270,135);//设置坐标
//    lv_btn_set_toggle(btn_ok,true);//设置为 Toggle 按钮
//    //设置按钮的起始状态为切换态下的释放状态
//    lv_btn_set_state(btn_ok,LV_BTN_STATE_TGL_REL);
//    //设置按钮切换态下的释放状态样式
//    lv_btn_set_style(btn_ok,LV_BTN_STYLE_TGL_REL,&my_style_btn_release);
//    lv_btn_set_style(btn_ok,LV_BTN_STYLE_TGL_PR,&my_style_btn_release);    
    btn_ok_label = lv_label_create(btn_ok,NULL);//给 btn_ok 添加 label 子对象
    lv_label_set_text(btn_ok_label,"OK");
    lv_obj_set_event_cb(btn_ok,btn_ok_event_cb);//设置事件回调函数
    
    //创建一个save按钮
    btn_save = lv_btn_create(scr, NULL);
    //设置按钮正常态下释放状态样式
    lv_btn_set_style(btn_save,LV_BTN_STYLE_REL,&my_style_btn_release);
    //设置按钮正常态下按下状态样式
    lv_btn_set_style(btn_save,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_save, 60, 30);//设置大小
    lv_obj_set_pos(btn_save,350,135);//设置坐标
    btn_save_label = lv_label_create(btn_save,NULL);//给 btn_save 添加 label 子对象
    lv_label_set_text(btn_save_label,"Save");
    lv_obj_set_event_cb(btn_save,btn_save_event_cb);//设置事件回调函数
    
    //1.1 创建ta_item_name文本域对象
    ta_item_name = lv_ta_create(scr,NULL);
    lv_obj_set_size(ta_item_name, 150, 30);//设置大小
    lv_obj_set_pos(ta_item_name,100,55);//设置坐标
    lv_ta_set_one_line(ta_item_name,true);//使能单行模式,在单行模式下,文本域的高度是不能被设置的
    lv_ta_set_text(ta_item_name,"");//设置为空文本
    lv_obj_set_event_cb(ta_item_name,ta_event_handler);//设置事件回调函数
    //1.2 创建ta_shelf_num文本域对象
    ta_shelf_num = lv_ta_create(scr,ta_item_name);//直接从ta_item_name进行复制
    lv_obj_set_pos(ta_shelf_num,100,95);//设置坐标
    lv_ta_set_cursor_type(ta_shelf_num,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//先隐藏掉光标    
    //1.3 创建ta_layer_num文本域对象
    ta_layer_num = lv_ta_create(scr,ta_item_name);//直接从ta_item_name进行复制
    lv_obj_set_pos(ta_layer_num,100,135);//设置坐标
    lv_ta_set_cursor_type(ta_layer_num,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//先隐藏掉光标


    //1.创建样式
    //1.1 创建背景样式
    lv_style_copy(&bg_style,&lv_style_plain);
    bg_style.body.main_color = LV_COLOR_WHITE;//纯白色背景
    bg_style.body.grad_color = bg_style.body.main_color;
    bg_style.body.border.width = 1;//边框宽度
    bg_style.body.border.color = LV_COLOR_MAKE(0xAA,0xAA,0xAA);
    //LV_COLOR_MAKE(0x30,0x30,0x30);
    bg_style.body.padding.left = 10;//设置左侧的内边距
    bg_style.text.color = LV_COLOR_BLACK;//文本颜色
    bg_style.body.shadow.color = bg_style.body.border.color;//阴影颜色
    bg_style.body.shadow.width = 4;//阴影宽度
    //1.2 创建选择项被选中时的样式
    lv_style_copy(&sel_style,&lv_style_plain);
    sel_style.body.main_color = LV_COLOR_MAKE(0x5F,0xB8,0x78);//浅绿色背景
    sel_style.body.grad_color = sel_style.body.main_color;
    sel_style.text.color = LV_COLOR_WHITE;//文本为白色
    //2.创建下拉列表框
    ddlist_mode = lv_ddlist_create(scr,NULL);
    lv_ddlist_set_options(ddlist_mode,"Single\nMulti");//设置列表选项
    lv_ddlist_set_selected(ddlist_mode,0);//设置默认选中值为 Single
    lv_ddlist_set_fix_width(ddlist_mode,100);//设置固定宽度
    lv_ddlist_set_draw_arrow(ddlist_mode,true);//使能绘制向下的箭头
    lv_ddlist_set_style(ddlist_mode,LV_DDLIST_STYLE_BG,&bg_style);//设置背景样式
    lv_ddlist_set_style(ddlist_mode,LV_DDLIST_STYLE_SEL,&sel_style);//设置背景样式
    lv_obj_set_event_cb(ddlist_mode, ddlist_event_handler);//注册事件回调函数
    lv_obj_set_pos(ddlist_mode,345,57);//设置坐标
    
    //1.创建样式
    //1.1 创建第一种单元格样式,用来修饰普通数据单元格
    lv_style_copy(&cell1_style,&lv_style_plain);
    cell1_style.body.border.width = 1;
    cell1_style.body.border.color = LV_COLOR_BLACK;
    cell1_style.text.font = &lv_font_roboto_16;
    //1.2 创建第二种单元格样式,用来修饰标题行中的单元格
    lv_style_copy(&cell2_style,&lv_style_plain_color);
    cell2_style.body.border.width = 1;
    cell2_style.body.border.color = LV_COLOR_BLACK;
    cell2_style.body.main_color = LV_COLOR_SILVER;//银色的背景
    cell2_style.body.grad_color = LV_COLOR_SILVER;
    cell2_style.text.color = LV_COLOR_BLACK;    
    cell2_style.text.font = &lv_font_roboto_16;
    
    //2.创建表格
    table_detail = lv_table_create(scr,NULL);
    lv_table_set_col_cnt(table_detail,TABLE_COL_CNT);//设置表格的总列数
    lv_table_set_row_cnt(table_detail,TABLE_ROW_CNT);//设置表格的总行数    
    uint16_t row,col;
    for(col=0;col<TABLE_COL_CNT;col++)
    {
        //设置每一列的宽度
        lv_table_set_col_width(table_detail,col,TABLE_COL_WIDTH[col]);    
        for (row = 0; row < TABLE_ROW_CNT; row++) {        
            //所有单元格的文本内容居中对齐
            lv_table_set_cell_align(table_detail,row,col,LV_LABEL_ALIGN_CENTER);
        }
        lv_table_set_cell_type(table_detail,0,col,2);//设置标题行为第二种单元格类型
    }        
    
    //设置单元格的文本内容
    lv_table_set_cell_value(table_detail,0,0,"No");
    lv_table_set_cell_value(table_detail,0,1,"TID");
    lv_table_set_cell_value(table_detail,0,2,"Time");
    //设置表格的背景样式,为透明
    lv_table_set_style(table_detail,LV_TABLE_STYLE_BG,&lv_style_transp_tight);
    //设置第一种单元格的样式
    lv_table_set_style(table_detail,LV_TABLE_STYLE_CELL1,&cell1_style);
    //设置第二种单元格的样式
    lv_table_set_style(table_detail,LV_TABLE_STYLE_CELL2,&cell2_style);    
    lv_obj_set_pos(table_detail,30,190);//设置坐标
}

/**
 * @brief   向table中添加一条数据
 * @param  TID
 * @retval   无
 */
void screen_instorage_record_add(uint8_t *tid)
{
    if (0 != InternalFlash_FindTIDAddr(tid, FLASH_USER_START_ADDR, sizeof(item_info_t)))   return;  //不是新的标签

    char buf[30] = {0};
    char buf2[30] = {0};
    record_no++;

    item_info_t *item_info_new = (item_info_t *)malloc(sizeof(item_info_t));
    item_info_new->next = NULL;
    item_info_new->mark = MARK_INSTORAGE;
    memcpy(item_info_new->TID, tid, 12);
    memcpy(item_info_new->name, item_name, sizeof(item_name));
    item_info_new->shelf = shelf_num;
    item_info_new->layer = layer_num;
    item_info_new->instorage_time = YMD_To_Unix_Time(&system_time);
    item_info_new->outstorage_time = 0xFFFFFFFF;
    append(&item_info_head, item_info_new);

    if (record_no < TABLE_ROW_CNT) {    //超过表格最长显示数量则不显示
        sprintf(buf, "%u", record_no);   
        lv_table_set_cell_value(table_detail,record_no,0,buf);
        memset(buf, 0, sizeof(buf));
        for (uint8_t i  = 0; i < 12; i++) {
            sprintf(buf2, "%02X", item_info_new->TID[i]);
            strcat(buf, buf2);
        }
        lv_table_set_cell_value(table_detail,record_no,1,buf);
        memset(buf, 0, sizeof(buf));
        Unix_To_YMD_Time(&system_time, item_info_new->instorage_time);
        sprintf(buf, "%u:%02u  %u/%u/%u", system_time.hour, system_time.minute, 
                                                            system_time.year, system_time.month, system_time.day);    
        lv_table_set_cell_value(table_detail,record_no,2,buf);        
    }
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
void screen_instorage_create(void)
{
    printf("screen_instorage_start\r\n");
    lv_screen_instorage_start();
}

 /*********************************END OF FILE*********************************/
