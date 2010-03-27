#pragma once

#include <boost/shared_ptr.hpp>

#include "Xcb.h"
#include "Window.h"

namespace manix {
namespace wm {

	class WindowFrame : public Window {
	public:
		WindowFrame(XcbPtr conn, xcb_window_t parent, WindowPtr client);
		
		void adjust_to_client();
		
	private:
		WindowPtr		client;
		
	};

	typedef boost::shared_ptr<WindowFrame> WindowFramePtr;

} // wm
} // manix