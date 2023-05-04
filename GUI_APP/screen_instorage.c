/**
  ******************************************************************************
  * @file    screen_instorage.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2023-03-23
  * @brief   lvgl screen_instorage create
  *	    �޸ļ�¼ :
  *		�汾��  ����              ����             ˵��
  *         V1.0    2023-03-23  SwaveYu     ��ʽ����
  ******************************************************************************
  * @attention
  *
  *
  *
  *
  *
  ******************************************************************************
  */

/******************************** ͷ�ļ� *************************************/
#include "demo.h"
#include "screen_instorage.h"
#include "screen_home.h"

/****************************** ˽�к궨�� ***********************************/
//�Ƿ������б��ĵ��������Ϊ����,Ĭ����������µ�
#define DDLIST_DIRECTION_UP_EN 0

/******************************* ȫ�ֱ��� ************************************/

/***************************** ˽��ȫ�ֱ��� **********************************/
static lv_style_t my_style_btn_release;//��ť�ͷ�״̬�µ���ʽ
static lv_style_t my_style_btn_press;//��ť����ʱ����ʽ
static lv_style_t my_style_btn_toggle;//��ť�л������ʽ

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
static bool is_cell_crop = false;//Ĭ����ʹ���ı��Զ����й��ܵ�
static bool is_cell_merge = false;//�Ƿ�ϲ���Ԫ��,Ĭ�ϲ��ϲ�
#define TABLE_COL_CNT 3 //��������
#define TABLE_ROW_CNT 11 //��������
//ÿһ�еĿ��
static const uint16_t TABLE_COL_WIDTH[TABLE_COL_CNT] = {40,260,120};

static uint8_t item_name[ITEM_NAME_SIZE];
static uint8_t shelf_num;   
static uint8_t layer_num;

static uint8_t record_no = 0;

static lv_obj_t * mbox_ok, *mbox_save, *mbox_upload;
/******************************* ˽�к��� ************************************/
static lv_obj_t * mbox_create(lv_obj_t * parent, char* msg, void (*mbox_event_cb)(lv_obj_t * , lv_event_t));//��������
static void mbox_ok_event_cb(lv_obj_t * obj, lv_event_t event);
static void mbox_upload_event_cb(lv_obj_t * obj, lv_event_t event);
static void mbox_save_event_cb(lv_obj_t * obj, lv_event_t event);
static void kb_create(lv_obj_t * parent);

//�¼��ص�����
static void btn_upload_event_cb(lv_obj_t * obj, lv_event_t event)
{
	switch (event)
	{
	case LV_EVENT_CLICKED:
	{
         //����һ����Ϣ�Ի���
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
        if (len > 0 && len < ITEM_NAME_SIZE) { //�������ݺϷ�
            strcpy(item_name, lv_ta_get_text(ta_item_name));
            if ((val = atoi(lv_ta_get_text(ta_shelf_num))) <= 0xFF) {
                shelf_num = val;
                if ((val = atoi(lv_ta_get_text(ta_layer_num))) <= 10) {
                    layer_num = val;        
                    char buf[32];
                    lv_ddlist_get_selected_str(ddlist_mode,buf,sizeof(buf));//��ȡѡ��ֵ���ı�����
                    if (strcmp(buf, "Single") == 0) rfid_scan_mode = SCAN_MODE_INSTORAGE_SINGLE;
                    else if (strcmp(buf, "Multi") == 0) rfid_scan_mode = SCAN_MODE_INSTORAGE_MULTI;                    
                    //����һ����Ϣ�Ի���
                    mbox_ok = mbox_create(lv_scr_act(), "Configuration Succeeded", mbox_ok_event_cb);
                    return ;
                }
            }
        }
        //�������ݲ��Ϸ�
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
        //����һ����Ϣ�Ի���
        mbox_save = mbox_create(lv_scr_act(), "Confirm Save", mbox_save_event_cb);
	}
		break;
	default:
		break;
	}
}

