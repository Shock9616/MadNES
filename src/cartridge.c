#include "cartridge.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Load the contents of the given rom into a cartridge
 *
 * @param cart - The struct representing the cartridge data
 * @param rom_path - The path to the rom file
 *
 * @returns 0 if the rom was loaded properly
 * @returns -1 if there was an error opening the rom file
 * @returns -2 if there was an error validating the magic number
 * @returns -3 if there was an error validating the PRG-ROM size
 * @returns -4 if there was an error validating the CHR-ROM size
 * @returns -5 if there was an error validating Flags 6
 * @returns -6 if there was an error validating Flags 7
 * @returns -7 if there was an error validating Flags 8
 * @returns -8 if there was an error validating Flags 9
 * @returns -9 if there was an error validating the reserved bits
 * @returns -10 if there was an error validating the read trainer size
 * @returns -11 if there was an error validating the read PRG-ROM size
 * @returns -12 if there was an error validating the read CHR-ROM size
 */
int loadRom(Cartridge* cart, const char* rom_path) {
    // Open the rom file and load its contents into the appropriate fields
    FILE* rom_data = fopen(rom_path, "rb");

    if (!rom_data) {
        fprintf(stderr, "ERROR: Failed to open rom file\n");
        return -1;
    }

    // Read magic number
    int read_size = fread(&cart->magic_num, 1, 4, rom_data);

    // Validate magic number
    if (read_size != 4 || memcmp(cart->magic_num, "NES\x1A", 4) != 0) {
        fprintf(stderr, "ERROR: Failed to validate magic number\n");
        return -2;
    }

    // Read PRG-ROM size
    read_size = fread(&cart->prg_rom_size, 1, 1, rom_data);

    // Validate PRG-ROM size
    if (read_size != 1) {
        fprintf(stderr, "ERROR: Failed to properly read PRG-ROM size\n");
        return -3;
    }

    // Read CHR-ROM size
    read_size = fread(&cart->chr_rom_size, 1, 1, rom_data);

    // Validate CHR-ROM size
    if (read_size != 1) {
        fprintf(stderr, "ERROR: Failed to properly read CHR-ROM size\n");
        return -4;
    }

    // Read Flags 6
    read_size = fread(&cart->flags6, 1, 1, rom_data);

    // Validate Flags 6
    if (read_size != 1) {
        fprintf(stderr, "ERROR: Failed to properly read Flags 6\n");
        return -5;
    }

    // Read Flags 7
    read_size = fread(&cart->flags7, 1, 1, rom_data);

    // Validate Flags 7
    if (read_size != 1) {
        fprintf(stderr, "ERROR: Failed to properly read Flags 7\n");
        return -6;
    }

    // Read Flags 8
    read_size = fread(&cart->flags8, 1, 1, rom_data);

    // Validate Flags 8
    if (read_size != 1) {
        fprintf(stderr, "ERROR: Failed to properly read Flags 8\n");
        return -7;
    }

    // Read Flags 9
    read_size = fread(&cart->flags9, 1, 1, rom_data);

    // Validate Flags 9
    if (read_size != 1) {
        fprintf(stderr, "ERROR: Failed to properly read Flags 9\n");
        return -8;
    }

    // Read Flags 10
    read_size = fread(&cart->flags10, 1, 1, rom_data);

    // Validate Flags 10
    if (read_size != 1) {
        fprintf(stderr, "ERROR: Failed to properly read Flags 10\n");
        return -8;
    }

    // Read reserved bits (should be 0)
    read_size = fread(&cart->reserved, 1, 5, rom_data);

    // Validate reserved bits
    if (read_size != 5 || cart->reserved[0] != 0 || cart->reserved[1] != 0 ||
        cart->reserved[2] != 0 || cart->reserved[3] != 0 || cart->reserved[4] != 0) {
        fprintf(stderr, "ERROR: Failed to properly read reserved bits\n");
        return -9;
    }

    // If the trainer flag is set, read in the trainer section
    if (cart->flags6 & 0x04) {
        cart->trainer = malloc(512);
        read_size = fread(cart->trainer, 1, 512, rom_data);

        // Validate trainer size
        if (read_size != 512) {
            fprintf(stderr, "ERROR: Failed to properly read trainer\n");
            free(cart->trainer);
            return -10;
        }
    } else {
        cart->trainer = NULL;
    }

    // Read in the PRG-ROM data
    cart->prg_rom = malloc(cart->prg_rom_size * 16 * 1024);
    read_size = fread(cart->prg_rom, 1, cart->prg_rom_size * 16 * 1024, rom_data);

    // Validate PRG-ROM read size
    if (read_size != cart->prg_rom_size * 16 * 1024) {
        fprintf(stderr, "ERROR: Failed to properly read PRG-ROM");
        if (cart->trainer) {
            free(cart->trainer);
        }
        free(cart->prg_rom);
        return -11;
    }

    // Read in the CHR-ROM data
    cart->chr_rom = malloc(cart->chr_rom_size * 8 * 1024);
    read_size = fread(cart->chr_rom, 1, cart->chr_rom_size * 8 * 1024, rom_data);

    // Validate CHR-ROM read size
    if (read_size != cart->chr_rom_size * 8 * 1024) {
        fprintf(stderr, "ERROR: Failed to properly read CHR-ROM");
        if (cart->trainer) {
            free(cart->trainer);
        }
        free(cart->prg_rom);
        free(cart->chr_rom);
        return -12;
    }

    return 0;
}

/**
 * Print out the metadata of the cartridge
 *
 * @param cart - The struct representing the cartridge data
 */
void printCartMetadata(const Cartridge* cart) {
    int mapper_num =
        ((cart->flags8 & 0x0F) << 8) | ((cart->flags7 & 0xF0) << 4) | (cart->flags6 & 0xF0);
    char* console_type;
    switch (cart->flags7 & 0x03) {
        case 0:
            console_type = "NES/Famicom";
            break;
        case 1:
            console_type = "Nintendo Vs. System";
            break;
        case 2:
            console_type = "Nintendo Playchoice 10";
            break;
        case 3:
            console_type = "Extended";
            break;
    }

    printf("---------- Cartridge Metadata ----------\n");
    printf("       PRG-ROM Size: %d KB\n", cart->prg_rom_size * 16);
    printf("       CHR-ROM Size: %d KB\n", cart->chr_rom_size * 8);
    printf("             Mapper: %d\n", mapper_num);
    printf("          Submapper: %d\n", cart->flags8 & 0xF0);
    printf("          Mirroring: %s\n", (cart->flags6 & 0x01) ? "Vertical" : "Horizontal");
    printf("            Battery: %s\n", (cart->flags6 & 0x02) ? "Present" : "Not Present");
    printf("            Trainer: %s\n", (cart->trainer != NULL) ? "Present" : "Not Present");
    printf("     Alt Nametables: %s\n", (cart->flags6 & 0x08) ? "Yes" : "No");
    printf("       Console Type: %s\n", console_type);
    printf("----------------------------------------\n");
}
