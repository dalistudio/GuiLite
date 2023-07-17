#pragma once

#define REAL_TIME_TASK_CYCLE_MS		50
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

#define GL_ARGB(a, r, g, b) ((((unsigned int)(a)) << 24) | (((unsigned int)(r)) << 16) | (((unsigned int)(g)) << 8) | ((unsigned int)(b)))
#define GL_ARGB_A(rgb) ((((unsigned int)(rgb)) >> 24) & 0xFF)

#define GL_RGB(r, g, b) ((0xFF << 24) | (((unsigned int)(r)) << 16) | (((unsigned int)(g)) << 8) | ((unsigned int)(b)))
#define GL_RGB_R(rgb) ((((unsigned int)(rgb)) >> 16) & 0xFF)
#define GL_RGB_G(rgb) ((((unsigned int)(rgb)) >> 8) & 0xFF)
#define GL_RGB_B(rgb) (((unsigned int)(rgb)) & 0xFF)
#define GL_RGB_32_to_16(rgb) (((((unsigned int)(rgb)) & 0xFF) >> 3) | ((((unsigned int)(rgb)) & 0xFC00) >> 5) | ((((unsigned int)(rgb)) & 0xF80000) >> 8))
#define GL_RGB_16_to_32(rgb) ((0xFF << 24) | ((((unsigned int)(rgb)) & 0x1F) << 3) | ((((unsigned int)(rgb)) & 0x7E0) << 5) | ((((unsigned int)(rgb)) & 0xF800) << 8))

#define ALIGN_HCENTER		0x00000000L // 水平居中对齐
#define ALIGN_LEFT			0x01000000L // 向左对齐
#define ALIGN_RIGHT			0x02000000L // 向右对齐
#define ALIGN_HMASK			0x03000000L // 居中掩码对齐

#define ALIGN_VCENTER		0x00000000L // 垂直居中对齐
#define ALIGN_TOP			0x00100000L // 向顶对齐
#define ALIGN_BOTTOM		0x00200000L // 向底对齐
#define ALIGN_VMASK			0x00300000L // 垂直掩码对齐

typedef struct
{
	unsigned short year; // 年
	unsigned short month; // 月
	unsigned short date; // 日期
	unsigned short day; // 日
	unsigned short hour; // 小时
	unsigned short minute; // 分钟
	unsigned short second; // 秒钟
}T_TIME;

void register_debug_function(void(*my_assert)(const char* file, int line), void(*my_log_out)(const char* log)); // 注册调试函数
void _assert(const char* file, int line); // 断言
#define ASSERT(condition)	\
	do{                     \
	if(!(condition))_assert(__FILE__, __LINE__);\
	}while(0)
void log_out(const char* log); // 日志

long get_time_in_second(); // 以秒为单位获取时间
T_TIME second_to_day(long second); // 将秒钟数转为日数
T_TIME get_time(); // 获得时间

void start_real_timer(void (*func)(void* arg)); // 开始真实定时器
void register_timer(int milli_second, void func(void* param), void* param); // 注册定时器

unsigned int get_cur_thread_id(); // 获取当前线程编号
void create_thread(unsigned long* thread_id, void* attr, void *(*start_routine) (void *), void* arg); // 创建线程
void thread_sleep(unsigned int milli_seconds); // 线程休眠
int build_bmp(const char *filename, unsigned int width, unsigned int height, unsigned char *data); // 构建位图

#define FIFO_BUFFER_LEN		1024 // 先进先出缓冲长度
class c_fifo
{
public:
	c_fifo();
	int read(void* buf, int len); // 读取
	int write(void* buf, int len); // 写入
private:
	unsigned char 	m_buf[FIFO_BUFFER_LEN]; // 缓存
	int		m_head; // 队列头
	int		m_tail; // 队列尾
	void* m_read_sem; // 读取信号量
	void* m_write_mutex; // 写入互斥量
};

class c_rect
{
public:
	c_rect(){ m_left = m_top = m_right = m_bottom = -1; }//empty rect 空矩形
	c_rect(int left, int top, int width, int height)
	{
		set_rect(left, top, width, height); // 设置矩形
	}

	// 设置矩形（左，顶，宽，高）
	void set_rect(int left, int top, int width, int height)
	{
		ASSERT(width > 0 && height > 0);
		m_left = left;
		m_top = top;
		m_right = left + width - 1;
		m_bottom = top + height -1;
	}

	// 
	bool pt_in_rect(int x, int y) const
	{
		return x >= m_left && x <= m_right && y >= m_top && y <= m_bottom;
	}

	// 操作
	int operator==(const c_rect& rect) const
	{
		return (m_left == rect.m_left) && (m_top == rect.m_top) && (m_right == rect.m_right) && (m_bottom == rect.m_bottom);
	}

	int width() const { return m_right - m_left + 1; } // 宽度
	int height() const { return m_bottom - m_top + 1 ; } // 高度

	int	    m_left; //左
	int     m_top; // 上
	int     m_right; // 右
	int     m_bottom; // 下
};
