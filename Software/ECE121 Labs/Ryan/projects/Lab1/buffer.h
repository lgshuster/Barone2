#include <xc.h>


/**
 * @Function int initBuffer(struct buffer *stack)
 * @param int, 0 for TX and 1 for RX
 * @return
 * @brief adds to circular buffer if space exists, if not returns ERROR
 * @author rwong */
void initBuffer(void);
/**
 * @Function int writeBuffer(unsigned char inData)
 * @param char, new char to be added to the circular buffer. int, 0 for TX and 1 for RX
 * @return
 * @brief adds to circular buffer if space exists, if not returns ERROR
 * @author rwong */
void writeBuffer(char inData, int TXorRX);
/**
 * @Function int readBuffer(void)
 * @param int, 0 for TX and 1 for RX
 * @return char, data
 * @brief reads data from circular buffer , if not returns ERROR
 * @author rwong */
char readBuffer(int TXorRX);
/**
 * @Function int emptyBuffer(void)
 * @param int, 0 for TX and 1 for RX
 * @return 1 if buffer is empty, 0 if not empty
 * @brief checks if buffer is empty
 * @author rwong */
int emptyBuffer(int TXorRX);
/**
 * @Function int fullBuffer(void)
 * @param int, 0 for TX and 1 for RX
 * @return '1' if full or '0' if empty
 * @brief checks if buffer is full
 * @author rwong */
int fullBuffer(int TXorRX);
/**
 * @Function int itemBuffer(void)
 * @param int, 0 for TX and 1 for RX
 * @return number of valid chars in data or ERROR
 * @brief reads data from circular buffer , if not returns ERROR
 * @author rwong */
int sizeBuffer(int TXorRX);