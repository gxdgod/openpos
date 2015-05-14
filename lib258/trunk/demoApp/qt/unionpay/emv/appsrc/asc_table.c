#if 0
#include <string.h>
#include "asc_table.h"
#include "asc_table_config.h"
#ifdef __cplusplus
extern "C"
{
#endif
	unsigned char Os__graph_display
		(
		unsigned char,
		unsigned char,
		unsigned char * pt_msg,
		unsigned char LEN
		)reentrant;
#ifdef __cplusplus
}
#endif

static const unsigned char sg_aucAscFont[][ASC_SIZE] =
{
// space
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
//! " # $ % & ' ( ) * + , - . /
	{0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x00,0x00,0x20,0x00},
	{0x28,0x00,0x50,0x00,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x28,0x00,0x28,0x00,0xFC,0x00,0x50,0x00,0xFC,0x00,0x50,0x00,0x50,0x00},
	{0x10,0x00,0x38,0x00,0x58,0x00,0x50,0x00,0x30,0x00,0x58,0x00,0x78,0x00,0x10,0x00},
	{0x00,0x00,0x64,0x00,0x68,0x00,0x68,0x00,0x7C,0x00,0x1C,0x00,0x1C,0x00,0x2C,0x00},
	{0x00,0x00,0x30,0x00,0x30,0x00,0x3C,0x00,0x28,0x00,0x58,0x00,0x50,0x00,0x3C,0x00},
	{0x10,0x00,0x10,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x08,0x00,0x10,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x10,0x00,0x08,0x00},
	{0x40,0x00,0x20,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x20,0x00,0x40,0x00},
	{0x00,0x00,0x10,0x00,0x54,0x00,0x38,0x00,0x38,0x00,0x54,0x00,0x10,0x00,0x00,0x00},
	{0x00,0x00,0x10,0x00,0x10,0x00,0x7C,0x00,0x10,0x00,0x10,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x10,0x00,0x20,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00},
	{0x08,0x00,0x08,0x00,0x10,0x00,0x10,0x00,0x20,0x00,0x20,0x00,0x40,0x00,0x40,0x00},
//0-9
	{0x30,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x30,0x00,0x00,0x00},
	{0x20,0x00,0x60,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x70,0x00,0x00,0x00},
	{0x30,0x00,0x48,0x00,0x08,0x00,0x10,0x00,0x20,0x00,0x40,0x00,0x78,0x00,0x00,0x00},
	{0x30,0x00,0x48,0x00,0x08,0x00,0x30,0x00,0x08,0x00,0x48,0x00,0x30,0x00,0x00,0x00},
	{0x08,0x00,0x18,0x00,0x28,0x00,0x48,0x00,0x7C,0x00,0x08,0x00,0x08,0x00,0x00,0x00},
	{0x78,0x00,0x40,0x00,0x70,0x00,0x08,0x00,0x08,0x00,0x48,0x00,0x30,0x00,0x00,0x00},
	{0x38,0x00,0x40,0x00,0x40,0x00,0x78,0x00,0x44,0x00,0x44,0x00,0x38,0x00,0x00,0x00},
	{0x3C,0x00,0x08,0x00,0x08,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x00,0x00},
	{0x30,0x00,0x48,0x00,0x48,0x00,0x30,0x00,0x48,0x00,0x48,0x00,0x30,0x00,0x00,0x00},
	{0x30,0x00,0x48,0x00,0x48,0x00,0x38,0x00,0x08,0x00,0x08,0x00,0x30,0x00,0x00,0x00},

//: ; < = > ? @
	{0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x40,0x00},
	{0x00,0x00,0x08,0x00,0x10,0x00,0x20,0x00,0x40,0x00,0x20,0x00,0x10,0x00,0x08,0x00},
	{0x00,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0x00,0x00,0x7C,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x40,0x00,0x20,0x00,0x10,0x00,0x08,0x00,0x10,0x00,0x20,0x00,0x40,0x00},
	{0x30,0x00,0x48,0x00,0x48,0x00,0x10,0x00,0x20,0x00,0x20,0x00,0x00,0x00,0x20,0x00},
	{0x38,0x00,0x44,0x00,0x9C,0x00,0xAC,0x00,0xAC,0x00,0x98,0x00,0x40,0x00,0x38,0x00},

//A-Z
	{0x10,0x00,0x10,0x00,0x28,0x00,0x28,0x00,0x38,0x00,0x28,0x00,0x6C,0x00,0x00,0x00},
	{0x78,0x00,0x24,0x00,0x38,0x00,0x24,0x00,0x24,0x00,0x24,0x00,0x78,0x00,0x00,0x00},
	{0x3C,0x00,0x44,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x44,0x00,0x38,0x00,0x00,0x00},
	{0x78,0x00,0x24,0x00,0x24,0x00,0x24,0x00,0x24,0x00,0x24,0x00,0x78,0x00,0x00,0x00},
	{0x7C,0x00,0x24,0x00,0x20,0x00,0x38,0x00,0x20,0x00,0x24,0x00,0x7C,0x00,0x00,0x00},
	{0x7C,0x00,0x24,0x00,0x28,0x00,0x38,0x00,0x28,0x00,0x20,0x00,0x70,0x00,0x00,0x00},
	{0x38,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x5C,0x00,0x48,0x00,0x30,0x00,0x00,0x00},
	{0xEC,0x00,0x48,0x00,0x48,0x00,0x78,0x00,0x48,0x00,0x48,0x00,0xEC,0x00,0x00,0x00},
	{0x38,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x38,0x00,0x00,0x00},
	{0x38,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x60,0x00},
	{0x74,0x00,0x28,0x00,0x30,0x00,0x30,0x00,0x28,0x00,0x28,0x00,0x6C,0x00,0x00,0x00},
	{0x70,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x24,0x00,0x7C,0x00,0x00,0x00},
	{0xCC,0x00,0x78,0x00,0x78,0x00,0x78,0x00,0x68,0x00,0x48,0x00,0xCC,0x00,0x00,0x00},
	{0xDC,0x00,0x48,0x00,0x68,0x00,0x68,0x00,0x58,0x00,0x48,0x00,0xE8,0x00,0x00,0x00},
	{0x38,0x00,0x44,0x00,0x44,0x00,0x44,0x00,0x44,0x00,0x44,0x00,0x38,0x00,0x00,0x00},
	{0x78,0x00,0x24,0x00,0x24,0x00,0x38,0x00,0x20,0x00,0x20,0x00,0x70,0x00,0x00,0x00},
	{0x38,0x00,0x44,0x00,0x44,0x00,0x44,0x00,0x74,0x00,0x4C,0x00,0x38,0x00,0x0C,0x00},
	{0xF0,0x00,0x48,0x00,0x70,0x00,0x50,0x00,0x48,0x00,0x48,0x00,0xEC,0x00,0x00,0x00},
	{0x38,0x00,0x48,0x00,0x40,0x00,0x30,0x00,0x08,0x00,0x48,0x00,0x70,0x00,0x00,0x00},
	{0x7C,0x00,0x54,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x38,0x00,0x00,0x00},
	{0xCC,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x30,0x00,0x00,0x00},
	{0x6C,0x00,0x28,0x00,0x28,0x00,0x28,0x00,0x28,0x00,0x10,0x00,0x10,0x00,0x00,0x00},
	{0xA4,0x00,0xA4,0x00,0xA4,0x00,0x58,0x00,0x58,0x00,0x48,0x00,0x48,0x00,0x00,0x00},
	{0x6C,0x00,0x28,0x00,0x28,0x00,0x10,0x00,0x28,0x00,0x28,0x00,0x6C,0x00,0x00,0x00},
	{0x6C,0x00,0x28,0x00,0x28,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x38,0x00,0x00,0x00},
	{0x7C,0x00,0x48,0x00,0x10,0x00,0x10,0x00,0x20,0x00,0x24,0x00,0x7C,0x00,0x00,0x00},

// [ \ ] ^ `
	{0x70,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x40,0x00,0x70,0x00},
	{0x40,0x00,0x40,0x00,0x20,0x00,0x20,0x00,0x10,0x00,0x10,0x00,0x08,0x00,0x08,0x00},
	{0x70,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x70,0x00},
	{0x20,0x00,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x00},
	{0x20,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},

//a-z
	{0x00,0x00,0x00,0x00,0x30,0x00,0x48,0x00,0x38,0x00,0x48,0x00,0x3C,0x00,0x00,0x00},
	{0x60,0x00,0x20,0x00,0x38,0x00,0x24,0x00,0x24,0x00,0x24,0x00,0x38,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x38,0x00,0x48,0x00,0x40,0x00,0x40,0x00,0x38,0x00,0x00,0x00},
	{0x18,0x00,0x08,0x00,0x38,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x3C,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x30,0x00,0x48,0x00,0x78,0x00,0x40,0x00,0x38,0x00,0x00,0x00},
	{0x18,0x00,0x20,0x00,0x78,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x70,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x78,0x00,0x50,0x00,0x70,0x00,0x40,0x00,0x78,0x00,0x78,0x00},
	{0xC0,0x00,0x40,0x00,0x70,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0xEC,0x00,0x00,0x00},
	{0x10,0x00,0x00,0x00,0x30,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x38,0x00,0x00,0x00},
	{0x08,0x00,0x00,0x00,0x18,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x38,0x00},
	{0x60,0x00,0x20,0x00,0x2C,0x00,0x28,0x00,0x30,0x00,0x28,0x00,0x6C,0x00,0x00,0x00},
	{0x30,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x38,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x78,0x00,0x54,0x00,0x54,0x00,0x54,0x00,0x54,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x78,0x00,0x24,0x00,0x24,0x00,0x24,0x00,0x76,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x38,0x00,0x44,0x00,0x44,0x00,0x44,0x00,0x38,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x78,0x00,0x24,0x00,0x24,0x00,0x24,0x00,0x38,0x00,0x70,0x00},
	{0x00,0x00,0x00,0x00,0x38,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x38,0x00,0x1C,0x00},
	{0x00,0x00,0x00,0x00,0x68,0x00,0x30,0x00,0x20,0x00,0x20,0x00,0x70,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x78,0x00,0x40,0x00,0x30,0x00,0x08,0x00,0x78,0x00,0x00,0x00},
	{0x20,0x00,0x20,0x00,0x70,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x10,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0xD8,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x3C,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x6C,0x00,0x28,0x00,0x28,0x00,0x28,0x00,0x10,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x54,0x00,0x54,0x00,0x54,0x00,0x6C,0x00,0x28,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x7C,0x00,0x28,0x00,0x10,0x00,0x28,0x00,0x7C,0x00,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x7C,0x00,0x28,0x00,0x10,0x00,0x10,0x00,0x20,0x00,0x60,0x00},
	{0x00,0x00,0x00,0x00,0x78,0x00,0x10,0x00,0x10,0x00,0x20,0x00,0x78,0x00,0x00,0x00},
//{|}~
	{0x10,0x00,0x20,0x00,0x20,0x00,0x40,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x10,0x00},
	{0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00},
	{0x40,0x00,0x20,0x00,0x20,0x00,0x10,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x40,0x00},
	{0x40,0x00,0xA8,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};

void ASC_GetAscFont(const unsigned char a_ucAsc, unsigned char *a_aucFont)
{
	if(a_ucAsc >= ' ' && a_ucAsc <= '~')
	{
		memcpy(a_aucFont, sg_aucAscFont[a_ucAsc - ' '], ASC_SIZE);
	}
	else
	{
		memcpy(a_aucFont, sg_aucAscFont[0], ASC_SIZE);
	}
}

void ASC_DrawAscString(int a_row, int a_column, int fontColSize,const unsigned char *a_aucString)
{
	int index;
	unsigned char aucAscFont[ASC_SIZE + 1];
	for(index = a_column; index < 128; index += ASC_SIZE)
	{
		Os__graph_display
			(
			a_row,
			index,
			(unsigned char *)sg_aucAscFont[0],
			ASC_SIZE
			);
	}

	for(index = 0; a_aucString[index] != 0; ++index)
	{
		ASC_GetAscFont(a_aucString[index], aucAscFont);
		Os__graph_display
			(
			a_row,
			index * fontColSize + a_column,
			aucAscFont,
			ASC_SIZE
			);
	}
}

#endif