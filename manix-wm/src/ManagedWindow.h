#pragma once

#include "Window.h"
#include "WindowFrame.h"
#include "Xcb.h"

namespace manix {
namespace wm {
	
	class ManagedWindow {
	public:
		ManagedWindow(XcbPtr conn, xcb_window_t parent, xcb_window_t client);
		
		void show();
		
	private:
		XcbPtr			conn;
		WindowPtr		client;
		WindowFramePtr	frame;
		
	};
	
	typedef boost::shared_ptr<ManagedWindow> ManagedWindowPtr;
	
} // wm
} // manix