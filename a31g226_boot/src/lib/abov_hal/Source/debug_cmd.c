#include <stdint.h>
#include "debug_cmd.h"
#include "debug_frmwrk.h"

//int	xmodemTransmit(uint32_t start,uint32_t size);
//int	xmodemReceive(uint32_t start,uint32_t size);
//int	flash_run(char *cmd);

static	uint32_t	dump_addr=0;

//########################################################
#define	MAX_INDATA_SIZE		80
#define	TERMINATE_CHAR		'\r'

char	InData[80];
int		InFlag;
int		InCount;

void	init_slib(void)
{
	InData[0] = 0;
	InFlag = 0;
	InCount = 0;
}

int	getstring(void)
{
	int	ch;

	ch = cgetchar();	//cgetq();
	if (ch > 0) {
		if (InCount < 80) {
			if (InCount == 0 && ch < 0x20) {
				InData[0] = 0;
				return ch;
			}
			cputc(ch);
			if (ch == 8) {
				InCount--;
				return ch;
			}
			if (ch != '\r')
				InData[InCount++] = ch;
		}
		if (ch == TERMINATE_CHAR) {
			InData[InCount] = 0;
			InFlag = 1;
			return ch;
		}
	}
	return 0;
}

char	*scani(char *s, uint32_t *result)
{
	uint32_t n=0;
	int i;

	*result=0;
	if (s == NULL)
		return NULL;

	while (*s != 0 && *s == ' ') s++;
	if (*s == 0) {
		return NULL;
	} else {
		while ((i = *s) != 0) {
			s++;
			if (i == ',' || i == ' ')
				break;
	
			if (i >= '0' && i <= '9')
				i -= '0';
			else if (i >= 'A' && i <= 'F')
				i -= 'A' - 10;
			else if (i >= 'a' && i <= 'f')
				i -= 'a' - 10;
			else {
				return NULL;
			}
			n = (n << 4) + i;
		}
	}
	*result = n;
	return s;
}

char	*scand(char *s,uint32_t *result)
{
	uint32_t	n = 0;
	int i;

	*result = 0;
	if (s == NULL)
		return NULL;

	while (*s != 0 && *s == ' ') s++;
	if (*s == 0) {
		return NULL;
	} else {
		while ((i = *s) != 0) {
			s++;
			if (i == ',' || i == ' ')
				break;

			if (i >= '0' && i <= '9')
				i -= '0';
			else {
				return NULL;
			}
			n = (n * 10) + i;
		}
	}
	*result = n;
	return s;
}

char	*scans(char *s,char *result)
{
	int32_t i;
	
	while (*s != 0 && *s == ' ') s++;
	while ((i = *s) != 0) {
		s++;
		if (i == ' ' || i == ',')
			break;
		*result++ = i;
	}
	*result = 0;
	return s;
}

//########################################################

void	cmd_init(void)
{
	dump_addr = 0;
}

int	cmd_dump(char *cmd)
{
	uint32_t	sta,eda;
	char	*p;
	int	j;
	char	ch;

//	sscanf(cmd+1,"%x,%x",&sta,&eda);
	p = cmd + 1;
	p = scani(p, &sta);
	p = scani(p, &eda);
	if (cmd[1] == 0 || cmd[1] == '\r' || cmd[1] == '\n')
		sta = dump_addr;

	if (eda == 0)
		eda = sta + 0x100;
	dump_addr = eda;			
	while (sta < eda) {
		cprintf("%08X: ", sta);
		for (j = 0; j < 0x10; j += 4) {
			cprintf("%08X ", *(volatile uint32_t*)(sta + j));
		}
		cputs(":");
		for (j = 0; j < 0x10; j++) {
			ch = *(volatile char*)(sta + j);
			if (ch < 0x20 || ch >= 0x80)
				ch = '.';
			cputc(ch);
		}
		cputs("\r\n");
		sta += 0x10;
	}
	return 1;
}

int	cmd_edit(char *cmd)
{
	int	i;
	int	flag;
	int	t;
	uint32_t	sta,eda;
	char	*p;
	char	ch;

	i = 1;
	flag = 1;
	t = 4;
	if (cmd[1] == 'h' || cmd[1] == 'H') {
		p = cmd + 2;
		p = scani(p, &sta);
		t = 2;
	} else if (cmd[1] == 's' || cmd[1] == 'S') {
		if (cmd[2] == 'k' || cmd[2] == 'K') {
			i = 0;
			p = cmd + 3;
			p = scani(p, &sta);
			t = 1;
		} else {
			p = cmd + 2;
			p = scani(p, &sta);
			t = 1;
		}
	}	else {
		p = cmd + 1;
		p = scani(p, &sta);
	}
	if (*p != 0) {
		p = scani(p, &eda);
		switch(t) {
		case 1:	*(volatile uint8_t*)sta = eda;break;
		case 2:	*(volatile uint16_t*)sta = eda;break;
		case 4:	*(volatile uint32_t*)sta = eda;break;
		}
	}
	else
	while(flag) {
		cprintf("%08X: ", sta);
		if(i) {
			switch(t) {
			case 1:	cprintf("%02X ", (*(volatile uint8_t*)sta));break;
			case 2:	cprintf("%04X ", (*(volatile uint16_t*)sta));break;
			case 4:	cprintf("%08X ", (*(volatile uint32_t*)sta));break;
			}
		}

		init_slib();
		do {
			ch = getstring();
			switch(ch) {
			case 0:	if (InCount > 0 && InData[InCount - 1] == '.')
							flag = 0;
							break;
			case 0x08:	
							sta -= t;
							break;
			case 0x0d:
							if (InCount != 0) {
								p = InData;
								p = scani(p,&eda);
								switch(t) {
								case 1:	*(volatile uint8_t*)sta = eda;break;
								case 2:	*(volatile uint16_t*)sta = eda;break;
								case 4:	*(volatile uint32_t*)sta = eda;break;
								}
							}
							sta += t;
							break;
			}
		} while(ch==0 && flag!=0);
		cputs("\r\n");
	}
	return 1;
}

