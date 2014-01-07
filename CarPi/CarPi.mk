##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=CarPi
ConfigurationName      :=Debug
WorkspacePath          := "/home/dhovemey/work/carpi/CarPi"
ProjectPath            := "/home/dhovemey/work/carpi/CarPi"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=David Hovemeyer
Date                   :=01/04/14
CodeLitePath           :="/home/dhovemey/.codelite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="CarPi.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)pthread $(LibrarySwitch)ncurses 
ArLibs                 :=  "pthread" "ncurses" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall -D_REENTRANT $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall -D_REENTRANT $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main$(ObjectSuffix) $(IntermediateDirectory)/play_sound$(ObjectSuffix) $(IntermediateDirectory)/event_queue$(ObjectSuffix) $(IntermediateDirectory)/event$(ObjectSuffix) $(IntermediateDirectory)/thread$(ObjectSuffix) $(IntermediateDirectory)/event_handler$(ObjectSuffix) $(IntermediateDirectory)/menu$(ObjectSuffix) $(IntermediateDirectory)/menu_view$(ObjectSuffix) $(IntermediateDirectory)/cons_menu_view$(ObjectSuffix) $(IntermediateDirectory)/console$(ObjectSuffix) \
	$(IntermediateDirectory)/abstract_event_visitor$(ObjectSuffix) $(IntermediateDirectory)/menu_controller$(ObjectSuffix) $(IntermediateDirectory)/cons_input_reader_thread$(ObjectSuffix) $(IntermediateDirectory)/main_menu_controller$(ObjectSuffix) $(IntermediateDirectory)/main_menu$(ObjectSuffix) $(IntermediateDirectory)/static_menu$(ObjectSuffix) $(IntermediateDirectory)/file_navigator_menu_controller$(ObjectSuffix) $(IntermediateDirectory)/car_pi_app$(ObjectSuffix) $(IntermediateDirectory)/composite_event_handler$(ObjectSuffix) $(IntermediateDirectory)/music_file_navigator_menu_controller$(ObjectSuffix) \
	$(IntermediateDirectory)/music_player_controller$(ObjectSuffix) $(IntermediateDirectory)/cons_music_player_view$(ObjectSuffix) $(IntermediateDirectory)/cons_car_pi_app$(ObjectSuffix) $(IntermediateDirectory)/video_file_navigator_menu_controller$(ObjectSuffix) $(IntermediateDirectory)/video_player_controller$(ObjectSuffix) $(IntermediateDirectory)/play_video$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main$(ObjectSuffix) -MF$(IntermediateDirectory)/main$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main$(PreprocessSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/play_sound$(ObjectSuffix): play_sound.cpp $(IntermediateDirectory)/play_sound$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/play_sound.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/play_sound$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/play_sound$(DependSuffix): play_sound.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/play_sound$(ObjectSuffix) -MF$(IntermediateDirectory)/play_sound$(DependSuffix) -MM "play_sound.cpp"

$(IntermediateDirectory)/play_sound$(PreprocessSuffix): play_sound.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/play_sound$(PreprocessSuffix) "play_sound.cpp"

$(IntermediateDirectory)/event_queue$(ObjectSuffix): event_queue.cpp $(IntermediateDirectory)/event_queue$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/event_queue.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/event_queue$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/event_queue$(DependSuffix): event_queue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/event_queue$(ObjectSuffix) -MF$(IntermediateDirectory)/event_queue$(DependSuffix) -MM "event_queue.cpp"

$(IntermediateDirectory)/event_queue$(PreprocessSuffix): event_queue.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/event_queue$(PreprocessSuffix) "event_queue.cpp"

$(IntermediateDirectory)/event$(ObjectSuffix): event.cpp $(IntermediateDirectory)/event$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/event.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/event$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/event$(DependSuffix): event.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/event$(ObjectSuffix) -MF$(IntermediateDirectory)/event$(DependSuffix) -MM "event.cpp"

$(IntermediateDirectory)/event$(PreprocessSuffix): event.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/event$(PreprocessSuffix) "event.cpp"

$(IntermediateDirectory)/thread$(ObjectSuffix): thread.cpp $(IntermediateDirectory)/thread$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/thread.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/thread$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/thread$(DependSuffix): thread.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/thread$(ObjectSuffix) -MF$(IntermediateDirectory)/thread$(DependSuffix) -MM "thread.cpp"

$(IntermediateDirectory)/thread$(PreprocessSuffix): thread.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/thread$(PreprocessSuffix) "thread.cpp"

$(IntermediateDirectory)/event_handler$(ObjectSuffix): event_handler.cpp $(IntermediateDirectory)/event_handler$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/event_handler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/event_handler$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/event_handler$(DependSuffix): event_handler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/event_handler$(ObjectSuffix) -MF$(IntermediateDirectory)/event_handler$(DependSuffix) -MM "event_handler.cpp"

$(IntermediateDirectory)/event_handler$(PreprocessSuffix): event_handler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/event_handler$(PreprocessSuffix) "event_handler.cpp"

$(IntermediateDirectory)/menu$(ObjectSuffix): menu.cpp $(IntermediateDirectory)/menu$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/menu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/menu$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/menu$(DependSuffix): menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/menu$(ObjectSuffix) -MF$(IntermediateDirectory)/menu$(DependSuffix) -MM "menu.cpp"

$(IntermediateDirectory)/menu$(PreprocessSuffix): menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/menu$(PreprocessSuffix) "menu.cpp"

$(IntermediateDirectory)/menu_view$(ObjectSuffix): menu_view.cpp $(IntermediateDirectory)/menu_view$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/menu_view.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/menu_view$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/menu_view$(DependSuffix): menu_view.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/menu_view$(ObjectSuffix) -MF$(IntermediateDirectory)/menu_view$(DependSuffix) -MM "menu_view.cpp"

$(IntermediateDirectory)/menu_view$(PreprocessSuffix): menu_view.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/menu_view$(PreprocessSuffix) "menu_view.cpp"

$(IntermediateDirectory)/cons_menu_view$(ObjectSuffix): cons_menu_view.cpp $(IntermediateDirectory)/cons_menu_view$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/cons_menu_view.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cons_menu_view$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cons_menu_view$(DependSuffix): cons_menu_view.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cons_menu_view$(ObjectSuffix) -MF$(IntermediateDirectory)/cons_menu_view$(DependSuffix) -MM "cons_menu_view.cpp"

$(IntermediateDirectory)/cons_menu_view$(PreprocessSuffix): cons_menu_view.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cons_menu_view$(PreprocessSuffix) "cons_menu_view.cpp"

$(IntermediateDirectory)/console$(ObjectSuffix): console.cpp $(IntermediateDirectory)/console$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/console.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/console$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/console$(DependSuffix): console.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/console$(ObjectSuffix) -MF$(IntermediateDirectory)/console$(DependSuffix) -MM "console.cpp"

$(IntermediateDirectory)/console$(PreprocessSuffix): console.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/console$(PreprocessSuffix) "console.cpp"

$(IntermediateDirectory)/abstract_event_visitor$(ObjectSuffix): abstract_event_visitor.cpp $(IntermediateDirectory)/abstract_event_visitor$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/abstract_event_visitor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/abstract_event_visitor$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/abstract_event_visitor$(DependSuffix): abstract_event_visitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/abstract_event_visitor$(ObjectSuffix) -MF$(IntermediateDirectory)/abstract_event_visitor$(DependSuffix) -MM "abstract_event_visitor.cpp"

$(IntermediateDirectory)/abstract_event_visitor$(PreprocessSuffix): abstract_event_visitor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/abstract_event_visitor$(PreprocessSuffix) "abstract_event_visitor.cpp"

$(IntermediateDirectory)/menu_controller$(ObjectSuffix): menu_controller.cpp $(IntermediateDirectory)/menu_controller$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/menu_controller.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/menu_controller$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/menu_controller$(DependSuffix): menu_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/menu_controller$(ObjectSuffix) -MF$(IntermediateDirectory)/menu_controller$(DependSuffix) -MM "menu_controller.cpp"

$(IntermediateDirectory)/menu_controller$(PreprocessSuffix): menu_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/menu_controller$(PreprocessSuffix) "menu_controller.cpp"

$(IntermediateDirectory)/cons_input_reader_thread$(ObjectSuffix): cons_input_reader_thread.cpp $(IntermediateDirectory)/cons_input_reader_thread$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/cons_input_reader_thread.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cons_input_reader_thread$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cons_input_reader_thread$(DependSuffix): cons_input_reader_thread.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cons_input_reader_thread$(ObjectSuffix) -MF$(IntermediateDirectory)/cons_input_reader_thread$(DependSuffix) -MM "cons_input_reader_thread.cpp"

$(IntermediateDirectory)/cons_input_reader_thread$(PreprocessSuffix): cons_input_reader_thread.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cons_input_reader_thread$(PreprocessSuffix) "cons_input_reader_thread.cpp"

$(IntermediateDirectory)/main_menu_controller$(ObjectSuffix): main_menu_controller.cpp $(IntermediateDirectory)/main_menu_controller$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/main_menu_controller.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main_menu_controller$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main_menu_controller$(DependSuffix): main_menu_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main_menu_controller$(ObjectSuffix) -MF$(IntermediateDirectory)/main_menu_controller$(DependSuffix) -MM "main_menu_controller.cpp"

$(IntermediateDirectory)/main_menu_controller$(PreprocessSuffix): main_menu_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main_menu_controller$(PreprocessSuffix) "main_menu_controller.cpp"

$(IntermediateDirectory)/main_menu$(ObjectSuffix): main_menu.cpp $(IntermediateDirectory)/main_menu$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/main_menu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main_menu$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main_menu$(DependSuffix): main_menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main_menu$(ObjectSuffix) -MF$(IntermediateDirectory)/main_menu$(DependSuffix) -MM "main_menu.cpp"

$(IntermediateDirectory)/main_menu$(PreprocessSuffix): main_menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main_menu$(PreprocessSuffix) "main_menu.cpp"

$(IntermediateDirectory)/static_menu$(ObjectSuffix): static_menu.cpp $(IntermediateDirectory)/static_menu$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/static_menu.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/static_menu$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/static_menu$(DependSuffix): static_menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/static_menu$(ObjectSuffix) -MF$(IntermediateDirectory)/static_menu$(DependSuffix) -MM "static_menu.cpp"

$(IntermediateDirectory)/static_menu$(PreprocessSuffix): static_menu.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/static_menu$(PreprocessSuffix) "static_menu.cpp"

$(IntermediateDirectory)/file_navigator_menu_controller$(ObjectSuffix): file_navigator_menu_controller.cpp $(IntermediateDirectory)/file_navigator_menu_controller$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/file_navigator_menu_controller.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/file_navigator_menu_controller$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/file_navigator_menu_controller$(DependSuffix): file_navigator_menu_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/file_navigator_menu_controller$(ObjectSuffix) -MF$(IntermediateDirectory)/file_navigator_menu_controller$(DependSuffix) -MM "file_navigator_menu_controller.cpp"

$(IntermediateDirectory)/file_navigator_menu_controller$(PreprocessSuffix): file_navigator_menu_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/file_navigator_menu_controller$(PreprocessSuffix) "file_navigator_menu_controller.cpp"

$(IntermediateDirectory)/car_pi_app$(ObjectSuffix): car_pi_app.cpp $(IntermediateDirectory)/car_pi_app$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/car_pi_app.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/car_pi_app$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/car_pi_app$(DependSuffix): car_pi_app.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/car_pi_app$(ObjectSuffix) -MF$(IntermediateDirectory)/car_pi_app$(DependSuffix) -MM "car_pi_app.cpp"

$(IntermediateDirectory)/car_pi_app$(PreprocessSuffix): car_pi_app.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/car_pi_app$(PreprocessSuffix) "car_pi_app.cpp"

$(IntermediateDirectory)/composite_event_handler$(ObjectSuffix): composite_event_handler.cpp $(IntermediateDirectory)/composite_event_handler$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/composite_event_handler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/composite_event_handler$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/composite_event_handler$(DependSuffix): composite_event_handler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/composite_event_handler$(ObjectSuffix) -MF$(IntermediateDirectory)/composite_event_handler$(DependSuffix) -MM "composite_event_handler.cpp"

$(IntermediateDirectory)/composite_event_handler$(PreprocessSuffix): composite_event_handler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/composite_event_handler$(PreprocessSuffix) "composite_event_handler.cpp"

$(IntermediateDirectory)/music_file_navigator_menu_controller$(ObjectSuffix): music_file_navigator_menu_controller.cpp $(IntermediateDirectory)/music_file_navigator_menu_controller$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/music_file_navigator_menu_controller.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/music_file_navigator_menu_controller$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/music_file_navigator_menu_controller$(DependSuffix): music_file_navigator_menu_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/music_file_navigator_menu_controller$(ObjectSuffix) -MF$(IntermediateDirectory)/music_file_navigator_menu_controller$(DependSuffix) -MM "music_file_navigator_menu_controller.cpp"

$(IntermediateDirectory)/music_file_navigator_menu_controller$(PreprocessSuffix): music_file_navigator_menu_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/music_file_navigator_menu_controller$(PreprocessSuffix) "music_file_navigator_menu_controller.cpp"

$(IntermediateDirectory)/music_player_controller$(ObjectSuffix): music_player_controller.cpp $(IntermediateDirectory)/music_player_controller$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/music_player_controller.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/music_player_controller$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/music_player_controller$(DependSuffix): music_player_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/music_player_controller$(ObjectSuffix) -MF$(IntermediateDirectory)/music_player_controller$(DependSuffix) -MM "music_player_controller.cpp"

$(IntermediateDirectory)/music_player_controller$(PreprocessSuffix): music_player_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/music_player_controller$(PreprocessSuffix) "music_player_controller.cpp"

$(IntermediateDirectory)/cons_music_player_view$(ObjectSuffix): cons_music_player_view.cpp $(IntermediateDirectory)/cons_music_player_view$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/cons_music_player_view.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cons_music_player_view$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cons_music_player_view$(DependSuffix): cons_music_player_view.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cons_music_player_view$(ObjectSuffix) -MF$(IntermediateDirectory)/cons_music_player_view$(DependSuffix) -MM "cons_music_player_view.cpp"

$(IntermediateDirectory)/cons_music_player_view$(PreprocessSuffix): cons_music_player_view.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cons_music_player_view$(PreprocessSuffix) "cons_music_player_view.cpp"

$(IntermediateDirectory)/cons_car_pi_app$(ObjectSuffix): cons_car_pi_app.cpp $(IntermediateDirectory)/cons_car_pi_app$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/cons_car_pi_app.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cons_car_pi_app$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cons_car_pi_app$(DependSuffix): cons_car_pi_app.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cons_car_pi_app$(ObjectSuffix) -MF$(IntermediateDirectory)/cons_car_pi_app$(DependSuffix) -MM "cons_car_pi_app.cpp"

$(IntermediateDirectory)/cons_car_pi_app$(PreprocessSuffix): cons_car_pi_app.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cons_car_pi_app$(PreprocessSuffix) "cons_car_pi_app.cpp"

$(IntermediateDirectory)/video_file_navigator_menu_controller$(ObjectSuffix): video_file_navigator_menu_controller.cpp $(IntermediateDirectory)/video_file_navigator_menu_controller$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/video_file_navigator_menu_controller.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/video_file_navigator_menu_controller$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/video_file_navigator_menu_controller$(DependSuffix): video_file_navigator_menu_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/video_file_navigator_menu_controller$(ObjectSuffix) -MF$(IntermediateDirectory)/video_file_navigator_menu_controller$(DependSuffix) -MM "video_file_navigator_menu_controller.cpp"

$(IntermediateDirectory)/video_file_navigator_menu_controller$(PreprocessSuffix): video_file_navigator_menu_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/video_file_navigator_menu_controller$(PreprocessSuffix) "video_file_navigator_menu_controller.cpp"

$(IntermediateDirectory)/video_player_controller$(ObjectSuffix): video_player_controller.cpp $(IntermediateDirectory)/video_player_controller$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/video_player_controller.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/video_player_controller$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/video_player_controller$(DependSuffix): video_player_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/video_player_controller$(ObjectSuffix) -MF$(IntermediateDirectory)/video_player_controller$(DependSuffix) -MM "video_player_controller.cpp"

$(IntermediateDirectory)/video_player_controller$(PreprocessSuffix): video_player_controller.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/video_player_controller$(PreprocessSuffix) "video_player_controller.cpp"

$(IntermediateDirectory)/play_video$(ObjectSuffix): play_video.cpp $(IntermediateDirectory)/play_video$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dhovemey/work/carpi/CarPi/play_video.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/play_video$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/play_video$(DependSuffix): play_video.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/play_video$(ObjectSuffix) -MF$(IntermediateDirectory)/play_video$(DependSuffix) -MM "play_video.cpp"

$(IntermediateDirectory)/play_video$(PreprocessSuffix): play_video.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/play_video$(PreprocessSuffix) "play_video.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/play_sound$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/play_sound$(DependSuffix)
	$(RM) $(IntermediateDirectory)/play_sound$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/event_queue$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/event_queue$(DependSuffix)
	$(RM) $(IntermediateDirectory)/event_queue$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/event$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/event$(DependSuffix)
	$(RM) $(IntermediateDirectory)/event$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/thread$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/thread$(DependSuffix)
	$(RM) $(IntermediateDirectory)/thread$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/event_handler$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/event_handler$(DependSuffix)
	$(RM) $(IntermediateDirectory)/event_handler$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/menu$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/menu$(DependSuffix)
	$(RM) $(IntermediateDirectory)/menu$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/menu_view$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/menu_view$(DependSuffix)
	$(RM) $(IntermediateDirectory)/menu_view$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/cons_menu_view$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/cons_menu_view$(DependSuffix)
	$(RM) $(IntermediateDirectory)/cons_menu_view$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/console$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/console$(DependSuffix)
	$(RM) $(IntermediateDirectory)/console$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/abstract_event_visitor$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/abstract_event_visitor$(DependSuffix)
	$(RM) $(IntermediateDirectory)/abstract_event_visitor$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/menu_controller$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/menu_controller$(DependSuffix)
	$(RM) $(IntermediateDirectory)/menu_controller$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/cons_input_reader_thread$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/cons_input_reader_thread$(DependSuffix)
	$(RM) $(IntermediateDirectory)/cons_input_reader_thread$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/main_menu_controller$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main_menu_controller$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main_menu_controller$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/main_menu$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/main_menu$(DependSuffix)
	$(RM) $(IntermediateDirectory)/main_menu$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/static_menu$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/static_menu$(DependSuffix)
	$(RM) $(IntermediateDirectory)/static_menu$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/file_navigator_menu_controller$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/file_navigator_menu_controller$(DependSuffix)
	$(RM) $(IntermediateDirectory)/file_navigator_menu_controller$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/car_pi_app$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/car_pi_app$(DependSuffix)
	$(RM) $(IntermediateDirectory)/car_pi_app$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/composite_event_handler$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/composite_event_handler$(DependSuffix)
	$(RM) $(IntermediateDirectory)/composite_event_handler$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/music_file_navigator_menu_controller$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/music_file_navigator_menu_controller$(DependSuffix)
	$(RM) $(IntermediateDirectory)/music_file_navigator_menu_controller$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/music_player_controller$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/music_player_controller$(DependSuffix)
	$(RM) $(IntermediateDirectory)/music_player_controller$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/cons_music_player_view$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/cons_music_player_view$(DependSuffix)
	$(RM) $(IntermediateDirectory)/cons_music_player_view$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/cons_car_pi_app$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/cons_car_pi_app$(DependSuffix)
	$(RM) $(IntermediateDirectory)/cons_car_pi_app$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/video_file_navigator_menu_controller$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/video_file_navigator_menu_controller$(DependSuffix)
	$(RM) $(IntermediateDirectory)/video_file_navigator_menu_controller$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/video_player_controller$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/video_player_controller$(DependSuffix)
	$(RM) $(IntermediateDirectory)/video_player_controller$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/play_video$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/play_video$(DependSuffix)
	$(RM) $(IntermediateDirectory)/play_video$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) ".build-debug/CarPi"


