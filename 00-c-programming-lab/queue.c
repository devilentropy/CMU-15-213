/* 
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q =  malloc(sizeof(queue_t));
    /* bd：处理错误 */
    if (!q) {
      return NULL;
    }
    /* cm：赋值 */
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* bd：注意判断指针 */
    /* rs：释放链表的每个元素 */
    if (!q) {
      return;
    }
    list_ele_t *temp;
    while (q->head) {
      temp = q->head;
      /* bd：注意判断指针！ */
      if (q->head->value) {
        free(q->head->value);
      }
      q->head = q->head->next;
      free(temp);
    }
    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* bd：q为空 */
    if (!q) {
      return false;
    }
    /* cm：新建节点 */
    /* bd：malloc失败 */
    list_ele_t *newh;
    /* cm：注意类型转化！ */
    newh = (list_ele_t *)malloc(sizeof(list_ele_t));
    if (!newh) {
      return false;
    }
    /* bd：节点赋值，添加结尾符！ */
    char *newh_v;
    if (s) {
      newh_v = (char *)malloc((strlen(s)+1) * sizeof(char));
      if (!newh_v) {
        /* bd：赋值失败，需要把节点也释放！ */
        free(newh);
        return false;
      }
      strcpy(newh_v, s);
      // free(s);
    } else {
        /* bd：容忍输入错误：s=NULL */
        newh_v = NULL;
      }
    newh->value = newh_v;
    /* cm：插入节点 */
    /* bd：如果插入的是第一个节点 */
    if (!q->head) {
      q->tail = newh;
    }
    newh->next = q->head;
    q->head = newh;
    q->size++;

    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* bd：q为空 */
    if (!q) {
      return false;
    }
    /* cm：新建节点并赋值 */
    list_ele_t *newt;
    newt = (list_ele_t *)malloc(sizeof(list_ele_t));
    /* bd：malloc失败 */
    if (!newt) {
      return false;
    }
    char *newt_v;
    if (s) {
      newt_v = (char *)malloc((strlen(s)+1) * sizeof(char));;
      if (!newt_v) {
        free(newt);
        return false;
      }
      strcpy(newt_v, s);
      // free(s);
    } else {
      /* bd：容忍输入错误：s=NULL */
      newt_v = NULL;
    }
    newt->value = newt_v;
    /* bd：尾部必须保证终止！*/
    newt->next = NULL;
    /* cm：插入节点 */
    /* bd：如果插入的是第一个节点 */
    if (!q->tail) {
      q->head = newt;
      q->tail = newt;
      q->size++;
      return true;
    }
    q->tail->next = newt;
    q->tail = newt;
    q->size++;
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* bd：q为NULL或空，sp为NULL */
    /* bd：这里不判断sp与bufsize的匹配性，由外部保证 */
    if (!q || !q->head || !sp) {
      return false;
    }
    /* cm：赋值操作 */
    /* bd：head节点没有值 */
    char *q_head_char = q->head->value;
    if (q_head_char){
      strncpy(sp, q_head_char, bufsize - 1);
      sp[bufsize - 1] = '\0';
      /* rs：及时释放内存！ */
      free(q_head_char);
    }
    /* rs：释放 */
    list_ele_t* q_head = q->head;
    q->head = q_head->next;
    free(q_head);
    /* cm：结构体一致性 */
    q->size--;
    /* bd：删除了最后一个节点 */
    if (!q->head) {
      q->tail = NULL;
    }
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* bd：判断为空 */
    if (!q || !q->head) {
      return 0;
    }
    return q->size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* bd：q为NULL或空，q只有一个元素*/
    if (!q || !q->head || q->size == 1) {
      return;
    }
    /* bd：先处理尾指针 */
    q->tail = q->head;
    /* cm：逐个将后面元素移到头部 */
    list_ele_t* sec_to_head;
    for (int i = 0; i < q->size - 1; i++) {
      sec_to_head = q->tail->next;
      /* cm：移动与连接*/
      q->tail->next = sec_to_head->next;
      sec_to_head->next = q->head;
      q->head = sec_to_head;
    }
    return;
}

