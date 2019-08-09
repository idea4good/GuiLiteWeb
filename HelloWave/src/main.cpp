#define REAL_TIME_TASK_CYCLE_MS     50

#define GL_ARGB(a, r, g, b) ((((unsigned int)(a)) << 24) | (((unsigned int)(r)) << 16) | (((unsigned int)(g)) << 8) | ((unsigned int)(b)))
#define GL_ARGB_A(rgb) ((((unsigned int)(rgb)) >> 24) & 0xFF)

#define GL_RGB(r, g, b) ((0xFF << 24) | (((unsigned int)(r)) << 16) | (((unsigned int)(g)) << 8) | ((unsigned int)(b)))
#define GL_RGB_R(rgb) ((((unsigned int)(rgb)) >> 16) & 0xFF)
#define GL_RGB_G(rgb) ((((unsigned int)(rgb)) >> 8) & 0xFF)
#define GL_RGB_B(rgb) (((unsigned int)(rgb)) & 0xFF)
#define GL_RGB_32_to_16(rgb) (((((unsigned int)(rgb)) & 0xFF) >> 3) | ((((unsigned int)(rgb)) & 0xFC00) >> 5) | ((((unsigned int)(rgb)) & 0xF80000) >> 8))
#define GL_RGB_16_to_32(rgb) ((0xFF << 24) | ((((unsigned int)(rgb)) & 0x1F) << 3) | ((((unsigned int)(rgb)) & 0x7E0) << 5) | ((((unsigned int)(rgb)) & 0xF800) << 8))

#define ALIGN_HCENTER       0x00000000L
#define ALIGN_LEFT          0x01000000L
#define ALIGN_RIGHT         0x02000000L
#define ALIGN_HMASK         0x03000000L

#define ALIGN_VCENTER       0x00000000L
#define ALIGN_TOP           0x00100000L
#define ALIGN_BOTTOM        0x00200000L
#define ALIGN_VMASK         0x00300000L

typedef struct
{
    unsigned short year;
    unsigned short month;
    unsigned short date;
    unsigned short day;
    unsigned short hour;
    unsigned short minute;
    unsigned short second;
}T_TIME;

void register_debug_function(void(*my_assert)(const char* file, int line), void(*my_log_out)(const char* log));
void _assert(const char* file, int line);
#define ASSERT(condition)   \
    do{                     \
    if(!(condition))_assert(__FILE__, __LINE__);\
    }while(0)
void log_out(const char* log);

long get_time_in_second();
T_TIME second_to_day(long second);
T_TIME get_time();

void start_real_timer(void (*func)(void* arg));
void register_timer(int milli_second, void func(void* ptmr, void* parg));

unsigned int get_cur_thread_id();
void create_thread(unsigned long* thread_id, void* attr, void *(*start_routine) (void *), void* arg);
void thread_sleep(unsigned int milli_seconds);
int build_bmp(const char *filename, unsigned int width, unsigned int height, unsigned char *data);

typedef struct struct_bitmap_info
{
    unsigned short XSize;
    unsigned short YSize;
    unsigned short BitsPerPixel;
    const unsigned char* pData; /* Pointer to picture data (indices) */
} BITMAP_INFO;

//FONT
typedef struct struct_lattice
{
    unsigned int            utf8_code;
    unsigned char           width;
    const unsigned char*    p_data;
} LATTICE;

typedef struct struct_font_info
{
    unsigned char   height;
    unsigned int    count;
    LATTICE*        lattice_array;
} FONT_INFO;

class c_rect
{
public:
    c_rect(){Empty();}
    c_rect(int left, int top, int right, int bottom){m_left = left;m_top = top;m_right = right;m_bottom = bottom;};
    void SetRect( int left, int top, int right, int bottom);
    c_rect(const c_rect&);
    c_rect& operator=(const c_rect&);
    void Empty();
    void Offset(int x, int y);
    int IsEmpty() const ;
    int PtInRect(int x, int y) const ;
    int operator==(const c_rect& ) const;
    c_rect  operator&(const c_rect& aRect) const;
    int Width() const   {return m_right - m_left + 1;}
    int Height() const {return m_bottom - m_top + 1;}

    int     m_left;
    int     m_top;
    int     m_right;
    int     m_bottom;
};

class c_cmd_target;

#define MSG_TYPE_INVALID    0xFFFF
#define MSG_TYPE_WND        0x0001
#define MSG_TYPE_USR        0x0002
#define USR_MSG_MAX         32

typedef void (c_cmd_target::*MsgFuncVV)();

enum MSG_CALLBACK_TYPE
{
    MSG_CALLBACK_NULL = 0,
    MSG_CALLBACK_VV,
    MSG_CALLBACK_IWL,
    MSG_CALLBACK_IWV,
    MSG_CALLBACK_VWV,
    MSG_CALLBACK_VVL,
    MSG_CALLBACK_VWL,
    MSG_CALLBACK_IVV
};

typedef union
{
    void (c_cmd_target::*func)();
    void (c_cmd_target::*func_vwv)(unsigned int w_param);
    int (c_cmd_target::*func_iwl)(unsigned int w_param, long l_param);
    int (c_cmd_target::*func_iwv)(unsigned int w_param);
    void (c_cmd_target::*func_vvl)(long l_param);
    void (c_cmd_target::*func_vwl)(unsigned int w_param, long l_param);
    int (c_cmd_target::*func_ivv)();
}MSGFUNCS;

struct GL_MSG_ENTRY
{
    unsigned int        msgType;
    unsigned int        msgId;
    c_cmd_target*       pObject;
    MSG_CALLBACK_TYPE   callbackType;
    MsgFuncVV           func;
};

#define ON_GL_USER_MSG(msgId, func)                    \
{MSG_TYPE_USR, msgId, 0, MSG_CALLBACK_VWL, (MsgFuncVV)(static_cast<void (c_cmd_target::*)(unsigned int, unsigned int)>(&func))},

#define GL_DECLARE_MESSAGE_MAP()                        \
protected:                                              \
virtual const GL_MSG_ENTRY* GetMSgEntries() const;  \
private:                                                \
static const GL_MSG_ENTRY mMsgEntries[];

#define GL_BEGIN_MESSAGE_MAP(theClass)                  \
const GL_MSG_ENTRY* theClass::GetMSgEntries() const \
{                                                       \
    return theClass::mMsgEntries;                       \
}                                                       \
const GL_MSG_ENTRY theClass::mMsgEntries[] =            \
{

#define GL_END_MESSAGE_MAP()                           \
{MSG_TYPE_INVALID, 0, (c_cmd_target*)0, MSG_CALLBACK_NULL, (MsgFuncVV)0}};

class c_cmd_target
{
public:
    c_cmd_target();
    virtual ~c_cmd_target();
    static int handle_usr_msg(unsigned int msgId, unsigned int wParam, unsigned int lParam);
protected:
    void load_cmd_msg();
    const GL_MSG_ENTRY* FindMsgEntry(const GL_MSG_ENTRY *pEntry, 
        unsigned int msgType, unsigned short msgId, unsigned short ctrlId);
private:
    static GL_MSG_ENTRY ms_usr_map_entries[USR_MSG_MAX];
    static unsigned short ms_user_map_size;
    GL_DECLARE_MESSAGE_MAP()
};

typedef struct
{
    unsigned int dwMsgId;
    unsigned int dwParam1;
    unsigned int dwParam2;
}MSG_INFO;

#define FIFO_BUFFER_LEN     1024
class c_fifo
{
public:
    c_fifo();
    int read(void* buf, int len);
    int write(void* buf, int len);
    
private:
    unsigned char   m_buf[FIFO_BUFFER_LEN];
    int     m_head;
    int     m_tail;
    void*   m_read_sem;
    void*   m_write_mutex;
};

typedef struct struct_font_info     FONT_INFO;
typedef struct struct_color_rect    COLOR_RECT;
typedef struct struct_bitmap_info   BITMAP_INFO;

//Rebuild gui library once you change this file
enum FONT_TYPE
{
    FONT_NULL,
    FONT_DEFAULT,
    FONT_CUSTOM1,
    FONT_CUSTOM2,
    FONT_CUSTOM3,
    FONT_CUSTOM4,
    FONT_CUSTOM5,
    FONT_CUSTOM6,
    FONT_MAX
};

enum BITMAP_TYPE
{
    BITMAP_CUSTOM1,
    BITMAP_CUSTOM2,
    BITMAP_CUSTOM3,
    BITMAP_CUSTOM4,
    BITMAP_CUSTOM5,
    BITMAP_CUSTOM6,

    BITMAP_MAX
};

enum COLOR_TYPE
{
    COLOR_WND_FONT,
    COLOR_WND_NORMAL,
    COLOR_WND_PUSHED,
    COLOR_WND_FOCUS,
    COLOR_WND_BORDER,

    COLOR_CUSTOME1,
    COLOR_CUSTOME2,
    COLOR_CUSTOME3,
    COLOR_CUSTOME4,
    COLOR_CUSTOME5,
    COLOR_CUSTOME6,

    COLOR_MAX
};

class c_theme
{
public:
    static int add_font(FONT_TYPE index, const FONT_INFO* font);
    static const FONT_INFO* get_font(FONT_TYPE index);

    static int add_bitmap(BITMAP_TYPE index, const BITMAP_INFO* bmp);
    static const BITMAP_INFO* get_bmp(BITMAP_TYPE index);

    static int add_color(COLOR_TYPE index, const unsigned int color);
    static const unsigned int get_color(COLOR_TYPE index);
};

#define DEFAULT_MASK_COLOR 0xFF080408
class c_surface;
class c_bitmap
{
public:
    static void draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO *pBitmap, int x, int y, unsigned int mask_rgb = DEFAULT_MASK_COLOR);
    static void draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO* pBitmap, int x, int y, int src_x, int src_y, int width, int height, unsigned int mask_rgb = DEFAULT_MASK_COLOR);
};

class c_frame_layer
{
public:
    c_frame_layer() { fb = 0;}
    unsigned short* fb;
    c_rect  visible_rect;
};

typedef enum
{
    Z_ORDER_LEVEL_0,//view/wave/page
    Z_ORDER_LEVEL_1,//dialog
    Z_ORDER_LEVEL_2,//editbox/spinbox/listbox/keyboard
    Z_ORDER_LEVEL_MAX
}Z_ORDER_LEVEL;

struct EXTERNAL_GFX_OP
{
    void(*draw_pixel)(int x, int y, int rgb);
    void(*fill_rect)(int x0, int y0, int x1, int y1, int rgb);
};

class c_display;
class c_surface {
    friend class c_display; friend class c_bitmap;
public:
    int get_width() { return m_width; }
    int get_height() { return m_height; }
    unsigned int get_pixel(int x, int y, unsigned int z_order);

    void draw_pixel(int x, int y, unsigned int rgb, unsigned int z_order);
    void fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order);
    void draw_hline(int x0, int x1, int y, unsigned int rgb, unsigned int z_order);
    void draw_vline(int x, int y0, int y1, unsigned int rgb, unsigned int z_order);
    void draw_line(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order);
    void draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order, unsigned int size = 1);

    inline void draw_rect(c_rect rect, unsigned int rgb, unsigned int size, unsigned int z_order)
    {
        draw_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, z_order, size);
    }
    inline void fill_rect(c_rect rect, unsigned int rgb, unsigned int z_order)
    {
        fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, rgb, z_order);
    }

    int flush_scrren(int left, int top, int right, int bottom);
    bool is_valid(c_rect rect);
    bool is_active() { return m_is_active; }
    c_display* get_display() { return m_display; }

    int set_frame_layer_visible_rect(c_rect& rect, unsigned int z_order);
    void set_active(bool flag){m_is_active = flag;}
protected:
    virtual void fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb);
    virtual void draw_pixel_on_fb(int x, int y, unsigned int rgb);
    void set_surface(void* wnd_root, Z_ORDER_LEVEL max_z_order);
    c_surface(c_display* display, unsigned int width, unsigned int height, unsigned int color_bytes);
    int                     m_width;        //in pixels
    int                     m_height;       //in pixels
    int                     m_color_bytes;  //16 bits, 32 bits only
    void*                   m_fb;           //Top frame buffer you could see
    c_frame_layer           m_frame_layers[Z_ORDER_LEVEL_MAX];//Top layber fb always be 0
    void*                   m_usr;
    bool                    m_is_active;
    Z_ORDER_LEVEL           m_max_zorder;
    Z_ORDER_LEVEL           m_top_zorder;
    void*                   m_phy_fb;
    int*                    m_phy_write_index;
    c_display*              m_display;
};

