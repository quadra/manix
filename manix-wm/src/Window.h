#pragma once

#include <boost/shared_ptr.hpp>
#include <xcb/xcb.h>

#include "Xcb.h"

namespace manix {
namespace wm {
	
	class Window;
	typedef boost::shared_ptr<Window> WindowPtr;
	
	class Window {
	public:
		Window(XcbPtr conn, xcb_window_t parent, xcb_window_t window, bool owner);
		Window(XcbPtr conn, xcb_window_t parent, int32_t x, int32_t y, uint32_t w, uint32_t h);
		virtual ~Window();
		
		void show();
		void hide();
		
		void translate_coords(xcb_window_t target, int32_t& x, int32_t& y);
		void get_geometry(int32_t& x, int32_t& y, uint32_t& width, uint32_t& height);
		void move_to(int32_t x, int32_t y);
		void resize_to(uint32_t width, uint32_t height);
		
		void put_inside(WindowPtr parent, int32_t x, int32_t y);
		
	protected:
		xcb_window_t	window;
		xcb_window_t	parent;
		XcbPtr			conn;
	
	private:
		bool			owner;
		
		void create_window(xcb_window_t parent, int32_t x, int32_t y, uint32_t width, uint32_t height);
		void handle_event(const xcb_generic_event_t& event);
	};
	
} // wm
} // manix