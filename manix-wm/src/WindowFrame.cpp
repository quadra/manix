#include "WindowFrame.h"

namespace manix {
namespace wm {
	
	namespace {
		xcb_window_t
		create_frame(XcbPtr conn, xcb_window_t parent, WindowPtr client)
		{
			int32_t			x, y;
			uint32_t		w, h;
			client->get_geometry(x,y,w,h);

			xcb_window_t	window = xcb_generate_id(conn->xcb());

			const xcb_screen_t*	screen = conn->get_root();
			uint32_t		values[1] = {0};
			conn->check(
				xcb_create_window_checked(
					conn->xcb(), 
					screen->root_depth, 
					window, 
					parent, 
					x, y, 
					w, h, 
					0, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, 0, values));
		
			return window;
		}
	}
	
	WindowFrame::WindowFrame(XcbPtr conn, xcb_window_t parent, WindowPtr client)
		: Window(conn, parent, create_frame(conn, parent, client), true)
		, client(client)
	{
		uint16_t		mask = XCB_CW_EVENT_MASK | XCB_CW_BACK_PIXEL;
		uint32_t		values[] = {XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS};
		
		xcb_change_window_attributes(conn->xcb(), window, mask, values);// reparent client, make sure it's "root" coordinates are unchanged
	}
	
	void
	WindowFrame::adjust_to_client()
	{
		int32_t	x, y;
		uint32_t w, h;
		
		client->get_geometry(x,y,w,h);
		
		x = y = 0;
		client->translate_coords(parent, x, y);
		client->move_to(10, 10);
		resize_to(w + 20, h + 20);
	}
	
} // wm
} // manix