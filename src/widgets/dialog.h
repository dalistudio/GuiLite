#pragma once

#include "../core/api.h"
#include "../core/wnd.h"
#include "../core/display.h"
#include "../core/resource.h"
#include "../core/word.h"
#include "../core/theme.h"

class c_surface;
class c_dialog;

typedef struct
{
	c_dialog* 	dialog; // 对话框
	c_surface*	surface; // 表面
} DIALOG_ARRAY; // 对话框数组

class c_dialog : public c_wnd
{
public:
	// 打开对话框
	static int open_dialog(c_dialog* p_dlg, bool modal_mode = true)
	{
		if (0 == p_dlg)
		{
			ASSERT(false);
			return 0;
		}
		c_dialog* cur_dlg = get_the_dialog(p_dlg->get_surface());
		if (cur_dlg == p_dlg)
		{
			return 1;
		}

		if (cur_dlg)
		{
			cur_dlg->set_attr(WND_ATTRIBUTION(0));
		}

		c_rect rc;
		p_dlg->get_screen_rect(rc);
		p_dlg->get_surface()->activate_layer(rc, p_dlg->m_z_order);

		p_dlg->set_attr(modal_mode ? (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS | ATTR_PRIORITY) : (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS));
		p_dlg->show_window();
		p_dlg->set_me_the_dialog();
		return 1;
	}

	// 关闭对话框
	static int close_dialog(c_surface* surface)
	{
		c_dialog* dlg = get_the_dialog(surface);

		if (0 == dlg)
		{
			return 0;
		}

		dlg->set_attr(WND_ATTRIBUTION(0));
		surface->activate_layer(c_rect(), dlg->m_z_order);//inactivate the layer of dialog by empty rect.

		//clear the dialog
		for (int i = 0; i < SURFACE_CNT_MAX; i++)
		{
			if (ms_the_dialogs[i].surface == surface)
			{
				ms_the_dialogs[i].dialog = 0;
				return 1;
			}
		}
		ASSERT(false);
		return -1;
	}

	// 获取对话框
	static c_dialog* get_the_dialog(c_surface* surface)
	{
		for (int i = 0; i < SURFACE_CNT_MAX; i++)
		{
			if (ms_the_dialogs[i].surface == surface)
			{
				return ms_the_dialogs[i].dialog;
			}
		}
		return 0;
	}
protected:
	// 预创建窗体
	virtual void pre_create_wnd()
	{
		m_attr = WND_ATTRIBUTION(0);// no focus/visible
		m_z_order = Z_ORDER_LEVEL_1;
		m_bg_color = GL_RGB(33, 42, 53);
	}

	// 绘画时
	virtual void on_paint()
	{
		c_rect rect;
		get_screen_rect(rect); // 获取屏幕矩形
		m_surface->fill_rect(rect, m_bg_color, m_z_order); // 填充矩形

		if (m_str)
		{
			// 绘画字符串
			c_word::draw_string(m_surface, m_z_order, m_str, rect.m_left + 35, rect.m_top, c_theme::get_font(FONT_DEFAULT), GL_RGB(255, 255, 255), GL_ARGB(0, 0, 0, 0));
		}
	}
private:
	// 设置对话框
	int set_me_the_dialog()
	{
		c_surface* surface = get_surface();
		for (int i = 0; i < SURFACE_CNT_MAX; i++)
		{
			if (ms_the_dialogs[i].surface == surface)
			{
				ms_the_dialogs[i].dialog = this;
				return 0;
			}
		}

		for (int i = 0; i < SURFACE_CNT_MAX; i++)
		{
			if (ms_the_dialogs[i].surface == 0)
			{
				ms_the_dialogs[i].dialog = this;
				ms_the_dialogs[i].surface = surface;
				return 1;
			}
		}
		ASSERT(false);
		return -2;
	}
	static DIALOG_ARRAY ms_the_dialogs[SURFACE_CNT_MAX];
};
