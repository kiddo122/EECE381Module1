/*
 * Game.cpp
 *
 *  Created on: Oct 12, 2014
 *      Author: Emmett
 */

#include "Game.h"

// Constructor
Game::Game() {

}

void Game::init() {
	this->sd_card.init();

	this->vga_screen.init(sd_card);
	this->vga_screen.clear_screen(vga_screen.pixel_buffer);
	this->vga_screen.clear_characters();
}

void Game::draw_map_and_player() {
	this->vga_screen.paint_screen(vga_screen.pixel_buffer, 0xFFF0);
	this->vga_screen.draw_map_from_array(matrix_map.map);
	this->vga_screen.draw_box_from_coordinate(0, 0, 'p');
}

void Game::menu_screen() {
	this->vga_screen.draw_line(this->vga_screen.pixel_buffer, 137, 182, 80, 80,
			0xFFFF);
	this->vga_screen.draw_line(this->vga_screen.pixel_buffer, 137, 182, 100,
			100, 0xFFFF);
	this->vga_screen.draw_line(this->vga_screen.pixel_buffer, 137, 137, 80, 100,
			0xFFFF);
	this->vga_screen.draw_line(this->vga_screen.pixel_buffer, 182, 182, 80, 100,
			0xFFFF);

	this->vga_screen.draw_line(this->vga_screen.pixel_buffer, 133, 186, 80 + 60,
			80 + 60, 0xFFFF);
	this->vga_screen.draw_line(this->vga_screen.pixel_buffer, 133, 186,
			100 + 60, 100 + 60, 0xFFFF);
	this->vga_screen.draw_line(this->vga_screen.pixel_buffer, 133, 133, 80 + 60,
			100 + 60, 0xFFFF);
	this->vga_screen.draw_line(this->vga_screen.pixel_buffer, 186, 186, 80 + 60,
			100 + 60, 0xFFFF);

	alt_up_char_buffer_string(this->vga_screen.char_buffer, "Start Game", 35,
			22);
	alt_up_char_buffer_string(this->vga_screen.char_buffer, "Toggle Sound", 34,
			37);

	while (1) {
		usleep(100000);
		if (this->player1.get_direction() == 'R')
			break;
	}

	return;

}

void Game::match_start() {
	this->vga_screen.clear_characters();
	this->draw_map_and_player();
	alt_up_char_buffer_string(this->vga_screen.char_buffer, "3", 40, 25);
	usleep(500000);
	this->vga_screen.clear_characters();
	usleep(500000);
	alt_up_char_buffer_string(this->vga_screen.char_buffer, "2", 40, 25);
	usleep(500000);
	this->vga_screen.clear_characters();
	usleep(500000);
	alt_up_char_buffer_string(this->vga_screen.char_buffer, "1", 40, 25);
	usleep(500000);
	this->vga_screen.clear_characters();
	usleep(500000);
	alt_up_char_buffer_string(this->vga_screen.char_buffer, "GO!", 39, 25);
	usleep(500000);
	this->vga_screen.clear_characters();
	usleep(500000);

	while (1) {

		this->game_logic(player1, matrix_map, vga_screen);
		this->game_drawing(player1, matrix_map, vga_screen);
		usleep(30000);
	}
}

void Game::game_drawing(Player &player1, MatrixMap &matrix_map,
		VGA_Screen &vga_screen) {
	vga_screen.erase_and_redraw_player(player1.get_old_x_cord(),
			player1.get_old_y_cord(), player1.get_x_cord(),
			player1.get_y_cord());
	if (player1.bomb.isActive()) {
		vga_screen.draw_bomb(player1.bomb.get_x_cord(),
				player1.bomb.get_y_cord());
	} else if (player1.bomb.isExploding()) {
		vga_screen.draw_explosion(player1.bomb.damaged_blocks, true);
	} else if (player1.bomb.finishedExploding()) {
		vga_screen.draw_explosion(player1.bomb.damaged_blocks, false);
	}
}

void Game::game_logic(Player &player1, MatrixMap &matrix_map,
		VGA_Screen &vga_screen) {
	player1.move(player1.get_direction(), matrix_map);
	player1.place_bomb(matrix_map);

	player1.bomb.increment_timer();
	if (player1.bomb.exploded()) {
		matrix_map.check_damaged_blocks(player1.bomb.get_x_cord(),
				player1.bomb.get_y_cord(), player1.bomb.get_explosion_range(),
				player1.bomb.damaged_blocks);
		player1.check_damage(player1.bomb.damaged_blocks);
	}
}

void Game::run() {
	this->menu_screen();
	this->match_start();
}

