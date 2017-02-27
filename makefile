WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -Igraphics -Itimers -Istates -Isprites -Isprite_components -Isprite_movement -Itext -Igoals -Iblock_conditions -Iblock_components -I/home/jjwmezun/Documents/cpp-projects/boskeopolis-land/ -Ilevels -Iinventory -Iexceptions
CFLAGS = -Wnon-virtual-dtor -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wmain -Wfatal-errors -Wextra -Wall -std=c++14 -g
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = -lSDL2 -lSDL2_image

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -Wnon-virtual-dtor -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wfloat-equal -Wunreachable-code -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++ -Wzero-as-null-pointer-constant -Wmain -Wfatal-errors -Wextra -Wall -std=c++14 -pg -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS) -pg
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/BoskeopolisLand

OBJ_DEBUG = $(OBJDIR_DEBUG)/sprites/buzz_saw_sprite.o $(OBJDIR_DEBUG)/sprites/bad_apple_sprite.o $(OBJDIR_DEBUG)/sprites/sprite_system.o $(OBJDIR_DEBUG)/sprite_movement/swimming_sprite_movement.o $(OBJDIR_DEBUG)/sprites/cloud_platform_sprite.o $(OBJDIR_DEBUG)/sprite_movement/sprite_movement.o $(OBJDIR_DEBUG)/sprite_movement/grounded_sprite_movement.o $(OBJDIR_DEBUG)/sprite_movement/fluttering_sprite_movement.o $(OBJDIR_DEBUG)/sprite_components/sprite_component_up_and_down.o $(OBJDIR_DEBUG)/sprites/fishstick_sprite.o $(OBJDIR_DEBUG)/sprites/iceblock_sprite.o $(OBJDIR_DEBUG)/sprites/hydrant_sprite.o $(OBJDIR_DEBUG)/sprites/handgun_sprite.o $(OBJDIR_DEBUG)/sprites/guard_sprite.o $(OBJDIR_DEBUG)/sprites/falling_bough_sprite.o $(OBJDIR_DEBUG)/sprites/enemy_cart_sprite.o $(OBJDIR_DEBUG)/sprites/eggnon_sprite.o $(OBJDIR_DEBUG)/levels/map_layer_image.o $(OBJDIR_DEBUG)/levels/map_layer_constellation.o $(OBJDIR_DEBUG)/levels/map.o $(OBJDIR_DEBUG)/levels/level.o $(OBJDIR_DEBUG)/inventory/inventory_level.o $(OBJDIR_DEBUG)/inventory/inventory.o $(OBJDIR_DEBUG)/input.o $(OBJDIR_DEBUG)/graphics/switch_graphics.o $(OBJDIR_DEBUG)/sprite_components/input_component_player.o $(OBJDIR_DEBUG)/sprite_components/sprite_component_right_and_left.o $(OBJDIR_DEBUG)/sprite_components/sprite_component_circle.o $(OBJDIR_DEBUG)/sprite_components/sprite_component.o $(OBJDIR_DEBUG)/sprite_components/input_component_sequence.o $(OBJDIR_DEBUG)/palette.o $(OBJDIR_DEBUG)/object.o $(OBJDIR_DEBUG)/main.o $(OBJDIR_DEBUG)/states/level_state.o $(OBJDIR_DEBUG)/text/text.o $(OBJDIR_DEBUG)/states/title_state.o $(OBJDIR_DEBUG)/states/pause_state.o $(OBJDIR_DEBUG)/states/message_state.o $(OBJDIR_DEBUG)/states/level_select_state.o $(OBJDIR_DEBUG)/states/level_message_state.o $(OBJDIR_DEBUG)/states/game_state.o $(OBJDIR_DEBUG)/water_effect.o $(OBJDIR_DEBUG)/timers/timer_switch.o $(OBJDIR_DEBUG)/timers/timer_simple.o $(OBJDIR_DEBUG)/sprites/sprite_status.o $(OBJDIR_DEBUG)/timers/timer_repeat.o $(OBJDIR_DEBUG)/timers/timer.o $(OBJDIR_DEBUG)/text/text_component_marquee.o $(OBJDIR_DEBUG)/text/text_component.o $(OBJDIR_DEBUG)/sprites/pufferbee_sprite.o $(OBJDIR_DEBUG)/sprites/player_sprite_fluttering.o $(OBJDIR_DEBUG)/sprites/player_sprite.o $(OBJDIR_DEBUG)/sprites/player_cart_sprite.o $(OBJDIR_DEBUG)/sprites/racer_sprite.o $(OBJDIR_DEBUG)/sprites/platform_sprite.o $(OBJDIR_DEBUG)/sprites/penguin_sprite.o $(OBJDIR_DEBUG)/sprites/overworld_player_sprite.o $(OBJDIR_DEBUG)/sprites/icecube_sprite.o $(OBJDIR_DEBUG)/sprites/rope_sprite.o $(OBJDIR_DEBUG)/sprites/saw_sprite.o $(OBJDIR_DEBUG)/sprites/sewer_monster_sprite.o $(OBJDIR_DEBUG)/sprites/sillyfish_sprite.o $(OBJDIR_DEBUG)/sprites/spike_egg_sprite.o $(OBJDIR_DEBUG)/sprites/spiky_fruit_sprite.o $(OBJDIR_DEBUG)/sprites/sprite.o $(OBJDIR_DEBUG)/block_components/block_component_switch.o $(OBJDIR_DEBUG)/block_components/block_component_swimmable.o $(OBJDIR_DEBUG)/block_components/block_component_solid.o $(OBJDIR_DEBUG)/block_components/block_component_single_use.o $(OBJDIR_DEBUG)/block_components/block_component_switch_flip.o $(OBJDIR_DEBUG)/block_components/block_component_money.o $(OBJDIR_DEBUG)/block_components/block_component_message.o $(OBJDIR_DEBUG)/block_components/block_component_low_slope_right.o $(OBJDIR_DEBUG)/block_conditions/block_condition_collide_bottom.o $(OBJDIR_DEBUG)/block_conditions/block_condition_enemy.o $(OBJDIR_DEBUG)/block_conditions/block_condition_collide_top.o $(OBJDIR_DEBUG)/block_conditions/block_condition_collide_right.o $(OBJDIR_DEBUG)/block_conditions/block_condition_collide_left.o $(OBJDIR_DEBUG)/block_components/block_component_low_slope_left.o $(OBJDIR_DEBUG)/block_conditions/block_condition_collide_any.o $(OBJDIR_DEBUG)/block_conditions/block_condition.o $(OBJDIR_DEBUG)/block_components/block_component_warp.o $(OBJDIR_DEBUG)/block_components/block_component_change_direction.o $(OBJDIR_DEBUG)/block_components/block_component_diamond.o $(OBJDIR_DEBUG)/block_components/block_component_conveyor.o $(OBJDIR_DEBUG)/block_components/block_component_climbable.o $(OBJDIR_DEBUG)/block_components/block_component_change_palette.o $(OBJDIR_DEBUG)/block_components/block_component_change_block.o $(OBJDIR_DEBUG)/block_components/block_component_bouncy.o $(OBJDIR_DEBUG)/block_components/block_component.o $(OBJDIR_DEBUG)/block_components/block_component_lose.o $(OBJDIR_DEBUG)/block_components/block_component_key.o $(OBJDIR_DEBUG)/block_components/block_component_hurt.o $(OBJDIR_DEBUG)/block_components/block_component_high_slope_right.o $(OBJDIR_DEBUG)/block_components/block_component_high_slope_left.o $(OBJDIR_DEBUG)/block_components/block_component_heal.o $(OBJDIR_DEBUG)/block_components/block_component_goal.o $(OBJDIR_DEBUG)/block_components/block_component_dropping.o $(OBJDIR_DEBUG)/graphics/hydrant_graphics.o $(OBJDIR_DEBUG)/graphics/guard_graphics.o $(OBJDIR_DEBUG)/graphics/graphics.o $(OBJDIR_DEBUG)/graphics/animated_graphics.o $(OBJDIR_DEBUG)/graphics/penguin_graphics.o $(OBJDIR_DEBUG)/goals/windy_goal.o $(OBJDIR_DEBUG)/goals/timed_goal.o $(OBJDIR_DEBUG)/goals/starving_goal.o $(OBJDIR_DEBUG)/goals/goal.o $(OBJDIR_DEBUG)/graphics/rotating_graphics.o $(OBJDIR_DEBUG)/graphics/sprite_graphics.o $(OBJDIR_DEBUG)/graphics/sillyfish_graphics.o $(OBJDIR_DEBUG)/graphics/sewer_monster_graphics.o $(OBJDIR_DEBUG)/graphics/saw_graphics.o $(OBJDIR_DEBUG)/graphics/rope_graphics.o $(OBJDIR_DEBUG)/graphics/pufferbee_graphics.o $(OBJDIR_DEBUG)/graphics/player_graphics.o $(OBJDIR_DEBUG)/clock.o $(OBJDIR_DEBUG)/camera.o $(OBJDIR_DEBUG)/block_type.o $(OBJDIR_DEBUG)/block_system.o $(OBJDIR_DEBUG)/block_conditions/block_condition_rival.o $(OBJDIR_DEBUG)/block_conditions/block_condition_not_ducking.o $(OBJDIR_DEBUG)/block_conditions/block_condition_key.o $(OBJDIR_DEBUG)/block_conditions/block_condition_hero.o $(OBJDIR_DEBUG)/block.o $(OBJDIR_DEBUG)/counter.o $(OBJDIR_DEBUG)/event_system.o $(OBJDIR_DEBUG)/game.o $(OBJDIR_DEBUG)/goals/avoid_money_goal.o $(OBJDIR_DEBUG)/goals/collect_goal.o $(OBJDIR_DEBUG)/goals/do_nothing_goal.o $(OBJDIR_DEBUG)/sprites/maze_chaser_sprite.o $(OBJDIR_DEBUG)/graphics/overworld_player_graphics.o $(OBJDIR_DEBUG)/graphics/maze_chaser_graphics.o $(OBJDIR_DEBUG)/sprites/heat_beam_sprite.o $(OBJDIR_DEBUG)/goals/survive_time_goal.o $(OBJDIR_DEBUG)/text/news_ticker.o

