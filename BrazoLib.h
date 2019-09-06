/*
 * BrazoLib.h
 *
 *  Created on: 13 jun. 2018
 *      Author: GUSTAVO
 */

#ifndef BRAZOLIB_H_
#define BRAZOLIB_H_



#define DC_LOAD 0x00004E1F
#define DC_75   0x00001387
#define DC_25   0x00003A97
#define DC_7_5  0x00004843
#define DC_5    0x00004A37
#define DC_10   0x0000464F


#define DC_PINZA_INICIAL     0x00004843   //7.5%
#define DC_ELEVACION_INICIAL 0x00004650   //10%
#define DC_ROTACION_INICIAL  0x000047E0   //8%

#define DC_PINZA_MAX         0x000046B4   //9.5%
#define DC_ELEVACION_MAX     0x00004588   //11%
#define DC_ROTACION_MAX      0x00004650   //10%

#define DC_PINZA_MIN         0x00004A38   //5%
#define DC_ELEVACION_MIN     0x0000493E   //6.25%
#define DC_ROTACION_MIN      0x000048DA   //6.75%

#define RED     0x00000002
#define GREEN   0x00000008
#define BLUE    0x00000004
#define WHITE   0x00000000E
#define YELLOW  0x0000000A
#define CYAN    0x0000000C
#define MAGENTA 0x00000006

//para el programa de control:
//-----------------------------------------------------------
#define SW1 (1<<4)
#define SW2 (1<<3)
#define SW3 (1<<2)
#define SW4 (1<<5)
    //

void Send (unsigned char);
void ConfigTx(void);
char LecturaPulsadores1(void);
void LedEnc(uint32_t);
    //funciones exclusivas para mas de un motor
uint8_t LecturaPulsadores2(void);
void ControlBrazo2(void);

//-----------------------------------------------------------

// funciones para el tiva recpetor de señales:
void config_led(void);
void PWM_PD1_PD0_PC4(void);
void turn_on_led(uint32_t);
void PWM_PD1_PD0_PC4(void);
void ConfigUART(void);
unsigned char Read(void);
void new_DC_PD0(uint32_t);
void new_DC_PD1(uint32_t);
void new_DC_PC4(uint32_t);
void MoverBrazo(char,uint32_t*,uint32_t*,uint32_t*);

    //funciones exclusivas para mas de un motor
void accionarMotor(uint8_t, uint32_t *, uint32_t *, uint32_t *);

//funciones para el envio de señales:


#endif /* BRAZOLIB_H_ */
