#pragma once

#include "../core/api.h"
#include "../core/resource.h"
#include "../core/word.h"
#include "../core/display.h"
#include "../core/theme.h"
#include "../core/wnd.h"

#define  MAX_COL_NUM  30
#define  MAX_ROW_NUM  30

// 表
class c_table: public c_wnd
{
public:
	void set_sheet_align(unsigned int align_type){ m_align_type = align_type;} // 设置对齐
	void set_row_num(unsigned int row_num){ m_row_num = row_num;} // 设置行数
	void set_col_num(unsigned int col_num){ m_col_num = col_num;} // 设置列数

	// 设置行高
	void set_row_height(unsigned int height)
	{
		for (unsigned int i = 0; i < m_row_num; i++)
		{
			m_row_height[i] = height;
		}
	}

	// 设置列宽
	void set_col_width(unsigned int width)
	{
		for (unsigned int i = 0; i < m_col_num; i++)
		{
			m_col_width[i] = width;
		}
	}

	// 设置行高
	int set_row_height(unsigned int index, unsigned int height)
	{
		if (m_row_num > index)
		{
			m_row_height[index] = height;
			return index;
		}
		return -1;
	}

	// 设置列宽
	int set_col_width(unsigned int index, unsigned int width)
	{
		if (m_col_num > index)
		{
			m_col_width[index] = width;
			return index;
		}
		return -1;
	}

	// 设置项
	void set_item(int row, int col, char* str, unsigned int color)
	{
		draw_item(row, col, str, color);
	}

	unsigned int get_row_num(){ return m_row_num;} // 获取行数
	unsigned int get_col_num(){ return m_col_num;} // 获取列数

	// 获取项的矩形
	c_rect get_item_rect(int row, int col)
	{
		static c_rect rect;
		if (row >= MAX_ROW_NUM || col >= MAX_COL_NUM)
		{
			return rect;
		}

		unsigned int width = 0;
		unsigned int height = 0;
		for (int i = 0; i < col; i++)
		{
			width += m_col_width[i];
		}
		for (int j = 0; j < row; j++)
		{
			height += m_row_height[j];
		}

		c_rect wRect;
		get_screen_rect(wRect);

		rect.m_left = wRect.m_left + width;
		rect.m_right = rect.m_left + m_col_width[col];
		if (rect.m_right > wRect.m_right)
		{
			rect.m_right = wRect.m_right;
		}
		rect.m_top = wRect.m_top + height;
		rect.m_bottom = rect.m_top + m_row_height[row];
		if (rect.m_bottom > wRect.m_bottom)
		{
			rect.m_bottom = wRect.m_bottom;
		}
		return rect;
	}
protected:
	// 预创建窗体
	virtual void pre_create_wnd()
	{
		m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE);
		m_font = c_theme::get_font(FONT_DEFAULT);
		m_font_color = c_theme::get_color(COLOR_WND_FONT);
	}

	// 画项
	void draw_item(int row, int col, const char* str, unsigned int color)
	{
		c_rect rect = get_item_rect(row, col);
		m_surface->fill_rect(rect.m_left + 1, rect.m_top + 1, rect.m_right - 1, rect.m_bottom - 1, color, m_z_order);
		c_word::draw_string_in_rect(m_surface, m_z_order, str, rect, m_font, m_font_color, GL_ARGB(0, 0, 0, 0), m_align_type);
	}

	unsigned int m_align_type;	
	unsigned int m_row_num;
	unsigned int m_col_num;
	unsigned int m_row_height[MAX_ROW_NUM];
	unsigned int m_col_width[MAX_COL_NUM];
};
