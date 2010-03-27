#pragma once

#include "RootWindow.h"
#include "Xcb.h"

namespace manix {
namespace wm {
	
	class App {
	public:
		App();
		
		void run();
		
	private:
		XcbPtr			conn;
		RootWindowPtr	root;
	};
	
} // wm
} // manix