all: debug

clean: clean_debug

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/sprites || mkdir -p $(OBJDIR_DEBUG)/sprites
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)
	test -d $(OBJDIR_DEBUG)/sprite_movement || mkdir -p $(OBJDIR_DEBUG)/sprite_movement
	test -d $(OBJDIR_DEBUG)/sprite_components || mkdir -p $(OBJDIR_DEBUG)/sprite_components
	test -d $(OBJDIR_DEBUG)/levels || mkdir -p $(OBJDIR_DEBUG)/levels
	test -d $(OBJDIR_DEBUG)/inventory || mkdir -p $(OBJDIR_DEBUG)/inventory
	test -d $(OBJDIR_DEBUG)/graphics || mkdir -p $(OBJDIR_DEBUG)/graphics
	test -d $(OBJDIR_DEBUG)/states || mkdir -p $(OBJDIR_DEBUG)/states
	test -d $(OBJDIR_DEBUG)/text || mkdir -p $(OBJDIR_DEBUG)/text
	test -d $(OBJDIR_DEBUG)/timers || mkdir -p $(OBJDIR_DEBUG)/timers
	test -d $(OBJDIR_DEBUG)/block_components || mkdir -p $(OBJDIR_DEBUG)/block_components
	test -d $(OBJDIR_DEBUG)/block_conditions || mkdir -p $(OBJDIR_DEBUG)/block_conditions
	test -d $(OBJDIR_DEBUG)/goals || mkdir -p $(OBJDIR_DEBUG)/goals
	test -d $(OBJDIR_DEBUG)/exceptions || mkdir -p $(OBJDIR_DEBUG)/exceptions

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/sprites/buzz_saw_sprite.o: sprites/buzz_saw_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/buzz_saw_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/buzz_saw_sprite.o