class c_surface_no_fb : public c_surface {//No physical framebuffer, memory fb is 32 bits
    friend class c_display;
    c_surface_no_fb(c_display* display, unsigned int width, unsigned int height, unsigned int color_bytes, struct EXTERNAL_GFX_OP* gfx_op) :
        c_surface(display, width, height, color_bytes) {m_gfx_op = gfx_op;}
protected:
    virtual void fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb);
    virtual void draw_pixel_on_fb(int x, int y, unsigned int rgb);
    struct EXTERNAL_GFX_OP* m_gfx_op;//Rendering by external method
};

#define SURFACE_CNT_MAX 6//root + pages

class c_hid_pipe;
class c_surface;

class c_display {
    friend class c_surface;
public:
    c_display(void* phy_fb, unsigned int display_width, unsigned int display_height,
                    unsigned int surface_width, unsigned int surface_height,
                    unsigned int color_bytes, unsigned int surface_cnt, EXTERNAL_GFX_OP* gfx_op = 0);
    c_surface* alloc_surface(void* usr, Z_ORDER_LEVEL max_zorder);
    int merge_surface(c_surface* s1, c_surface* s2, int x0, int x1, int y0, int y2, int offset);
    unsigned int get_width() { return m_width; }
    unsigned int get_height() { return m_height; }

    void* get_updated_fb(int* width, int* height, bool force_update = false);
    int snap_shot(const char* file_name);
private:
    unsigned int    m_width;        //in pixels
    unsigned int    m_height;       //in pixels
    unsigned int    m_color_bytes;  //16 bits, 32 bits only
    void*           m_phy_fb;
    int             m_phy_read_index;
    int             m_phy_write_index;
    c_surface*      m_surface_group[SURFACE_CNT_MAX];
    unsigned int    m_surface_cnt;
};

typedef struct struct_font_info     FONT_INFO;
typedef struct struct_color_rect    COLOR_RECT;

class c_wnd;
class c_surface;

typedef enum
{
    ATTR_VISIBLE    = 0x80000000L,
    ATTR_DISABLED   = 0x40000000L,
    ATTR_FOCUS      = 0x20000000L,
    ATTR_MODAL      = 0x10000000L// Handle touch action at high priority
}WND_ATTRIBUTION;

typedef enum
{
    STATUS_NORMAL,
    STATUS_PUSHED,
    STATUS_FOCUSED,
    STATUS_DISABLED
}WND_STATUS;

typedef enum
{
    KEY_FORWARD,
    KEY_BACKWARD,
    KEY_ENTER
}KEY_TYPE;

typedef enum
{
    TOUCH_DOWN,
    TOUCH_UP
}TOUCH_ACTION;

typedef struct struct_wnd_tree
{
    c_wnd*                  p_wnd;
    unsigned int            resource_id;
    const char*             str;
    short                   x;
    short                   y;
    short                   width;
    short                   height;
    struct struct_wnd_tree* p_child_tree;
}WND_TREE;

class c_wnd : public c_cmd_target
{
    friend class c_dialog;
public:
    c_wnd();
    virtual ~c_wnd() {};
    virtual int connect(c_wnd *parent, unsigned short resource_id, const char* str,
        short x, short y, short width, short height, WND_TREE* p_child_tree = 0);
    virtual c_wnd* connect_clone(c_wnd *parent, unsigned short resource_id, const char* str,
        short x, short y, short width, short height, WND_TREE* p_child_tree = 0);
    void disconnect();
    virtual c_wnd* clone() = 0;
    virtual void on_init_children() {}
    virtual void on_paint() {}
    virtual void show_window();

    unsigned short get_id() const { return m_resource_id; }
    int get_z_order() { return m_z_order; }
    c_wnd* get_wnd_ptr(unsigned short id) const;
    unsigned int get_attr() const { return m_attr; }
    void set_attr(WND_ATTRIBUTION attr);

    void set_str(const char* str) { m_str = str; }
    int is_focus_wnd() const;

    void set_font_color(unsigned int color) { m_font_color = color; }
    unsigned int get_font_color() { return m_font_color; }
    void set_bg_color(unsigned int color) { m_bg_color = color; }
    unsigned int get_bg_color() { return m_bg_color; }
    void set_font_type(const FONT_INFO *font_type) { m_font_type = font_type; }
    const FONT_INFO* get_font_type() { return m_font_type; }

    void set_wnd_pos(short x, short y, short width, short height);
    void get_wnd_rect(c_rect &rect) const;
    void get_screen_rect(c_rect &rect) const;

    c_wnd* set_child_focus(c_wnd *focus_child);

    c_wnd* get_parent() const { return m_parent; }
    c_wnd* get_last_child() const;
    int unlink_child(c_wnd *child);
    c_wnd* get_prev_sibling() const { return m_prev_sibling; }
    c_wnd* get_next_sibling() const { return m_next_sibling; }

    void notify_parent(unsigned int msg_id, unsigned int ctrl_id, int param);

    virtual bool on_touch(int x, int y, TOUCH_ACTION action);// return true: handled; false: not be handled.
    virtual bool on_key(KEY_TYPE key);// return false: skip handling by parent;

    c_surface* get_surface() { return m_surface; }
    void set_surface(c_surface* surface) { m_surface = surface; }
protected:
    virtual void pre_create_wnd() {};
    void add_child_2_tail(c_wnd *child);

    void wnd2screen(int &x, int &y) const;
    void wnd2screen(c_rect &rect) const;
    void screen2wnd(short &x, short &y) const;
    void screen2wnd(c_rect &rect) const;

    int load_child_wnd(WND_TREE *p_child_tree);
    int load_clone_child_wnd(WND_TREE *p_child_tree);
    void set_active_child(c_wnd* child) { m_focus_child = child; }

    virtual void on_focus() {};
    virtual void on_kill_focus() {};
protected:
    WND_STATUS      m_status;
    WND_ATTRIBUTION m_attr;
    c_rect          m_wnd_rect;// position relative to parent wnd.
    c_wnd*          m_parent;
    c_wnd*          m_top_child;
    c_wnd*          m_prev_sibling;
    c_wnd*          m_next_sibling;
    const char*     m_str;

    const FONT_INFO*    m_font_type;
    unsigned int        m_font_color;
    unsigned int        m_bg_color;

    unsigned short      m_resource_id;

    int                 m_z_order;
    c_wnd*              m_focus_child;//current focused wnd
    c_surface*          m_surface;
private:
    c_wnd(const c_wnd &win);
    c_wnd& operator=(const c_wnd &win);
};

