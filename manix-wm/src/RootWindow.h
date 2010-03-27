#pragma once

#include <boost/shared_ptr.hpp>

#include "ManagedWindow.h"
#include "Xcb.h"

namespace manix {
namespace wm {
	
	class RootWindow {
	public:
		RootWindow(XcbPtr conn, xcb_window_t window);
		
	private:
		typedef std::map<xcb_window_t, ManagedWindowPtr>	WindowMap;
		
		XcbPtr			conn;
		xcb_window_t	root;
		WindowMap		windows;
		
		void handle_map_request(const xcb_map_request_event_t& event);
		void handle_configure_request(const xcb_configure_request_event_t& event);
		
		void handle_event(const xcb_generic_event_t& event);
	};
	
	typedef boost::shared_ptr<RootWindow> RootWindowPtr;
		
} // manix
} // wm