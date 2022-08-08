/*
 * flash.c
 *
 *  Created on: 2022. 8. 8.
 *      Author: sunta
 */



#include "flash.h"
#include "cli.h"


#ifdef _USE_HW_FLASH

#define FLASH_SECTOR_ADDR   0x00000000      //flash의 시작 주소
#define FLASH_SECTOR_MAX    256
#define FLASH_SECTOR_SIZE   1024


static bool flashInSector(uint16_t sector_num, uint32_t addr, uint32_t length);
static void flashUnlock(void);
static void flashLock(void);

#ifdef _USE_HW_CLI
static void cliFlash(cli_args_t *args);
#endif

bool flashInit(void)
{
  bool ret = true;

  #ifdef _USE_HW_CLI
  cliAdd("flash", cliFlash);
  #endif

  return ret;
}

bool flashErase(uint32_t addr, uint32_t length)
{
  bool ret = false;
  int status;

  int16_t start_sector_num = -1;    // 루프 시작했을때 한번만 값을 얻기 위해 -1을 사용
  uint32_t sector_count = 0;


  for(int i=0; i<FLASH_SECTOR_MAX; i++)
  {
    if(flashInSector(i, addr, length) == true)
    {
      if(start_sector_num < 0)
      {
        start_sector_num = i;   // sector start 주소
      }
      sector_count++;           // 지워야 할 flash 영역에 맞는 섹터 갯수를 구하기 위해
    }
  }

  if(sector_count > 0)
  {
    flashUnlock();

    for(int i=0; i<sector_count; i++)
    {
      status = HAL_CFMC_SelfErase (((start_sector_num+i) * FLASH_SECTOR_SIZE) , FLASH_SECTOR_SIZE);
    }


    if(status == HAL_OK)
    {
      ret = true;
    }

    flashLock();
  }

  logPrintf("start_sector_num : %d, sector_count : %ld\n",start_sector_num, sector_count);
  return ret;
}

bool flashWrite(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  int status;

#if 0
  if(addr%4 != 0)   // 4byte로 얼라인된 주소일때만 가능하도록
  {
    return false;
  }
#endif

  flashUnlock();

  for(int i=0; i<length; i+=1)
  {
    //uint32_t data;
    //data = p_data[i+0] << 0;   // p_data[i] == *(p_data+i);
    //data |= p_data[i+1] << 8;
    //data |= p_data[i+2] << 16;
    //data |= p_data[i+3] << 24;

    status = HAL_CFMC_SelfWrite (addr + i, FLASH_SECTOR_SIZE, p_data);

    if(status != HAL_OK)
    {
      ret = false;
      break;
    }
  }

  flashLock();
  return ret;
}

bool flashRead(uint32_t addr, uint8_t *p_data, uint32_t length)
{
  bool ret = true;

  uint8_t *p_byte = (uint8_t *)addr;      // 받은 addr은 정수값이므로 포인터에 대입하기 위해 타입전환을 했음 주소로값으로

  for(int i=0; i<length; i++)
  {
    p_data[i] = p_byte[i];                //p_byte[i]가 가지고 있는 값의 주소로 가서 값을 가지고 p_data[i]에 저장
  }

  return ret;
}

bool flashInSector(uint16_t sector_num, uint32_t addr, uint32_t length)
{
  bool ret = false;

  uint32_t sector_start;
  uint32_t sector_end;
  uint32_t flash_start;
  uint32_t flash_end;

  sector_start  = FLASH_SECTOR_ADDR + (sector_num * FLASH_SECTOR_SIZE);
  sector_end    = sector_start + FLASH_SECTOR_SIZE -1;
  flash_start   = addr;
  flash_end     = addr + length -1;

  if(sector_start >= flash_start && sector_start <= flash_end)
  {
    ret = true;
  }

  if(sector_end >= flash_start && sector_end <= flash_end)
  {
    ret = true;
  }

  if(flash_start >= sector_start && flash_start <= sector_end)
  {
    ret = true;
  }

  if(flash_end >= sector_start && flash_end <= sector_end)
  {
    ret = true;
  }

  return ret;
}

void flashUnlock(void)
{
  HAL_CFMC_FLASH_ALL_ENABLE();
}

void flashLock(void)
{

}




#ifdef _USE_HW_CLI
void cliFlash(cli_args_t *args)
{
  bool ret = false;

  if(args->argc == 1 && args->isStr(0,"info") == true)
  {
    for(int i=0; i<FLASH_SECTOR_MAX; i++)
    {
      cliPrintf("Sector num : %d - 0x%X : %dKB\n", i, flash_tbl[i].addr, flash_tbl[i].length/1024);
    }

    ret = true;
  }

  if(args->argc == 3 && args->isStr(0, "read") == true)
  {
    uint32_t addr;
    uint32_t length;

    addr = (uint32_t)args->getData(1);
    length = (uint32_t)args->getData(2);

    for(int i=0; i<length; i++)
    {
      cliPrintf("0x%X : 0x%X\n", addr+i, *((uint8_t *)(addr+i)));     //
    }

    ret = true;
  }

  if(args->argc == 3 && args->isStr(0, "erase") == true)
  {
    uint32_t addr;
    uint32_t length;

    addr = (uint32_t)args->getData(1);
    length = (uint32_t)args->getData(2);

    if(flashErase(addr, length) == true)
    {
      cliPrintf("Erase OK\n");
    }
    else
    {
      cliPrintf("Erase Fail\n");
    }

    ret = true;
  }

  if(args->argc == 3 && args->isStr(0, "write") == true)
  {
    uint32_t addr;
    uint32_t data;

    addr = (uint32_t)args->getData(1);
    data = (uint32_t)args->getData(2);

    if(flashWrite(addr, (uint8_t *)&data, 4) == true)
    {
      cliPrintf("Write OK\n");
    }
    else
    {
      cliPrintf("Write Fail\n");
    }

    ret = true;
  }

  if(args->argc ==3 && args->isStr(0, "write2") == true)
  {
    uint32_t addr;
    uint32_t data = 0x12345678;
    uint32_t length;

    addr = (uint32_t)args->getData(1);
    length = (uint32_t)args->getData(2);

    for(int i=0; i<length; i+=4)
    {
      flashWrite(addr+i, (uint8_t *)&data, 4);
    }
    cliPrintf("Write OK\n");

#if 0
    if(flashWrite(addr, (uint8_t *)&data, 4) == true)
    {
      cliPrintf("Write OK\n");
    }
    else
    {
      cliPrintf("Write Fail\n");
    }
#endif

    ret = true;
  }

  if(ret != true)
  {
    cliPrintf("flash info\n");
    cliPrintf("flash read addr length\n");
    cliPrintf("flash erase addr length\n");
    cliPrintf("flash write addr data\n");
    cliPrintf("flash write2 addr length\n");

  }


}
  #endif


#endif