//�¼��ص�����
static void kb_event_handler(lv_obj_t * obj,lv_event_t event)
{
  if(obj==kb)
  {
    const char * key_txt = lv_btnm_get_active_btn_text(kb);//��ȡ���¼����ı�����,�ŵ�lv_kb_def_event_cb��ǰ�����
    lv_kb_def_event_cb(kb,event);//���ü��̵�Ĭ���¼��ص�����,�����Ǵ����һЩͨ�õĲ���,����������Լ�д����������,�Ǿ�̫�鷳��
    //����Լ��Ĺ��ܴ���
    if(event==LV_EVENT_VALUE_CHANGED)
    {
      //uint16_t key_id = *(uint16_t*)lv_event_get_data();//��ȡ���¼���id,��һ��������idΪ0,����İ���������1
      if(key_txt==NULL)
        return;
    }else if(event==LV_EVENT_APPLY)
    {
      /*
        ������Ը����û�������ı���Ϣ,����Ӧ��ҵ���߼�����
      */
      lv_obj_del(kb);//����̼�ʱ�Ѽ���ɾ����
      lv_ta_set_cursor_type(ta_item_name,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ص����
      lv_ta_set_cursor_type(ta_shelf_num,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ص����
      lv_ta_set_cursor_type(ta_layer_num,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ص����
      kb = NULL;
    }else if(event==LV_EVENT_CANCEL)
    {
      /*
        ����Ҳ���Ը����û�������ı���Ϣ,����Ӧ��ҵ���߼�����
      */
      lv_ta_set_cursor_type(ta_item_name,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ص����
      lv_ta_set_cursor_type(ta_shelf_num,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ص����
      lv_ta_set_cursor_type(ta_layer_num,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ص����        
      lv_obj_del(kb);//�������ʱ�Ѽ���ɾ����        
      kb = NULL;
    }
  }
}

static void ta_event_handler(lv_obj_t * obj,lv_event_t event)
{
  if(obj==ta_item_name||obj==ta_shelf_num||obj==ta_layer_num)
  {
    if(event==LV_EVENT_RELEASED)//�����¼�
    {
      if(kb==NULL) {
        kb_create(lv_scr_act());//������̲����ڵĻ�,���ȴ�������
        lv_kb_set_ta(kb,obj);//���°��ı������          
      }
      else
        lv_kb_set_ta(kb,obj);//���°��ı������
    }
  }
}

//�¼��ص�����
static void ddlist_event_handler(lv_obj_t * obj,lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED)
    {
        char buf[32];
        uint16_t selected_index = lv_ddlist_get_selected(obj);//��ȡѡ��ֵ������
        lv_ddlist_get_selected_str(obj,buf,sizeof(buf));//��ȡѡ��ֵ���ı�����
        if (strcmp(buf, "Single") == 0) rfid_scan_mode = SCAN_MODE_INSTORAGE_SINGLE;
        else if (strcmp(buf, "Multi") == 0) rfid_scan_mode = SCAN_MODE_INSTORAGE_MULTI;
    }
}

static void mbox_ok_event_cb(lv_obj_t * obj, lv_event_t event)
{
    uint16_t btn_id;
    if (obj == mbox_ok) {
        if(event == LV_EVENT_VALUE_CHANGED) {
            //��ȡ��ť id
            btn_id = lv_mbox_get_active_btn(obj);
            if (btn_id == 0) { //OK ��ť
                rfid_scan_enable = SCAN_ENABLE;
                printf("OK\r\n");
            } else if(btn_id == 1) {//Cancel ��ť
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
            //��ȡ��ť id
            btn_id = lv_mbox_get_active_btn(obj);
            if (btn_id == 0) { //OK ��ť
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
            //��ȡ��ť id
            btn_id = lv_mbox_get_active_btn(obj);
            if (btn_id == 0) { //OK ��ť
                printf("save\r\n");
                InternalFlash_WriteRecord(&item_info_head, ITEM_INFO_SIZE / 4);
                deleteList(&item_info_head);
            }
            lv_mbox_start_auto_close(obj,0);
        }
    }
}

//��������
void kb_create(lv_obj_t * parent)
{
  kb = lv_kb_create(parent,NULL);
  lv_kb_set_cursor_manage(kb,true);//ʹ�ܶԹ��Ľӹ�
  lv_kb_set_mode(kb,LV_KB_MODE_TEXT);//����Ϊ�ı�����ģʽ,��Ҳ��Ĭ��ֵ
  lv_kb_set_ta(kb,ta_item_name);//��Ĭ�ϰ�ta_item_name�ı������,�������ͨ�����ĳ�ı��������°󶨵���Ӧ���ı������
  lv_obj_set_event_cb(kb,kb_event_handler);//�����Զ�����¼��ص�����
}

//�������
void lv_screen_instorage_start()
{
    lv_obj_t* scr = lv_scr_act();//��ȡ��ǰ��Ծ����Ļ����

    //���� label_instorage ��ǩ,������ʾ label_time ��ǩ�ĳ��ı�ģʽ
    label_instorage = lv_label_create(scr,NULL);//������ǩ
    lv_label_set_long_mode(label_instorage,LV_LABEL_LONG_BREAK);//���ó��ı�ģʽ
    //���ÿ��,һ���÷��� lv_label_set_long_mode �ĺ���,���������õ�
    lv_obj_set_width(label_instorage,150);
    lv_label_set_align(label_instorage,LV_LABEL_ALIGN_CENTER);//�ı����ж���
    lv_label_set_text(label_instorage,"InStorage");//�����ı�
    lv_obj_set_pos(label_instorage, 165, 10);

    //���� label_time ��ǩ
    static lv_style_t label_text_style; 
    lv_style_copy(&label_text_style,&lv_style_plain);
    label_text_style.text.font = &lv_font_roboto_16;
    label_time = lv_label_create(scr,NULL);//������ǩ
    lv_label_set_long_mode(label_time,LV_LABEL_LONG_BREAK);//���ó��ı�ģʽ    
    lv_obj_set_width(label_time,100);
    lv_label_set_align(label_time,LV_LABEL_ALIGN_CENTER);//�ı����ж���    
    lv_label_set_style(label_time, LV_LABEL_STYLE_MAIN, &label_text_style);
    uint8_t buf[30];
    RTC_Get(&system_time.year, &system_time.month, &system_time.day, &system_time.hour, &system_time.minute, &system_time.second);// ��ȡ����RTCʱ��
    sprintf(buf, "%u:%02u\n%u/%u/%u", system_time.hour, system_time.minute, 
                                                        system_time.year, system_time.month, system_time.day);
    lv_label_set_text(label_time, buf);//�����ı�
    lv_obj_set_pos(label_time, 375, 5);

    //���� label_item ��ǩ
    label_item = lv_label_create(scr,NULL);//������ǩ
    lv_label_set_long_mode(label_item,LV_LABEL_LONG_BREAK);//���ó��ı�ģʽ
    lv_obj_set_width(label_item,80);
    lv_label_set_align(label_item,LV_LABEL_ALIGN_RIGHT);
    lv_label_set_text(label_item,"Item:");//�����ı�
    lv_obj_set_pos(label_item, 10, 60);

    //���� label_shelf ��ǩ
    label_shelf = lv_label_create(scr,NULL);//������ǩ
    lv_label_set_long_mode(label_shelf,LV_LABEL_LONG_BREAK);//���ó��ı�ģʽ
    lv_obj_set_width(label_shelf,80);
    lv_label_set_align(label_shelf,LV_LABEL_ALIGN_RIGHT);
    lv_label_set_text(label_shelf,"Shelf:");//�����ı�
    lv_obj_set_pos(label_shelf, 10, 100);
    
    //���� label_layer ��ǩ
    label_layer = lv_label_create(scr,NULL);//������ǩ
    lv_label_set_long_mode(label_layer,LV_LABEL_LONG_BREAK);//���ó��ı�ģʽ
    lv_obj_set_width(label_layer,80);
    lv_label_set_align(label_layer,LV_LABEL_ALIGN_RIGHT);
    lv_label_set_text(label_layer,"Layer:");//�����ı�
    lv_obj_set_pos(label_layer, 10, 140);

    //���� label_mode ��ǩ
    label_mode = lv_label_create(scr,NULL);//������ǩ
    lv_label_set_long_mode(label_mode,LV_LABEL_LONG_BREAK);//���ó��ı�ģʽ
    lv_obj_set_width(label_mode,80);
    lv_label_set_align(label_mode,LV_LABEL_ALIGN_RIGHT);
    lv_label_set_text(label_mode,"Mode:");//�����ı�
    lv_obj_set_pos(label_mode, 260, 60);

    //1.�ȴ��� 2 ��״̬�°�ť��ʽ
    //1.1 �ͷ�״̬�µ���ʽ
    lv_style_copy(&my_style_btn_release,&lv_style_plain_color);
    //���ô�ɫ�ı���
    my_style_btn_release.body.main_color = LV_COLOR_MAKE(0x1E,0x9F,0xFF);
    my_style_btn_release.body.grad_color = my_style_btn_release.body.main_color;
    my_style_btn_release.body.opa = LV_OPA_COVER;//���ñ���ɫ��ȫ��͸��
    my_style_btn_release.body.radius = LV_RADIUS_CIRCLE;//����Բ�ǰ�ť
    my_style_btn_release.body.shadow.color = LV_COLOR_MAKE(0x1E,0x9F,0xFF);
    my_style_btn_release.body.shadow.type = LV_SHADOW_FULL;//�����ı߶�����Ӱ
    my_style_btn_release.body.shadow.width = 3;//������Ӱ�Ŀ��
    my_style_btn_release.text.color = LV_COLOR_WHITE;
    my_style_btn_release.body.padding.left = 10;//�������ڱ߾�
    my_style_btn_release.body.padding.right = 10;//�������ڱ߾�
    //1.2 ����״̬�µ���ʽ
    lv_style_copy(&my_style_btn_press,&lv_style_plain_color);
    my_style_btn_press.body.opa = LV_OPA_0;//���ñ���ɫ͸��
    my_style_btn_press.body.radius = LV_RADIUS_CIRCLE;//����Բ�ǰ�ť
    //���ñ߿����ɫ
    my_style_btn_press.body.border.color = LV_COLOR_MAKE(0xC9,0xC9,0xC9);
    my_style_btn_press.body.border.part = LV_BORDER_FULL;//�����߿򶼻���
    my_style_btn_press.body.border.width = 2;//���ñ߿�Ŀ��
    my_style_btn_press.body.border.opa = LV_OPA_COVER;//���ñ߿���ȫ��͸��
    my_style_btn_press.text.color = LV_COLOR_BLACK;
    my_style_btn_press.body.padding.left = 10;//�������ڱ߾�
    my_style_btn_press.body.padding.right = 10;//�������ڱ߾�
    //1.3 �л�״̬�µ���ʽ
    lv_style_copy(&my_style_btn_toggle,&my_style_btn_press);
    my_style_btn_toggle.body.main_color = LV_COLOR_MAKE(0x04,0xBE,0x02);
    my_style_btn_toggle.body.grad_color = my_style_btn_toggle.body.main_color;
    my_style_btn_toggle.body.border.color = my_style_btn_toggle.body.main_color;
    my_style_btn_toggle.body.opa = LV_OPA_COVER;//���ñ���ɫ��ȫ��͸��
    my_style_btn_toggle.text.color = LV_COLOR_WHITE;
    
    //����һ��home��ť   
    btn_home = lv_btn_create(scr, NULL);    
    //���ð�ť����̬���ͷ�״̬��ʽ
    lv_btn_set_style(btn_home,LV_BTN_STYLE_REL,&my_style_btn_release);
    //���ð�ť����̬�°���״̬��ʽ
    lv_btn_set_style(btn_home,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_home, 100, 50);//���ô�С
    lv_obj_set_pos(btn_home,190,750);//�������� 
       
    btn_home_label = lv_label_create(btn_home,NULL);//�� btn_home ��� label �Ӷ���
    lv_label_set_text(btn_home_label,"Home");
    lv_obj_set_event_cb(btn_home,btn_home_event_cb);//�����¼��ص�����

    
    //����һ��upload��ť   
    btn_upload = lv_btn_create(scr, NULL);
    //���ð�ť����̬���ͷ�״̬��ʽ
    lv_btn_set_style(btn_upload,LV_BTN_STYLE_REL,&my_style_btn_release);
    //���ð�ť����̬�°���״̬��ʽ
    lv_btn_set_style(btn_upload,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_upload, 100, 35);//���ô�С
    lv_obj_set_pos(btn_upload,5,5);//�������� 
    btn_upload_label = lv_label_create(btn_upload,NULL);//�� btn_upload ��� label �Ӷ���
    lv_label_set_text(btn_upload_label,"Upload");
    lv_obj_set_event_cb(btn_upload,btn_upload_event_cb);//�����¼��ص�����

    //����һ��ok��ť
    btn_ok = lv_btn_create(scr, NULL);
//    //���ð�ť����̬���ͷ�״̬��ʽ
//    lv_btn_set_style(btn_ok,LV_BTN_STYLE_REL,&my_style_btn_toggle);
//    //���ð�ť����̬�°���״̬��ʽ
//    lv_btn_set_style(btn_ok,LV_BTN_STYLE_PR,&my_style_btn_toggle);
    //���ð�ť����̬���ͷ�״̬��ʽ
    lv_btn_set_style(btn_ok,LV_BTN_STYLE_REL,&my_style_btn_release);
    //���ð�ť����̬�°���״̬��ʽ
    lv_btn_set_style(btn_ok,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_ok, 60, 30);//���ô�С
    lv_obj_set_pos(btn_ok,270,135);//��������
//    lv_btn_set_toggle(btn_ok,true);//����Ϊ Toggle ��ť
//    //���ð�ť����ʼ״̬Ϊ�л�̬�µ��ͷ�״̬
//    lv_btn_set_state(btn_ok,LV_BTN_STATE_TGL_REL);
//    //���ð�ť�л�̬�µ��ͷ�״̬��ʽ
//    lv_btn_set_style(btn_ok,LV_BTN_STYLE_TGL_REL,&my_style_btn_release);
//    lv_btn_set_style(btn_ok,LV_BTN_STYLE_TGL_PR,&my_style_btn_release);    
    btn_ok_label = lv_label_create(btn_ok,NULL);//�� btn_ok ��� label �Ӷ���
    lv_label_set_text(btn_ok_label,"OK");
    lv_obj_set_event_cb(btn_ok,btn_ok_event_cb);//�����¼��ص�����
    
    //����һ��save��ť
    btn_save = lv_btn_create(scr, NULL);
    //���ð�ť����̬���ͷ�״̬��ʽ
    lv_btn_set_style(btn_save,LV_BTN_STYLE_REL,&my_style_btn_release);
    //���ð�ť����̬�°���״̬��ʽ
    lv_btn_set_style(btn_save,LV_BTN_STYLE_PR,&my_style_btn_press);
    lv_obj_set_size(btn_save, 60, 30);//���ô�С
    lv_obj_set_pos(btn_save,350,135);//��������
    btn_save_label = lv_label_create(btn_save,NULL);//�� btn_save ��� label �Ӷ���
    lv_label_set_text(btn_save_label,"Save");
    lv_obj_set_event_cb(btn_save,btn_save_event_cb);//�����¼��ص�����
    
    //1.1 ����ta_item_name�ı������
    ta_item_name = lv_ta_create(scr,NULL);
    lv_obj_set_size(ta_item_name, 150, 30);//���ô�С
    lv_obj_set_pos(ta_item_name,100,55);//��������
    lv_ta_set_one_line(ta_item_name,true);//ʹ�ܵ���ģʽ,�ڵ���ģʽ��,�ı���ĸ߶��ǲ��ܱ����õ�
    lv_ta_set_text(ta_item_name,"");//����Ϊ���ı�
    lv_obj_set_event_cb(ta_item_name,ta_event_handler);//�����¼��ص�����
    //1.2 ����ta_shelf_num�ı������
    ta_shelf_num = lv_ta_create(scr,ta_item_name);//ֱ�Ӵ�ta_item_name���и���
    lv_obj_set_pos(ta_shelf_num,100,95);//��������
    lv_ta_set_cursor_type(ta_shelf_num,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ص����    
    //1.3 ����ta_layer_num�ı������
    ta_layer_num = lv_ta_create(scr,ta_item_name);//ֱ�Ӵ�ta_item_name���и���
    lv_obj_set_pos(ta_layer_num,100,135);//��������
    lv_ta_set_cursor_type(ta_layer_num,LV_CURSOR_LINE|LV_CURSOR_HIDDEN);//�����ص����


    //1.������ʽ
    //1.1 ����������ʽ
    lv_style_copy(&bg_style,&lv_style_plain);
    bg_style.body.main_color = LV_COLOR_WHITE;//����ɫ����
    bg_style.body.grad_color = bg_style.body.main_color;
    bg_style.body.border.width = 1;//�߿���
    bg_style.body.border.color = LV_COLOR_MAKE(0xAA,0xAA,0xAA);
    //LV_COLOR_MAKE(0x30,0x30,0x30);
    bg_style.body.padding.left = 10;//���������ڱ߾�
    bg_style.text.color = LV_COLOR_BLACK;//�ı���ɫ
    bg_style.body.shadow.color = bg_style.body.border.color;//��Ӱ��ɫ
    bg_style.body.shadow.width = 4;//��Ӱ���
    //1.2 ����ѡ���ѡ��ʱ����ʽ
    lv_style_copy(&sel_style,&lv_style_plain);
    sel_style.body.main_color = LV_COLOR_MAKE(0x5F,0xB8,0x78);//ǳ��ɫ����
    sel_style.body.grad_color = sel_style.body.main_color;
    sel_style.text.color = LV_COLOR_WHITE;//�ı�Ϊ��ɫ
    //2.���������б��
    ddlist_mode = lv_ddlist_create(scr,NULL);
    lv_ddlist_set_options(ddlist_mode,"Single\nMulti");//�����б�ѡ��
    lv_ddlist_set_selected(ddlist_mode,0);//����Ĭ��ѡ��ֵΪ Single
    lv_ddlist_set_fix_width(ddlist_mode,100);//���ù̶����
    lv_ddlist_set_draw_arrow(ddlist_mode,true);//ʹ�ܻ������µļ�ͷ
    lv_ddlist_set_style(ddlist_mode,LV_DDLIST_STYLE_BG,&bg_style);//���ñ�����ʽ
    lv_ddlist_set_style(ddlist_mode,LV_DDLIST_STYLE_SEL,&sel_style);//���ñ�����ʽ
    lv_obj_set_event_cb(ddlist_mode, ddlist_event_handler);//ע���¼��ص�����
    lv_obj_set_pos(ddlist_mode,345,57);//��������
    
    //1.������ʽ
    //1.1 ������һ�ֵ�Ԫ����ʽ,����������ͨ���ݵ�Ԫ��
    lv_style_copy(&cell1_style,&lv_style_plain);
    cell1_style.body.border.width = 1;
    cell1_style.body.border.color = LV_COLOR_BLACK;
    cell1_style.text.font = &lv_font_roboto_16;
    //1.2 �����ڶ��ֵ�Ԫ����ʽ,�������α������еĵ�Ԫ��
    lv_style_copy(&cell2_style,&lv_style_plain_color);
    cell2_style.body.border.width = 1;
    cell2_style.body.border.color = LV_COLOR_BLACK;
    cell2_style.body.main_color = LV_COLOR_SILVER;//��ɫ�ı���
    cell2_style.body.grad_color = LV_COLOR_SILVER;
    cell2_style.text.color = LV_COLOR_BLACK;    
    cell2_style.text.font = &lv_font_roboto_16;
    
    //2.�������
    table_detail = lv_table_create(scr,NULL);
    lv_table_set_col_cnt(table_detail,TABLE_COL_CNT);//���ñ���������
    lv_table_set_row_cnt(table_detail,TABLE_ROW_CNT);//���ñ���������    
    uint16_t row,col;
    for(col=0;col<TABLE_COL_CNT;col++)
    {
        //����ÿһ�еĿ��
        lv_table_set_col_width(table_detail,col,TABLE_COL_WIDTH[col]);    
        for (row = 0; row < TABLE_ROW_CNT; row++) {        
            //���е�Ԫ����ı����ݾ��ж���
            lv_table_set_cell_align(table_detail,row,col,LV_LABEL_ALIGN_CENTER);
        }
        lv_table_set_cell_type(table_detail,0,col,2);//���ñ�����Ϊ�ڶ��ֵ�Ԫ������
    }        
    
    //���õ�Ԫ����ı�����
    lv_table_set_cell_value(table_detail,0,0,"No");
    lv_table_set_cell_value(table_detail,0,1,"TID");
    lv_table_set_cell_value(table_detail,0,2,"Time");
    //���ñ��ı�����ʽ,Ϊ͸��
    lv_table_set_style(table_detail,LV_TABLE_STYLE_BG,&lv_style_transp_tight);
    //���õ�һ�ֵ�Ԫ�����ʽ
    lv_table_set_style(table_detail,LV_TABLE_STYLE_CELL1,&cell1_style);
    //���õڶ��ֵ�Ԫ�����ʽ
    lv_table_set_style(table_detail,LV_TABLE_STYLE_CELL2,&cell2_style);    
    lv_obj_set_pos(table_detail,30,190);//��������
}

/**
 * @brief   ��table�����һ������
 * @param  TID
 * @retval   ��
 */
void screen_instorage_record_add(uint8_t *tid)
{
    if (0 != InternalFlash_FindTIDAddr(tid, FLASH_USER_START_ADDR, sizeof(item_info_t)))   return;  //�����µı�ǩ

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

    if (record_no < TABLE_ROW_CNT) {    //����������ʾ��������ʾ
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

//�����Լ���װ֮�����Ϣ�Ի���
//parent:������,msg��ʾ����Ϣ
//����ֵ: ���ش�����������Ϣ�Ի������
lv_obj_t * mbox_create(lv_obj_t * parent, char* msg, void (*mbox_event_cb)(lv_obj_t * , lv_event_t))
{
    #define MBOX_WIDTH 200 //��Ϣ�Ի���Ŀ��
    #define MBOX_BTN_HEIGHT 30 //���ڲ�ÿ����ť�ĸ߶�

    static const char * btns[] ={"OK", "Cancel",""};

    lv_obj_t * mbox = lv_mbox_create(parent, NULL);
    lv_mbox_set_text(mbox, msg);
    lv_mbox_add_btns(mbox, btns);
    lv_obj_set_width(mbox, MBOX_WIDTH);
    lv_obj_set_event_cb(mbox, mbox_event_cb);
    lv_obj_align(mbox, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the corner*/

    //���ð�ť�ͷ�״̬����ʽ
    lv_mbox_set_style(mbox,LV_MBOX_STYLE_BTN_REL,&my_style_btn_release);
    //���ð�ť����״̬����ʽ
    lv_mbox_set_style(mbox,LV_MBOX_STYLE_BTN_PR,&my_style_btn_press);
    lv_obj_t * btnm_of_mbox = lv_mbox_get_btnm(mbox);//��ȡ���ڲ��ľ���ť����
    //���þ���ť�Ĵ�С
    lv_obj_set_size(btnm_of_mbox,MBOX_WIDTH,MBOX_BTN_HEIGHT);
    return mbox;
}

/**
 * @brief   �����û���������
 * @param  ��
 * @retval   ��
 */
void screen_instorage_create(void)
{
    printf("screen_instorage_start\r\n");
    lv_screen_instorage_start();
}

 /*********************************END OF FILE*********************************/