int	cmd_read(char *cmd)
{
	int		t;
	uint32_t	sta,result;
	char	*p;

	t = 4;
	if (cmd[1] == 'h' || cmd[1] == 'H') {
		p = cmd + 2;
		p = scani(p, &sta);
		t = 2;
	} else if (cmd[1] == 's' || cmd[1] == 'S') {
		if (cmd[2] == 'k' || cmd[2] == 'K') {
			p = cmd + 3;
			p = scani(p, &sta);
			t = 1;
		}	else {
			p = cmd + 2;
			p = scani(p, &sta);
			t = 1;
		}
	} else {
		p = cmd + 1;
		p = scani(p, &sta);
	}
	result = 0xcccccccc;
	switch(t) {
	case 1:	result = *(volatile uint8_t*)sta;break;
	case 2:	result = *(volatile uint16_t*)sta;break;
	case 4:	result = *(volatile uint32_t*)sta;break;
	}
	return result;
}

int	cmd_fill(char *cmd)
{
	int	i;
	uint32_t	sta,eda,tga;
	char	*p;

//	sscanf(cmd+1,"%x,%x,%x",&sta,&eda,&tga);
	p = cmd + 1;
	p = scani(p, &sta);
	if (p == NULL) return -1;
	p = scani(p, &eda);
	if (p == NULL) return -1;
	p = scani(p, &tga);
	if (p == NULL) return -1;

	cputs("Fill memory ");
	for (i = 0; i <= eda - sta; i += 4)
		*(volatile uint32_t*)(sta + i) = tga;
	for (i = 0; i <= eda - sta; i += 4) {
		if (*(volatile uint32_t*)(sta + i) != tga) {
			cprintf(" :    Error at %08X -   %08X -   \r\n", sta + i, *(volatile uint32_t*)(sta + i));
			return 0;
		}
	}
	cputs("O.K.\r\n");
	return 1;
}

int	cmd_compare(char *cmd)
{
	int	i;
	uint32_t	sta,eda,tga;
	char	*p;

//	sscanf(cmd+1,"%x,%x,%x",&sta,&eda,&tga);
	p = cmd + 1;
	p = scani(p, &sta);
	if (p == NULL) return -1;
	p = scani(p, &eda);
	if (p == NULL) return -1;
	p = scani(p, &tga);
	if (p == NULL) return -1;

	cputs("Compare memory ");
	for (i = 0; i <= eda - sta; i += 4) {
		if (*(volatile uint32_t*)(tga + i) != *(uint32_t*)(sta + i)) {
			break;
		}
	}
	if (i < eda - sta) {
		cprintf(" :    Error at %08X -   $08X\r\n",*(volatile uint32_t*)(tga + i), *(uint32_t*)(sta + i));
		return 0;
	}
	cputs("O.K.\r\n");
	return 1;
}

int	cmd_move(char *cmd)
{
	int	i;
	uint32_t	sta,eda,tga;
	char	*p;

//	sscanf(cmd+1,"%x,%x,%x",&sta,&eda,&tga);
	p = cmd+1;
	p = scani(p,&sta);
	if (p == NULL) return -1;
	p = scani(p,&eda);
	if (p == NULL) return -1;
	p = scani(p,&tga);
	if (p == NULL) return -1;

	cputs("Move memory ");
	for (i = 0; i < eda - sta; i += 4)
		*(volatile uint32_t*)(tga+i) = *(volatile uint32_t*)(sta + i);
	cputs("O.K.\r\n");
	return cmd_compare(cmd);
}

int	cmd_execute(char *cmd)
{
	uint32_t	sta;
	char	*p;

//	sscanf(cmd+1,"%x",&sta);
	p = cmd + 1;
	p = scani(p, &sta);
	if (p == NULL) return -1;
	__disable_irq();
	return ((int (*)(void))sta)();
}

uint32_t	ctol(uint8_t *sn)
{
	int		i;
	uint32_t	n;
	uint8_t	ch;
	for (i = n = 0; i < 8; i++) {
		ch = sn[i] - '0';
		if (ch > 9)
			ch -= 7;
		if (ch > 15)
			ch = 0;
		n = (n << 4) + ch;
	}
	return n;
}

int	cmd_run(char *InData)
{
	int	result = 0;
	
	switch(InData[0]) {
	case 'd':
	case 'D':
		cmd_dump(InData);
		break;
	case 'e':
	case 'E':
		cmd_edit(InData);
		break;
	case 'r':
	case 'R':
		cmd_read(InData);
	case 'f':
	case 'F':
		cmd_fill(InData);
		break;
	case 'm':
	case 'M':
		result=cmd_move(InData);
		break;
	case 'c':
	case 'C':
		result=cmd_compare(InData);
		break;
	case 'h':
	case 'H':
		break;
	case 'g':
	case 'G':
		result=cmd_execute(InData);
		break;
	}
	return result;
}