class c_surface;
class c_word
{
public:
    static void draw_string(c_surface* surface, int z_order, const char *s, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
    static void draw_string_in_rect(c_surface* surface, int z_order, const char *s, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
    static void draw_value(c_surface* surface, int z_order, int value, int dot_position, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
    static void draw_value_in_rect(c_surface* surface, int z_order, int value, int dot_position, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type = ALIGN_LEFT);
    static void value_2_string(int value, int dot_position, char* buf, int len);

    static int get_str_size(const char *s, const FONT_INFO* font, int& width, int& height);
private:
    static int draw_single_char(c_surface* surface, int z_order, unsigned int utf8_code, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color);
    static void draw_lattice(c_surface* surface, int z_order, int x, int y, int width, int height, const unsigned char* p_data, unsigned int font_color, unsigned int bg_color);
    
    static const LATTICE* get_lattice(const FONT_INFO* font, unsigned int utf8_code);
    static void get_string_pos(const char *s, const FONT_INFO* font, c_rect rect, unsigned int align_type, int &x, int &y);
};

#define WAVE_BUFFER_LEN	1024
#define WAVE_READ_CACHE_LEN	8
#define BUFFER_EMPTY	-1111
#define BUFFER_FULL		-2222;
class c_wave_buffer
{
public:
	c_wave_buffer();
	int write_wave_data(short data);
	int read_wave_data_by_frame(short &max, short &min, short frame_len, unsigned int sequence, short offset);
	void reset();
	void clear_data();
	short get_cnt();
private:
	int read_data();
	short m_wave_buf[WAVE_BUFFER_LEN];
	short m_head;
	short m_tail;

	int m_min_old;
	int m_max_old;
	int m_min_older;
	int m_max_older;
	int m_last_data;

	short 	m_read_cache_min[WAVE_READ_CACHE_LEN];
	short 	m_read_cache_mid[WAVE_READ_CACHE_LEN];
	short 	m_read_cache_max[WAVE_READ_CACHE_LEN];
	short	m_read_cache_sum;
	unsigned int m_refresh_sequence;
};

typedef enum
{
	FILL_MODE,
	SCAN_MODE
}E_WAVE_DRAW_MODE;

class c_wave_buffer;
class c_wave_ctrl : public c_wnd
{
public:
	c_wave_ctrl();
	virtual c_wnd* clone(){return new c_wave_ctrl();}
	virtual void on_init_children();
	virtual void on_paint();

	void set_wave_name(char* wave_name){ m_wave_name = wave_name;}
	void set_wave_unit(char* wave_unit){ m_wave_unit = wave_unit;}

	void set_wave_name_font(const FONT_INFO* wave_name_font_type){ m_wave_name_font = wave_name_font_type;}
	void set_wave_unit_font(const FONT_INFO* wave_unit_font_type){ m_wave_unit_font = wave_unit_font_type;}

	void set_wave_name_color(unsigned int wave_name_color){ m_wave_name_color = wave_name_color;}
	void set_wave_unit_color(unsigned int wave_unit_color){ m_wave_unit_color = wave_unit_color;}
	void set_wave_color(unsigned int color){ m_wave_color = color;}
	void set_wave_in_out_rate(unsigned int data_rate, unsigned int refresh_rate);
	void set_wave_speed(unsigned int speed);

	void set_max_min(short max_data, short min_data);
	void set_wave(c_wave_buffer* wave){m_wave = wave;}
	c_wave_buffer* get_wave(){return m_wave;}
	void clear_data();
	bool is_data_enough();
	void refresh_wave(unsigned char frame);
	void clear_wave();
	
protected:
	void draw_smooth_vline(int y_min, int y_max, int mid, unsigned int rgb);
	void restore_background();
	void save_background();

	char* m_wave_name;
	char* m_wave_unit;

	const FONT_INFO* m_wave_name_font;
	const FONT_INFO* m_wave_unit_font;

	unsigned int m_wave_name_color;
	unsigned int m_wave_unit_color;

	unsigned int m_wave_color;
	unsigned int m_back_color;

	int m_wave_left;
	int m_wave_right;
	int m_wave_top;
	int m_wave_bottom;

	short m_max_data;
	short m_min_data;
	
private:
	c_wave_buffer*	m_wave;
	unsigned int*	m_bg_fb;			//background frame buffer, could be used to draw scale line.
	int 			m_wave_cursor;
	int 			m_wave_speed;		//pixels per refresh
	unsigned int	m_wave_data_rate;	//data sample rate
	unsigned int	m_wave_refresh_rate;//refresh cycle in millisecond
	unsigned char 	m_frame_len_map[64];
	unsigned char 	m_frame_len_map_index;
};

///////////////////////////////


void c_bitmap::draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO *pBitmap, int x, int y, unsigned int mask_rgb)
{
    if (0 == pBitmap)
    {
        return;
    }
    unsigned short* lower_fb = 0;
    int lower_fb_width = surface->m_width;
    if (z_order >= Z_ORDER_LEVEL_1)
    {
        lower_fb = surface->m_frame_layers[z_order - 1].fb;
    }
    unsigned int mask_rgb_16 = GL_RGB_32_to_16(mask_rgb);
    int xsize = pBitmap->XSize;
    int ysize = pBitmap->YSize;
    const unsigned short* pData = (const unsigned short*)pBitmap->pData;
    for (int j = 0; j < ysize; j++)
    {
        for (int i = 0; i < xsize; i++)
        {
            unsigned int rgb = *pData++;
            if (mask_rgb_16 == rgb)
            {
                if (lower_fb)
                {//restore lower layer
                    surface->draw_pixel(x + i, y + j, GL_RGB_16_to_32(lower_fb[(y + j) * lower_fb_width + x + i]), z_order);
                }
            }
            else
            {
                surface->draw_pixel(x + i, y + j, GL_RGB_16_to_32(rgb), z_order);
            }
        }
    }
}

void c_bitmap::draw_bitmap(c_surface* surface, int z_order, const BITMAP_INFO* pBitmap, int x, int y, int src_x, int src_y, int width, int height, unsigned int mask_rgb)
{
    if (0 == pBitmap || (src_x + width > pBitmap->XSize) || (src_y + height > pBitmap->YSize))
    {
        return;
    }

    unsigned short* lower_fb = 0;
    int lower_fb_width = surface->m_width;
    if (z_order >= Z_ORDER_LEVEL_1)
    {
        lower_fb = surface->m_frame_layers[z_order - 1].fb;
    }
    unsigned int mask_rgb_16 = GL_RGB_32_to_16(mask_rgb);
    const unsigned short* pData = (const unsigned short*)pBitmap->pData;
    for (int j = 0; j < height; j++)
    {
        const unsigned short* p = &pData[src_x + (src_y + j) * pBitmap->XSize];
        for (int i = 0; i < width; i++)
        {
            unsigned int rgb = *p++;
            if (mask_rgb_16 == rgb)
            {
                if (lower_fb)
                {//restore lower layer
                    surface->draw_pixel(x + i, y + j, GL_RGB_16_to_32(lower_fb[(y + j) * lower_fb_width + x + i]), z_order);
                }
            }
            else
            {
                surface->draw_pixel(x + i, y + j, GL_RGB_16_to_32(rgb), z_order);
            }
        }
    }
}

GL_MSG_ENTRY c_cmd_target::ms_usr_map_entries[USR_MSG_MAX];
unsigned short c_cmd_target::ms_user_map_size;

GL_BEGIN_MESSAGE_MAP(c_cmd_target)
GL_END_MESSAGE_MAP()

c_cmd_target::c_cmd_target()
{
}

c_cmd_target::~c_cmd_target()
{
}

int c_cmd_target::handle_usr_msg(unsigned int msgId, unsigned int wParam, unsigned int lParam)
{
    int i;
    c_cmd_target* p_wnd = 0;
    MSGFUNCS msg_funcs;
    for (i = 0; i < ms_user_map_size; i++)
    {
        if (msgId == ms_usr_map_entries[i].msgId)
        {
            p_wnd = (c_cmd_target*)ms_usr_map_entries[i].pObject;
            msg_funcs.func = ms_usr_map_entries[i].func;
            (p_wnd->*msg_funcs.func_vwl)(wParam , lParam);
        }
    }
    return 1;
}

void c_cmd_target::load_cmd_msg()
{
    const GL_MSG_ENTRY* p_entry = GetMSgEntries();
    if (0 == p_entry)
    {
        return;
    }

    bool bExist = false;
    while(MSG_TYPE_INVALID != p_entry->msgType)
    {
        if (MSG_TYPE_WND == p_entry->msgType)
        {
            p_entry++;
            continue;
        }

        bExist = false;
        for (int i = 0; i < ms_user_map_size; i++)
        {
            //repeat register, return.
            if (p_entry->msgId == ms_usr_map_entries[i].msgId
                && ms_usr_map_entries[i].pObject == this)
            {
                bExist = true;
                break;
            }
        }
        if (true == bExist)
        {
            p_entry++;
            continue;
        }

        if (MSG_TYPE_USR == p_entry->msgType)
        {
            ms_usr_map_entries[ms_user_map_size] = *p_entry;
            ms_usr_map_entries[ms_user_map_size].pObject = this;
            ms_user_map_size++;
            if (USR_MSG_MAX == ms_user_map_size)
            {
                ASSERT(false);
            }
        }
        else
        {
            ASSERT(false);
            break;
        }
        p_entry++;
    }
}

const GL_MSG_ENTRY* c_cmd_target::FindMsgEntry(const GL_MSG_ENTRY *pEntry, 
    unsigned int msgType, unsigned short msgId, unsigned short ctrlId)
{
    if ( MSG_TYPE_INVALID == msgType)
    {
        return 0;
    }

    while (MSG_CALLBACK_NULL != pEntry->callbackType)
    {
        if ( (msgType == pEntry->msgType) && (msgId == pEntry->msgId) && (void*)(unsigned long)ctrlId == pEntry->pObject)
        {
            return pEntry;
        }
        pEntry++;
    }
    return 0;
}

#include <string.h>
#include <stdio.h>

c_display::c_display(void* phy_fb, unsigned int display_width, unsigned int display_height,
                        unsigned int surface_width, unsigned int surface_height,
                        unsigned int color_bytes, unsigned int surface_cnt, EXTERNAL_GFX_OP* gfx_op)
{
    if (color_bytes != 2 && color_bytes != 4)
    {
        log_out("Support 16 bits, 32 bits color only!");
        ASSERT(false);
    }

    m_width = display_width;
    m_height = display_height;
    m_color_bytes = color_bytes;
    m_phy_fb = phy_fb;
    m_phy_read_index = m_phy_write_index = 0;
    memset(m_surface_group, 0, sizeof(m_surface_group));
    m_surface_cnt = surface_cnt;
    ASSERT(m_surface_cnt <= SURFACE_CNT_MAX);
    
    for (int i = 0; i < m_surface_cnt; i++)
    {
        m_surface_group[i] = phy_fb ? new c_surface(this, surface_width, surface_height, color_bytes) : new c_surface_no_fb(this, surface_width, surface_height, color_bytes, gfx_op);
    }
}

c_surface* c_display::alloc_surface(void* usr, Z_ORDER_LEVEL max_zorder)
{
    int i = 0;
    ASSERT(max_zorder < Z_ORDER_LEVEL_MAX);

    while (i < m_surface_cnt)
    {
        if (m_surface_group[i]->m_usr == usr)
        {
            //repeat register
            ASSERT(false);
            return m_surface_group[i];
        }
        i++;
    }

    i = 0;
    while (i < m_surface_cnt)
    {
        if (m_surface_group[i]->m_usr == 0)
        {   
            m_surface_group[i]->set_surface(usr, max_zorder);
            return m_surface_group[i];
        }
        i++;
    }
    //no surface for use
    ASSERT(false);
    return 0;
}

int c_display::merge_surface(c_surface* s0, c_surface* s1, int x0, int x1, int y0, int y1, int offset)
{
    int surface_width = s0->get_width();
    int surface_height = s0->get_height();

    if (offset < 0 || offset > surface_width || y0 < 0 || y0 >= surface_height ||
        y1 < 0 || y1 >= surface_height || x0 < 0 || x0 >= surface_width || x1 < 0 || x1 >= surface_width)
    {
        ASSERT(false);
        return -1;
    }

    int width = (x1 - x0 + 1);
    if (width < 0 || width > surface_width || width < offset)
    {
        ASSERT(false);
        return -1;
    }

    x0 = (x0 >= m_width) ? (m_width - 1) : x0;
    x1 = (x1 >= m_width) ? (m_width - 1) : x1;
    y0 = (y0 >= m_height) ? (m_height - 1) : y0;
    y1 = (y1 >= m_height) ? (m_height - 1) : y1;
    
    if (m_phy_fb)
    {
        for (int y = y0; y <= y1; y++)
        {
            //Left surface
            char* addr_s = ((char*)(s0->m_fb) + (y * (s0->get_width()) + x0 + offset) * m_color_bytes);
            char* addr_d = ((char*)(m_phy_fb)+(y * m_width + x0) * m_color_bytes);
            memcpy(addr_d, addr_s, (width - offset) * m_color_bytes);
            //Right surface
            addr_s = ((char*)(s1->m_fb) + (y * (s1->get_width()) + x0) * m_color_bytes);
            addr_d = ((char*)(m_phy_fb)+(y * m_width + x0 + (width - offset)) * m_color_bytes);
            memcpy(addr_d, addr_s, offset * m_color_bytes);
        }
    }
    else if(m_color_bytes == 4)
    {
        void(*draw_pixel)(int x, int y, int rgb) = ((c_surface_no_fb*)s0)->m_gfx_op->draw_pixel;
        for (int y = y0; y <= y1; y++)
        {
            //Left surface
            for (int x = x0; x <= (x1 - offset); x++)
            {
                draw_pixel(x, y, ((unsigned int*)s0->m_fb)[y * m_width + x + offset]);
            }
            //Right surface
            for (int x = x1 - offset; x <= x1; x++)
            {
                draw_pixel(x, y, ((unsigned int*)s1->m_fb)[y * m_width + x + offset - x1 + x0]);
            }
        }
    }
    else if (m_color_bytes == 2)
    {
        void(*draw_pixel)(int x, int y, int rgb) = ((c_surface_no_fb*)s0)->m_gfx_op->draw_pixel;
        for (int y = y0; y <= y1; y++)
        {
            //Left surface
            for (int x = x0; x <= (x1 - offset); x++)
            {
                draw_pixel(x, y, GL_RGB_16_to_32(((unsigned short*)s0->m_fb)[y * m_width + x + offset]));
            }
            //Right surface
            for (int x = x1 - offset; x <= x1; x++)
            {
                draw_pixel(x, y, GL_RGB_16_to_32(((unsigned short*)s1->m_fb)[y * m_width + x + offset - x1 + x0]));
            }
        }
    }

    m_phy_write_index++;
    return 0;
}

void* c_display::get_updated_fb(int* width, int* height, bool force_update)
{
    if (width && height) 
    {
        *width = get_width();
        *height = get_height();
    }
    if (force_update)
    {
        return m_phy_fb;
    }
    if (m_phy_read_index == m_phy_write_index)
    {//No update
        return 0;
    }
    m_phy_read_index = m_phy_write_index;
    return m_phy_fb;
}

int c_display::snap_shot(const char* file_name)
{
    if (!m_phy_fb)
    {
        return -1;
    }

    unsigned int width = get_width();
    unsigned int height = get_height();

    //16 bits framebuffer
    if (m_color_bytes == 2)
    {
        return build_bmp(file_name, width, height, (unsigned char*)m_phy_fb);
    }

    //32 bits framebuffer
    unsigned short* p_bmp565_data = new unsigned short[width * height];
    unsigned int* p_raw_data = (unsigned int*)m_phy_fb;

    for (int i = 0; i < width * height; i++)
    {
        unsigned int rgb = *p_raw_data++;
        p_bmp565_data[i] = GL_RGB_32_to_16(rgb);
    }

    int ret = build_bmp(file_name, width, height, (unsigned char*)p_bmp565_data);
    delete []p_bmp565_data;
    return ret;
}

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

c_rect::c_rect(const c_rect& rect)
{
    SetRect(rect.m_left,rect.m_top,rect.m_right,rect.m_bottom);
}

c_rect& c_rect::operator=(const c_rect& rect)
{
    SetRect(rect.m_left,rect.m_top,rect.m_right,rect.m_bottom);
    return *this;
}

void c_rect::SetRect( int Left, int Top, int Right, int Bottom)
{ 
    m_left = MIN(Left, Right);
    m_top = MIN(Top, Bottom);
    m_right = MAX(Left, Right);
    m_bottom = MAX(Top, Bottom);
}

c_rect  c_rect::operator&(const c_rect& rect) const
{
    c_rect  dst;
    dst.m_left = MAX(m_left, rect.m_left);
    dst.m_top = MAX(m_top, rect.m_top);
    dst.m_right = MIN(m_right, rect.m_right);
    dst.m_bottom = MIN(m_bottom, rect.m_bottom);
    if(dst.m_left >= dst.m_right || dst.m_top >= dst.m_bottom)
        dst.Empty();
    return dst; 
}

void c_rect::Empty()
{
    m_left = m_top = m_right = m_bottom = 0;
}

void c_rect::Offset(int x, int y)
{
    m_left +=x;
    m_right +=x;
    m_top += y;
    m_bottom += y;
}

int c_rect::IsEmpty() const
{
    return m_top == m_bottom || m_left == m_right;
}

int c_rect::PtInRect(int x, int y) const 
{
    return x >= m_left && x <= m_right && y >= m_top && y <= m_bottom;
}

int c_rect::operator==(const c_rect& rect) const
{
    return (m_left == rect.m_left) && (m_top==rect.m_top) &&
        (m_right == rect.m_right) && (m_bottom==rect.m_bottom);
}


#include <string.h>
#include <stdlib.h>

#define GL_ROUND_RGB_32(rgb) (rgb & 0xFFF8FCF8) //make RGB32 = RGB16

c_surface::c_surface(c_display* display,  unsigned int width, unsigned int height, unsigned int color_bytes)
{
    m_width = width;
    m_height = height;
    m_color_bytes = color_bytes;
    m_display = display;
    m_phy_fb = display->m_phy_fb;
    m_phy_write_index = &display->m_phy_write_index;
    m_fb = m_usr = 0;
    m_top_zorder = m_max_zorder = Z_ORDER_LEVEL_0;
    m_is_active = false;
    m_frame_layers[Z_ORDER_LEVEL_0].visible_rect = c_rect(0, 0, m_width, m_height);
}

void c_surface::set_surface(void* wnd_root, Z_ORDER_LEVEL max_z_order)
{
    m_usr = wnd_root;
    m_max_zorder = max_z_order;

    if (m_display->m_surface_cnt > 1)
    {
        m_fb = calloc(m_width * m_height, m_color_bytes);
    }
    
    for(int i = Z_ORDER_LEVEL_0; i < m_max_zorder; i++)
    {//Top layber fb always be 0
        m_frame_layers[i].fb = (unsigned short*)calloc(m_width * m_height, sizeof(unsigned short));
        ASSERT(0 != m_frame_layers[i].fb);
    }
}

void c_surface::draw_pixel(int x, int y, unsigned int rgb, unsigned int z_order)
{
    if (x >= m_width || y >= m_height || x < 0 || y < 0)
    {
        return;
    }
    if (z_order > m_max_zorder)
    {
        ASSERT(false);
        return;
    }
    rgb = GL_ROUND_RGB_32(rgb);
    if (z_order == m_max_zorder)
    {
        return draw_pixel_on_fb(x, y, rgb);
    }

    if (z_order > m_top_zorder)
    {
        m_top_zorder = (Z_ORDER_LEVEL)z_order;
    }

    if (0 == m_frame_layers[z_order].visible_rect.PtInRect(x, y))
    {
        ASSERT(false);
        return;
    }
    ((unsigned short*)(m_frame_layers[z_order].fb))[x + y * m_width] = GL_RGB_32_to_16(rgb);

    if (z_order == m_top_zorder)
    {
        return draw_pixel_on_fb(x, y, rgb);
    }

    bool is_covered = false;
    for (int tmp_z_order = Z_ORDER_LEVEL_MAX - 1; tmp_z_order > z_order; tmp_z_order--)
    {
        if (true == m_frame_layers[tmp_z_order].visible_rect.PtInRect(x, y))
        {
            is_covered = true;
            break;
        }
    }

    if (!is_covered)
    {
        draw_pixel_on_fb(x, y, rgb);
    }
}

void c_surface::draw_pixel_on_fb(int x, int y, unsigned int rgb)
{
    if (m_fb)
    {
        (m_color_bytes == 4) ? ((unsigned int*)m_fb)[y * m_width + x] = rgb : ((unsigned short*)m_fb)[y * m_width + x] = GL_RGB_32_to_16(rgb);
    }

    int display_width = m_display->get_width();
    int display_height = m_display->get_height();
    if (m_is_active && (x < display_width) && (y < display_height))
    {       
        if (m_color_bytes == 4)
        {
            ((unsigned int*)m_phy_fb)[y * (m_display->get_width()) + x] = rgb;
        }
        else
        {
            ((unsigned short*)m_phy_fb)[y * (m_display->get_width()) + x] = GL_RGB_32_to_16(rgb);
        }
        *m_phy_write_index = *m_phy_write_index + 1;
    }
}

void c_surface::fill_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order)
{
    rgb = GL_ROUND_RGB_32(rgb);
    if (z_order == m_max_zorder)
    {
        return fill_rect_on_fb(x0, y0, x1, y1, rgb);
    }

    if (z_order == m_top_zorder)
    {
        int x, y;
        unsigned short *mem_fb;
        unsigned int rgb_16 = GL_RGB_32_to_16(rgb);
        for (y = y0; y <= y1; y++)
        {
            x = x0;
            mem_fb = &((unsigned short*)m_frame_layers[z_order].fb)[y * m_width + x];
            for (; x <= x1; x++)
            {
                *mem_fb++ = rgb_16;
            }
        }
        return fill_rect_on_fb(x0, y0, x1, y1, rgb);
    }

    for (; y0 <= y1; y0++)
    {
        draw_hline(x0, x1, y0, rgb, z_order);
    }
}

void c_surface::fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb)
{
    if (x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0 ||
        x0 >= m_width || x1 >= m_width || y0 >= m_height || y1 >= m_height)
    {
        ASSERT(false);
    }
    int display_width = m_display->get_width();
    int display_height = m_display->get_height();

    if (m_color_bytes == 4)
    {
        int x;
        unsigned int *fb, *phy_fb;
        for (; y0 <= y1; y0++)
        {
            x = x0;
            fb = m_fb ? &((unsigned int*)m_fb)[y0 * m_width + x] : 0;
            phy_fb = &((unsigned int*)m_phy_fb)[y0 * display_width + x];
            *m_phy_write_index = *m_phy_write_index + 1;
            for (; x <= x1; x++)
            {
                if (fb)
                {
                    *fb++ = rgb;
                }
                if (m_is_active && (x < display_width) && (y0 < display_height))
                {
                    *phy_fb++ = rgb;
                }
            }
        }
    }
    else if(m_color_bytes == 2)
    {
        int x;
        unsigned short *fb, *phy_fb;
        rgb = GL_RGB_32_to_16(rgb);
        for (; y0 <= y1; y0++)
        {
            x = x0;
            fb = m_fb ? &((unsigned short*)m_fb)[y0 * m_width + x] : 0;
            phy_fb = &((unsigned short*)m_phy_fb)[y0 * display_width + x];
            *m_phy_write_index = *m_phy_write_index + 1;
            for (; x <= x1; x++)
            {
                if (fb)
                {
                    *fb++ = rgb;
                }
                if (m_is_active && (x < display_width) && (y0 < display_height))
                {
                    *phy_fb++ = rgb;
                }
            }
        }
    }
    
}

