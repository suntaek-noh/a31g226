/*
 * qblock.c
 *
 *  Created on: 2022. 8. 11.
 *      Author: sunta
 */


#include "qblock.h"

void qblockInit(void)
{

}

bool qblockCreate(qblock_t *p_node, uint8_t *p_buf, uint32_t length)
{
  bool ret = true;

  p_node->in    = 0;
  p_node->out   = 0;
  p_node->len   = length;
  p_node->size  = 1;
  p_node->p_buf = p_buf;

  return ret;
}

bool qblockCreateBySize(qblock_t *p_node, uint8_t *p_buf, uint32_t size, uint32_t length)
{
  bool ret = true;

  p_node->in    = 0;
  p_node->out   = 0;
  p_node->len   = length;
  p_node->size  = size;
  p_node->p_buf = p_buf;

  return ret;
}

bool qblockWrite(qblock_t *p_node, uint8_t *p_data, uint32_t length)
{
  bool ret = true;
  uint32_t next_in;


  for (int i=0; i<length; i++)
  {
    next_in = (p_node->in + 1) % p_node->len;

    if (next_in != p_node->out)
    {
      if (p_node->p_buf != NULL && p_data != NULL)
      {
        uint8_t *p_buf;

        p_buf = &p_node->p_buf[p_node->in*p_node->size];
        for (int i=0; i<p_node->size; i++)
        {
          p_buf[i] = p_data[i];
        }
        p_data += p_node->size;
      }
      p_node->in = next_in;
    }
    else
    {
      ret = false;
      break;
    }
  }

  return ret;
}

bool qblockRead(qblock_t *p_node, uint8_t *p_data, uint32_t length)
{
  bool ret = true;


  for (int i=0; i<length; i++)
  {
    if (p_node->p_buf != NULL && p_data != NULL)
    {
      uint8_t *p_buf;

      p_buf = &p_node->p_buf[p_node->out*p_node->size];
      for (int i=0; i<p_node->size; i++)
      {
        p_data[i] = p_buf[i];
      }

      p_data += p_node->size;
    }

    if (p_node->out != p_node->in)
    {
      p_node->out = (p_node->out + 1) % p_node->len;
    }
    else
    {
      ret = false;
      break;
    }
  }

  return ret;
}

uint8_t *qblockPeekWrite(qblock_t *p_node)
{
  return &p_node->p_buf[p_node->in*p_node->size];
}

uint8_t *qblockPeekRead(qblock_t *p_node)
{
  return &p_node->p_buf[p_node->out*p_node->size];
}


uint32_t qblockAvailable(qblock_t *p_node)
{
  uint32_t ret;

  ret = (p_node->len + p_node->in - p_node->out) % p_node->len;

  return ret;
}

void qblockFlush(qblock_t *p_node)
{
  p_node->in  = 0;
  p_node->out = 0;
}
