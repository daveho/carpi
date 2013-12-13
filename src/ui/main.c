// CarPi - Raspberry Pi Car Entertainment Application
// Copyright (c) David H. Hovemeyer <david.hovemeyer@gmail.com>

// This file is part of CarPi.

// CarPi is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <gtk/gtk.h>

int main(int argc, char **argv)
{
	GtkWidget *window;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show(window);

	gtk_main();

	return 0;
}