$(OBJDIR_DEBUG)/sprites/bad_apple_sprite.o: sprites/bad_apple_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/bad_apple_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/bad_apple_sprite.o

$(OBJDIR_DEBUG)/sprites/sprite_system.o: sprites/sprite_system.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/sprite_system.cpp -o $(OBJDIR_DEBUG)/sprites/sprite_system.o

$(OBJDIR_DEBUG)/sprite_movement/swimming_sprite_movement.o: sprite_movement/swimming_sprite_movement.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprite_movement/swimming_sprite_movement.cpp -o $(OBJDIR_DEBUG)/sprite_movement/swimming_sprite_movement.o

$(OBJDIR_DEBUG)/sprites/cloud_platform_sprite.o: sprites/cloud_platform_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/cloud_platform_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/cloud_platform_sprite.o

$(OBJDIR_DEBUG)/sprite_movement/sprite_movement.o: sprite_movement/sprite_movement.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprite_movement/sprite_movement.cpp -o $(OBJDIR_DEBUG)/sprite_movement/sprite_movement.o

$(OBJDIR_DEBUG)/sprite_movement/grounded_sprite_movement.o: sprite_movement/grounded_sprite_movement.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprite_movement/grounded_sprite_movement.cpp -o $(OBJDIR_DEBUG)/sprite_movement/grounded_sprite_movement.o

$(OBJDIR_DEBUG)/sprite_movement/fluttering_sprite_movement.o: sprite_movement/fluttering_sprite_movement.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprite_movement/fluttering_sprite_movement.cpp -o $(OBJDIR_DEBUG)/sprite_movement/fluttering_sprite_movement.o

