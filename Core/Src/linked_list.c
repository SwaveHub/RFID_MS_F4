/**
  ******************************************************************************
  * @file    linked_list.c
  * @author  SwaveYu
  * @version V1.0
  * @date    2023-03-25
  * @brief   链表相关功能
  *	    修改记录 :
  *		版本号  日期              作者             说明
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

/******************************** 头文件 *************************************/
#include "linked_list.h"

/****************************** 私有宏定义 ***********************************/

/******************************* 全局变量 ************************************/
item_info_t *item_info_head;

/***************************** 私有全局变量 **********************************/


/******************************* 私有函数 ************************************/

/**
 * @brief  添加节点到链表的末尾
 * @param  
 * @retval   无
 */
void append(item_info_t** head_ref, item_info_t* new_node)
{
    // 分配新节点的内存
    item_info_t* last = *head_ref;
    
    // 如果链表为空，则新节点是第一个节点
    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }

    // 找到链表的最后一个节点
    while (last->next != NULL) {
        last = last->next;
    }    

    // 将新节点添加到链表的末尾
    last->next = new_node;
    return;    
}


/**
 * @brief  删除链表
 * @param  
 * @retval   无
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

//// 删除链表中的指定节点
//void deleteNode(struct Node** head_ref, int key) {
//    struct Node* temp = *head_ref, *prev;

//    // 如果头节点是待删除节点
//    if (temp != NULL && temp->data == key) {
//        *head_ref = temp->next;  // 更改头指针
//        free(temp);  // 释放原头节点的内存
//        return;
//    }

//    // 在链表中查找待删除节点
//    while (temp != NULL && temp->data != key) {
//        prev = temp;
//        temp = temp->next;
//    }

//    // 如果找到了待删除节点
//    if (temp != NULL) {
//        prev->next = temp->next;  // 将待删除节点的前一个节点指向待删除节点的下一个节点
//        free(temp);  // 释放待删除节点的内存
//    }
//}

 /*********************************END OF FILE*********************************/
