#include <stdlib.h>
#include "uart.h"
#include "fbtimer.h"
#include "ringbuf.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <fcntl.h>
#include <poll.h>

CUart* uart_list = NULL;

CUart gps_uart     ("/dev/tty.usbserial-A602HTRY", B4800);
CUart turret_uart  ("/dev/ttyS2", B230400);

CUart::CUart(const char* dev)
{
    this->next = uart_list;
    uart_list = this;
    uart_fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(uart_fd == -1) {
	fprintf(stderr, "Unable to open %s\n", dev);
    } else {
	fprintf(stdout, "Connected to %s\n", dev);
    }
    SetBaudRate(B4800);

}


CUart::CUart(const char* dev, unsigned int baud)
{
    this->next = uart_list;
    uart_list = this;
    uart_fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(uart_fd == -1) {
	fprintf(stderr, "Unable to open %s\n", dev);
    } else {
	fprintf(stdout, "Connected to %s\n", dev);
    }
    SetBaudRate(baud);
}


CUart::~CUart()
{
}


void CUart::ReadBufStuff(const char* str)
{
    while (*str) {
        readbuf.Put(*str++);
    }
}


char CUart::ReadByte()
{
    while (!HasData());
    return readbuf.Get();
}


void CUart::WriteByte(char c)
{
    while (!CanWrite());
    writebuf.Put(c);
}


long CUart::GetString(char* buf, long nbytes)
{
    long pos = 0;
    while (1) {
        char c = ReadByte();
        if (pos+1 >= nbytes) {
            buf[pos] = '\0';
            return pos;
        }
        if (c == '\n') {
            buf[pos] = '\0';
            return pos;
        }
        buf[pos++] = c;
    }
}


void CUart::Write(char* buf, long nbytes)
{
    char* ptr = buf;
    for (int i = 0; i < nbytes; i++) {
        WriteByte(*ptr++);
    }
}


void CUart::WriteString(const char* buf)
{
    const char* ptr = buf;
    while (*ptr) {
        WriteByte(*ptr++);
    }
}



void CUart::SetBaudRate(unsigned int baud)
{
    struct termios newkey;
    struct sigaction saio;

    newkey.c_cflag = CRTSCTS | CS8 | CLOCAL | CREAD;
    newkey.c_iflag = IGNPAR;
    newkey.c_oflag = 0;
    newkey.c_lflag = 0;
    newkey.c_cc[VMIN]=1;
    newkey.c_cc[VTIME]=0;
    cfsetispeed(&newkey, baud);
    tcflush(uart_fd, TCIFLUSH);
    tcsetattr(uart_fd, TCSANOW, &newkey);

    fcntl(uart_fd, F_SETOWN, getpid());
    fcntl(uart_fd, F_SETFL, O_ASYNC);
    fcntl(uart_fd, F_SETFL, O_NONBLOCK);

    saio.sa_handler = CUart::signal_handler_IO;
    sigemptyset(&saio.sa_mask);
    saio.sa_flags = 0;
    sigaction(SIGIO, &saio, NULL);
    baudrate = baud;
}


void CUart::signal_handler_IO (int status)
{
    CUart* ptr = uart_list;
    while (ptr) {
	CUart::Poll(ptr);
        ptr = ptr->next;
    }
}


void CUart::Poll(void* ctx)
{
    int done;
    char buf[5];
    struct pollfd pfds[1];
    CUart* ptr = (CUart*)ctx;
    do {
	done = 1;
	pfds[0].fd = ptr->uart_fd;
	pfds[0].events = POLLRDNORM | POLLWRNORM;
	pfds[0].revents = 0;
	poll(pfds, 1, 0);
	if ((pfds[0].revents & POLLRDNORM)) {
	    if (read(ptr->uart_fd, buf, 1) > 0) {
		if (!ptr->readbuf.Full()) {
		    ptr->readbuf.Put(buf[0]);
		}
		done = 0;
	    }
	}
	if ((pfds[0].revents & POLLWRNORM)) {
	    if (ptr->writebuf.Count() > 0) {
		buf[0] = ptr->writebuf.Get();
		write(ptr->uart_fd, buf, 1);
		done = 0;
	    }
	}
    } while (!done);
}

