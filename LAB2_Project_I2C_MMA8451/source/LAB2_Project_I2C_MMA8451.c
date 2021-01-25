/*! @file : LAB2_Project_I2C_MMA8451.c
 * @author  Harrison David Cabana Ponzon
 * @version 1.0.0
 * @date    24/01/2021
 * @brief   Archivo principal
 * @details
 *
*/
/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"

#include "sdk_hal_uart0.h"
#include "sdk_hal_gpio.h"
#include "sdk_hal_i2c0.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Private Prototypes
 ******************************************************************************/

/*******************************************************************************
 * External vars
 ******************************************************************************/

/*******************************************************************************
 * Local vars
 ******************************************************************************/

/*******************************************************************************
 * Private Source Code
 ******************************************************************************/
/*
 * @brief   Application entry point.
 */
int main(void) {
	status_t status;
	uint8_t nuevo_byte_uart;
	uint8_t	nuevo_dato_i2c;

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif

    (void)uart0Inicializar(115200);	//115200bps
    (void)i2c0MasterInit(100000);	//100kbps

    while(1) {
    	if(uart0CuantosDatosHayEnBuffer()>0){
    		status=uart0LeerByteDesdeBuffer(&nuevo_byte_uart);
    		if(status==kStatus_Success){
    			printf("key:%c\r\n",nuevo_byte_uart);
    			switch (nuevo_byte_uart) {
				case 'a':
				case 'A':
					gpioPutToggle(KPTB10);
					break;

				case 'v':
					gpioPutHigh(KPTB7);
					break;
				case 'V':
					gpioPutLow(KPTB7);
					break;

				case 'r':
					gpioPutValue(KPTB6,1);
					break;
				case 'R':
					gpioPutValue(KPTB6,0);
					break;

				case 'M':
					status=i2c0MasterReadByte(&nuevo_dato_i2c,1, 0x1D, 0x0D);

					if(nuevo_dato_i2c==0x1A)
						printf("MMA8460 detected!\r\n");
					else
						printf("MMA8460 error\r\n");
					break;
				}
    		}else{
    			printf("error\r\n");
    		}
    	}
    }
    return 0 ;
}
