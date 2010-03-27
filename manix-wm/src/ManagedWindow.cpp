#include "ManagedWindow.h"

namespace manix {
namespace wm {
	
	ManagedWindow::ManagedWindow(XcbPtr conn, xcb_window_t parent, xcb_window_t client)
		: conn(conn)
		, client(new Window(conn, parent, client, false))
		, frame(new WindowFrame(conn, parent, this->client))
	{	
		// TODO: not the nicest design
		this->client->put_inside(frame, 0, 0);
	}
	
	void
	ManagedWindow::show()
	{
		frame->adjust_to_client();
		frame->show();
		client->show();
	}

	
} // wm
} // manix