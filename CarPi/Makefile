CONFIG = Debug
#CONFIG = Release

ifeq ($(CONFIG),Debug)
DEBUG := -g
endif
ifeq ($(CONFIG),Release)
OPT := -O1 -DNDEBUG
endif

INSTALL_DIR = $(HOME)/Software/carpi/bin

# See if we're compiling on a Raspberry Pi.
ifeq ($(shell uname -m),armv6l)
IS_RASPBERRY_PI = true
PLATFORM_DEFINES = -DRASPBERRY_PI
endif

CXX = g++
CXXFLAGS = $(DEBUG) $(OPT) $(PLATFORM_DEFINES) -Wall -D_REENTRANT -std=c++0x

MAIN_SRCS := \
	abstract_event_visitor.cpp \
	car_pi_app.cpp \
	composite_event_handler.cpp \
	cons_car_pi_app.cpp \
	cons_input_reader_thread.cpp \
	cons_menu_view.cpp \
	cons_music_player_view.cpp \
	cons_playback_settings_view.cpp \
	cons_video_player_view.cpp \
	console.cpp \
	event.cpp \
	event_handler.cpp \
	event_queue.cpp \
	file_navigator_menu_controller.cpp \
	main.cpp \
	main_menu_controller.cpp \
	main_menu.cpp \
	menu_controller.cpp \
	menu.cpp \
	menu_view.cpp \
	music_file_navigator_menu_controller.cpp \
	music_player_controller.cpp \
	notification_event_filter.cpp \
	playback_settings.cpp \
	playback_settings_controller.cpp \
	play_sound.cpp \
	play_video.cpp \
	thread.cpp \
	video_file_navigator_menu_controller.cpp \
	video_player_controller.cpp

ifeq ($(IS_RASPBERRY_PI),true)
MAIN_SRCS += button_input_reader_thread.cpp gpio_pin.cpp
endif

MAIN_OBJS := $(MAIN_SRCS:%.cpp=$(CONFIG)/%.o)

$(CONFIG)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp -o $(CONFIG)/$*.o

$(CONFIG)/CarPi : $(CONFIG) $(MAIN_OBJS)
	$(CXX) -o $@ $(MAIN_OBJS) -lncurses -lpthread

install : $(CONFIG)/CarPi
	mkdir -p $(INSTALL_DIR)
	cp $(CONFIG)/CarPi $(INSTALL_DIR)
	chmod a+x $(INSTALL_DIR)/CarPi

$(CONFIG) :
	mkdir -p $(CONFIG)

$(CONFIG)/depend.mak : $(CONFIG)
	touch $@

depend :
	$(CXX) $(CXXFLAGS) -M $(MAIN_SRCS) | \
		perl -n -e 's,^([a-z]),$(CONFIG)/\1,; print' > $(CONFIG)/depend.mak

clean :
	rm -f $(CONFIG)/*.o

include $(CONFIG)/depend.mak
