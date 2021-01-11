#ifndef FILE_IO_H_INCLUDED
#define FILE_IO_H_INCLUDED

extern int m_file_write(int fd,char *file,char *f_str);
extern int m_file_read(int fd,char *file);
extern void m_file_close(int fd,char *f_name);
#endif // FILE_IO_H_INCLUDED
