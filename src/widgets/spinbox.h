#pragma once

#include "../core/api.h"
#include "../core/wnd.h"
#include "../core/resource.h"
#include "../core/word.h"
#include "../core/display.h"
#include "../core/theme.h"
#include "../widgets/button.h"

#define ID_BT_ARROW_UP      	0x1111
#define ID_BT_ARROW_DOWN    	0x2222

// 数字设定框
class c_spin_box;
class c_spin_button : public c_button
{
	friend class c_spin_box;
	inline virtual void on_touch(int x, int y, TOUCH_ACTION action);
	c_spin_box* m_spin_box;
};

class c_spin_box : public c_wnd
{
	friend class c_spin_button;
public:
	short get_value() { return m_value; } // 获取值
	void set_value(unsigned short value) { m_value = m_cur_value = value; } // 设置值
	void set_max_min(short max, short min) { m_max = max; m_min = min; } // 设置最大和最小值
	void set_step(short step) { m_step = step; } // 设置步进值
	short get_min() { return m_min; } // 获得最小值
	short get_max() { return m_max; } // 获取最大值
	short get_step() { return m_step; } // 获取步进值
	void set_value_digit(short digit) { m_digit = digit; } // 设置数值
	short get_value_digit() { return m_digit; } // 获取数值
	void set_on_change(WND_CALLBACK on_change) { this->on_change = on_change; } // 设置改变
protected:
	// 绘画时
	virtual void on_paint()
	{
		c_rect rect;
		get_screen_rect(rect);
		rect.m_right = rect.m_left + (rect.width() * 2 / 3);

		m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
		c_word::draw_value_in_rect(m_surface, m_parent->get_z_order(), m_cur_value, m_digit, rect, m_font, m_font_color, c_theme::get_color(COLOR_WND_NORMAL), ALIGN_HCENTER | ALIGN_VCENTER);
	}

	// 预创建窗体
	virtual void pre_create_wnd()
	{
		m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE);
		m_font = c_theme::get_font(FONT_DEFAULT);
		m_font_color = c_theme::get_color(COLOR_WND_FONT);
		m_max = 6;
		m_min = 1;
		m_digit = 0;
		m_step = 1;

		//link arrow button position.
		c_rect rect;
		get_wnd_rect(rect);
		m_bt_down.m_spin_box = m_bt_up.m_spin_box = this;
		m_bt_up.connect(m_parent, ID_BT_ARROW_UP, "+", (rect.m_left + rect.width() * 2 / 3), rect.m_top, (rect.width() / 3), (rect.height() / 2));
		m_bt_down.connect(m_parent, ID_BT_ARROW_DOWN, "-", (rect.m_left + rect.width() * 2 / 3), (rect.m_top + rect.height() / 2), (rect.width() / 3), (rect.height() / 2));
	}

	// 点击上键时
	void on_arrow_up_bt_click()
	{
		if (m_cur_value + m_step > m_max)
		{
			return;
		}
		m_cur_value += m_step;
		if(on_change)
		{
			(m_parent->*(on_change))(m_id, m_cur_value);
		}
		on_paint();
	}

	// 点击下键时
	void on_arrow_down_bt_click()
	{
		if (m_cur_value - m_step < m_min)
		{
			return;
		}
		m_cur_value -= m_step;
		if(on_change)
		{
			(m_parent->*(on_change))(m_id, m_cur_value);
		}
		on_paint();
	}

	short			m_cur_value;
	short			m_value;
	short			m_step;
	short			m_max;
	short			m_min;
	short			m_digit;
	c_spin_button  	m_bt_up;
	c_spin_button  	m_bt_down;
	WND_CALLBACK 	on_change;
};

// 触摸时
inline void c_spin_button::on_touch(int x, int y, TOUCH_ACTION action)
{
	if (action == TOUCH_UP)
	{
		(m_id == ID_BT_ARROW_UP) ? m_spin_box->on_arrow_up_bt_click() : m_spin_box->on_arrow_down_bt_click();
	}
	c_button::on_touch(x, y, action);
}
