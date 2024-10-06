/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#pragma once

/**
 * @file pmw3389.h
 *
 * @brief Header file for the pmw3389 driver.
 */

#include <zephyr/drivers/sensor.h>
#include "../pixart.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* Timings defined by spec (in us) */
#define T_NCS_SCLK 1                    /* 120 ns (rounded to 1us?)*/
#define T_SRX (20 - T_NCS_SCLK)         /* 20 us */
#define T_SCLK_NCS_WR (35 - T_NCS_SCLK) /* 35 us */
#define T_SWX (180 - T_SCLK_NCS_WR)     /* 180 us */
#define T_SRAD 160                      /* 160 us */
#define T_SRAD_MOTBR 35                 /* 35 us */
#define T_BEXIT 1                       /* 500 ns (rounded to 1us?)*/

/* Timing defined on SROM download burst mode figure */
#define T_BRSEP 15 /* 15 us */

/* Sensor registers */
#define PMW3389_REG_PRODUCT_ID 0x00
#define PMW3389_REG_REVISION_ID 0x01
#define PMW3389_REG_MOTION 0x02
#define PMW3389_REG_DELTA_X_L 0x03
#define PMW3389_REG_DELTA_X_H 0x04
#define PMW3389_REG_DELTA_Y_L 0x05
#define PMW3389_REG_DELTA_Y_H 0x06
#define PMW3389_REG_SQUAL 0x07
#define PMW3389_REG_RAW_DATA_SUM 0x08
#define PMW3389_REG_MAXIMUM_RAW_DATA 0x09
#define PMW3389_REG_MINIMUM_RAW_DATA 0x0A
#define PMW3389_REG_SHUTTER_LOWER 0x0B
#define PMW3389_REG_SHUTTER_UPPER 0x0C
#define PMW3389_REG_CONTROL 0x0D
#define PMW3389_REG_CONFIG1 0x0F
#define PMW3389_REG_CONFIG2 0x10
#define PMW3389_REG_ANGLE_TUNE 0x11
#define PMW3389_REG_FRAME_CAPTURE 0x12
#define PMW3389_REG_SROM_ENABLE 0x13
#define PMW3389_REG_RUN_DOWNSHIFT 0x14
#define PMW3389_REG_REST1_RATE_LOWER 0x15
#define PMW3389_REG_REST1_RATE_UPPER 0x16
#define PMW3389_REG_REST1_DOWNSHIFT 0x17
#define PMW3389_REG_REST2_RATE_LOWER 0x18
#define PMW3389_REG_REST2_RATE_UPPER 0x19
#define PMW3389_REG_REST2_DOWNSHIFT 0x1A
#define PMW3389_REG_REST3_RATE_LOWER 0x1B
#define PMW3389_REG_REST3_RATE_UPPER 0x1C
#define PMW3389_REG_OBSERVATION 0x24
#define PMW3389_REG_DATA_OUT_LOWER 0x25
#define PMW3389_REG_DATA_OUT_UPPER 0x26
#define PMW3389_REG_RAW_DATA_DUMP 0x29
#define PMW3389_REG_SROM_ID 0x2A
#define PMW3389_REG_MIN_SQ_RUN 0x2B
#define PMW3389_REG_RAW_DATA_THRESHOLD 0x2C
#define PMW3389_REG_CONFIG5 0x2F
#define PMW3389_REG_POWER_UP_RESET 0x3A
#define PMW3389_REG_SHUTDOWN 0x3B
#define PMW3389_REG_INVERSE_PRODUCT_ID 0x3F
#define PMW3389_REG_LIFTCUTOFF_TUNE3 0x41
#define PMW3389_REG_ANGLE_SNAP 0x42
#define PMW3389_REG_LIFTCUTOFF_TUNE1 0x4A
#define PMW3389_REG_MOTION_BURST 0x50
#define PMW3389_REG_LIFTCUTOFF_TUNE_TIMEOUT 0x58
#define PMW3389_REG_LIFTCUTOFF_TUNE_MIN_LENGTH 0x5A
#define PMW3389_REG_SROM_LOAD_BURST 0x62
#define PMW3389_REG_LIFT_CONFIG 0x63
#define PMW3389_REG_RAW_DATA_BURST 0x64
#define PMW3389_REG_LIFTCUTOFF_TUNE2 0x65

/* Sensor identification values */
#define PMW3389_PRODUCT_ID 0x47
#define PMW3389_FIRMWARE_ID 0xE8

/* Power-up register commands */
#define PMW3389_POWERUP_CMD_RESET 0x5A

/* Max register count readable in a single motion burst */
#define PMW3389_MAX_BURST_SIZE 12

/* Register count used for reading a single motion burst */
#define PMW3389_BURST_SIZE 6

/* Position of X in motion burst data */
#define PMW3389_DX_POS 2
#define PMW3389_DY_POS 4

/* Rest_En position in Config2 register. */
#define PMW3389_REST_EN_POS 5

#define PMW3389_MAX_CPI 12000
#define PMW3389_MIN_CPI 100

#define SPI_WRITE_BIT BIT(7)

/* Helper macros used to convert sensor values. */
#define PMW3389_SVALUE_TO_CPI(svalue) ((uint32_t)(svalue).val1)
#define PMW3389_SVALUE_TO_TIME(svalue) ((uint32_t)(svalue).val1)
#define PMW3389_SVALUE_TO_BOOL(svalue) ((svalue).val1 != 0)

/** @brief Sensor specific attributes of PMW3389. */
enum pmw3389_attribute {
	/** Sensor CPI for both X and Y axes. */
	PMW3389_ATTR_CPI = SENSOR_ATTR_PRIV_START,

	/** Enable or disable sleep modes. */
	PMW3389_ATTR_REST_ENABLE,

	/** Entering time from Run mode to REST1 mode [ms]. */
	PMW3389_ATTR_RUN_DOWNSHIFT_TIME,

	/** Entering time from REST1 mode to REST2 mode [ms]. */
	PMW3389_ATTR_REST1_DOWNSHIFT_TIME,

	/** Entering time from REST2 mode to REST3 mode [ms]. */
	PMW3389_ATTR_REST2_DOWNSHIFT_TIME,

	/** Sampling frequency time during REST1 mode [ms]. */
	PMW3389_ATTR_REST1_SAMPLE_TIME,

	/** Sampling frequency time during REST2 mode [ms]. */
	PMW3389_ATTR_REST2_SAMPLE_TIME,

	/** Sampling frequency time during REST3 mode [ms]. */
	PMW3389_ATTR_REST3_SAMPLE_TIME,
};

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

//#endif /* ZEPHYR_INCLUDE_PMW3389_H_ */
