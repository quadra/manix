#include "Window.h"

namespace manix {
namespace wm {
	
	namespace {
		void throw_if_error(xcb_generic_error_t* error){
			if(error != NULL){
				std::auto_ptr<xcb_generic_error_t>	ptr(error);
				throw XcbException(error);
			}
		}
	}
	
	Window::Window(XcbPtr conn, xcb_window_t parent, xcb_window_t window, bool owner)
		: conn(conn)
		, parent(parent)
		, window(window)
		, owner(owner)
	{
		
	}
	
	Window::Window(XcbPtr conn, xcb_window_t parent, int32_t x, int32_t y, uint32_t w, uint32_t h)
		: conn(conn)
		, parent(parent)
	{
		create_window(parent, x, y, w, h);
	}
	
	Window::~Window()
	{
		if(owner)
			xcb_destroy_window(conn->xcb(), window);
	}
	
	void
	Window::show()
	{
		xcb_map_window(conn->xcb(), window);
	}
	
	void
	Window::hide()
	{
		xcb_unmap_window(conn->xcb(), window);
	}
	
	void
	Window::translate_coords(xcb_window_t target, int32_t& x, int32_t& y)
	{
		xcb_translate_coordinates_cookie_t c = xcb_translate_coordinates(conn->xcb(), window, target, x, y);
		xcb_generic_error_t* error;
		xcb_translate_coordinates_reply_t* reply = xcb_translate_coordinates_reply(conn->xcb(), c, &error);
		throw_if_error(error);
		
		x = reply->dst_x;
		y = reply->dst_y;
		
		free(reply);
	}
	
	void
	Window::get_geometry(int32_t& x, int32_t& y, uint32_t& width, uint32_t& height)
	{
		xcb_get_geometry_cookie_t	c = xcb_get_geometry(conn->xcb(), window);
		xcb_generic_error_t*		error;
		xcb_get_geometry_reply_t*	reply = xcb_get_geometry_reply(conn->xcb(), c, &error);
		throw_if_error(error);
		
		x = reply->x;
		y = reply->y;
		width = reply->width;
		height = reply->height;
		
		free(reply);
	}
	
	void
	Window::move_to(int32_t x, int32_t y)
	{
		uint16_t	mask = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;
		uint32_t	values[] = {x, y};
		xcb_configure_window(conn->xcb(), window, mask, values);
	}

	void
	Window::resize_to(uint32_t width, uint32_t height)
	{
		assert(width != 0);
		assert(height != 0);
		
		uint16_t	mask = XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;
		uint32_t	values[] = {width, height};
		xcb_configure_window(conn->xcb(), window, mask, values);
	}

	void
	Window::put_inside(WindowPtr parent, int32_t x, int32_t y)
	{
		xcb_reparent_window(conn->xcb(), window, parent->window, x, y);
	}

	void
	Window::create_window(xcb_window_t parent, int32_t x, int32_t y, uint32_t w, uint32_t h)
	{
		window = xcb_generate_id(conn->xcb());
		
		const xcb_screen_t*	screen = conn->get_root();
		uint32_t		values[1] = {0};
		conn->check(xcb_create_window_checked(conn->xcb(), screen->root_depth, window, parent, x, y, w, h, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, 0, values));
	}
	
	void
	Window::handle_event(const xcb_generic_event_t& event)
	{
		
	}
	
} // wm
} // manix


