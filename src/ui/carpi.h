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

#ifndef CARPI_H
#define CARPI_H

#include <gtk/gtk.h>

// Window width and height.
// Currently hard-coded for 320x240 (QVGA).
#define CARPI_WINDOW_WIDTH 320
#define CARPI_WINDOW_HEIGHT 240

// Main window data.
typedef struct CarPiMainWindow_ {
	// A GtkFixed container represents the main display area.
	GtkWidget *container;
} CarPiMainWindow;

// Create a CarPiMainWindow, embedding its container widget in the
// given frame.
CarPiMainWindow *carpi_mainwin_create(GtkWidget *frame);

#endif // CARPI_H
