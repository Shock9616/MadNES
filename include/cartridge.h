#include "types.h"

#include <stdio.h>

/*
 * Read the contents of the rom file header and fill the cartridge metadata
 *
 * @param cart - The struct representing the cartridge data
 * @param rom_file - The file containing the rom data
 *
 * @returns - 0 if the header is read correctly
 */
int readHeader(Cartridge* cart, FILE* rom_file);

/*
 * Load the contents of the given rom into a cartridge
 *
 * @param cart - The struct representing the cartridge data
 * @param rom_path - The path to the rom file
 *
 * @returns - 0 if the cartridge data is loaded correctly
 */
int loadRom(Cartridge* cart, const char* rom_path);

/*
 * Print out the metadata of the cartridge
 *
 * @param cart - The struct representing the cartridge data
 */
void printCartMetadata(const Cartridge* cart);
