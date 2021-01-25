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
#include <sdk_pph_mma8451Q.h>
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

#include "sdk_mdlw_leds.h"
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
	mma8451_data_t	mma8451_data;

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif

    (void)uart0Inicializar(115200);	//115200bps
    (void)i2c0MasterInit(100000);	//100kbps
    if (mma8451QWhoAmI() == kStatus_Success){
    	(void)mma8451QInit();	//inicializa acelerometro MMA8451Q
    	printf("MMA8451Q detected!\r\n");
    }else{
    	printf("MMA8451Q error\r\n");
    }


    while(1) {
    	if(uart0CuantosDatosHayEnBuffer()>0){
    		status=uart0LeerByteDesdeBuffer(&nuevo_byte_uart);
    		if(status==kStatus_Success){
    			printf("\r\nkey:%c\r\n",nuevo_byte_uart);
    			switch (nuevo_byte_uart) {
				case 'a':
				case 'A':
					toggleLedAzul();
					break;

				case 'v':
					apagarLedVerde();
					break;
				case 'V':
					encenderLedVerde();
					break;

				case 'r':
					apagarLedRojo();
					break;

				case 'R':
					encenderLedRojo();
					break;

				case 'X':
					if(mma8451QReadAccel(&mma8451_data)== kStatus_Success){
						printf("x:0x%04X\r\n",mma8451_data.x_value);
						printf("y:0x%04X\r\n",mma8451_data.y_value);
						printf("z:0x%04X\r\n",mma8451_data.z_value);
					}else{
						printf("MMA8451Q error\r\n");
					}
					break;

				case 'P':
					i2c0MasterReadByte(&nuevo_byte_uart, 1, 0x1D, 0x09);
					printf("REG_F_SETUP:%02X\r\n",nuevo_byte_uart);
					break;
				}
    		}else{
    			printf("error\r\n");
    		}
    	}
    }
    return 0 ;
}