$(OBJDIR_DEBUG)/sprite_components/sprite_component_up_and_down.o: sprite_components/sprite_component_up_and_down.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprite_components/sprite_component_up_and_down.cpp -o $(OBJDIR_DEBUG)/sprite_components/sprite_component_up_and_down.o

$(OBJDIR_DEBUG)/sprites/fishstick_sprite.o: sprites/fishstick_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/fishstick_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/fishstick_sprite.o

$(OBJDIR_DEBUG)/sprites/iceblock_sprite.o: sprites/iceblock_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/iceblock_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/iceblock_sprite.o

$(OBJDIR_DEBUG)/sprites/hydrant_sprite.o: sprites/hydrant_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/hydrant_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/hydrant_sprite.o

$(OBJDIR_DEBUG)/sprites/handgun_sprite.o: sprites/handgun_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/handgun_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/handgun_sprite.o

$(OBJDIR_DEBUG)/sprites/guard_sprite.o: sprites/guard_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/guard_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/guard_sprite.o

$(OBJDIR_DEBUG)/sprites/falling_bough_sprite.o: sprites/falling_bough_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/falling_bough_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/falling_bough_sprite.o

$(OBJDIR_DEBUG)/sprites/enemy_cart_sprite.o: sprites/enemy_cart_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/enemy_cart_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/enemy_cart_sprite.o

$(OBJDIR_DEBUG)/sprites/eggnon_sprite.o: sprites/eggnon_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/eggnon_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/eggnon_sprite.o

$(OBJDIR_DEBUG)/levels/map_layer_image.o: levels/map_layer_image.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c levels/map_layer_image.cpp -o $(OBJDIR_DEBUG)/levels/map_layer_image.o

$(OBJDIR_DEBUG)/levels/map_layer_constellation.o: levels/map_layer_constellation.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c levels/map_layer_constellation.cpp -o $(OBJDIR_DEBUG)/levels/map_layer_constellation.o

$(OBJDIR_DEBUG)/levels/map.o: levels/map.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c levels/map.cpp -o $(OBJDIR_DEBUG)/levels/map.o

$(OBJDIR_DEBUG)/levels/level.o: levels/level.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c levels/level.cpp -o $(OBJDIR_DEBUG)/levels/level.o

$(OBJDIR_DEBUG)/inventory/inventory_level.o: inventory/inventory_level.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c inventory/inventory_level.cpp -o $(OBJDIR_DEBUG)/inventory/inventory_level.o

$(OBJDIR_DEBUG)/inventory/inventory.o: inventory/inventory.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c inventory/inventory.cpp -o $(OBJDIR_DEBUG)/inventory/inventory.o

$(OBJDIR_DEBUG)/input.o: input.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c input.cpp -o $(OBJDIR_DEBUG)/input.o

$(OBJDIR_DEBUG)/graphics/switch_graphics.o: graphics/switch_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/switch_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/switch_graphics.o

$(OBJDIR_DEBUG)/sprite_components/input_component_player.o: sprite_components/input_component_player.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprite_components/input_component_player.cpp -o $(OBJDIR_DEBUG)/sprite_components/input_component_player.o

$(OBJDIR_DEBUG)/sprite_components/sprite_component_right_and_left.o: sprite_components/sprite_component_right_and_left.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprite_components/sprite_component_right_and_left.cpp -o $(OBJDIR_DEBUG)/sprite_components/sprite_component_right_and_left.o

$(OBJDIR_DEBUG)/sprite_components/sprite_component_circle.o: sprite_components/sprite_component_circle.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprite_components/sprite_component_circle.cpp -o $(OBJDIR_DEBUG)/sprite_components/sprite_component_circle.o

$(OBJDIR_DEBUG)/sprite_components/sprite_component.o: sprite_components/sprite_component.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprite_components/sprite_component.cpp -o $(OBJDIR_DEBUG)/sprite_components/sprite_component.o

$(OBJDIR_DEBUG)/sprite_components/input_component_sequence.o: sprite_components/input_component_sequence.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprite_components/input_component_sequence.cpp -o $(OBJDIR_DEBUG)/sprite_components/input_component_sequence.o

$(OBJDIR_DEBUG)/palette.o: palette.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c palette.cpp -o $(OBJDIR_DEBUG)/palette.o

$(OBJDIR_DEBUG)/object.o: object.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c object.cpp -o $(OBJDIR_DEBUG)/object.o

$(OBJDIR_DEBUG)/main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

$(OBJDIR_DEBUG)/states/level_state.o: states/level_state.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c states/level_state.cpp -o $(OBJDIR_DEBUG)/states/level_state.o

