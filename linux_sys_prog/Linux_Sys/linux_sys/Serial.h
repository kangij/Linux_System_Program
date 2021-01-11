#ifndef SERIAL_H_INCLUDED
#define SERIAL_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h> // serial header
#include <sys/types.h>
#include <sys/poll.h>
#include <sys/select.h>
#include <sys/types.h>

#define BAUD_RATE B115200 // BuadRate Set

extern int serial_open(char *deivce, int mod);
extern int serial_write(int fd, char *buf, int size);
extern int serial_read(int fd, char *buf, int size, int mode);
extern int serial_read_select(int fd, char *buf, int size, int wait);
extern int serial_read_poll(int fd, char *buf, int size, int wait);
extern int serial_write_select(int fd, char *buf, int size, int wait);
extern int serial_write_poll(int fd, char *buf, int size, int wait);
extern void serieal_close(int fd);
extern void Serial_Task(void);

extern void serial_set_config(int fd);
extern void serial_set_none_config(int fd, int size);

#endif // SERIAL_H_INCLUDED
