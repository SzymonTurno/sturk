/**
 * @file cn/os/fstream.h
 * @brief Header file for file stream.
 *
 * *** todo ***.
 */

#ifndef CN_FSTREAM_H
#define CN_FSTREAM_H

#include <stdarg.h>
#include <stddef.h>

/**
 * @var typedef struct CnFstream CnFstream
 * @brief *** todo ***.
 */
typedef struct CnFstream CnFstream;

/**
 * @fn CnFstream* cn_fopen(const char* filename, const char* mode)
 * @brief *** todo ***.
 * @param[in] filename Input.
 * @param[in] mode Input.
 * @returns *** todo ***.
 */
CnFstream* cn_fopen(const char* filename, const char* mode);

/**
 * @fn void cn_fclose(CnFstream* stream)
 * @brief *** todo ***.
 * @param[in,out] stream Input/output.
 */
void cn_fclose(CnFstream* stream);

/**
 * @fn char* cn_fgets(char* str, int size, CnFstream* stream)
 * @brief *** todo ***.
 * @param[in,out] str Input/output.
 * @param[in] size Input.
 * @param[in,out] stream Input/output.
 * @returns *** todo ***.
 */
char* cn_fgets(char* str, int size, CnFstream* stream);

/**
 * @fn int cn_fseekset(CnFstream* stream, long int offset)
 * @brief *** todo ***.
 * @param[in,out] stream Input/output.
 * @param[in] offset Input.
 * @returns *** todo ***.
 */
int cn_fseekset(CnFstream* stream, long int offset);

/**
 * @fn CnFstream* cn_stdout(void)
 * @brief *** todo ***.
 * @returns *** todo ***.
 */
CnFstream* cn_stdout(void);

/**
 * @fn CnFstream* cn_stderr(void)
 * @brief *** todo ***.
 * @returns *** todo ***.
 */
CnFstream* cn_stderr(void);

/**
 * @fn int cn_fvprintf(CnFstream* stream, const char* format, va_list vlist)
 * @brief *** todo ***.
 * @param[in,out] stream Input/output.
 * @param[in] format Input.
 * @param[in] vlist Input.
 * @returns *** todo ***.
 */
int cn_fvprintf(CnFstream* stream, const char* format, va_list vlist);

#endif /* CN_FSTREAM_H */
