/*
 * touch.c
 *
 *  Created on: 2022. 8. 18.
 *      Author: sunta
 */

#include "touch.h"
#include "touch/gt216l.h"

#include "cli.h"

#ifdef _USE_HW_CLI
static void cliCmd(cli_args_t *);
#endif

static bool is_init = false;
static touch_driver_t touch;


bool touchInit(void)
{
  #ifdef _USE_HW_GT216L
  is_init = gt216lInitDriver(&touch);
  #endif

  #ifdef _USE_HW_GT316L
  is_init = gt316lInitDriver(&touch);
  #endif

  if (is_init)
  {
    if (touch.init() != true)
    {
      is_init = false;
    }
  }

#ifdef _USE_HW_CLI
  cliAdd("touch", cliCmd);
#endif
  return is_init;
}

bool touchGetTouchedData(uint16_t *p_data)
{
  if (is_init)
  {
    return touch.getTouchedData(p_data);
  }
  else
  {
    return false;
  }
}

bool touchIoOut(uint8_t ch, uint8_t value)
{
  if (is_init)
  {
    return touch.setTouchedData(ch, value);
  }
  else
  {
    return false;
  }
}





#ifdef _USE_HW_CLI
void cliCmd(cli_args_t * args)
{
#if 0

  bool ret = true;

  if (args->argc == 1  && args->isStr(0, "info") == true)
  {
    uint8_t touch_cnt;
    touch_data_t touch_data;

    while(cliKeepLoop())
    {
#if 1
      touch_cnt = touchGetTouchedCount();
      if ( touch_cnt> 0)
      {
        cliPrintf("Touch : %d, ", touch_cnt);

        for (int i=0; i<touch_cnt; i++)
        {
          touchGetTouchedData(i, &touch_data);
          cliPrintf("[%d, 0x%x, x=%03d y=%03d w=%03d] ", touch_data.id, touch_data.event, touch_data.x, touch_data.y, touch_data.w);
        }
        cliPrintf("\r\n");
      }
#endif

      delay(20);
    }
  }
  else
  {
    ret = false;
  }


  if (ret == false)
  {
    cliPrintf( "touch info \n");
  }

#endif
}
#endif

