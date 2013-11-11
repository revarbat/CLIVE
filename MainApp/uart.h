#ifndef UART_H
#define UART_H

#include "ringbuf.h"


class CUart {
public:
    CUart(const char* dev);
    CUart(const char* dev, unsigned int baud);
    ~CUart();

    short DataCount()   { return readbuf.Count(); }
    bool  HasData()     { return (readbuf.Count() > 0); }
    bool  CanWrite()    { return (!writebuf.Full()); }
    bool  ReadBufFull() { return (readbuf.Full()); }
    unsigned int GetBaudRate() { return baudrate; }
    bool CharInReadBuf(char c) { return (readbuf.Has(c)); }

    char ReadByte();
    void WriteByte(char c);

    long GetString(char* buf, long nbytes);
    void Write(char* buf, long nbytes);
    void WriteString(const char* buf);

    void ReadBufStuff(const char* str);

    void SetBaudRate(unsigned int baud);
    static void Poll(void* ctx);

private:
    static void signal_handler_IO(int status);
    int uart_fd;
    CUart* next;

    CRingBuf<char,256>readbuf;
    CRingBuf<char,256>writebuf;
    unsigned int baudrate;
};


extern CUart gps_uart;
extern CUart turret_uart;


#endif

