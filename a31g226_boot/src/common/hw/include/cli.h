/*
 * cli.h
 *
 *  Created on: Feb 3, 2022
 *      Author: suntaek.noh
 */

#ifndef SRC_COMMON_HW_INCLUDE_CLI_H_
#define SRC_COMMON_HW_INCLUDE_CLI_H_



#include "hw_def.h"


#ifdef _USE_HW_CLI

#define CLI_CMD_LIST_MAX      HW_CLI_CMD_LIST_MAX     // 명령어의 갯수
#define CLI_CMD_NAME_MAX      HW_CLI_CMD_NAME_MAX     // 명령어의 글자수

#define CLI_LINE_HIS_MAX      HW_CLI_LINE_HIS_MAX
#define CLI_LINE_BUF_MAX      HW_CLI_LINE_BUF_MAX

typedef struct
{
  uint16_t   argc;    // 아규먼트의 갯수
  char     **argv;    // 아규먼트 문자열(스트링)

  // 입력된 문자열을 변환 해주는 함수 (16byte)
  int32_t  (*getData)(uint8_t index);
  float    (*getFloat)(uint8_t index);
  char    *(*getStr)(uint8_t index);
  bool     (*isStr)(uint8_t index, char *p_str);
} cli_args_t;


bool cliInit(void);
bool cliOpen(uint8_t ch, uint32_t baud);
bool cliOpenLog(uint8_t ch, uint32_t baud);
bool cliMain(void);
void cliPrintf(const char *fmt, ...);
bool cliAdd(const char *cmd_str, void (*p_func)(cli_args_t *));
bool cliKeepLoop(void);

uint32_t cliAvailable(void);
uint8_t  cliRead(void);
uint32_t cliWrite(uint8_t *p_data, uint32_t length);


#endif

#endif /* SRC_COMMON_HW_INCLUDE_CLI_H_ */
