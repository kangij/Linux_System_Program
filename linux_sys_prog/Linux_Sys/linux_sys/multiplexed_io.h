#ifndef MULTIPLEXED_IO_H_INCLUDED
#define MULTIPLEXED_IO_H_INCLUDED

extern int m_select_ex(int fd);
extern int select_ex2(int fd);

extern int poll_ex(int fd);

#endif // MULTIPLEXED_IO_H_INCLUDED
