/*
 * tbutton.c
 *
 *  Created on: 2022. 8. 23.
 *      Author: sunta
 */

#include "tbutton.h"
#include "touch/gt216l.h"
#include "gpio.h"
#include "cli.h"

#ifdef _USE_HW_TOUCH

#define TBUTTON_MAX_IC            HW_TBUTTON_MAX_IC
#define TBUTTON_MAX_TOUCH_IC1_CH  HW_MAX_TOUCH_IC1_CH
#define TBUTTON_MAX_TOUCH_IC2_CH  HW_MAX_TOUCH_IC2_CH


#ifdef _USE_HW_CLI
static void cliCmd(cli_args_t *);
#endif

static bool is_init = false;
static bool is_touched = false;

static tbutton_driver_t tbutton;

static uint8_t touch_data[2];
static uint16_t touch_value=0;

uint8_t touch1_int_pin = 11;
uint8_t touch2_int_pin = 12;


void tbuttonISR(void)
{
  if (is_init)
  {
    if(gpioPinRead(touch1_int_pin) == true)
    {
      if(tbuttonGetTouchedData(0, &touch_data[0]) != true)
      {
        is_init = false;
      }
      else
      {
        touch_value = (touch_data[1] << 8) | touch_data[0];
      }
      //logPrintf("touch_value = 0x%X\r\n", touch_value);
    }
    else if(gpioPinRead(touch2_int_pin) == true)
    {
      if(tbuttonGetTouchedData(1, &touch_data[0]) != true)
      {
        is_init = false;
      }
      else
      {
        touch_value = (touch_data[1] << 8) | touch_data[0];
      }
      //logPrintf("touch_value = 0x%X\r\n", touch_value);
    }
    else
    {
      touch_value =0;
    }
  }
  else
  {

  }
}

bool tbuttonInit(void)
{
  #ifdef _USE_HW_GT216L
  gt216lInitDriver(&tbutton);
  #endif

  #ifdef _USE_HW_GT316L
  gt316lInitDriver(&tbutton);
  #endif

  //if (is_init)
  {
    if (tbutton.init() == true)
    {
      is_init = true;
    }
  }

#ifdef _USE_HW_CLI
  cliAdd("tbutton", cliCmd);
#endif
  return is_init;
}

uint8_t tbuttonGetTouchStatus(uint8_t ic)
{
  if (is_init)
  {
    return tbutton.getTocuhedStatus(ic);
  }
  else
  {
    return 0;
  }
}

bool tbuttonGetTouchedData(uint8_t ic, uint8_t *p_data)
{
  if (is_init)
  {
    return tbutton.getTouchedData(ic, p_data);
  }
  else
  {
    return false;
  }
}

bool tbuttonIoOut(uint8_t ch, uint8_t value)
{
  if (is_init)
  {
    return tbutton.setTouchedData(ch, value);
  }
  else
  {
    return false;
  }
}

#if HW_TOUCH_OBJ_USE == 1
enum TbuttonObjState
{
  TOUCH_OBJ_WAIT_FOR_RELEASED,
  TOUCH_OBJ_WAIT_FOR_PRESSED,
  TOUCH_OBJ_PRESSED,
  TOUCH_OBJ_REPEATED_START,
  TOUCH_OBJ_REPEATED,
};

void tbuttonObjCreate(tbutton_obj_t *p_obj, uint8_t ic, uint8_t ch, uint32_t pressed_time, uint32_t repeat_start_time, uint32_t repeat_pressed_time)
{
  p_obj->ic                   = ic;
  p_obj->ch                   = ch;
  p_obj->state                = 0;
  p_obj->pre_time             = millis();
  p_obj->pressed_time         = pressed_time;
  p_obj->repeat_start_time    = repeat_start_time;
  p_obj->repeat_pressed_time  = repeat_pressed_time;
  p_obj->event_flag           = 0;
  p_obj->state_flag           = 0;
  p_obj->touch_count          = 0;
}

#if 1
uint16_t tbuttonGetValue(void)
{
  return touch_value;
}

#endif