unsigned int c_surface::get_pixel(int x, int y, unsigned int z_order)
{
    if (x >= m_width || y >= m_height || x < 0 || y < 0 ||
            z_order >= Z_ORDER_LEVEL_MAX)
    {
        ASSERT(false);
        return 0;
    }

    if (z_order == m_max_zorder)
    {
        if (m_fb)
        {
            return (m_color_bytes == 4) ? ((unsigned int*)m_fb)[y * m_width + x] : GL_RGB_16_to_32(((unsigned short*)m_fb)[y * m_width + x]);
        }
        else if(m_phy_fb)
        {
            return (m_color_bytes == 4) ? ((unsigned int*)m_phy_fb)[y * m_width + x] : GL_RGB_16_to_32(((unsigned short*)m_phy_fb)[y * m_width + x]);
        }
        return 0;
    }
    
    unsigned short rgb_16 = ((unsigned short*)(m_frame_layers[z_order].fb))[y * m_width + x];
    return GL_RGB_16_to_32(rgb_16);
}

void c_surface::draw_hline(int x0, int x1, int y, unsigned int rgb, unsigned int z_order)
{
    for (;x0 <= x1; x0++)
    { draw_pixel(x0, y, rgb, z_order); }
}

void c_surface::draw_vline(int x, int y0, int y1, unsigned int rgb, unsigned int z_order)
{
    for (;y0 <= y1; y0++)
    { draw_pixel(x, y0, rgb, z_order); }
}

void c_surface::draw_line(int x1, int y1, int x2, int y2, unsigned int rgb, unsigned int z_order)
{
    int dx, dy, e;
    dx = x2 - x1;
    dy = y2 - y1;

    if ((dx >= 0) && (dy >= 0))
    {
        if (dx >= dy)
        {
            e = dy - dx / 2;
            for(; x1 <= x2; x1++, e += dy)
            {
                draw_pixel(x1, y1, rgb, z_order);
                if (e>0) { y1++; e -= dx; }
            }
        }
        else
        {
            e = dx - dy / 2;
            for(; y1 <= y2; y1++, e += dx)
            {
                draw_pixel(x1, y1, rgb, z_order);
                if (e>0) { x1++; e -= dy; }
            }
        }
    }

    else if ((dx >= 0) && (dy < 0))
    {
        dy = -dy;
        if (dx >= dy)
        {
            e = dy - dx / 2;
            for(; x1 <= x2; x1++, e += dy)
            {
                draw_pixel(x1, y1, rgb, z_order);
                if (e>0) { y1--; e -= dx; }
            }
        }
        else
        {
            e = dx - dy / 2;
            for(; y1 >= y2; y1--, e += dx)
            {
                draw_pixel(x1, y1, rgb, z_order);
                if (e>0) { x1++; e -= dy; }
            }
        }
    }

    else if ((dx < 0) && (dy >= 0))
    {
        dx = -dx;
        if (dx >= dy)
        {
            e = dy - dx / 2;
            for(; x1 >= x2; x1--, e += dy)
            {
                draw_pixel(x1, y1, rgb, z_order);
                if (e>0) { y1++; e -= dx; }
            }
        }
        else
        {
            e = dx - dy / 2;
            for(; y1 <= y2; y1++, e += dx)
            {
                draw_pixel(x1, y1, rgb, z_order);
                if (e>0) { x1--; e -= dy; }
            }
        }
    }

    else if ((dx < 0) && (dy < 0))
    {
        dx = -dx;
        dy = -dy;
        if (dx >= dy)
        {
            e = dy - dx / 2;
            for(; x1 >= x2; x1--, e += dy)
            {
                draw_pixel(x1, y1, rgb, z_order);
                if (e>0) { y1--; e -= dx; }
            }
        }
        else
        {
            e = dx - dy / 2;
            while (y1-- >= y2)
            for(; y1 >= y2; y1--, e += dx)
            {
                draw_pixel(x1, y1, rgb, z_order);
                if (e>0) { x1--; e -= dy; }
            }
        }
    }
}

void c_surface::draw_rect(int x0, int y0, int x1, int y1, unsigned int rgb, unsigned int z_order, unsigned int size)
{
    for (unsigned int offset = 0; offset < size; offset++)
    {
        draw_hline(x0 + offset, x1 - offset, y0 + offset, rgb, z_order);
        draw_hline(x0 + offset, x1 - offset, y1 - offset, rgb, z_order);
        draw_vline(x0 + offset, y0 + offset, y1 - offset, rgb, z_order);
        draw_vline(x1 - offset, y0 + offset, y1 - offset, rgb, z_order);
    }
}

int c_surface::set_frame_layer_visible_rect(c_rect& rect, unsigned int z_order)
{
    if (rect == m_frame_layers[z_order].visible_rect)
    {
        return 0;
    }
    if (rect.m_left < 0 || rect.m_left >= m_width ||
        rect.m_right < 0 || rect.m_right >= m_width ||
        rect.m_top < 0 || rect.m_top >= m_height ||
        rect.m_bottom < 0 || rect.m_bottom >=m_height)
    {
        ASSERT(false);
        return -1;
    }
    if (!(z_order > Z_ORDER_LEVEL_0 && z_order < Z_ORDER_LEVEL_MAX))
    {
        ASSERT(false);
        return -2;
    }
    if (z_order < m_top_zorder)
    {
        ASSERT(false);
        return -3;
    }
    m_top_zorder = (Z_ORDER_LEVEL)z_order;
    
    c_rect old_rect = m_frame_layers[z_order].visible_rect;
    //Recover the lower layer
    int src_zorder = (Z_ORDER_LEVEL)(z_order - 1);
    int display_width = m_display->get_width();
    int display_height = m_display->get_height();

    for (int y = old_rect.m_top; y <= old_rect.m_bottom; y++)
    {
        for (int x = old_rect.m_left; x <= old_rect.m_right; x++)
        {
            if (!rect.PtInRect(x, y))
            {
                unsigned int rgb = ((unsigned short*)(m_frame_layers[src_zorder].fb))[x + y * m_width];
                draw_pixel_on_fb(x, y, GL_RGB_16_to_32(rgb));
            }
        }
    }

    m_frame_layers[z_order].visible_rect = rect;
    if (rect.IsEmpty())
    {
        m_top_zorder = (Z_ORDER_LEVEL)(z_order - 1);
    }
    return 0;
}

int c_surface::flush_scrren(int left, int top, int right, int bottom)
{
    if(left < 0 || left >= m_width || right < 0 || right >= m_width ||
        top < 0 || top >= m_height || bottom < 0 || bottom >= m_height)
    {
        ASSERT(false);
    }

    if(!m_is_active || (0 == m_phy_fb) || (0 == m_fb))
    {
        return -1;
    }

    int display_width = m_display->get_width();
    int display_height = m_display->get_height();

    left = (left >= display_width) ? (display_width - 1) : left;
    right = (right >= display_width) ? (display_width - 1) : right;
    top = (top >= display_height) ? (display_height - 1) : top;
    bottom = (bottom >= display_height) ? (display_height - 1) : bottom;

    for (int y = top; y < bottom; y++)
    {
        void* s_addr = (char*)m_fb + ((y * m_width + left) * m_color_bytes);
        void* d_addr = (char*)m_phy_fb + ((y * display_width + left) * m_color_bytes);
        memcpy(d_addr, s_addr, (right - left) * m_color_bytes);
    }
    *m_phy_write_index = *m_phy_write_index + 1;
    return 0;
}