$(OBJDIR_DEBUG)/text/text.o: text/text.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c text/text.cpp -o $(OBJDIR_DEBUG)/text/text.o

$(OBJDIR_DEBUG)/states/title_state.o: states/title_state.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c states/title_state.cpp -o $(OBJDIR_DEBUG)/states/title_state.o

$(OBJDIR_DEBUG)/states/pause_state.o: states/pause_state.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c states/pause_state.cpp -o $(OBJDIR_DEBUG)/states/pause_state.o

$(OBJDIR_DEBUG)/states/message_state.o: states/message_state.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c states/message_state.cpp -o $(OBJDIR_DEBUG)/states/message_state.o

$(OBJDIR_DEBUG)/states/level_select_state.o: states/level_select_state.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c states/level_select_state.cpp -o $(OBJDIR_DEBUG)/states/level_select_state.o

$(OBJDIR_DEBUG)/states/level_message_state.o: states/level_message_state.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c states/level_message_state.cpp -o $(OBJDIR_DEBUG)/states/level_message_state.o

$(OBJDIR_DEBUG)/states/game_state.o: states/game_state.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c states/game_state.cpp -o $(OBJDIR_DEBUG)/states/game_state.o

$(OBJDIR_DEBUG)/water_effect.o: water_effect.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c water_effect.cpp -o $(OBJDIR_DEBUG)/water_effect.o

$(OBJDIR_DEBUG)/timers/timer_switch.o: timers/timer_switch.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c timers/timer_switch.cpp -o $(OBJDIR_DEBUG)/timers/timer_switch.o

$(OBJDIR_DEBUG)/timers/timer_simple.o: timers/timer_simple.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c timers/timer_simple.cpp -o $(OBJDIR_DEBUG)/timers/timer_simple.o

$(OBJDIR_DEBUG)/sprites/sprite_status.o: sprites/sprite_status.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/sprite_status.cpp -o $(OBJDIR_DEBUG)/sprites/sprite_status.o

$(OBJDIR_DEBUG)/timers/timer_repeat.o: timers/timer_repeat.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c timers/timer_repeat.cpp -o $(OBJDIR_DEBUG)/timers/timer_repeat.o

$(OBJDIR_DEBUG)/timers/timer.o: timers/timer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c timers/timer.cpp -o $(OBJDIR_DEBUG)/timers/timer.o

$(OBJDIR_DEBUG)/text/text_component_marquee.o: text/text_component_marquee.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c text/text_component_marquee.cpp -o $(OBJDIR_DEBUG)/text/text_component_marquee.o

$(OBJDIR_DEBUG)/text/text_component.o: text/text_component.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c text/text_component.cpp -o $(OBJDIR_DEBUG)/text/text_component.o

$(OBJDIR_DEBUG)/sprites/pufferbee_sprite.o: sprites/pufferbee_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/pufferbee_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/pufferbee_sprite.o

$(OBJDIR_DEBUG)/sprites/player_sprite_fluttering.o: sprites/player_sprite_fluttering.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/player_sprite_fluttering.cpp -o $(OBJDIR_DEBUG)/sprites/player_sprite_fluttering.o

$(OBJDIR_DEBUG)/sprites/player_sprite.o: sprites/player_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/player_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/player_sprite.o

$(OBJDIR_DEBUG)/sprites/player_cart_sprite.o: sprites/player_cart_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/player_cart_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/player_cart_sprite.o

$(OBJDIR_DEBUG)/sprites/racer_sprite.o: sprites/racer_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/racer_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/racer_sprite.o

$(OBJDIR_DEBUG)/sprites/platform_sprite.o: sprites/platform_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/platform_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/platform_sprite.o

$(OBJDIR_DEBUG)/sprites/penguin_sprite.o: sprites/penguin_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/penguin_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/penguin_sprite.o

$(OBJDIR_DEBUG)/sprites/overworld_player_sprite.o: sprites/overworld_player_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/overworld_player_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/overworld_player_sprite.o

$(OBJDIR_DEBUG)/sprites/icecube_sprite.o: sprites/icecube_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/icecube_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/icecube_sprite.o

$(OBJDIR_DEBUG)/sprites/rope_sprite.o: sprites/rope_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/rope_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/rope_sprite.o

$(OBJDIR_DEBUG)/sprites/saw_sprite.o: sprites/saw_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/saw_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/saw_sprite.o

$(OBJDIR_DEBUG)/sprites/sewer_monster_sprite.o: sprites/sewer_monster_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/sewer_monster_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/sewer_monster_sprite.o

$(OBJDIR_DEBUG)/sprites/sillyfish_sprite.o: sprites/sillyfish_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/sillyfish_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/sillyfish_sprite.o

