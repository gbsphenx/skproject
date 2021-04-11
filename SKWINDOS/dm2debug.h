#ifndef DM2DEBUG_H
#define DM2DEBUG_H

#define LOGX(X) SPX_DEBUG_LOGFILE_INFO X

void DEBUG_DISPLAY_GDAT_MAIN_INFO();
void DEBUG_DUMP_BIN_DATA(unsigned char* ptr, unsigned int iDumpSize, unsigned int iBytesPerRow);
void DEBUG_DUMP_ULP();

#endif