bool c_surface::is_valid(c_rect rect)
{
    if (rect.m_left < 0 || rect.m_top < 0)
    {
        return false;
    }

    if (rect.m_right >= m_width || rect.m_bottom >= m_height)
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////
void c_surface_no_fb::fill_rect_on_fb(int x0, int y0, int x1, int y1, unsigned int rgb)
{
    if (!m_gfx_op)
    {
        return;
    }
    if (m_gfx_op->fill_rect)
    {
        return m_gfx_op->fill_rect(x0, y0, x1, y1, rgb);
    }

    if (m_gfx_op->draw_pixel && m_is_active)
    {
        for (int y = y0; y <= y1; y++)
        {
            for (int x = x0; x <= x1; x++)
            {
                m_gfx_op->draw_pixel(x, y, rgb);
            }
        }
    }

    if (!m_fb) { return; }
    if(m_color_bytes == 4)
    {
        unsigned int *fb;
        for (int y = y0; y <= y1; y++)
        {
            fb = &((unsigned int*)m_fb)[y0 * m_width + x0];
            for (int x = x0; x <= x1; x++)
            {
                *fb++ = rgb;
            }
        }
    }
    else if (m_color_bytes == 2)
    {
        unsigned short *fb;
        rgb = GL_RGB_32_to_16(rgb);
        for (int y = y0; y <= y1; y++)
        {
            fb = &((unsigned short*)m_fb)[y0 * m_width + x0];
            for (int x = x0; x <= x1; x++)
            {
                *fb++ = rgb;
            }
        }
    }
}

void c_surface_no_fb::draw_pixel_on_fb(int x, int y, unsigned int rgb)
{
    if (m_gfx_op && m_gfx_op->draw_pixel && m_is_active)
    {
        m_gfx_op->draw_pixel(x, y, rgb);
    }

    if (!m_fb) { return; }
    if (m_color_bytes == 4)
    {
        ((unsigned int*)m_fb)[y * m_width + x] = rgb;
    }
    else if (m_color_bytes == 2)
    {
        ((unsigned short*)m_fb)[y * m_width + x] = GL_RGB_32_to_16(rgb);
    }
}

static const FONT_INFO* s_font_map[FONT_MAX];
static const BITMAP_INFO* s_bmp_map[BITMAP_MAX];
static unsigned int s_color_map[COLOR_MAX];

int c_theme::add_font(FONT_TYPE index, const FONT_INFO* font)
{
    if (index >= FONT_MAX)
    {
        ASSERT(false);
        return -1;
    }
    s_font_map[index] = font;
    return 0;
}

const FONT_INFO* c_theme::get_font(FONT_TYPE index)
{
    if (index >= FONT_MAX)
    {
        ASSERT(false);
        return 0;
    }
    return s_font_map[index];
}

int c_theme::add_bitmap(BITMAP_TYPE index, const BITMAP_INFO* bmp)
{
    if (index >= BITMAP_MAX)
    {
        ASSERT(false);
        return -1;
    }
    s_bmp_map[index] = bmp;
    return 0;
}

const BITMAP_INFO* c_theme::get_bmp(BITMAP_TYPE index)
{
    if (index >= BITMAP_MAX)
    {
        ASSERT(false);
        return 0;
    }
    return s_bmp_map[index];
}

int c_theme::add_color(COLOR_TYPE index, const unsigned int color)
{
    if (index >= COLOR_MAX)
    {
        ASSERT(false);
        return -1;
    }
    s_color_map[index] = color;
    return 0;
}

const unsigned int c_theme::get_color(COLOR_TYPE index)
{
    if (index >= COLOR_MAX)
    {
        ASSERT(false);
        return 0;
    }
    return s_color_map[index];
}

c_wnd::c_wnd(): m_status(STATUS_NORMAL), m_attr(ATTR_VISIBLE), m_parent(0), m_top_child(0), m_prev_sibling(0), m_next_sibling(0),
m_str(0), m_font_color(0), m_bg_color(0), m_resource_id(0), m_z_order(Z_ORDER_LEVEL_0), m_focus_child(0), m_surface(0)
{
    m_attr = (WND_ATTRIBUTION)(ATTR_VISIBLE | ATTR_FOCUS);
}

int c_wnd::connect(c_wnd *parent, unsigned short resource_id, const char* str,
           short x, short y, short width, short height, WND_TREE* p_child_tree )
{
    if(0 == resource_id)
    {
        ASSERT(false);
        return -1;
    }

    m_resource_id = resource_id;
    set_str(str);
    m_parent  = parent;
    m_status = STATUS_NORMAL;

    if (parent)
    {
        m_z_order = parent->m_z_order;
        m_surface = parent->m_surface;
    }
    if(0 == m_surface)
    {
        ASSERT(false);
        return -2;
    }

    /* (cs.x = x * 1024 / 768) for 1027*768=>800*600 quickly*/
    m_wnd_rect.m_left   = x;
    m_wnd_rect.m_top    = y;
    m_wnd_rect.m_right  = (x + width - 1);
    m_wnd_rect.m_bottom = (y + height - 1);

    c_rect rect;
    get_screen_rect(rect);
    ASSERT(m_surface->is_valid(rect));

    pre_create_wnd();

    if ( 0 != parent )
    {
        parent->add_child_2_tail(this);
    }

    if (load_child_wnd(p_child_tree) >= 0)
    {
        load_cmd_msg();
        on_init_children();
    }
    return 0;
}

int c_wnd::load_child_wnd(WND_TREE *p_child_tree)
{
    if (0 == p_child_tree)
    {
        return 0;
    }
    int sum = 0;

    WND_TREE* p_cur = p_child_tree;
    while(p_cur->p_wnd)
    {
        if (0 != p_cur->p_wnd->m_resource_id)
        {//This wnd has been used! Do not share!
            ASSERT(false);
            return -1;
        }
        else
        {
            p_cur->p_wnd->connect(this, p_cur->resource_id, p_cur->str,
                p_cur->x, p_cur->y, p_cur->width, p_cur->height,p_cur->p_child_tree);
        }
        p_cur++;
        sum++;
    }
    return sum;
}

c_wnd* c_wnd::connect_clone(c_wnd *parent, unsigned short resource_id, const char* str,
           short x, short y, short width, short height, WND_TREE* p_child_tree )
{
    if(0 == resource_id)
    {
        ASSERT(false);
        return 0;
    }

    c_wnd* wnd = clone();
    wnd->m_resource_id = resource_id;
    wnd->set_str(str);
    wnd->m_parent  = parent;
    wnd->m_status = STATUS_NORMAL;

    if (parent)
    {
        wnd->m_z_order =  parent->m_z_order;
        wnd->m_surface = parent->m_surface;
    }
    else
    {
        wnd->m_surface = m_surface;
    }
    if(0 == wnd->m_surface)
    {
        ASSERT(false);
        return 0;
    }

    /* (cs.x = x * 1024 / 768) for 1027*768=>800*600 quickly*/
    wnd->m_wnd_rect.m_left   = x;
    wnd->m_wnd_rect.m_top    = y;
    wnd->m_wnd_rect.m_right  = (x + width - 1);
    wnd->m_wnd_rect.m_bottom = (y + height - 1);

    c_rect rect;
    wnd->get_screen_rect(rect);
    ASSERT(wnd->m_surface->is_valid(rect));

    wnd->pre_create_wnd();
    
    if ( 0 != parent )
    {
        parent->add_child_2_tail(wnd);
    }

    if (wnd->load_clone_child_wnd(p_child_tree) >= 0)
    {
        wnd->load_cmd_msg();
        wnd->on_init_children();
    }
    return wnd;
}

int c_wnd::load_clone_child_wnd(WND_TREE *p_child_tree)
{
    if (0 == p_child_tree)
    {
        return 0;
    }
    int sum = 0;

    WND_TREE* p_cur = p_child_tree;
    while(p_cur->p_wnd)
    {
        p_cur->p_wnd->connect_clone(this, p_cur->resource_id, p_cur->str,
                                    p_cur->x, p_cur->y, p_cur->width, p_cur->height,p_cur->p_child_tree);
        p_cur++;
        sum++;
    }
    return sum;
}

void c_wnd::disconnect()
{
    if (0 == m_resource_id)
    {
        return;
    }

    if (0 != m_top_child)
    {
        c_wnd *child = m_top_child;
        c_wnd *next_child = 0;

        while (child)
        {
            next_child = child->m_next_sibling;
            child->disconnect();
            child = next_child;
        }
    }

    if (0 != m_parent)
    {
        m_parent->unlink_child(this);
    }
    m_focus_child = 0;
    m_resource_id = 0;
}

c_wnd* c_wnd::get_wnd_ptr(unsigned short id) const
{
    c_wnd *child = m_top_child;

    while ( child )
    {
        if ( child->get_id() == id )
        {
            break;
        }

        child = child->m_next_sibling;
    }

    return child;
}

void c_wnd::set_attr(WND_ATTRIBUTION attr)
{
    m_attr = attr;

    if ( ATTR_DISABLED == (attr & ATTR_DISABLED) )
    {
        m_status = STATUS_DISABLED;
    }
    else
    {
        if (m_status == STATUS_DISABLED)
        {
            m_status = STATUS_NORMAL;
        }
    }
}

int c_wnd::is_focus_wnd() const
{
    if ( (m_attr & ATTR_VISIBLE)
        && !(m_attr & ATTR_DISABLED)
        && (m_attr & ATTR_FOCUS))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void c_wnd::set_wnd_pos(short x, short y, short width, short height)
{
    m_wnd_rect.m_left   = x;
    m_wnd_rect.m_top    = y;
    m_wnd_rect.m_right  = x + width - 1;
    m_wnd_rect.m_bottom = y + height - 1;
}

void c_wnd::get_wnd_rect(c_rect &rect) const
{
    rect = m_wnd_rect;
}

void c_wnd::get_screen_rect(c_rect &rect) const
{
    rect.SetRect(0, 0, (m_wnd_rect.Width() - 1), (m_wnd_rect.Height() - 1));
    wnd2screen(rect);
}

void c_wnd::wnd2screen(int &x, int &y) const
{
    c_wnd *parent = m_parent;
    c_rect rect;

    x += m_wnd_rect.m_left;
    y += m_wnd_rect.m_top;

    while ( 0 != parent )
    {
        parent->get_wnd_rect(rect);
        x += rect.m_left;
        y += rect.m_top;

        parent = parent->m_parent;
    }
}

void c_wnd::wnd2screen(c_rect &rect) const
{
    int l = rect.m_left;
    int t = rect.m_top;
    wnd2screen(l, t);

    int r = (l + rect.Width() - 1);
    int b = (t + rect.Height() - 1);
    rect.SetRect(l, t, r, b);
}

void c_wnd::screen2wnd(short &x, short &y) const
{
    c_wnd *parent = m_parent;
    c_rect rect;

    x -= m_wnd_rect.m_left;
    y -= m_wnd_rect.m_top;

    while ( 0 != parent )
    {
        parent->get_wnd_rect(rect);
        x -= rect.m_left;
        y -= rect.m_top;
        parent = parent->m_parent;
    }
}

void c_wnd::screen2wnd(c_rect &rect) const
{
    short l = rect.m_left;
    short t = rect.m_top;
    screen2wnd(l, t);

    short r = l + rect.Width() - 1;
    short b = t + rect.Height() - 1;
    rect.SetRect(l, t, r, b);
}

c_wnd* c_wnd::set_child_focus(c_wnd * focus_child)
{
    ASSERT(0 != focus_child);
    ASSERT(focus_child->m_parent == this);

    c_wnd *old_focus_child = m_focus_child;
    if (focus_child->is_focus_wnd())
    {
        if (focus_child != old_focus_child )
        {
            if (old_focus_child)
            {
                old_focus_child->on_kill_focus();
            }
            m_focus_child = focus_child;

            if (m_parent)
            {
                m_parent->set_child_focus(this);
            }
            m_focus_child->on_focus();
        }
    }
    return m_focus_child;
}

void c_wnd::add_child_2_tail(c_wnd *child)
{
    if( 0 == child )return;
    if(child == get_wnd_ptr(child->m_resource_id))return;

    if ( 0 == m_top_child )
    {
        m_top_child = child;
        child->m_prev_sibling = 0;
        child->m_next_sibling = 0;
    }
    else
    {
        c_wnd *last_child = get_last_child();
        if (0 == last_child)
        {
            ASSERT(false);
        }
        last_child->m_next_sibling = child;
        child->m_prev_sibling = last_child;
        child->m_next_sibling = 0;
    }
}

c_wnd* c_wnd::get_last_child() const
{
    if ( 0 == m_top_child )
    {
        return 0;
    }

    c_wnd *child = m_top_child;

    while ( child->m_next_sibling)
    {
        child = child->m_next_sibling;
    }

    return child;
}

int c_wnd::unlink_child(c_wnd *child)
{
    if ((0 == child)
        || (this != child->m_parent))
    {
        return -1;
    }

    if (0 == m_top_child)
    {
        return -2;
    }

    int find = false;

    c_wnd *tmp_child = m_top_child;
    if (tmp_child == child)
    {
        m_top_child = child->m_next_sibling;
        if (0 != child->m_next_sibling)
        {
            child->m_next_sibling->m_prev_sibling = 0;
        }

        find = true;
    }
    else
    {
        while (tmp_child->m_next_sibling)
        {
            if (child == tmp_child->m_next_sibling)
            {
                tmp_child->m_next_sibling = child->m_next_sibling;
                if (0 != child->m_next_sibling)
                {
                    child->m_next_sibling->m_prev_sibling = tmp_child;
                }

                find = true;
                break;
            }

            tmp_child = tmp_child->m_next_sibling;
        }
    }

    if (true == find)
    {
        if (m_focus_child == child)
        {
            m_focus_child = 0;
        }

        child->m_next_sibling = 0;
        child->m_prev_sibling = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

void c_wnd::show_window()
{
    if (ATTR_VISIBLE == (m_attr & ATTR_VISIBLE))
    {
        on_paint();
        c_wnd *child = m_top_child;
        if ( 0 != child )
        {
            while ( child )
            {
                child->show_window();
                child = child->m_next_sibling;
            }
        }
    }
}

bool c_wnd::on_touch(int x, int y, TOUCH_ACTION action)
{
    c_rect rect;
    x -= m_wnd_rect.m_left;
    y -= m_wnd_rect.m_top;
    c_wnd* child = m_top_child;

    c_wnd* target_wnd = 0;
    int target_z_order = Z_ORDER_LEVEL_0;

    while (child)
    {
        if (ATTR_VISIBLE == (child->m_attr & ATTR_VISIBLE))
        {
            child->get_wnd_rect(rect);
            if (true == rect.PtInRect(x, y) || child->m_attr & ATTR_MODAL)
            {
                if (true == child->is_focus_wnd())
                {
                    if (child->m_z_order >= target_z_order)
                    {
                        target_wnd = child;
                        target_z_order = child->m_z_order;
                    }
                }
            }
        }
        child = child->m_next_sibling;
    }

    if (target_wnd == 0)
    {
        return false;
    }
    return target_wnd->on_touch(x, y, action);
}

bool c_wnd::on_key(KEY_TYPE key)
{
    ASSERT(key == KEY_FORWARD || key == KEY_BACKWARD || key == KEY_ENTER);

    // Find current focus wnd.
    c_wnd* old_focus_wnd = m_focus_child;
    while (m_focus_child && m_focus_child->m_focus_child)
    {
        old_focus_wnd = m_focus_child->m_focus_child;
    }
    if (old_focus_wnd && !old_focus_wnd->on_key(key))
    {
        return true;
    }

    // Default moving focus(Default handle KEY_FOWARD/KEY_BACKWARD)
    if (key == KEY_ENTER)
    {
        return true;
    }
    if (!old_focus_wnd)
    {// No current focus wnd, new one.
        c_wnd *child = m_top_child;
        c_wnd *new_focus_wnd = 0;
        while (child)
        {
            if (ATTR_VISIBLE == (child->m_attr & ATTR_VISIBLE))
            {
                if (true == child->is_focus_wnd())
                {
                    new_focus_wnd = child;
                    new_focus_wnd->m_parent->set_child_focus(new_focus_wnd);
                    child = child->m_top_child;
                    continue;
                }
            }
            child = child->m_next_sibling;
        }
        return true;
    }

    // Move focus from old wnd to next wnd
    c_wnd* next_focus_wnd = (key == KEY_FORWARD) ? old_focus_wnd->m_next_sibling : old_focus_wnd->m_prev_sibling;
    while (next_focus_wnd && (!next_focus_wnd->is_focus_wnd()))
    {// Search neighbor of old focus wnd
        next_focus_wnd = (key == KEY_FORWARD) ? next_focus_wnd->m_next_sibling : next_focus_wnd->m_prev_sibling;
    }
    if (!next_focus_wnd)
    {// Search whole brother wnd
        next_focus_wnd = (key == KEY_FORWARD) ? old_focus_wnd->m_parent->m_top_child : old_focus_wnd->m_parent->get_last_child();
        while (next_focus_wnd && (!next_focus_wnd->is_focus_wnd()))
        {
            next_focus_wnd = (key == KEY_FORWARD) ? next_focus_wnd->m_next_sibling : next_focus_wnd->m_prev_sibling;
        }
    }
    if (next_focus_wnd)
    {
        next_focus_wnd->m_parent->set_child_focus(next_focus_wnd);
    }
    return true;
}

void c_wnd::notify_parent(unsigned int msg_id, unsigned int ctrl_id, int param)
{
    if (!m_parent)
    {
        return;
    }
    const GL_MSG_ENTRY* entry = m_parent->FindMsgEntry(m_parent->GetMSgEntries(), MSG_TYPE_WND, msg_id, ctrl_id);
    if (0 == entry)
    {
        return;
    }

    MSGFUNCS msg_funcs;
    msg_funcs.func = entry->func;

    switch (entry->callbackType)
    {
    case MSG_CALLBACK_VV:
        (m_parent->*msg_funcs.func)();
        break;
    case MSG_CALLBACK_VVL:
        (m_parent->*msg_funcs.func_vvl)(param);
        break;
    case MSG_CALLBACK_VWV:
        (m_parent->*msg_funcs.func_vwv)(ctrl_id);
        break;
    case MSG_CALLBACK_VWL:
        (m_parent->*msg_funcs.func_vwl)(ctrl_id, param);
        break;
    default:
        ASSERT(false);
        break;
    }
}

#define BUFFER_LEN  16
unsigned char s_utf8_length_table[256] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};

inline static int get_utf8_code(const char* s, unsigned int& output_utf8_code)
{
    unsigned char* us = (unsigned char*)s;
    int utf8_bytes = s_utf8_length_table[*us];
    switch (utf8_bytes)
    {
    case 1:
        output_utf8_code = *us;
        break;
    case 2:
        output_utf8_code = (*us << 8) | (*(us + 1));
        break;
    case 3:
        output_utf8_code = (*us << 16) | ((*(us + 1)) << 8) | *(us + 2);
        break;
    case 4:
        output_utf8_code = (*us << 24) | ((*(us + 1)) << 16) | (*(us + 2) << 8) | *(us + 3);
        break;
    default:
        ASSERT(false);
        break;
    }
    return utf8_bytes;
}

void c_word::draw_value_in_rect(c_surface* surface, int z_order, int value, int dot_position, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
    char buf[BUFFER_LEN];
    value_2_string(value, dot_position, buf, BUFFER_LEN);
    draw_string_in_rect(surface, z_order, buf, rect, font, font_color, bg_color, align_type);
}

void c_word::draw_value(c_surface* surface, int z_order, int value, int dot_position, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
    char buf[BUFFER_LEN];
    value_2_string(value, dot_position, buf, BUFFER_LEN);
    draw_string(surface, z_order, buf, x, y, font, font_color, bg_color, align_type);
}

void c_word::draw_string_in_rect(c_surface* surface, int z_order, const char *s, c_rect rect, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
    if(0 == s)
    {
        return;
    }
    int x, y;
    get_string_pos(s, font, rect, align_type, x, y);
    draw_string(surface, z_order, s, rect.m_left + x, rect.m_top + y, font, font_color, bg_color, ALIGN_LEFT);
}

void c_word::draw_string(c_surface* surface, int z_order, const char *s, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color, unsigned int align_type)
{
    if (0 == s)
    {
        return;
    }

    int offset = 0;
    unsigned int utf8_code;
    while (*s)
    {
        s += get_utf8_code(s, utf8_code);
        offset += draw_single_char(surface, z_order, utf8_code, (x + offset), y, font, font_color, bg_color);
    }
}

void c_word::value_2_string(int value, int dot_position, char* buf, int len)
{
    memset(buf, 0, len);
    switch (dot_position)
    {
    case 0:
        sprintf(buf, "%d", value);
        break;
    case 1:
        sprintf(buf, "%.1f", value*1.0 / 10);
        break;
    case 2:
        sprintf(buf, "%.2f", value*1.0 / 100);
        break;
    case 3:
        sprintf(buf, "%.3f", value*1.0 / 1000);
        break;
    default:
        ASSERT(false);
        break;
    }
}

const LATTICE* c_word::get_lattice(const FONT_INFO* font, unsigned int utf8_code)
{
    int first = 0;
    int last = font->count - 1;
    int middle = (first + last) / 2;

    while (first <= last) 
    {
        if (font->lattice_array[middle].utf8_code < utf8_code)
            first = middle + 1;
        else if (font->lattice_array[middle].utf8_code == utf8_code)
        {
            return &font->lattice_array[middle];
        }
        else
        {
            last = middle - 1;
        }
        middle = (first + last) / 2;
    }
    return 0;
}

int c_word::draw_single_char(c_surface* surface, int z_order, unsigned int utf8_code, int x, int y, const FONT_INFO* font, unsigned int font_color, unsigned int bg_color)
{
    unsigned int error_color = 0xFFFFFFFF;
    if (font)
    {
        const LATTICE* p_lattice = get_lattice(font, utf8_code);
        if (p_lattice)
        {
            draw_lattice(surface, z_order, x, y, p_lattice->width, font->height, p_lattice->p_data, font_color, bg_color);
            return p_lattice->width;
        }
    }
    else
    {
        error_color = GL_RGB(255, 0, 0);
    }
    
    //lattice/font not found, draw "X"
    int len = 16;
    for (int y_ = 0; y_ < len; y_++)
    {
        for (int x_ = 0; x_ < len; x_++)
        {
            int diff = (x_ - y_);
            int sum = (x_ + y_);
            (diff == 0 || diff == -1 || diff == 1 || sum == len || sum == (len - 1) || sum == (len + 1)) ?
            surface->draw_pixel((x + x_), (y + y_), error_color, z_order) : surface->draw_pixel((x + x_), (y + y_), 0, z_order);
        }
    }
    return len;
}

void c_word::draw_lattice(c_surface* surface, int z_order, int x, int y, int width, int height,
                        const unsigned char* p_data, unsigned int font_color, unsigned int bg_color)
{
    unsigned int r, g, b, rgb;
    unsigned char blk_value = *p_data++;
    unsigned char blk_cnt = *p_data++;
    b = (GL_RGB_B(font_color) * blk_value + GL_RGB_B(bg_color) * (255 - blk_value)) >> 8;
    g = (GL_RGB_G(font_color) * blk_value + GL_RGB_G(bg_color) * (255 - blk_value)) >> 8;
    r = (GL_RGB_R(font_color) * blk_value + GL_RGB_R(bg_color) * (255 - blk_value)) >> 8;
    rgb = GL_RGB(r, g, b);
    for (int y_ = 0; y_ < height; y_++)
    {
        for (int x_ = 0; x_ < width; x_++)
        {
            ASSERT(blk_cnt);
            if (0x00 == blk_value)
            {
                if (GL_ARGB_A(bg_color))
                {
                    surface->draw_pixel(x + x_, y + y_, bg_color, z_order);
                }
            }
            else
            {
                surface->draw_pixel((x + x_), (y + y_), rgb, z_order);
            }
            if (--blk_cnt == 0)
            {//reload new block
                blk_value = *p_data++;
                blk_cnt = *p_data++;
                b = (GL_RGB_B(font_color) * blk_value + GL_RGB_B(bg_color) * (255 - blk_value)) >> 8;
                g = (GL_RGB_G(font_color) * blk_value + GL_RGB_G(bg_color) * (255 - blk_value)) >> 8;
                r = (GL_RGB_R(font_color) * blk_value + GL_RGB_R(bg_color) * (255 - blk_value)) >> 8;
                rgb = GL_RGB(r, g, b);
            }
        }
    }
}

int c_word::get_str_size(const char *s, const FONT_INFO* font, int& width, int& height)
{
    if(0 == s || 0 == font)
    {
        width = height = 0;
        return -1;
    }

    int lattice_width = 0;
    unsigned int utf8_code;
    int utf8_bytes;
    while (*s)
    {
        utf8_bytes = get_utf8_code(s, utf8_code);
        const LATTICE* p_lattice = get_lattice(font, utf8_code);
        lattice_width += p_lattice?p_lattice->width:font->height;
        s += utf8_bytes;
    }
    width = lattice_width;
    height = font->height;
    return 0;
}

void c_word::get_string_pos(const char *s, const FONT_INFO* font, c_rect rect, unsigned int align_type, int &x, int &y)
{
    int x_size, y_size;
    get_str_size(s, font, x_size, y_size);

    int height = rect.m_bottom - rect.m_top + 1;
    int width  = rect.m_right - rect.m_left + 1;

    x = y = 0;

    switch (align_type & ALIGN_HMASK)
    {
    case ALIGN_HCENTER:
        //m_text_org_x=0
        if (width > x_size)
        {
            x = (width - x_size)/2;
        }
        break;

    case ALIGN_LEFT:
        x = 0;
        break;

    case ALIGN_RIGHT:
        //m_text_org_x=0
        if (width > x_size)
        {
            x = width - x_size;
        }
        break;

    default:
        ASSERT(0);
        break;
    }

    switch (align_type & ALIGN_VMASK)
    {
    case ALIGN_VCENTER:
        //m_text_org_y=0
        if (height > y_size)
        {
            y = (height - y_size)/2;
        }
        break;

    case ALIGN_TOP:
        y = 0;
        break;

    case ALIGN_BOTTOM:
        //m_text_org_y=0
        if (height > y_size)
        {
            y = height - y_size;
        }
        break;

    default:
        ASSERT(0);
        break;
    }
}

static void(*do_assert)(const char* file, int line);
static void(*do_log_out)(const char* log);
void register_debug_function(void(*my_assert)(const char* file, int line), void(*my_log_out)(const char* log))
{
    do_assert = my_assert;
    do_log_out = my_log_out;
}

void _assert(const char* file, int line)
{
    if(do_assert)
    {
        do_assert(file, line);
    }
    while(1);
}

void log_out(const char* log)
{
    if (do_log_out)
    {
        do_log_out(log);
    }
}

long get_time_in_second()
{
    return 0;
}

T_TIME second_to_day(long second)
{
    T_TIME ret = {0};
    return ret;
}

T_TIME get_time()
{
    T_TIME ret = {0};
    return ret;
}

void start_real_timer(void (*func)(void* arg))
{
    log_out("Not support now");
}

void register_timer(int milli_second, void func(void* ptmr, void* parg))
{
    log_out("Not support now");
}

unsigned int get_cur_thread_id()
{
    log_out("Not support now");
    return 0;
}

void create_thread(unsigned long* thread_id, void* attr, void *(*start_routine) (void *), void* arg)
{
    log_out("Not support now");
}

extern "C" void delay_ms(unsigned short nms);
void thread_sleep(unsigned int milli_seconds)
{//MCU alway implemnet driver code in APP.
        //delay_ms(milli_seconds);
}

int build_bmp(const char *filename, unsigned int width, unsigned int height, unsigned char *data)
{
    log_out("Not support now");
    return 0;
}

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

c_wave_buffer::c_wave_buffer()
{
	m_head = m_tail = m_min_old = m_max_old =
	m_min_older = m_max_older = m_last_data = m_read_cache_sum = m_refresh_sequence = 0;
	memset(m_wave_buf, 0, sizeof(m_wave_buf));
	memset(m_read_cache_min, 0, sizeof(m_read_cache_min));
	memset(m_read_cache_mid, 0, sizeof(m_read_cache_mid));
	memset(m_read_cache_max, 0, sizeof(m_read_cache_max));
}

short c_wave_buffer::get_cnt()
{
	return (m_tail >= m_head)?(m_tail - m_head):(m_tail - m_head + WAVE_BUFFER_LEN);
}

int c_wave_buffer::write_wave_data(short data)
{
	if ((m_tail + 1) % WAVE_BUFFER_LEN == m_head)
	{//full
		//log_out("wave buf full\n");
		return BUFFER_FULL;
	}
	m_wave_buf[m_tail] = data;
	m_tail = (m_tail + 1) % WAVE_BUFFER_LEN;
	return 1;
}

int c_wave_buffer::read_data()
{
	if (m_head == m_tail)
	{//empty
		//log_out("wave buf empty\n");
		return BUFFER_EMPTY;
	}
	int ret = m_wave_buf[m_head];
	m_head = (m_head + 1) % WAVE_BUFFER_LEN;
	return ret;
}

int c_wave_buffer::read_wave_data_by_frame(short &max, short &min, short frame_len, unsigned int sequence, short offset)
{
	if (m_refresh_sequence != sequence)
	{
		m_refresh_sequence = sequence;
		m_read_cache_sum = 0;
	}
	else if(offset < m_read_cache_sum)//(m_refresh_sequence == sequence && offset < m_fb_sum)
	{
		max = m_read_cache_max[offset];
		min = m_read_cache_min[offset];
		return m_read_cache_mid[offset];
	}
	
	m_read_cache_sum++;
	ASSERT(m_read_cache_sum <= WAVE_READ_CACHE_LEN);
	int i, data;
	int tmp_min = m_last_data;
	int tmp_max = m_last_data;
	int mid = (m_min_old + m_max_old)>>1;

	i = 0;
	while(i++ < frame_len)
	{
		data = read_data();
		if(BUFFER_EMPTY == data)
		{
			break;
		}
		m_last_data = data;

		if(data < tmp_min){tmp_min = data;}
		if(data > tmp_max){tmp_max = data;}
	}

	min = m_read_cache_min[offset] = MIN(m_min_old, MIN(tmp_min, m_min_older));
	max = m_read_cache_max[offset] = MAX(m_max_old, MAX(tmp_max, m_max_older));

	m_min_older = m_min_old;
	m_max_older = m_max_old;
	m_min_old = tmp_min;
	m_max_old = tmp_max;
	return (m_read_cache_mid[offset] = mid);
}

void c_wave_buffer::clear_data()
{
	m_head = m_tail = 0;
	memset(m_wave_buf, 0, sizeof(m_wave_buf));
}

void c_wave_buffer::reset()
{
	m_head = m_tail;
}

#define CORRECT(x, high_limit, low_limit)	{\
	x = (x > high_limit) ? high_limit : x;\
	x = (x < low_limit) ? low_limit : x;\
}while(0)

#define WAVE_CURSOR_WIDTH		8
#define	WAVE_LINE_WIDTH			1
#define	WAVE_MARGIN				5

c_wave_ctrl::c_wave_ctrl()
{
	m_wave = 0;
	m_bg_fb = 0;
	m_wave_name_font  = m_wave_unit_font = 0;
	m_wave_name = m_wave_unit = 0;
	m_max_data = 500;
	m_min_data = 0;
	m_wave_speed = 1;
	m_wave_data_rate = 0;
	m_wave_refresh_rate = 1000;
	m_frame_len_map_index = 0;

	m_wave_name_color  = m_wave_unit_color = m_wave_color = GL_RGB(255,0,0);
	m_back_color = GL_RGB(0,0,0);
}

void c_wave_ctrl::on_init_children()
{
	c_rect rect;
	get_screen_rect(rect);

	m_wave_left 	= rect.m_left + WAVE_MARGIN;
	m_wave_right	= rect.m_right - WAVE_MARGIN;
	m_wave_top		= rect.m_top + WAVE_MARGIN;
	m_wave_bottom	= rect.m_bottom - WAVE_MARGIN;
	m_wave_cursor	= m_wave_left;

	m_bg_fb = (unsigned int*)calloc(rect.Width() * rect.Height(), 4);
}

void c_wave_ctrl::set_max_min(short max_data, short min_data)
{
	m_max_data = max_data;
	m_min_data = min_data;
}

void c_wave_ctrl::set_wave_in_out_rate(unsigned int data_rate, unsigned int refresh_rate)
{
	m_wave_data_rate = data_rate;
	m_wave_refresh_rate = refresh_rate;
	int read_times_per_second = m_wave_speed * 1000 / m_wave_refresh_rate;

	memset(m_frame_len_map, 0, sizeof(m_frame_len_map));
	for (unsigned int i = 1; i < sizeof(m_frame_len_map) + 1; i++)
	{
		m_frame_len_map[i-1] = data_rate * i / read_times_per_second - data_rate * (i-1) / read_times_per_second;
	}
	m_frame_len_map_index = 0;
}

void c_wave_ctrl::set_wave_speed(unsigned int speed)
{
	m_wave_speed = speed;
	set_wave_in_out_rate(m_wave_data_rate, m_wave_refresh_rate);
}

void c_wave_ctrl::clear_data()
{
	if(m_wave == 0)
	{
		ASSERT(false);
		return;
	}
	m_wave->clear_data();
}

bool c_wave_ctrl::is_data_enough()
{
	if(m_wave == 0)
	{
		ASSERT(false);
		return false;
	}
	return (m_wave->get_cnt() - m_frame_len_map[m_frame_len_map_index] * m_wave_speed);
}

void c_wave_ctrl::refresh_wave(unsigned char frame)
{
	if(m_wave == 0)
	{
		ASSERT(false);
		return;
	}

	short max, min, mid;
	for(short offset = 0; offset < m_wave_speed; offset++)
	{
		//get wave value
		mid = m_wave->read_wave_data_by_frame(max, min,
							m_frame_len_map[m_frame_len_map_index++],
							frame, offset);
		m_frame_len_map_index %= sizeof(m_frame_len_map);
		
		//map to wave ctrl
		int y_min,y_max;
		if(m_max_data == m_min_data)
		{
			ASSERT(false);
		}
		y_max = m_wave_bottom + WAVE_LINE_WIDTH - (m_wave_bottom - m_wave_top)*(min - m_min_data)/(m_max_data - m_min_data);
		y_min = m_wave_bottom - WAVE_LINE_WIDTH - (m_wave_bottom - m_wave_top)*(max - m_min_data)/(m_max_data - m_min_data);
		mid = m_wave_bottom - (m_wave_bottom - m_wave_top)*(mid - m_min_data)/(m_max_data - m_min_data);

		CORRECT(y_min, m_wave_bottom, m_wave_top);
		CORRECT(y_max, m_wave_bottom, m_wave_top);
		CORRECT(mid, m_wave_bottom, m_wave_top);

		if (m_wave_cursor > m_wave_right)
		{
			m_wave_cursor = m_wave_left;
		}
		draw_smooth_vline(y_min, y_max, mid, m_wave_color);
		restore_background();
		m_wave_cursor++;
	}
}

void c_wave_ctrl::draw_smooth_vline(int y_min, int y_max, int mid, unsigned int rgb)
{
	int dy = y_max - y_min;
	short r = GL_RGB_R(rgb);
	short g = GL_RGB_G(rgb);
	short b = GL_RGB_B(rgb);
	int  index = (dy >> 1) + 2;
	int  y;

	m_surface->draw_pixel(m_wave_cursor, mid, rgb, m_z_order);

	if (dy < 1)
	{
		return;
	}

	unsigned char cur_r,cur_g,cur_b;
	unsigned int cur_rgb;
	for (int i = 1; i <= (dy >> 1) + 1; ++i )
	{
		if ( (mid + i) <= y_max )
		{
			y = mid + i;
			cur_r = r * (index - i) / index;
			cur_g = g * (index - i) / index;
			cur_b = b * (index - i) / index;
			cur_rgb = GL_RGB(cur_r, cur_g, cur_b);
			m_surface->draw_pixel(m_wave_cursor, y, cur_rgb, m_z_order);
		}
		if ( (mid - i) >= y_min )
		{
			y = mid - i;
			cur_r = r * (index - i) / index;
			cur_g = g * (index - i) / index;
			cur_b = b * (index - i) / index;
			cur_rgb = GL_RGB(cur_r, cur_g, cur_b);
			m_surface->draw_pixel(m_wave_cursor, y, cur_rgb, m_z_order);
		}
	}
}

void c_wave_ctrl::on_paint()
{
	c_rect rect;
	get_screen_rect(rect);
	
	m_surface->fill_rect(rect.m_left, rect.m_top, rect.m_right, rect.m_bottom, m_back_color, m_z_order);
	
	//show name
	c_word::draw_string(m_surface, m_z_order, m_wave_name, m_wave_left + 10, rect.m_top, m_wave_name_font, m_wave_name_color, GL_ARGB(0, 0, 0, 0), ALIGN_LEFT);
	//show unit
	c_word::draw_string(m_surface, m_z_order, m_wave_unit, m_wave_left + 60, rect.m_top, m_wave_unit_font, m_wave_unit_color, GL_ARGB(0, 0, 0, 0), ALIGN_LEFT);

	save_background();
}

void c_wave_ctrl::clear_wave(void)
{
	m_surface->fill_rect(m_wave_left, m_wave_top, m_wave_right, m_wave_bottom, m_back_color, m_z_order);
	m_wave_cursor = m_wave_left;
}

void c_wave_ctrl::restore_background()
{
	int  x = m_wave_cursor + WAVE_CURSOR_WIDTH;
	if (x > m_wave_right)
	{
		x -= (m_wave_right - m_wave_left + 1);
	}

	c_rect rect;
	get_screen_rect(rect);
	register int width = rect.Width();
	register int top = rect.m_top;
	register int left = rect.m_left;
	for (int y_pos = (m_wave_top - 1); y_pos <= (m_wave_bottom + 1); y_pos++)
	{
		(m_bg_fb) ? m_surface->draw_pixel(x, y_pos, m_bg_fb[(y_pos - top) * width + (x - left)], m_z_order) : m_surface->draw_pixel(x, y_pos, 0, m_z_order);
	}
}

void c_wave_ctrl::save_background()
{
	if (!m_bg_fb)
	{
		return;
	}
	c_rect rect;
	get_screen_rect(rect);
	
	register unsigned int* p_des = m_bg_fb;
	for (int y = rect.m_top; y <= rect.m_bottom; y++)
	{
		for (int x = rect.m_left; x <= rect.m_right; x++)
		{
			*p_des++ = m_surface->get_pixel(x, y, m_z_order);
		}
	}
}

//////////////////////// start UI ////////////////////////
const int UI_WIDTH = 240;
const int UI_HEIGHT = 320;
const unsigned char	s_wave_data1[] =
{
	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,
	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,
	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,
	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7E, 0x7E, 0x7F, 0x81, 0x81, 0x82, 0x82, 0x83,	0x84, 0x84, 0x84, 0x84, 0x83, 0x82, 0x82, 0x81,
	0x81, 0x7F, 0x7F, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7C, 0x7B, 0x7A, 0x79, 0x7A, 0x82,
	0x89, 0x91, 0x98, 0x9F, 0xA2, 0x9B, 0x94, 0x8C,	0x85, 0x7E, 0x78, 0x79, 0x7A, 0x7B, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,
	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D,	0x7E, 0x7F, 0x7F, 0x81, 0x81, 0x82, 0x83, 0x84,	0x84, 0x85, 0x86, 0x87, 0x87, 0x87, 0x88, 0x88,
	0x89, 0x89, 0x89, 0x8A, 0x8A, 0x8A, 0x8A, 0x89,	0x89, 0x88, 0x88, 0x87, 0x87, 0x85, 0x85, 0x85,	0x83, 0x82, 0x81, 0x81, 0x7F, 0x7E, 0x7E, 0x7D,	0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D, 0x7D
};
const char s_wave_data2[] =
{
	0x21, 0x22, 0x23, 0x22, 0x21, 0x20, 0x1E, 0x1C,	0x1B, 0x19, 0x18, 0x16, 0x14, 0x12, 0x11, 0x10,	0x0F, 0x0D, 0x0C, 0x0A, 0x09, 0x07, 0x06, 0x05,	0x04, 0x03, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x06, 0x0E, 0x18, 0x23, 0x2D, 0x37, 0x40,	0x49, 0x51, 0x57, 0x5A, 0x5A, 0x59, 0x55, 0x50,	0x49, 0x42, 0x3B, 0x34, 0x2F, 0x29, 0x24, 0x20,	0x1F, 0x1e, 0x1d, 0x1f
};
const unsigned char s_wave_data3[] =
{
	0x53, 0x53, 0x53, 0x52, 0x53, 0x53, 0x52, 0x52,	0x52, 0x52, 0x52, 0x52, 0x52, 0x52, 0x52, 0x52,	0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53, 0x53,	0x53, 0x54, 0x54, 0x54, 0x54, 0x55, 0x55, 0x55,
	0x55, 0x56, 0x57, 0x57, 0x58, 0x59, 0x5A, 0x5B,	0x5C, 0x5D, 0x5F, 0x60, 0x62, 0x64, 0x66, 0x68,	0x6A, 0x6C, 0x6E, 0x70, 0x72, 0x75, 0x77, 0x79,	0x7B, 0x7D, 0x80, 0x82, 0x84, 0x87, 0x89, 0x8B,
	0x8D, 0x8F, 0x91, 0x93, 0x94, 0x96, 0x97, 0x98,	0x9A, 0x9B, 0x9C, 0x9C, 0x9D, 0x9E, 0x9E, 0x9E,	0x9E, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F, 0x9F,	0x9F, 0x9E, 0x9F, 0x9E, 0x9E, 0x9E, 0x9E, 0x9E,
	0x9E, 0x9E, 0x9D, 0x9D, 0x9D, 0x9D, 0x9C, 0x9C,	0x9C, 0x9C, 0x9B, 0x9B, 0x9A, 0x9A, 0x9A, 0x99,	0x99, 0x98, 0x98, 0x98, 0x97, 0x97, 0x96, 0x96,	0x96, 0x95, 0x95, 0x94, 0x94, 0x93, 0x93, 0x92,
	0x92, 0x91, 0x91, 0x90, 0x90, 0x8F, 0x8F, 0x8E,	0x8E, 0x8D, 0x8D, 0x8C, 0x8B, 0x8B, 0x8A, 0x8A,	0x89, 0x89, 0x88, 0x88, 0x87, 0x87, 0x86, 0x85,	0x85, 0x85, 0x84, 0x83, 0x83, 0x82, 0x82, 0x81,
	0x81, 0x80, 0x7F, 0x7F, 0x7E, 0x7D, 0x7D, 0x7C,	0x7B, 0x7B, 0x7A, 0x7A, 0x79, 0x78, 0x77, 0x77,	0x76, 0x75, 0x74, 0x74, 0x73, 0x72, 0x71, 0x71,	0x70, 0x6F, 0x6F, 0x6E, 0x6D, 0x6D, 0x6C, 0x6B,
	0x6A, 0x6A, 0x69, 0x69, 0x68, 0x67, 0x66, 0x66,	0x65, 0x65, 0x64, 0x64, 0x63, 0x63, 0x62, 0x62,	0x61, 0x61, 0x61, 0x60, 0x60, 0x5F, 0x5F, 0x5F,	0x5E, 0x5E, 0x5D, 0x5D, 0x5D, 0x5C, 0x5C, 0x5C,
	0x5B, 0x5B, 0x5B, 0x5B, 0x5B, 0x5A, 0x5A, 0x5A,	0x59, 0x59, 0x59, 0x58, 0x58, 0x57, 0x57, 0x57,	0x56, 0x56, 0x55, 0x55, 0x55, 0x55, 0x54, 0x54,	0x54, 0x54, 0x54, 0x54, 0x53, 0x54, 0x53, 0x53,
};

//////////////////////// define widgets & map message ////////////////////////
enum WND_ID
{
	ID_ROOT = 1,
	ID_WAVE1,
	ID_WAVE2,
	ID_WAVE3
};

class c_myUI : public c_wnd
{
	virtual c_wnd* clone() { return new c_myUI(); }
	virtual void on_init_children(void) {
		c_wave_ctrl *p_wave = (c_wave_ctrl*)get_wnd_ptr(ID_WAVE1);
		p_wave->set_wave_speed(1);
		p_wave->set_wave_color(GL_RGB(0, 255, 0));
		p_wave->set_wave_in_out_rate(256, 17);
		p_wave->set_wave(&m_wave_buffer1);
		p_wave->set_max_min(170, 110);

		p_wave = (c_wave_ctrl*)get_wnd_ptr(ID_WAVE2);
		p_wave->set_wave_speed(1);
		p_wave->set_wave_color(GL_RGB(41, 189, 189));
		p_wave->set_wave_in_out_rate(60, 17);
		p_wave->set_wave(&m_wave_buffer2);
		p_wave->set_max_min(100, 0);

		p_wave = (c_wave_ctrl*)get_wnd_ptr(ID_WAVE3);
		p_wave->set_wave_speed(1);
		p_wave->set_wave_color(GL_RGB(255, 255, 0));
		p_wave->set_wave_in_out_rate(128, 17);
		p_wave->set_wave(&m_wave_buffer3);
		p_wave->set_max_min(180, 80);
	}
	virtual void on_paint(void)
	{
		c_rect rect;
		get_screen_rect(rect);
		m_surface->fill_rect(rect, GL_RGB(33, 33, 33), m_z_order);
	}
public:
	c_wave_buffer m_wave_buffer1;
	c_wave_buffer m_wave_buffer2;
	c_wave_buffer m_wave_buffer3;
};

//////////////////////// layout UI ////////////////////////
static c_myUI s_myUI;
static c_wave_ctrl s_wave1;
static c_wave_ctrl s_wave2;
static c_wave_ctrl s_wave3;
static WND_TREE s_myUI_children[] =
{
	{&s_wave1, ID_WAVE1, 0, 0, 0, 240, 90, NULL},
	{&s_wave2, ID_WAVE2, 0, 0, 95, 240, 90, NULL},
	{&s_wave3, ID_WAVE3, 0, 0, 190, 240, 90, NULL},
	{ NULL,0,0,0,0,0,0 }
};

//////////////////////// start UI ////////////////////////
static c_display* s_display;
void create_ui(void* phy_fb, int screen_width, int screen_height, int color_bytes, struct EXTERNAL_GFX_OP* gfx_op)
{
	s_display = new c_display(phy_fb, screen_width, screen_height, UI_WIDTH, UI_HEIGHT, color_bytes, 1, gfx_op);
	c_surface* surface = s_display->alloc_surface(&s_myUI, Z_ORDER_LEVEL_0);
	surface->set_active(true);
	s_myUI.set_surface(surface);
	s_myUI.connect(NULL, ID_ROOT, 0, 0, 0, UI_WIDTH, UI_HEIGHT, s_myUI_children);
	s_myUI.show_window();
}

///////////////// WebAssembly Code /////////////////

#include <stdio.h>
#include <sys/uio.h>

#define WASM_EXPORT __attribute__((visibility("default")))

/* External function that is implemented in JavaScript. */
extern "C" void drawGuiLitePixel(int x, int y, int color);
struct EXTERNAL_GFX_OP my_gfx_op;

WASM_EXPORT
int main(int mode) {
  switch(mode){
    case 0:
      create_ui(calloc(UI_WIDTH * UI_HEIGHT, 4), UI_WIDTH, UI_HEIGHT, 4, NULL);
    break;
    default:
      my_gfx_op.draw_pixel = drawGuiLitePixel;
	    my_gfx_op.fill_rect = NULL;//gfx_fill_rect;
      create_ui(NULL, UI_WIDTH, UI_HEIGHT, 4, &my_gfx_op);
    break;
  }
  printf("GuiLite ready\n");
}

WASM_EXPORT
extern "C" unsigned char* updateHelloWave(void) {
  static unsigned int data_index1, data_index2, data_index3, wave_index;

  s_myUI.m_wave_buffer1.write_wave_data(s_wave_data1[data_index1++%sizeof(s_wave_data1)]);
  s_myUI.m_wave_buffer1.write_wave_data(s_wave_data1[data_index1++ % sizeof(s_wave_data1)]);
  s_myUI.m_wave_buffer1.write_wave_data(s_wave_data1[data_index1++ % sizeof(s_wave_data1)]);
  s_myUI.m_wave_buffer1.write_wave_data(s_wave_data1[data_index1++ % sizeof(s_wave_data1)]);

  s_myUI.m_wave_buffer2.write_wave_data(s_wave_data2[data_index2++%sizeof(s_wave_data2)]);
  
  s_myUI.m_wave_buffer3.write_wave_data(s_wave_data3[data_index3++ % sizeof(s_wave_data3)]);
  s_myUI.m_wave_buffer3.write_wave_data(s_wave_data3[data_index3++ % sizeof(s_wave_data3)]);
  s_wave1.refresh_wave(wave_index);
  s_wave2.refresh_wave(wave_index);
  s_wave3.refresh_wave(wave_index);
  wave_index++;
  return (unsigned char*)s_display->get_updated_fb(NULL, NULL);
}

/* External function that is implemented in JavaScript. */
extern "C" void putc_js(char c);

/* Basic implementation of the writev sys call. */ 
WASM_EXPORT
extern "C" size_t writev_c(int fd, const struct iovec *iov, int iovcnt) {
  size_t cnt = 0;
  for (int i = 0; i < iovcnt; i++) {
    for (int j = 0; j < iov[i].iov_len; j++) {
      putc_js(((char *)iov[i].iov_base)[j]);
    }
    cnt += iov[i].iov_len;
  }
  return cnt;
}
