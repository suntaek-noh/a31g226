#include <stdint.h>
#include "debug_frmwrk.h"

char	var_a;
char	var_b;
int32_t	msec;
int32_t	count;

int	clear_variables(int32_t arg)
{
	cprintf("a = %d, b = %d\r\nmsec = %d, count = %d\r\n", var_a, var_b, msec, count);

	// clear variables
	var_a = 0;
	var_b = 0;
	msec = 0;
	count = 0;

	return 0;
}

DEBUG_VARIABLES dbg_variables[] = {
	{	1,	"a", (void*)&var_a,	0	},
	{	1,	"b", (void*)&var_a,	0	},
	{	4,	"msec", (void*)&msec,	0	},
	{	4,	"count", (void*)&count,	0	},
	{	0,	"clear", (void*)clear_variables, 0	},
	{	-1,	"",		(void*)NULL,	0	}
};

