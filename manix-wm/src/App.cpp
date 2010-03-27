#include "App.h"

namespace manix {
namespace wm {

	App::App()
		:conn(new Xcb())
		,root(new RootWindow(conn, conn->get_root()->root))
	{
		
	}
	
	void
	App::run()
	{
		for(;;)
			conn->dispatch_next();
	}

} // wm
} // manix