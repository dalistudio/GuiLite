#pragma once

#include "../core/api.h"
#include "../core/wnd.h"
#include "../core/resource.h"
#include "../core/word.h"
#include "../core/display.h"
#include "../core/theme.h"
#include "../widgets/button.h"
#include "../widgets/label.h"
#include "../widgets/keyboard.h"
#include <string.h>

#define MAX_EDIT_STRLEN		32
#define IDD_KEY_BOARD		0x1

// 文本编辑框
class c_edit : public c_wnd
{
	friend class c_keyboard; // 键盘
public:
	const char* get_text(){return m_str;} // 获得文本

	// 设置文本
	void set_text(const char* str)
	{
		if (str != 0 && strlen(str) < sizeof(m_str))
		{
			strcpy(m_str, str);
		}
	}

	// 设置键盘样式
	void set_keyboard_style(KEYBOARD_STYLE kb_sytle) { m_kb_style = kb_sytle; }
	
protected:
	// 预创建窗体
	virtual void pre_create_wnd()
	{
		m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
		m_kb_style = STYLE_ALL_BOARD;
		m_font = c_theme::get_font(FONT_DEFAULT);
		m_font_color = c_theme::get_color(COLOR_WND_FONT);

		memset(m_str_input, 0, sizeof(m_str_input));
		memset(m_str, 0, sizeof(m_str));
		set_text(c_wnd::m_str);
	}

	// 绘画时
	virtual void on_paint()
	{
		c_rect rect, kb_rect;
		get_screen_rect(rect);
		s_keyboard.get_screen_rect(kb_rect);
		switch (m_status)
		{
		case STATUS_NORMAL: // 正常状态
			if ((s_keyboard.get_attr()&ATTR_VISIBLE) == ATTR_VISIBLE)
			{
				s_keyboard.close_keyboard();
				m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
			}
			m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_NORMAL), m_z_order);
			c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str, rect, m_font, m_font_color, c_theme::get_color(COLOR_WND_NORMAL), ALIGN_HCENTER | ALIGN_VCENTER);
			break;
		case STATUS_FOCUSED: // 聚焦状态
			if ((s_keyboard.get_attr()&ATTR_VISIBLE) == ATTR_VISIBLE)
			{
				s_keyboard.close_keyboard();
				m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
			}
			m_surface->fill_rect(rect, c_theme::get_color(COLOR_WND_FOCUS), m_z_order);
			c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str, rect, m_font, m_font_color, c_theme::get_color(COLOR_WND_FOCUS), ALIGN_HCENTER | ALIGN_VCENTER);
			break;
		case STATUS_PUSHED: // 忙碌状态
			if ((s_keyboard.get_attr()&ATTR_VISIBLE) != ATTR_VISIBLE)
			{
				m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS | ATTR_PRIORITY);
				s_keyboard.open_keyboard(this, IDD_KEY_BOARD, m_kb_style, WND_CALLBACK(&c_edit::on_key_board_click));
			}
			m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, c_theme::get_color(COLOR_WND_PUSHED), m_parent->get_z_order());
			m_surface->draw_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, c_theme::get_color(COLOR_WND_BORDER), m_parent->get_z_order(), 2);
			strlen(m_str_input) ? c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str_input, rect, m_font, m_font_color, c_theme::get_color(COLOR_WND_PUSHED), ALIGN_HCENTER | ALIGN_VCENTER) :
				c_word::draw_string_in_rect(m_surface, m_parent->get_z_order(), m_str, rect, m_font, m_font_color, c_theme::get_color(COLOR_WND_PUSHED), ALIGN_HCENTER | ALIGN_VCENTER);
			break;
		default:
			ASSERT(false);
		}
	}

	// 聚焦时
	virtual void on_focus()
	{
		m_status = STATUS_FOCUSED;
		on_paint();
	}

	// 退出聚焦时
	virtual void on_kill_focus()
	{
		m_status = STATUS_NORMAL;
		on_paint();
	}

	// 导航时
	virtual void on_navigate(NAVIGATION_KEY key)
	{
		switch (key)
		{
		case NAV_ENTER: // 进入
			(m_status == STATUS_PUSHED) ? s_keyboard.on_navigate(key) : (on_touch(m_wnd_rect.m_left, m_wnd_rect.m_top, TOUCH_DOWN), on_touch(m_wnd_rect.m_left, m_wnd_rect.m_top, TOUCH_UP));
			return;
		case NAV_BACKWARD: // 后一个
		case NAV_FORWARD:  // 前一个
			return (m_status == STATUS_PUSHED) ? s_keyboard.on_navigate(key) : c_wnd::on_navigate(key);
		}
	}

	// 触摸时
	virtual void on_touch(int x, int y, TOUCH_ACTION action)
	{
		(action == TOUCH_DOWN) ? on_touch_down(x, y) : on_touch_up(x, y);
	}	

	// 键盘点击时
	void on_key_board_click(int id, int param)
	{
		switch (param)
		{
		case CLICK_CHAR: // 点击字符
			strcpy(m_str_input, s_keyboard.get_str());
			on_paint();
			break;
		case CLICK_ENTER: // 点击回车
			if (strlen(m_str_input))
			{
				memcpy(m_str, m_str_input, sizeof(m_str_input));
			}
			m_status = STATUS_FOCUSED;
			on_paint();
			break;
		case CLICK_ESC: // 点击退出
			memset(m_str_input, 0, sizeof(m_str_input));
			m_status = STATUS_FOCUSED;
			on_paint();
			break;
		default:
			ASSERT(false);
			break;
		}
	}
private:
	// 触摸按下时
	void on_touch_down(int x, int y)
	{
		c_rect kb_rect_relate_2_edit_parent;
		s_keyboard.get_wnd_rect(kb_rect_relate_2_edit_parent);
		kb_rect_relate_2_edit_parent.m_left += m_wnd_rect.m_left;
		kb_rect_relate_2_edit_parent.m_right += m_wnd_rect.m_left;
		kb_rect_relate_2_edit_parent.m_top += m_wnd_rect.m_top;
		kb_rect_relate_2_edit_parent.m_bottom += m_wnd_rect.m_top;

		if (m_wnd_rect.pt_in_rect(x, y))
		{//click edit box
			if (STATUS_NORMAL == m_status)
			{
				m_parent->set_child_focus(this);
			}
		}
		else if (kb_rect_relate_2_edit_parent.pt_in_rect(x, y))
		{//click key board
			c_wnd::on_touch(x, y, TOUCH_DOWN);
		}
		else
		{
			if (STATUS_PUSHED == m_status)
			{
				m_status = STATUS_FOCUSED;
				on_paint();
			}
		}
	}

	// 触摸松开时
	void on_touch_up(int x, int y)
	{
		if (STATUS_FOCUSED == m_status)
		{
			m_status = STATUS_PUSHED;
			on_paint();
		}
		else if (STATUS_PUSHED == m_status)
		{
			if (m_wnd_rect.pt_in_rect(x, y))
			{//click edit box
				m_status = STATUS_FOCUSED;
				on_paint();
			}
			else
			{
				c_wnd::on_touch(x, y, TOUCH_UP);
			}
		}
	}
	static c_keyboard  s_keyboard;
	KEYBOARD_STYLE m_kb_style;
	char m_str_input[MAX_EDIT_STRLEN];
	char m_str[MAX_EDIT_STRLEN];
};
