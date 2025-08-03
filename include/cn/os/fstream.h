/**
 * @file cn/os/fstream.h
 *
 * @brief File stream.
 */

#ifndef CN_FSTREAM_H
#define CN_FSTREAM_H

#include <stdarg.h>
#include <stddef.h>

/**
 * @var typedef struct CnFstream CnFstream
 *
 * @brief File stream.
 *
 * @see FILE
 */
typedef struct CnFstream CnFstream;

/**
 * @fn CnFstream* cn_fopen(const char* filename, const char* mode)
 *
 * @see fopen()
 */
CnFstream* cn_fopen(const char* filename, const char* mode);

/**
 * @fn void cn_fclose(CnFstream* stream)
 *
 * @see fclose()
 */
void cn_fclose(CnFstream* stream);

/**
 * @fn char* cn_fgets(char* str, int size, CnFstream* stream)
 *
 * @see fgets()
 */
char* cn_fgets(char* str, int size, CnFstream* stream);

/**
 * @fn int cn_fseekset(CnFstream* stream, long int offset)
 *
 * This is fseek() with mode set to SEEK_SET.
 *
 * @see fseek()
 */
int cn_fseekset(CnFstream* stream, long int offset);

/**
 * @fn CnFstream* cn_stdout(void)
 *
 * @return The stdout stream.
 *
 * @see stdout
 */
CnFstream* cn_stdout(void);

/**
 * @fn CnFstream* cn_stderr(void)
 *
 * @return The stderr stream.
 *
 * @see stderr
 */
CnFstream* cn_stderr(void);

/**
 * @fn int cn_vfprintf(CnFstream* stream, const char* format, va_list vlist)
 *
 * @see vfprintf()
 */
int cn_vfprintf(CnFstream* stream, const char* format, va_list vlist);

#endif /* CN_FSTREAM_H */
