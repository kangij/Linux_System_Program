#ifndef MULTIPLEXED_IO_H_INCLUDED
#define MULTIPLEXED_IO_H_INCLUDED

#include <stdio.h>
#include <unistd.h>
#include <sys/epoll.h>

extern int m_select_ex(int fd);
extern int select_ex2(int fd);

extern int poll_ex(int fd);
extern void epoll_Task(void);
#endif // MULTIPLEXED_IO_H_INCLUDED
