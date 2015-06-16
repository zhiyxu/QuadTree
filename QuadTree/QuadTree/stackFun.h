#ifndef STACKFUN_H_
#define STACKFUN_H_

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "bmp.h"

struct InfoStruct
{
    WORD Mvalue;		//Morton值
    BYTE DIB;			//栅格属性值
};

struct Node				//栈节点
{
    InfoStruct info;
    struct Node *pred;
    struct Node *succ;
};

void stackPush(Node *header, InfoStruct str)	//向栈中压入一个节点
{
    Node *newNode = new Node;
    newNode->info = str;
    newNode->pred = header;
    newNode->succ = header->succ;

    header->succ = newNode;		//关系要阐述完全
    header->succ->pred = newNode;

    newNode = NULL;
    return;
}
void stackPop(Node *header, int p)				//从栈中删除p个节点
{
    while(p > 0)
    {
        Node *record = header->succ;
        header->succ = record->succ;
        if (record->succ != NULL) record->succ->pred = header;
        delete record;
        record = NULL;

        p--;
    }

    return;
}

// Node* stackPop(Node *header, Node *record, int p)	//删除record前面的p个节点
// {
//     while(p > 0)
//     {
//         Node *del = record->pred;
//
//         //record->pred = del->pred;
//         //del->pred->succ = record;
//         del->pred->succ = del->succ;
//         del->succ->pred = del->pred;
//
//         delete del;
//         del = NULL;
//
//         p--;
//     }
//
//     return record;
// }

#endif