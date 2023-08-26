
#ifndef __SPACE_TRASH_NVM_H__
#define __SPACE_TRASH_NVM_H__

#include <stdio.h>
#include <inttypes.h>

int nvm_init(void);

int nvm_read(uint16_t nv_id, uint8_t *data, uint16_t data_size, bool initialize, uint8_t *init_data);

int nvm_write(uint16_t nv_id, uint8_t *data, uint16_t data_size);

#endif