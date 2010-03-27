#include <boost/bind.hpp>
#include <cstdio>

#include "RootWindow.h"

namespace manix {
namespace wm {
	
	namespace {
		
		
	}
	
	RootWindow::RootWindow(XcbPtr conn, xcb_window_t window)
		: conn(conn)
		, root(window)
	{
		std::printf("RootWindow: Grabbing %08X\n", window);
		uint32_t		values[] = {XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_RESIZE_REDIRECT};
		conn->check(xcb_change_window_attributes_checked(conn->xcb(), window, XCB_CW_EVENT_MASK, values));
		
		conn->add_event_handler(window, boost::bind(&RootWindow::handle_event, this, _1));
	}
	
	void
	RootWindow::handle_map_request(const xcb_map_request_event_t& event)
	{
		std::printf("RootWindow: Got map request: window=%08X\n", (uint32_t) event.window);
		
		if(windows.count(event.window) == 0){
			ManagedWindowPtr	window = ManagedWindowPtr(new ManagedWindow(conn, event.parent, event.window));
			window->show();
			
			windows[event.window] = window;
		}else{
			printf("    window still marked as mapped?");
		}
	}
	
	/**
	 * Configure requests on the root window will only happend
	 * for unmapped windows. Unmapped windows are not really
	 * interesting so we allow them to configure how they wish.
	 * Once they map we will apply any constraints.
	 *
	 */
	void
	RootWindow::handle_configure_request(const xcb_configure_request_event_t& event)
	{
		std::printf("RootWindow: Got configure request; window=%08X; x=%d; y=%d; width=%d; height=%d\n", (uint32_t) event.window, event.x, event.y, event.width, event.height);
		
		if(windows.count(event.window) == 0){
			uint32_t			mask = { XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT | XCB_CONFIG_WINDOW_BORDER_WIDTH };
			uint32_t			values[] = { event.x, event.y, event.width, event.height, 0 };
			xcb_configure_window(conn->xcb(), event.window, mask, values);
		}else{
			std::printf("    window still marked as mapped\n");
		}
	}
	
	void
	RootWindow::handle_event(const xcb_generic_event_t& event)
	{
		switch(event.response_type & 0x7F){
			
			case XCB_MAP_REQUEST:
				handle_map_request(reinterpret_cast<const xcb_map_request_event_t&>(event));
				break;
			
			case XCB_CONFIGURE_REQUEST:
				handle_configure_request(reinterpret_cast<const xcb_configure_request_event_t&>(event));
				break;
			
			default:
				std::printf("RootWindow: Unknown event %d\n", event.response_type);
				break;
			
		}
	}
	
} // wm
} // manix