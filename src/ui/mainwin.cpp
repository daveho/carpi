#include <stdlib.h>
#include "carpi.h"

CarPiMainWindow::CarPiMainWindow(GtkWidget *frame)
{
	// Create the container widget and set it to the appropriate size
	container = gtk_fixed_new();
	gtk_widget_set_size_request(container, CARPI_WINDOW_WIDTH, CARPI_WINDOW_HEIGHT);
	
	// Add the container widget to the frame
	gtk_container_add(GTK_CONTAINER(frame), container);
	gtk_widget_show(container);
}

CarPiMainWindow::~CarPiMainWindow()
{
}