bool tbuttonObjUpdate(tbutton_obj_t *p_obj)
{
  bool ret = false;

  switch(p_obj->state)
  {
    case TOUCH_OBJ_WAIT_FOR_RELEASED:
      if(touch_value == 0)
      {
        p_obj->state = TOUCH_OBJ_WAIT_FOR_PRESSED;
      }
      break;

    case TOUCH_OBJ_WAIT_FOR_PRESSED:
      if(touch_value & (1 << p_obj->ch))
      {
        p_obj->state = TOUCH_OBJ_PRESSED;
        p_obj->pre_time = millis();
        p_obj->touch_count=0;
      }
      break;

    case TOUCH_OBJ_PRESSED:
      if(touch_value & (1 << p_obj->ch))
      {
        if(millis() - p_obj->pre_time >= p_obj->pressed_time)
        {
          ret = true;
          p_obj->state = TOUCH_OBJ_REPEATED_START;
          p_obj->pre_time = millis();
          p_obj->event_flag |= TOUCH_EVT_CLICKED;

          p_obj->state_flag |= TOUCH_STATE_PRESSED;
          p_obj->touch_count++;
        }
      }
      else
      {
        p_obj->state = TOUCH_OBJ_WAIT_FOR_PRESSED;
        if (p_obj->state_flag & TOUCH_STATE_PRESSED)
        {
          p_obj->event_flag |= TOUCH_EVT_RELEASED;
          p_obj->state_flag |= TOUCH_STATE_RELEASED;

          p_obj->state_flag &= ~TOUCH_STATE_PRESSED;
          p_obj->state_flag &= ~TOUCH_STATE_REPEATED;
        }
      }
      break;

    case TOUCH_OBJ_REPEATED_START:
      if(touch_value & (1 << p_obj->ch))
      {
        if (millis()-p_obj->pre_time >= p_obj->repeat_start_time)
        {
          ret = true;
          p_obj->pre_time = millis();


          p_obj->state = TOUCH_OBJ_REPEATED;

          p_obj->event_flag |= TOUCH_EVT_CLICKED;
          p_obj->event_flag |= TOUCH_EVT_REPEATED;

          p_obj->state_flag |= TOUCH_STATE_REPEATED;
          p_obj->touch_count++;
        }
      }
      else
      {
        p_obj->state = TOUCH_OBJ_PRESSED;
        p_obj->pre_time = millis();
      }
      break;

    case TOUCH_OBJ_REPEATED:
      if(touch_value & (1 << p_obj->ch))
      {
        if (millis()-p_obj->pre_time >= p_obj->repeat_pressed_time)
        {
          ret = true;
          p_obj->pre_time = millis();

          p_obj->event_flag |= TOUCH_EVT_CLICKED;
          p_obj->event_flag |= TOUCH_EVT_REPEATED;

          p_obj->state_flag |= TOUCH_STATE_REPEATED;
          p_obj->touch_count++;
        }
      }
      else
      {
        p_obj->state = TOUCH_OBJ_PRESSED;
        p_obj->pre_time = millis();
      }
      break;
  }
  return ret;
}

uint8_t tbuttonObjGetEvent(tbutton_obj_t *p_obj)
{
  return p_obj->event_flag;
}

void tbuttonObjClearEvent(tbutton_obj_t *p_obj)
{
  p_obj->event_flag = 0;
}

uint8_t tbuttonObjGetState(tbutton_obj_t *p_obj)
{
  return p_obj->state_flag;
}

#endif


void tbutton1ISR(void)
{
  is_touched = true;
  logPrintf("exti portA pin2 falling edge\r\n");
}

void tbutton2ISR(void)
{
  logPrintf("exti portC pin12 falling edge\r\n");
}



#ifdef _USE_HW_CLI
void cliCmd(cli_args_t * args)
{
#if 1
  bool ret = true;

  if (args->argc == 1  && args->isStr(0, "info") == true)
  {
    //uint8_t touch_status;
    //uint8_t touch_data[2];
    //uint16_t touch_value;

    while(cliKeepLoop())
    {
      //touch_status = tbuttonGetTouchStatus(0);
      //if ( touch_status & (1<<0))
      {
        //cliPrintf("Touch status : %d, ", touch_status);

        //tbuttonGetTouchedData(0, touch_data);
        //touch_value = (touch_data[1] << 8) | touch_data[0];
        cliPrintf("tbutton touch_value = 0x%X\r\n", touch_value);
      }

      delay(20);
    }
  }
  else
  {
    ret = false;
  }

#if HW_TOUCH_OBJ_USE == 1
  if (args->argc == 3 && args->isStr(0, "event"))
  {
    uint8_t tbutton_ic;
    uint8_t tbutton_ch;
    tbutton_obj_t tbutton_sw;
    uint8_t tbutton_event;

    tbutton_ic = constrain(args->getData(1), 0, TBUTTON_MAX_IC-1);
    tbutton_ch = constrain(args->getData(2), 0, 15);
    tbuttonObjCreate(&tbutton_sw, tbutton_ic, tbutton_ch, 50, 1000, 100);

    while(cliKeepLoop())
    {
      tbuttonObjUpdate(&tbutton_sw);

      tbutton_event = tbuttonObjGetEvent(&tbutton_sw);

     if (tbutton_event > 0)
     {
       if (tbutton_event & TOUCH_EVT_PRESSED)
         cliPrintf("tbutton pressed\n");
       if (tbutton_event & TOUCH_EVT_CLICKED)
         cliPrintf("tbutton clicked %d\n", tbutton_sw.touch_count);
       if (tbutton_event & TOUCH_EVT_RELEASED)
         cliPrintf("tbutton released\n");

       tbuttonObjClearEvent(&tbutton_sw);
     }

     delay(5);
   }

   ret = true;
  }

#endif


  if (ret == false)
  {
    cliPrintf( "tbutton info \n");
    #if HW_TOUCH_OBJ_USE == 1
    cliPrintf("tbutton event ic: 0~%d, ch: 0~%d\n", TBUTTON_MAX_IC-1, 15);
    #endif
  }

#endif
}
#endif

#endif  //_USE_HW_TOUCH
