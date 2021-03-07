/* 
 * File:   CircBuff.h
 * Author: Instructor
 *
 * Created on August 15, 2019, 9:24 AM
 */

#ifndef CIRCBUFF_H
#define	CIRCBUFF_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

#define MAX_BUFFER_LENGTH 128 // note that this does include the ID

struct circBuffer{
    unsigned int head;
    unsigned int tail;
    unsigned char data [ MAX_BUFFER_LENGTH ];
};

int CircBuff_Init(struct circBuffer *cb);

void CircBuff_Add_Data(unsigned char indata, struct circBuffer *cb);

unsigned char CircBuff_Read_Data(struct circBuffer *cb);

int CircBuff_Test_Empty(struct circBuffer cb);

int CircBuff_Test_Full(struct circBuffer cb);

int CircBuff_Length(struct circBuffer cb);

#endif	/* CIRCBUFF_H */

