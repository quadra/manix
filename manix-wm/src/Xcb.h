#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/signal.hpp>
#include <map>
#include <stdint.h>
#include <xcb/xcb.h>

namespace manix {
namespace wm {
		
	typedef boost::signal<void (const xcb_generic_event_t&)> EventHandler;
	
	struct XcbException {
		const char*	reason;
		uint8_t		response_type;
		uint8_t		error_code;
		uint16_t	sequence;
		
		XcbException(const char* reason)
			: reason(reason)
		{
			
		}
		
		XcbException(const xcb_generic_error_t* error)
			: reason(NULL)
			, response_type(error->response_type)
			, error_code(error->error_code)
			, sequence(error->sequence)
		{
			
		}
	};
	
	class Xcb {
	public:
		Xcb();
		
		void dispatch_next();
		
		void add_event_handler(xcb_window_t window, const EventHandler::slot_type& handler);
		void window_closed(xcb_window_t window);
		
		void check(xcb_void_cookie_t cookie);
		
		const xcb_screen_t*	get_root() { return root_screen; }
		xcb_connection_t*	xcb() { return conn; }
	private:
		typedef boost::shared_ptr<EventHandler> EventHandlerPtr;
		typedef std::map<xcb_window_t, EventHandlerPtr>	EventHandlerMap;
		
		xcb_connection_t* 	conn;
		EventHandlerMap		handlers;
		xcb_screen_t*		root_screen;
		
		void read_settings();
		void dispatch(xcb_generic_event_t* event);
	};

	typedef boost::shared_ptr<Xcb> XcbPtr;
		
} // wm
} // manix