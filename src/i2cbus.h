#ifndef I2CBUS_H_
#define I2CBUS_H_

#include "McuRTOS.h"

/*!
 * \brief I2CBus module initialization
 */
void I2CBus_Init(void);

/*!
 * \brief I2CBus module de-initialization
 */
void I2CBus_Deinit(void);

void I2CBus_Lock(void);

void I2CBus_Unlock(void);

#endif /* I2CBUS_H_ */