$(OBJDIR_DEBUG)/sprites/spike_egg_sprite.o: sprites/spike_egg_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/spike_egg_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/spike_egg_sprite.o

$(OBJDIR_DEBUG)/sprites/spiky_fruit_sprite.o: sprites/spiky_fruit_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/spiky_fruit_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/spiky_fruit_sprite.o

$(OBJDIR_DEBUG)/sprites/sprite.o: sprites/sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/sprite.cpp -o $(OBJDIR_DEBUG)/sprites/sprite.o

$(OBJDIR_DEBUG)/block_components/block_component_switch.o: block_components/block_component_switch.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_switch.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_switch.o

$(OBJDIR_DEBUG)/block_components/block_component_swimmable.o: block_components/block_component_swimmable.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_swimmable.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_swimmable.o

$(OBJDIR_DEBUG)/block_components/block_component_solid.o: block_components/block_component_solid.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_solid.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_solid.o

$(OBJDIR_DEBUG)/block_components/block_component_single_use.o: block_components/block_component_single_use.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_single_use.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_single_use.o

$(OBJDIR_DEBUG)/block_components/block_component_switch_flip.o: block_components/block_component_switch_flip.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_switch_flip.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_switch_flip.o

$(OBJDIR_DEBUG)/block_components/block_component_money.o: block_components/block_component_money.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_money.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_money.o

$(OBJDIR_DEBUG)/block_components/block_component_message.o: block_components/block_component_message.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_message.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_message.o

$(OBJDIR_DEBUG)/block_components/block_component_low_slope_right.o: block_components/block_component_low_slope_right.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_low_slope_right.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_low_slope_right.o

$(OBJDIR_DEBUG)/block_conditions/block_condition_collide_bottom.o: block_conditions/block_condition_collide_bottom.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_conditions/block_condition_collide_bottom.cpp -o $(OBJDIR_DEBUG)/block_conditions/block_condition_collide_bottom.o

$(OBJDIR_DEBUG)/block_conditions/block_condition_enemy.o: block_conditions/block_condition_enemy.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_conditions/block_condition_enemy.cpp -o $(OBJDIR_DEBUG)/block_conditions/block_condition_enemy.o

$(OBJDIR_DEBUG)/block_conditions/block_condition_collide_top.o: block_conditions/block_condition_collide_top.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_conditions/block_condition_collide_top.cpp -o $(OBJDIR_DEBUG)/block_conditions/block_condition_collide_top.o

$(OBJDIR_DEBUG)/block_conditions/block_condition_collide_right.o: block_conditions/block_condition_collide_right.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_conditions/block_condition_collide_right.cpp -o $(OBJDIR_DEBUG)/block_conditions/block_condition_collide_right.o

$(OBJDIR_DEBUG)/block_conditions/block_condition_collide_left.o: block_conditions/block_condition_collide_left.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_conditions/block_condition_collide_left.cpp -o $(OBJDIR_DEBUG)/block_conditions/block_condition_collide_left.o

$(OBJDIR_DEBUG)/block_components/block_component_low_slope_left.o: block_components/block_component_low_slope_left.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_low_slope_left.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_low_slope_left.o

$(OBJDIR_DEBUG)/block_conditions/block_condition_collide_any.o: block_conditions/block_condition_collide_any.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_conditions/block_condition_collide_any.cpp -o $(OBJDIR_DEBUG)/block_conditions/block_condition_collide_any.o

$(OBJDIR_DEBUG)/block_conditions/block_condition.o: block_conditions/block_condition.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_conditions/block_condition.cpp -o $(OBJDIR_DEBUG)/block_conditions/block_condition.o

$(OBJDIR_DEBUG)/block_components/block_component_warp.o: block_components/block_component_warp.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_warp.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_warp.o

$(OBJDIR_DEBUG)/block_components/block_component_change_direction.o: block_components/block_component_change_direction.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_change_direction.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_change_direction.o

$(OBJDIR_DEBUG)/block_components/block_component_diamond.o: block_components/block_component_diamond.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_diamond.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_diamond.o

$(OBJDIR_DEBUG)/block_components/block_component_conveyor.o: block_components/block_component_conveyor.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_conveyor.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_conveyor.o

$(OBJDIR_DEBUG)/block_components/block_component_climbable.o: block_components/block_component_climbable.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_climbable.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_climbable.o

$(OBJDIR_DEBUG)/block_components/block_component_change_palette.o: block_components/block_component_change_palette.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_change_palette.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_change_palette.o

