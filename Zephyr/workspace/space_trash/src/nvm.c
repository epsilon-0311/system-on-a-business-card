
#include <stdio.h>
#include <inttypes.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>

#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>


#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(nvm, LOG_LEVEL_DBG);

static struct nvs_fs fs;
static struct flash_pages_info info;

#define NVS_PARTITION		storage_partition
#define NVS_PARTITION_DEVICE	FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET	FIXED_PARTITION_OFFSET(NVS_PARTITION)


int nvm_init(void)
{
    int rc = 0;
	/* define the nvs file system by settings with:
	 *	sector_size equal to the pagesize,
	 *	3 sectors
	 *	starting at NVS_PARTITION_OFFSET
	 */
	fs.flash_device = NVS_PARTITION_DEVICE;
    if (!device_is_ready(fs.flash_device)) {
		LOG_ERR("Flash device %s is not ready", fs.flash_device->name);
		return -1;
	}
	fs.offset = NVS_PARTITION_OFFSET;
	rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
	if (rc) {
		LOG_ERR("Unable to get page info");
		return rc;
	}
	fs.sector_size = info.size;
	fs.sector_count = 3U;

	rc = nvs_mount(&fs);
	if (rc) {
		LOG_ERR("Flash Init failed");
		return rc;
	}

    LOG_DBG("NVS initialized");
    return 0;
}

int nvm_read(uint16_t nv_id, uint8_t *data, uint16_t data_size, bool initialize, uint8_t *init_data)
{
    if(data == NULL || data_size ==0)
    {
        return -1;
    }

    int rc = nvs_read(&fs, nv_id, data, data_size);
    if (rc > 0) { /* item was found, show it */
		LOG_DBG("Found entry for ID %u, read %d bytes", nv_id, rc);
        if(rc != data_size)
        {
            LOG_ERR("Reda data of entry %d, expected %d bytes, got %d", nv_id, data_size, rc);
            return -2;
        }
        else
        {
            /*
            for (int n = 0; n < data_size; n++) {
                k_sleep(K_MSEC(1000)); 
                printk("%x ", data[n]);
            }
            printk("\n");
            */
        }
	} else  if(initialize == true &&init_data != NULL )  {/* item was not found, add it */
		LOG_DBG("ID not found found, adding entry %d\n", nv_id);
		rc = nvs_write(&fs, nv_id, init_data, data_size);
        if(rc != data_size)
        {
            LOG_ERR("Written data to entry %d, expected %d bytes, got %d", nv_id, data_size, rc);
            return -2;
        }
    }

    return rc;
}

int nvm_write(uint16_t nv_id, uint8_t *data, uint16_t data_size)
{
    if(data == NULL || data_size == 0)
    {
        return -1;
    }
    int rc = nvs_write(&fs, nv_id, data, data_size);
    if(rc != data_size)
    {
        LOG_ERR("Written data to entry %d, expected %d bytes, got %d", nv_id, data_size, rc);
        return -2;
    }
    LOG_DBG("Written %d bytes to entry %d",  rc, nv_id);
    return rc;
}