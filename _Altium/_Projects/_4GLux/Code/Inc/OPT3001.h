/*
 * OPT3001.h
 *
 *  Created on: Oct 28, 2019
 *      Author: tomas.forsen
 */

#ifndef INC_OPT3001_H_
#define INC_OPT3001_H_

#define OPT_3001_OK 100
#define OPT_3001_DATA_READY 128
#define OPT_3001_Error 200
#define OPT_3001_Wait 220
#define OPT_3001_REQUEST_SENT 254
#define OPT_3001_NO_NEW_DATA 255

void OPT_3001_shutdown(void);
uint8_t OPT_3001_init_for_single_shot(void);
uint8_t OPT_3001_request_reading(void);
uint8_t OPT_3001_reading_ready(void);
uint8_t OPT_3001_get_reading(float* lux_ptr);
uint8_t all_sensors_have_data_ready(void);
#endif /* INC_OPT3001_H_ */