$(OBJDIR_DEBUG)/block_components/block_component_change_block.o: block_components/block_component_change_block.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_change_block.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_change_block.o

$(OBJDIR_DEBUG)/block_components/block_component_bouncy.o: block_components/block_component_bouncy.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_bouncy.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_bouncy.o

$(OBJDIR_DEBUG)/block_components/block_component.o: block_components/block_component.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component.cpp -o $(OBJDIR_DEBUG)/block_components/block_component.o

$(OBJDIR_DEBUG)/block_components/block_component_lose.o: block_components/block_component_lose.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_lose.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_lose.o

$(OBJDIR_DEBUG)/block_components/block_component_key.o: block_components/block_component_key.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_key.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_key.o

$(OBJDIR_DEBUG)/block_components/block_component_hurt.o: block_components/block_component_hurt.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_hurt.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_hurt.o

$(OBJDIR_DEBUG)/block_components/block_component_high_slope_right.o: block_components/block_component_high_slope_right.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_high_slope_right.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_high_slope_right.o

$(OBJDIR_DEBUG)/block_components/block_component_high_slope_left.o: block_components/block_component_high_slope_left.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_high_slope_left.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_high_slope_left.o

$(OBJDIR_DEBUG)/block_components/block_component_heal.o: block_components/block_component_heal.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_heal.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_heal.o

$(OBJDIR_DEBUG)/block_components/block_component_goal.o: block_components/block_component_goal.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_goal.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_goal.o

$(OBJDIR_DEBUG)/block_components/block_component_dropping.o: block_components/block_component_dropping.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_components/block_component_dropping.cpp -o $(OBJDIR_DEBUG)/block_components/block_component_dropping.o

$(OBJDIR_DEBUG)/graphics/hydrant_graphics.o: graphics/hydrant_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/hydrant_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/hydrant_graphics.o

$(OBJDIR_DEBUG)/graphics/guard_graphics.o: graphics/guard_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/guard_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/guard_graphics.o

$(OBJDIR_DEBUG)/graphics/graphics.o: graphics/graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/graphics.cpp -o $(OBJDIR_DEBUG)/graphics/graphics.o

$(OBJDIR_DEBUG)/graphics/animated_graphics.o: graphics/animated_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/animated_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/animated_graphics.o

$(OBJDIR_DEBUG)/graphics/penguin_graphics.o: graphics/penguin_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/penguin_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/penguin_graphics.o

$(OBJDIR_DEBUG)/goals/windy_goal.o: goals/windy_goal.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c goals/windy_goal.cpp -o $(OBJDIR_DEBUG)/goals/windy_goal.o

$(OBJDIR_DEBUG)/goals/timed_goal.o: goals/timed_goal.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c goals/timed_goal.cpp -o $(OBJDIR_DEBUG)/goals/timed_goal.o

$(OBJDIR_DEBUG)/goals/starving_goal.o: goals/starving_goal.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c goals/starving_goal.cpp -o $(OBJDIR_DEBUG)/goals/starving_goal.o

$(OBJDIR_DEBUG)/goals/goal.o: goals/goal.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c goals/goal.cpp -o $(OBJDIR_DEBUG)/goals/goal.o

$(OBJDIR_DEBUG)/graphics/rotating_graphics.o: graphics/rotating_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/rotating_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/rotating_graphics.o

$(OBJDIR_DEBUG)/graphics/sprite_graphics.o: graphics/sprite_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/sprite_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/sprite_graphics.o

$(OBJDIR_DEBUG)/graphics/sillyfish_graphics.o: graphics/sillyfish_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/sillyfish_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/sillyfish_graphics.o

$(OBJDIR_DEBUG)/graphics/sewer_monster_graphics.o: graphics/sewer_monster_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/sewer_monster_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/sewer_monster_graphics.o

$(OBJDIR_DEBUG)/graphics/saw_graphics.o: graphics/saw_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/saw_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/saw_graphics.o

$(OBJDIR_DEBUG)/graphics/rope_graphics.o: graphics/rope_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/rope_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/rope_graphics.o

$(OBJDIR_DEBUG)/graphics/pufferbee_graphics.o: graphics/pufferbee_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/pufferbee_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/pufferbee_graphics.o

$(OBJDIR_DEBUG)/graphics/player_graphics.o: graphics/player_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/player_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/player_graphics.o

$(OBJDIR_DEBUG)/clock.o: clock.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c clock.cpp -o $(OBJDIR_DEBUG)/clock.o

$(OBJDIR_DEBUG)/camera.o: camera.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c camera.cpp -o $(OBJDIR_DEBUG)/camera.o

