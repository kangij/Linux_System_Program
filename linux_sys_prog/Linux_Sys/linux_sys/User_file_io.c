
#include "Linux_Sys_Header.h"

void file_stream_open(FILE *stream)
{
    fopen(stream,"w+");
}
