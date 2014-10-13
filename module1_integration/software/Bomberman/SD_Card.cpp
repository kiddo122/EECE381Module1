/*
 * SD_Card.cpp
 *
 *  Created on: Oct 7, 2014
 *      Author: Emmett
 */
#include "SD_Card.h"
#include <stdio.h>

void SD_Card::look_for_other_files(char *filename) {
	int status = alt_up_sd_card_find_next(filename);
	while (status == 0) {
		printf("%s\n", filename);
		status = alt_up_sd_card_find_next(filename);
	}

	switch (status) {
	case 1: //invalid directory
		printf("Invalid Directory.\n");
		break;
	case 2: //no card present or no FAT 16 partition
		printf("Card not present or no FAT 16 partition.\n");
		break;
	case 3: //Function alt_up_sd_card_find_first should be called first
		printf("Function alt_up_sd_card_find_first should be called first.\n");
		break;
	case -1: //No files were found in the directory
		printf("No (more) files were found.\n");
		break;
	}
}

void SD_Card::scan_sd_card(char *directory, char *filename) {
	switch (alt_up_sd_card_find_first(directory, filename)) {
	case 0: //success
		printf("%s\n", filename);
		look_for_other_files(filename);
		break;
	case 1: //invalid directory
		printf("Invalid Directory.\n");
		break;
	case 2: //no card present or no FAT 16 partition
		printf("Card not present or no FAT 16 partition.\n");
		break;
	case -1: //No files were found in the directory
		printf("The directory is empty.\n");
		break;
	}
}

// Returns true if sd card working
bool SD_Card::init() {
	this->device_reference = NULL;
	this->connected = 0;
	this->directory = "folder/";
	this->device_reference = alt_up_sd_card_open_dev(
			"/dev/Altera_UP_SD_Card_Avalon_Interface_0");
	printf("Program running\n");

	if (this->device_reference != NULL) {
		while (1) {
			if ((this->connected == 0) && (alt_up_sd_card_is_Present())) {
				printf("Card connected.\n");
				if (alt_up_sd_card_is_FAT16()) {
					printf("FAT16 file system detected.\n");
					this->setup_bitmaps_from_sd_card();
					return true;
				} else {
					printf("Unknown file system.\n");
					return false;

				}
				connected = 1;
			} else if ((connected == 1)
					&& (alt_up_sd_card_is_Present() == false)) {
				printf("Card disconnected.\n");
				connected = 0;
				return false;
			}
		}
		return false;
	} else {
		printf("Device reference is wrong\n");
		return false;
	}
}

/*
 * add in new bitmaps here
 */
void SD_Card::setup_bitmaps_from_sd_card(){
	this->change_bitmap_array_from_file(Flame_Bitmap, flame_array);
	this->change_bitmap_array_from_file(Speed_Powerup_Bitmap, speed_powerup_array);
	this->change_bitmap_array_from_file(Bomberman_Right_B0, bomberman_backwards_f00_array);
	this->change_bitmap_array_from_file(Bomberman_Right_B1, bomberman_backwards_f01_array);
	this->change_bitmap_array_from_file(Bomberman_Right_B2, bomberman_backwards_f02_array);
	this->change_bitmap_array_from_file(Bomberman_Right_B3, bomberman_backwards_f03_array);
	this->change_bitmap_array_from_file(Bomberman_Right_B4, bomberman_backwards_f04_array);
	this->change_bitmap_array_from_file(Bomberman_Right_B5, bomberman_backwards_f05_array);
	this->change_bitmap_array_from_file(Bomberman_Right_B6, bomberman_backwards_f06_array);
	this->change_bitmap_array_from_file(Bomberman_Right_B7, bomberman_backwards_f07_array);
	this->change_bitmap_array_from_file(Solid_Block, solid_block);
	this->change_bitmap_array_from_file(Destr_Block, destr_block);
	this->change_bitmap_array_from_file(Bomb_Map, bomb_Bmap);
}

void SD_Card::change_bitmap_array_from_file(char* filename, short int bitmap_array[20][20]) {

	short int file_handle = alt_up_sd_card_fopen(filename,
			false);
	short int header_buffer[54];
	printf("\n");
	for (int i = 0; i < 54; i++) {
		header_buffer[i] = (alt_up_sd_card_read(file_handle) & 0xFF);
		printf("%hx, ", header_buffer[i]);
	}
	unsigned char r;
	unsigned char g;
	unsigned char b;
	short pixel;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			r = (alt_up_sd_card_read(file_handle));
			g = (alt_up_sd_card_read(file_handle));
			b = (alt_up_sd_card_read(file_handle));
			r = r >> 3;
			b = b >> 3;
			g = g >> 2;

			pixel = b;
			pixel = pixel << 6;
			pixel = pixel | g;
			pixel = pixel << 5;
			pixel = pixel | r;
			//				pixel = (short)((((r<<6)|g)<<5)|b);
			bitmap_array[j][19-i] = pixel;
		}
	}
}
