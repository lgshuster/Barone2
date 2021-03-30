#include <xc.h>
#include "BOARD.h"
#include "buffer.h"

#define BUFF_LEN 128


static struct {
    unsigned char head, tail;
    unsigned char character[BUFF_LEN];
}TXBuffer, RXBuffer;

void initBuffer(void){
    TXBuffer.head = 0;
    TXBuffer.tail = 0;
    RXBuffer.head = 0;
    RXBuffer.tail = 0;
}

void writeBuffer(char inData, int TXorRX){
    if (TXorRX == 0){   // TX
            TXBuffer.character[TXBuffer.tail] = inData;
            TXBuffer.tail = (TXBuffer.tail + 1) % BUFF_LEN; // Increment tail, if tail is on last pointer then wrap around
    }
    if (TXorRX == 1){   // RX
            RXBuffer.character[RXBuffer.tail] = inData;
            RXBuffer.tail = (RXBuffer.tail + 1) % BUFF_LEN;
    }
}

char readBuffer(int TXorRX){
    if (TXorRX == 0){   // TX
            unsigned char outData = TXBuffer.character[TXBuffer.head]; // Load data to be ready to be read
            TXBuffer.head = (TXBuffer.head + 1) % BUFF_LEN;     // Move header to next char
            return outData;
    }
    
    if (TXorRX == 1){   // TX
            unsigned char outData = RXBuffer.character[RXBuffer.head]; // Load data to be ready to be read
            RXBuffer.head = (RXBuffer.head + 1) % BUFF_LEN;     // Move header to next char
            return outData;
    }
}

int emptyBuffer(int TXorRX){
    if (TXorRX == 0){   // TX
        if (TXBuffer.tail == TXBuffer.head)
            return 1;       //buffer is empty
        else 
            return 0;       //buffer isn't empty
    }
    if (TXorRX == 1){   // RX
        if (RXBuffer.tail == RXBuffer.head)
            return 1;       //buffer is empty
        else 
            return 0;       //buffer isn't empty
    }
}

int fullBuffer(int TXorRX){
    if (TXorRX == 0){   // TX
        if ((TXBuffer.tail + 1) % BUFF_LEN == (TXBuffer.head))
            return 1;       //buffer is full
        else
            return 0;       //buffer isn't full
    }
    if (TXorRX == 1){   // RX
        if ((RXBuffer.tail + 1) % BUFF_LEN == (RXBuffer.head))
            return 1;       //buffer is full
        else
            return 0;       //buffer isn't full
    }
}

int sizeBuffer(int TXorRX){
    if (TXorRX == 0){   // TX
        if (TXBuffer.tail > TXBuffer.head)
            return (TXBuffer.tail - TXBuffer.head);
        else if (TXBuffer.head > TXBuffer.tail)
            return (BUFF_LEN - TXBuffer.head + TXBuffer.tail + 1);
        else
            return ERROR;
    }
    if (TXorRX == 1){   // RX
        if (RXBuffer.tail > RXBuffer.head)
            return (RXBuffer.tail - RXBuffer.head);
        else if (RXBuffer.head > RXBuffer.tail)
            return (BUFF_LEN - RXBuffer.head + RXBuffer.tail + 1);
        else
            return ERROR;
    }
}