$(OBJDIR_DEBUG)/block_type.o: block_type.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_type.cpp -o $(OBJDIR_DEBUG)/block_type.o

$(OBJDIR_DEBUG)/block_system.o: block_system.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_system.cpp -o $(OBJDIR_DEBUG)/block_system.o

$(OBJDIR_DEBUG)/block_conditions/block_condition_rival.o: block_conditions/block_condition_rival.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_conditions/block_condition_rival.cpp -o $(OBJDIR_DEBUG)/block_conditions/block_condition_rival.o

$(OBJDIR_DEBUG)/block_conditions/block_condition_not_ducking.o: block_conditions/block_condition_not_ducking.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_conditions/block_condition_not_ducking.cpp -o $(OBJDIR_DEBUG)/block_conditions/block_condition_not_ducking.o

$(OBJDIR_DEBUG)/block_conditions/block_condition_key.o: block_conditions/block_condition_key.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_conditions/block_condition_key.cpp -o $(OBJDIR_DEBUG)/block_conditions/block_condition_key.o

$(OBJDIR_DEBUG)/block_conditions/block_condition_hero.o: block_conditions/block_condition_hero.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block_conditions/block_condition_hero.cpp -o $(OBJDIR_DEBUG)/block_conditions/block_condition_hero.o

$(OBJDIR_DEBUG)/block.o: block.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c block.cpp -o $(OBJDIR_DEBUG)/block.o

$(OBJDIR_DEBUG)/counter.o: counter.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c counter.cpp -o $(OBJDIR_DEBUG)/counter.o

$(OBJDIR_DEBUG)/event_system.o: event_system.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c event_system.cpp -o $(OBJDIR_DEBUG)/event_system.o

$(OBJDIR_DEBUG)/game.o: game.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c game.cpp -o $(OBJDIR_DEBUG)/game.o

$(OBJDIR_DEBUG)/goals/avoid_money_goal.o: goals/avoid_money_goal.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c goals/avoid_money_goal.cpp -o $(OBJDIR_DEBUG)/goals/avoid_money_goal.o

$(OBJDIR_DEBUG)/goals/collect_goal.o: goals/collect_goal.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c goals/collect_goal.cpp -o $(OBJDIR_DEBUG)/goals/collect_goal.o

$(OBJDIR_DEBUG)/goals/do_nothing_goal.o: goals/do_nothing_goal.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c goals/do_nothing_goal.cpp -o $(OBJDIR_DEBUG)/goals/do_nothing_goal.o
	
$(OBJDIR_DEBUG)/sprites/maze_chaser_sprite.o: sprites/maze_chaser_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/maze_chaser_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/maze_chaser_sprite.o
	
$(OBJDIR_DEBUG)/graphics/overworld_player_graphics.o: graphics/overworld_player_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/overworld_player_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/overworld_player_graphics.o
	
$(OBJDIR_DEBUG)/graphics/maze_chaser_graphics.o: graphics/maze_chaser_graphics.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c graphics/maze_chaser_graphics.cpp -o $(OBJDIR_DEBUG)/graphics/maze_chaser_graphics.o
	
$(OBJDIR_DEBUG)/sprites/heat_beam_sprite.o: sprites/heat_beam_sprite.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c sprites/heat_beam_sprite.cpp -o $(OBJDIR_DEBUG)/sprites/heat_beam_sprite.o
	
$(OBJDIR_DEBUG)/goals/survive_time_goal.o: goals/survive_time_goal.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c goals/survive_time_goal.cpp -o $(OBJDIR_DEBUG)/goals/survive_time_goal.o
	
$(OBJDIR_DEBUG)/text/news_ticker.o: text/news_ticker.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c text/news_ticker.cpp -o $(OBJDIR_DEBUG)/text/news_ticker.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)/sprites
	rm -rf $(OBJDIR_DEBUG)
	rm -rf $(OBJDIR_DEBUG)/sprite_movement
	rm -rf $(OBJDIR_DEBUG)/sprite_components
	rm -rf $(OBJDIR_DEBUG)/levels
	rm -rf $(OBJDIR_DEBUG)/inventory
	rm -rf $(OBJDIR_DEBUG)/graphics
	rm -rf $(OBJDIR_DEBUG)/states
	rm -rf $(OBJDIR_DEBUG)/text
	rm -rf $(OBJDIR_DEBUG)/timers
	rm -rf $(OBJDIR_DEBUG)/block_components
	rm -rf $(OBJDIR_DEBUG)/block_conditions
	rm -rf $(OBJDIR_DEBUG)/goals
	rm -rf $(OBJDIR_DEBUG)/exceptions

.PHONY: before_debug after_debug clean_debug