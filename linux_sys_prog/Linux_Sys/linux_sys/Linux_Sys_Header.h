#ifndef LINUX_SYS_HEADER_H_INCLUDED
#define LINUX_SYS_HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Linux Libaray
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/poll.h>

// custom Book Libaray
#include "file_io.h"
#include "multiplexed_io.h"


// custom User Libary
#include "User_file_io.h"
#include "Serial.h"
#include "file_stream.h"
#include "Scatter_gather.h"
#include "process.h"
#include "signal_ex.h"
#endif // LINUX_SYS_HEADER_H_INCLUDED
