/* 
 * File:   CircBuff.c
 * Author: Instructor
 *
 * Created on August 15, 2019, 9:24 AM
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <GenericTypeDefs.h>
#include <xc.h>
#include <sys/attribs.h>

#include "CircBuff.h"

void CircBuff_Init(struct circBuffer *cb)
{
    cb->head = 0; //sets head and tail of the buffer to index 0
    cb->tail = 0;
    int i;
    for (i = 0; i < MAX_BUFFER_LENGTH; i++) {//also sets each index to NULL to overwrite any data
        cb->data[i] = NULL;
    }
    return;
}

void CircBuff_Add_Data(unsigned char indata, struct circBuffer *cb)
{
    if (CircBuff_Test_Full((*cb)) == 1) { //stops process if buffer is full, trashes data
        return;
    }
    cb->data[cb->tail] = indata; //turns the tail data into indata
    cb->tail = (cb->tail + 1) % MAX_BUFFER_LENGTH; //increments tail, wraps around
    return;
}

unsigned char CircBuff_Read_Data(struct circBuffer *cb)
{
    if (CircBuff_Test_Empty((*cb)) == 1) { //does not read if buffer empty, returns NULL
        return ('\0');
    }
    unsigned char outdata = cb->data[cb->head]; //turns head data into outdata
    cb->data[cb->head] = NULL; //turns head data into NULL
    cb->head = (cb->head + 1) % MAX_BUFFER_LENGTH; //increments head, wraps around
    return (outdata);
}

int CircBuff_Test_Empty(struct circBuffer cb)
{
    if (cb.head == cb.tail) {
        return (1); //if head and tail are in same place, buffer empty
    } else {
        return (0);
    }
}

int CircBuff_Test_Full(struct circBuffer cb)
{
    if (((cb.tail + 1) % MAX_BUFFER_LENGTH) == cb.head) {
        return (1); //checks to see if an incremented tail would hit the head
    } else {
        return (0);
    }
}

int CircBuff_Length(struct circBuffer cb)
{
    int len = 0;
    int tail = cb.tail; //puts head and tail variables independent
    int head = cb.head;
    while (tail != head) { //increments head until it hits the tail
        head = (head + 1) % MAX_BUFFER_LENGTH;
        len += 1;
    }
    return (len); //returns number of times head incremented
}

#ifdef CIRCBUFF_TEST
#include "BOARD.h"
#include "Protocol.h"
int main(void)
{
    BOARD_Init();
    char string[] = "Hello World!";
    char string1[MAX_BUFFER_LENGTH]; //initializes input/output strings
    static struct circBuffer cb1; //initializes test circular buffer

    CircBuff_Init(&cb1);
    if (CircBuff_Test_Empty(cb1) == 1) { //tests empty testing and init
        printf("Starts Empty\n");
    }
    int i;
    for (i = 0; i < strlen(string); i++) { //tests inputting a string into buffer
        CircBuff_Add_Data(string[i], &cb1);
    }
    i = CircBuff_Length(cb1); //tests getting the length of buffer string
    printf("Length of string: %d\n", i);
    for (i = 0; i < strlen(string); i++) { //tests outputting the same string from buffer
        string1[i] = CircBuff_Read_Data(&cb1);
    }
    printf("%s\n", string1);
    if (CircBuff_Test_Empty(cb1) == 1) { //test buffer ends empty
        printf("Buffer is empty\n");
    }
    printf("Head:%d Tail: %d\n",cb1.head, cb1.tail);




    char string2[] = "sidcpvnjqroidnzdjdgdjntfuqglnmsehzmidfbrextncvcckgfcldsjpardxvnzkirfenngjztuzzzyizezahqaavkrushmswnpplnmdudhiunovfaksjzigxvpyanvj";
    char string3[MAX_BUFFER_LENGTH];
    //testing 129 character string
    for (i = 0; i < strlen(string2); i++) { //tests inputting a long string into buffer, throws out last 2 chars
        CircBuff_Add_Data(string2[i], &cb1);
    }
    printf("Length of next string: %d\n", strlen(string2));
    if (CircBuff_Test_Full(cb1) == 1) { //tests full testing
        printf("Buffer is full\n");
    }
    printf("Head:%d Tail: %d\n",cb1.head, cb1.tail);
    printf("Length of Buffer: %d\n",CircBuff_Length(cb1));
    
    for (i = 0; i < strlen(string2); i++) { //tests outputting the same string from buffer
        string3[i] = CircBuff_Read_Data(&cb1);
    }
    printf("%s\n\n", string3);
    if (CircBuff_Test_Empty(cb1) == 1) { //test buffer ends empty
        printf("Buffer ends empty\n");
    }
    printf("Head:%d Tail: %d\n",cb1.head, cb1.tail);
    for (i = 0; i < strlen(string2); i++) { //tests inputting a long string into buffer, throws out last 2 chars
        CircBuff_Add_Data(string2[i], &cb1);
    }
    CircBuff_Init(&cb1);
    printf("After Initialization Head:%d Tail: %d\n",cb1.head, cb1.tail);
    for (i = 0; i < MAX_BUFFER_LENGTH; i++) { //tests inputting a long string into buffer, throws out last 2 chars
        printf("After Initialization Data:%d p%d\n",cb1.data[i], i);
    }
    printf("After Initialization Data:%d\n",cb1.data[cb1.head]);

    unsigned short sh = Protocol_ShortEndednessConversion(0xDEAD); //also tests endedness conversions
    printf("%x\n", sh);

    unsigned int in = Protocol_IntEndednessConversion(0xDEADBEEF);
    printf("%x\n", in);

    BOARD_End;
    return (0);
}
#endif