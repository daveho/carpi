#include <stdlib.h>
#include "carpi.h"

CarPiMainWindow *carpi_mainwin_create(GtkWidget *frame)
{
	CarPiMainWindow *mainwin;

	mainwin = (CarPiMainWindow*) malloc(sizeof(CarPiMainWindow));

	// Create the container widget and set it to the appropriate size
	mainwin->container = gtk_fixed_new();
	gtk_widget_set_size_request(mainwin->container, CARPI_WINDOW_WIDTH, CARPI_WINDOW_HEIGHT);
	
	// Add the container widget to the frame
	gtk_container_add(GTK_CONTAINER(frame), mainwin->container);
	gtk_widget_show(mainwin->container);

	return mainwin;
}
