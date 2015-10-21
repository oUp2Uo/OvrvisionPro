/**************************************************************************
 *
 *              Copyright (c) 2014-2015 by Wizapply.
 *
 *  This software is copyrighted by and is the sole property of Wizapply
 *  All rights, title, ownership, or other interests in the software
 *  remain the property of Wizapply.  This software may only be used
 *  in accordance with the corresponding license agreement.
 *  Any unauthorized use, duplication, transmission, distribution,
 *  or disclosure of this software is expressly forbidden.
 *
 *  This Copyright notice may not be removed or modified without prior
 *  written consent of Wizapply.
 *
 *  Wizpply reserves the right to modify this software without notice.
 *
 *  Wizapply                                info@wizapply.com
 *  5F, KS Building,                        http://wizapply.com
 *  3-7-10 Ichiokamotomachi, Minato-ku,
 *  Osaka, 552-0002, Japan
 *
***************************************************************************/

/**************************************************************************
 *
 *  Ovrvision Pro FirmWare v1.0
 *
 *  Language is 'C' code source
 *  Files : ov5653_sensor.h
 *
***************************************************************************/

#ifndef _OV5653_SENSOR_H_
#define _OV5653_SENSOR_H_

#include <cyu3types.h>

// Define

/* I2C Slave address for the image sensor. */
#define I2C_SENSOR_ADDR_WR 0x6C         /* Slave address used to write sensor registers. Default set to EEPROM. */
#define I2C_SENSOR_ADDR_RD 0x6D         /* Slave address used to read from sensor registers. Default set to EEPROM */
#define I2C_MEMORY_ADDR_WR 0xA0         /* I2C slave address used to write to an EEPROM. */
#define I2C_MEMORY_ADDR_RD 0xA1         /* I2C slave address used to read from an EEPROM. */

#define I2C_SLAVEADDR_MASK 0xFE         /* Mask to get actual I2C slave address value without direction bit. */

/* GPIO 25 on FX3 is used to frex the Image sensor. */
#define OV5653_SENSOR_FREX_GPIO_PIN 25

// I2C functions
extern CyU3PReturnStatus_t WriteI2C (uint8_t slaveAddr, uint16_t address, uint8_t data);
extern CyU3PReturnStatus_t ReadI2C (uint8_t slaveAddr, uint16_t address, uint8_t *data);

#define WI2C(x,y) WriteI2C(I2C_SENSOR_ADDR_WR,x,y)

// Sensor system control function
extern void OV5653SensorInit(void);			// Initialize sensor
extern void OV5653SensorReset(void);		// Reset sensor
extern uint8_t OV5653SensorBusTest(void);	// Test sensor

// Sensor setup
extern void OV5653Sensor_5MP15FPS();		//2560 x 1920 @ 15fps	X 2
extern void OV5653Sensor_FullHD30FPS();		//1920 x 1080 @ 30fps	X 2
extern void OV5653Sensor_UVGA45FPS();		//1280 x 960 @ 45fps	X 2
extern void OV5653Sensor_VR60FPS();			//1280 x 800 @ 60fps	X 2 Oculus Rift
extern void OV5653Sensor_VR10FPS();			//1280 x 800 @ 10fps	X 2 Oculus Rift, For USB2.0
extern void OV5653Sensor_VGA90FPS();		//640 x 480 @ 90fps	    X 2
extern void OV5653Sensor_VGA60FPS();		//640 x 480 @ 60fps		X 2, For USB2.0
extern void OV5653Sensor_QVGA120FPS();		//320 x 240 @ 120fps	X 2



#endif /*_OV5653_SENSOR_H_*/
