#pragma once

#include "../core/api.h"
#include "../core/resource.h"

//Rebuild gui library once you change this file
// 修改此文件后需要重新生成GUI库

// 字体列表
enum FONT_LIST
{
	FONT_NULL, // 空字体
	FONT_DEFAULT, // 默认字体
	FONT_CUSTOM1, // 自定义字体1
	FONT_CUSTOM2, // 自定义字体2
	FONT_CUSTOM3, // 自定义字体3
	FONT_CUSTOM4, // 自定义字体4
	FONT_CUSTOM5, // 自定义字体5
	FONT_CUSTOM6, // 自定义字体6

	FONT_MAX // 最大字体
};

// 图形列表
enum IMAGE_LIST
{
	IMAGE_CUSTOM1, // 自定义图形1
	IMAGE_CUSTOM2, // 自定义图形2
	IMAGE_CUSTOM3, // 自定义图形3
	IMAGE_CUSTOM4, // 自定义图形4
	IMAGE_CUSTOM5, // 自定义图形5
	IMAGE_CUSTOM6, // 自定义图形6

	IMAGE_MAX // 最大图形
};

// 颜色列表
enum COLOR_LIST
{
	COLOR_WND_FONT, // 窗体字体
	COLOR_WND_NORMAL, // 标准窗体
	COLOR_WND_PUSHED, // 窗体忙碌
	COLOR_WND_FOCUS, // 窗体焦点
	COLOR_WND_BORDER, // 窗体边界

	COLOR_CUSTOME1, // 自定义1
	COLOR_CUSTOME2, // 自定义2
	COLOR_CUSTOME3, // 自定义3
	COLOR_CUSTOME4, // 自定义4
	COLOR_CUSTOME5, // 自定义5
	COLOR_CUSTOME6, // 自定义6

	COLOR_MAX // 最大颜色
};

// 主题
class c_theme
{
public:
	// 添加字体
	static int add_font(FONT_LIST index, const void* font)
	{
		if (index >= FONT_MAX)
		{
			ASSERT(false);
			return -1;
		}
		s_font_map[index] = font;
		return 0;
	}

	// 获取字体
	static const void* get_font(FONT_LIST index)
	{
		if (index >= FONT_MAX)
		{
			ASSERT(false);
			return 0;
		}
		return s_font_map[index];
	}

	// 添加图形
	static int add_image(IMAGE_LIST index, const void* image_info)
	{
		if (index >= IMAGE_MAX)
		{
			ASSERT(false);
			return -1;
		}
		s_image_map[index] = image_info;
		return 0;
	}

	// 获取图形
	static const void* get_image(IMAGE_LIST index)
	{
		if (index >= IMAGE_MAX)
		{
			ASSERT(false);
			return 0;
		}
		return s_image_map[index];
	}
	
	// 添加颜色
	static int add_color(COLOR_LIST index, const unsigned int color)
	{
		if (index >= COLOR_MAX)
		{
			ASSERT(false);
			return -1;
		}
		s_color_map[index] = color;
		return 0;
	}

	// 获取颜色
	static const unsigned int get_color(COLOR_LIST index)
	{
		if (index >= COLOR_MAX)
		{
			ASSERT(false);
			return 0;
		}
		return s_color_map[index];
	}

private:
	static const void* s_font_map[FONT_MAX];
	static const void* s_image_map[IMAGE_MAX];
	static unsigned int s_color_map[COLOR_MAX];
};
