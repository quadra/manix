#include <cstdio>
#include <memory>
#include <xcb/xcb.h>

#include "Xcb.h"

namespace manix {
namespace wm {
	
	Xcb::Xcb()
	{
		conn = xcb_connect(NULL, NULL);
		if(conn == NULL)
			throw XcbException("Could not create X connection");
		
		read_settings();
	}
	
	void
	Xcb::dispatch_next()
	{
		xcb_flush(conn);
		
		xcb_generic_event_t*		event = xcb_wait_for_event(conn);
		dispatch(event);
		free(event);
	}
	
	void
	Xcb::add_event_handler(xcb_window_t window, const EventHandler::slot_type& handler)
	{
		if(handlers.count(window) == 0)
			handlers[window] = EventHandlerPtr(new EventHandler());
		
		handlers[window]->connect(handler);
	}
	
	void
	Xcb::window_closed(xcb_window_t window)
	{
		handlers.erase(window);
	}
	
	void
	Xcb::check(xcb_void_cookie_t cookie)
	{
		xcb_generic_error_t*	error = xcb_request_check(conn, cookie);
		if(error != NULL){
			std::auto_ptr<xcb_generic_error_t>	ptr(error);
			throw XcbException(error);
		}
	}
	
	void
	Xcb::read_settings()
	{
		const xcb_setup_t*		setup = xcb_get_setup(conn);
		
		// TODO: ugly
		root_screen = xcb_setup_roots_iterator (xcb_get_setup (conn)).data;
		
	}
	
	// Please don't look :(
	#define TARGET_WINDOW(RESPONSE_TYPE, EVENT_TYPE, FIELD_NAME) \
		case RESPONSE_TYPE: \
			target = reinterpret_cast<EVENT_TYPE*>(event)->FIELD_NAME; \
			break;
	
	void
	Xcb::dispatch(xcb_generic_event_t* event)
	{
		xcb_window_t	target;
		
		// Fugly
		switch(event->response_type & 0x7F){
			TARGET_WINDOW(XCB_KEY_PRESS, xcb_key_press_event_t, event);
			TARGET_WINDOW(XCB_KEY_RELEASE, xcb_key_release_event_t, event);
			TARGET_WINDOW(XCB_BUTTON_PRESS, xcb_button_press_event_t, event);
			TARGET_WINDOW(XCB_BUTTON_RELEASE, xcb_button_release_event_t, event);
			TARGET_WINDOW(XCB_MOTION_NOTIFY, xcb_motion_notify_event_t, event);
			TARGET_WINDOW(XCB_ENTER_NOTIFY, xcb_enter_notify_event_t, event);
			TARGET_WINDOW(XCB_LEAVE_NOTIFY, xcb_leave_notify_event_t, event);
			TARGET_WINDOW(XCB_FOCUS_IN, xcb_focus_in_event_t, event);
			TARGET_WINDOW(XCB_FOCUS_OUT, xcb_focus_out_event_t, event);
//			TARGET_WINDOW(XCB_KEYMAP_NOTIFY, xcb_keymap_notify_event_t, event);
			TARGET_WINDOW(XCB_EXPOSE, xcb_expose_event_t, window);
//			TARGET_WINDOW(XCB_GRAPHICS_EXPOSE, xcb_graphics_expose_event_t, ...);
//			NO_EXPOSURE
			TARGET_WINDOW(XCB_VISIBILITY_NOTIFY, xcb_visibility_notify_event_t, window);
			TARGET_WINDOW(XCB_CREATE_NOTIFY, xcb_create_notify_event_t, parent);
			TARGET_WINDOW(XCB_DESTROY_NOTIFY, xcb_destroy_notify_event_t, window);
			TARGET_WINDOW(XCB_UNMAP_NOTIFY, xcb_unmap_notify_event_t, window);
			TARGET_WINDOW(XCB_MAP_REQUEST, xcb_map_request_event_t, parent);
			TARGET_WINDOW(XCB_REPARENT_NOTIFY, xcb_reparent_notify_event_t, window);
			TARGET_WINDOW(XCB_CONFIGURE_NOTIFY, xcb_configure_notify_event_t, window);
			TARGET_WINDOW(XCB_CONFIGURE_REQUEST, xcb_configure_request_event_t, parent);
			TARGET_WINDOW(XCB_GRAVITY_NOTIFY, xcb_gravity_notify_event_t, window);
			TARGET_WINDOW(XCB_RESIZE_REQUEST, xcb_resize_request_event_t, window);
			
			case XCB_KEYMAP_NOTIFY:
				printf("Got keymap  notify\n");
				break;
			
			case XCB_MAPPING_NOTIFY:
				printf("Got mapping notify\n");
				break;
			
			default:
				printf("Xcb: Unknown event %d, can't dispatch\n", event->response_type);
				assert(0 && "unknown event");
		}
		
		if(handlers.count(target) != 0)
			(*handlers[target])(*event);
	}
	
} // wm
} // manix