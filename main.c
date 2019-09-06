
#include<stdint.h>
#include "tm4c123gh6pm.h"
#include"BrazoLib.h"

/*PD0   M1PWM0  M1G0    <=PINZA
 *PD1 M1PWM1  M1G0      <=ELEVACION
 *PC4 M0PWM6  M0G3      <=ROTACION
 *
 *PARA EL PROGRAMA AUXILIAR DE CONTROL:
 *
 *EN EL MODULO VERDE:
 *SW1:ROTACION++
 *SW2:ELEVACION--
 *SW3:ROTACION--
 *SW4:ELVACION++
 *
 *EN EL TIVA
 *SW1:ROTACION--
 *SW2:ROTACION++
 */

void main(void)
{
    //recpecion:
    uint8_t Decod[27]={0x00,0x01,0x02,0x04,0x05,0x06,0x08,0x09,0x0A,0x10,0x11,0x12,0x14,
                             0x15,0x16,0x18,0x19,0x1A,0x20,0x21,0x22,0x24,0x25,0x26,0x28,0x29,
                             0x2A};
    unsigned char datoRx;
    ConfigUART();
    PWM_PD1_PD0_PC4();
    config_led();
    unsigned char num;
    uint32_t DC_temp1,DC_temp2,DC_temp3;
    DC_temp1=DC_PINZA_INICIAL; //
    DC_temp2=DC_ELEVACION_INICIAL; //
    DC_temp3=DC_ROTACION_INICIAL; //
    while(1)
    {
        datoRx = Read();                    //Se lee dato
        num = Decod[datoRx-'@'];
        accionarMotor(num,&DC_temp1,&DC_temp2,&DC_temp3);
    }
}
