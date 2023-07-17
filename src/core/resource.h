#pragma once

// 位图 BITMAP
typedef struct struct_bitmap_info
{
	unsigned short width; // 宽度
	unsigned short height; // 高度
	unsigned short color_bits;//仅支持16位 ，support 16 bits only
	const unsigned short* pixel_color_array; // 位图数据
} BITMAP_INFO; // 位图信息

// 字体 FONT
typedef struct struct_lattice
{
	unsigned int			utf8_code; // UTF8编码
	unsigned char			width; // 宽度
	const unsigned char*	pixel_buffer; // 像素缓冲
} LATTICE; // 格栅

typedef struct struct_lattice_font_info
{
	unsigned char	height; // 高度
	unsigned int	count; // 总数
	LATTICE*		lattice_array; // 格栅数组
} LATTICE_FONT_INFO; // 字体格栅信息
