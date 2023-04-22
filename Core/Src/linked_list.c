/**
  ******************************************************************************
  * @file    linked_list.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2023-03-25
  * @brief   ������ع���
  *	    �޸ļ�¼ :
  *		�汾��  ����              ����             ˵��
  *
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
#include "linked_list.h"

/****************************** ˽�к궨�� ***********************************/

/******************************* ȫ�ֱ��� ************************************/
item_info_t *item_info_head;

/***************************** ˽��ȫ�ֱ��� **********************************/


/******************************* ˽�к��� ************************************/

/**
 * @brief  ��ӽڵ㵽�����ĩβ
 * @param  
 * @retval   ��
 */
void append(item_info_t** head_ref, item_info_t* new_node)
{
    // �����½ڵ���ڴ�
    item_info_t* last = *head_ref;
    
    // �������Ϊ�գ����½ڵ��ǵ�һ���ڵ�
    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }

    // �ҵ���������һ���ڵ�
    while (last->next != NULL) {
        last = last->next;
    }    

    // ���½ڵ���ӵ������ĩβ
    last->next = new_node;
    return;    
}


/**
 * @brief  ɾ������
 * @param  
 * @retval   ��
 */
void deleteList(item_info_t** head_ref)
{
    item_info_t* current = *head_ref;
    item_info_t* next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    *head_ref = NULL;
}

//// ɾ�������е�ָ���ڵ�
//void deleteNode(struct Node** head_ref, int key) {
//    struct Node* temp = *head_ref, *prev;

//    // ���ͷ�ڵ��Ǵ�ɾ���ڵ�
//    if (temp != NULL && temp->data == key) {
//        *head_ref = temp->next;  // ����ͷָ��
//        free(temp);  // �ͷ�ԭͷ�ڵ���ڴ�
//        return;
//    }

//    // �������в��Ҵ�ɾ���ڵ�
//    while (temp != NULL && temp->data != key) {
//        prev = temp;
//        temp = temp->next;
//    }

//    // ����ҵ��˴�ɾ���ڵ�
//    if (temp != NULL) {
//        prev->next = temp->next;  // ����ɾ���ڵ��ǰһ���ڵ�ָ���ɾ���ڵ����һ���ڵ�
//        free(temp);  // �ͷŴ�ɾ���ڵ���ڴ�
//    }
//}

 /*********************************END OF FILE*********************************/
