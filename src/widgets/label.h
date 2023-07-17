#pragma once

#include "../core/api.h"
#include "../core/wnd.h"
#include "../core/display.h"
#include "../core/resource.h"
#include "../core/theme.h"
#include "../core/word.h"

// 标签
class c_label : public c_wnd
{
public:
	// 绘画时
	virtual void on_paint()
	{
		c_rect rect;
		unsigned int bg_color = m_bg_color ? m_bg_color : m_parent->get_bg_color(); // 获取背景色
		get_screen_rect(rect); // 获取屏幕矩形
		if (m_str)
		{
			m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, bg_color, m_z_order); // 填充矩形
			c_word::draw_string_in_rect(m_surface, m_z_order, m_str, rect, m_font, m_font_color, bg_color, ALIGN_LEFT | ALIGN_VCENTER); // 绘画字符串到矩形
		}
	}
protected:
	// 预创建窗体
	virtual void pre_create_wnd()
	{
		m_attr = ATTR_VISIBLE;
		m_font_color = c_theme::get_color(COLOR_WND_FONT); // 获取字体颜色
		m_font = c_theme::get_font(FONT_DEFAULT); // 获取字体
	}
};
