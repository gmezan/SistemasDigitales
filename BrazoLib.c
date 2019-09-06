/*
 * BrazoLib.c
 *
 *  Created on: 13 jun. 2018
 *      Author: GUSTAVO
 */

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include"BrazoLib.h"


void config_led(void){ // configuracion de  LEDs
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // se activa la señal de reloj para F
    while ( (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5)==0); // esperando activacion
    GPIO_PORTF_AMSEL_R &= ~(0xE); //se desactiva el modo analogico para los 5 bits menos significativos
    //GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R & 0xFFF00000) | 0x0;
    //Se borra campos PCTL para seleccionar funcion GPIO
    GPIO_PORTF_DIR_R |= 0x0E; // se asigna PF1,PF2,PF3 como salida (LEDs)
    GPIO_PORTF_AFSEL_R  &=~(0xE); // se desactivan las funciones alternativas
    GPIO_PORTF_DR8R_R |= 0x0E; // driver de 8mA para los LEDs
    GPIO_PORTF_DEN_R |= 0xE; // pines digitales
    GPIO_PORTF_DATA_R &= ~(0x0E); // LEDs off
}
void turn_on_led(uint32_t var){ // funcion para prender el led RGB de un color
    GPIO_PORTF_DATA_R &= ~(0x0E);// turn off led
    GPIO_PORTF_DATA_R |= var;} // se prende led, el color depende de la entrada


void PWM_PD1_PD0_PC4(void)
{
    //PINES PD0 PD1 Y PC4, CONFIGURACION COMO PWM
    volatile unsigned long temp;
    SYSCTL_RCGC2_R |= (SYSCTL_RCGC2_GPIOD | SYSCTL_RCGC2_GPIOC);
    temp = SYSCTL_RCGC2_R;
    temp = SYSCTL_RCGC2_R;
    temp = SYSCTL_RCGC2_R;//RETARDO
    GPIO_PORTD_DIR_R |= 0x03;
    GPIO_PORTC_DR8R_R |= 0X03;
    GPIO_PORTD_AMSEL_R &= ~(0x03);
    // SE HABILITAN FUNCIONES ALTERNAS
    GPIO_PORTD_AFSEL_R |= 0x03;
    // PD0 Y PD1 COMO M1PWM0 Y M1PWM1
    GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & 0xFFFFFF00) | 0x55;
    GPIO_PORTD_DEN_R |= 0x03;
    //PC4
    GPIO_PORTC_DIR_R |= (1<<4);
    GPIO_PORTC_DR8R_R |=(1<<4);
    GPIO_PORTC_DEN_R |= (1<<4);
    //SE HABILITA FUNCIONES ALTERNAS
    GPIO_PORTC_AFSEL_R |= (1<<4);
    //M0PWM6
    GPIO_PORTC_PCTL_R=((GPIO_PORTC_PCTL_R & 0xFFF0FFFF)|0x00040000);


    // SE ACTIVA LA SEÑAL DE RELOJ PARA M0PWM Y M1PWM
    SYSCTL_RCGCPWM_R |= (SYSCTL_RCGCPWM_R1 |SYSCTL_RCGCPWM_R0);
    while(!(SYSCTL_PRPWM_R & SYSCTL_PRPWM_R1));
    while(!(SYSCTL_PRPWM_R & SYSCTL_PRPWM_R0));
    // SE DIVIDE LA SEÑAL DEL SISTEMA ENTRE 16
    SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV;
    SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M;
    SYSCTL_RCC_R |= SYSCTL_RCC_PWMDIV_16;
    // INHABILITAR PWM
    PWM1_0_CTL_R &= ~(0x03FF);
    PWM1_0_GENA_R = (PWM1_0_GENA_R & 0xFFFFF000) | 0x08C;
    PWM1_0_GENB_R = (PWM1_0_GENB_R & 0xFFFFF000) | 0x80C;
    //VALOR PARA 50Hz
    PWM1_0_LOAD_R = (PWM1_0_LOAD_R & 0xFFFF0000) | 0x4E1F;
    // DUTY CYCLE PD0 PD1
    PWM1_0_CMPA_R = (PWM1_0_CMPA_R & 0xFFFF0000) | DC_PINZA_INICIAL;
    PWM1_0_CMPB_R = (PWM1_0_CMPB_R & 0xFFFF0000) | DC_ELEVACION_INICIAL;
    // SE HABILITA GEN
    PWM1_0_CTL_R |= PWM_0_CTL_ENABLE;
    //HABILITADO PWM1 Y PWM0
    PWM1_ENABLE_R |= 0x3;

    //PC4:
    //DESHABILITADO
    PWM0_3_CTL_R&= ~(0x03FF);
    PWM0_3_GENA_R = (PWM0_3_GENA_R & 0xFFFFF000) | 0x08C;
    PWM0_ENABLE_R &= ~0x40;
    //SE DESHABILITA
    PWM0_3_CTL_R &= ~1;
    // 50 Hz
    PWM0_3_LOAD_R = (PWM0_3_LOAD_R & 0xFFFF0000) | 0x4E1F;
    PWM0_3_CMPA_R = (PWM0_3_CMPA_R & 0xFFFF0000) | DC_ROTACION_INICIAL;
    PWM0_3_CTL_R |=1;
    //PWM6
    PWM0_ENABLE_R |= (1<<6);
}


void ConfigUART(void)
{
    SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART1;   //Se habilita el reloj de UART1
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;   //Se habilita reloj de puerto B

    UART1_CTL_R &= ~UART_CTL_UARTEN;                    //Desactivamos UART1
    UART1_IBRD_R = (UART1_IBRD_R & 0xFFFF0000) | 8;     //Parte entera del divisor bps
    UART1_FBRD_R = (UART1_FBRD_R & 0xFFFFFFC0) | 44;    //Parte decimal divisor bps
    UART1_LCRH_R = (UART1_LCRH_R & 0xFFFFFF00) | 0x70;  //Configuracion 8bits, NoParidad, 1bit de parada, Fifo habilitado (chars)
    UART1_CTL_R |= UART_CTL_UARTEN;                     //Activamos UART0

    GPIO_PORTB_AMSEL_R &= ~(0x03);                                  //Desactivamos modo analogico para pin B0 y B1
    GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFFFF00)|0x00000011;  //Conectamos UART1 a B0 y B1
    GPIO_PORTB_AFSEL_R |= 0x03;                                     //Activamos función alternativa
    GPIO_PORTB_DEN_R |= 0x03;                                       //Configuramos como pin digital

}

unsigned char Read ()
{
    while(UART1_FR_R & UART_FR_RXFE);   //Se espera a que no esté lleno el Tx
    return UART1_DR_R;                  //Se devuelve lo leido
}

void new_DC_PD0(uint32_t DC)
{   //FUNCION QUE MODIFICA EL DUTY CYCLE DE TODAS LAS ONDAS DE ACUERDO A UN PARAMETRO DE ENTRADA:
    PWM1_0_CMPA_R = (PWM1_0_CMPA_R & 0xFFFF0000) | DC;
}

void new_DC_PD1(uint32_t DC)
{   //FUNCION QUE MODIFICA EL DUTY CYCLE DE TODAS LAS ONDAS DE ACUERDO A UN PARAMETRO DE ENTRADA:
    PWM1_0_CMPB_R = (PWM1_0_CMPB_R & 0xFFFF0000) | DC;
}

void new_DC_PC4(uint32_t DC)
{   //FUNCION QUE MODIFICA EL DUTY CYCLE DE TODAS LAS ONDAS DE ACUERDO A UN PARAMETRO DE ENTRADA:
    PWM0_3_CMPA_R = (PWM0_3_CMPA_R & 0xFFFF0000) | DC;
}

void MoverBrazo(char datoRx,uint32_t* DC1,uint32_t* DC2,uint32_t* DC3)
{
    int n=0;
    if(datoRx == 'R')
    {
        (*DC1)+=10;
        if((*DC1)>DC_PINZA_MIN)
        {
            *DC1=DC_PINZA_MIN;
        }
        new_DC_PD0(*DC1);
        turn_on_led(RED);
        for(n=0;n<20000;n++);//
    }
    else if(datoRx == 'G')
    {
        (*DC1)-=10;
        if((*DC1)<DC_PINZA_MAX)
        {
            (*DC1)=DC_PINZA_MAX;
        }
        new_DC_PD0(*DC1);
        turn_on_led(GREEN);
        for(n=0;n<20000;n++);//
    }
    else if(datoRx == 'B')
    {
        (*DC2)+=10;
        if((*DC2)>DC_ELEVACION_MIN)
        {
            (*DC2)=DC_ELEVACION_MIN;
        }
        new_DC_PD1(*DC2);
        turn_on_led(BLUE);
        for(n=0;n<20000;n++);//
    }
    else if(datoRx == 'Y')
    {
        (*DC2)-=10;
        if((*DC2)<DC_ELEVACION_MAX)
        {
            *DC2=DC_ELEVACION_MAX;
        }
        new_DC_PD1(*DC2);
        turn_on_led(YELLOW);
        for(n=0;n<20000;n++);//
    }
    else if(datoRx == 'C')
    {
        (*DC3)+=10;
        if((*DC3)>DC_ROTACION_MIN)
        {
            (*DC3)=DC_ROTACION_MIN;
        }
        new_DC_PC4(*DC3);
        turn_on_led(CYAN);
        for(n=0;n<20000;n++);//
    }
    else if(datoRx == 'M')
    {
        (*DC3)-=10;
        if((*DC3)<DC_ROTACION_MAX)
        {
            (*DC3)=DC_ROTACION_MAX;
        }
        new_DC_PC4(*DC3);
        turn_on_led(MAGENTA);
        for(n=0;n<20000;n++);//
    }
    else if(datoRx == 'W')
    {
        turn_on_led(WHITE);
    }
}


// funciones para programa de control

void LedEnc(uint32_t color)
{
    GPIO_PORTF_DATA_R &= ~(RED|BLUE|GREEN);
    GPIO_PORTF_DATA_R |= color;
}

char LecturaPulsadores1(void)
{
    char temp=0 ;
    uint32_t n;
    for(n=0;n<10000;n++);
    if(GPIO_PORTA_DATA_R & SW1)
    {
        temp='M';
        LedEnc(RED|BLUE);
    }
    else if(GPIO_PORTA_DATA_R & SW3)
    {
        temp ='C';
        LedEnc(BLUE|GREEN);
    }
    else if(GPIO_PORTA_DATA_R & SW2)
    {
        temp ='B';
        LedEnc(BLUE);
    }
    else if(GPIO_PORTA_DATA_R & SW4)
    {
        temp ='Y';
        LedEnc(RED|GREEN);
    }
    else if((GPIO_PORTF_DATA_R & (1<<0)) == 0)
    {
        temp ='G';
        LedEnc(GREEN);
    }
    else if((GPIO_PORTF_DATA_R & (1<<4)) == 0)
    {
        temp ='R';
        LedEnc(RED);
    }
    else
    {
        temp ='W';
        LedEnc(BLUE|RED|GREEN);
    }

    return temp;
}

void ConfigTx(void)
{
    SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART1;   //Se habilita el reloj de UART1
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;   //Se habilita reloj de puerto C
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;   //Se habilita reloj de puerto F
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;   //Se habilita reloj de puerto F

    UART1_CTL_R &= ~UART_CTL_UARTEN;                    //Desactivamos UART1
    UART1_IBRD_R = (UART1_IBRD_R & 0xFFFF0000) | 8;     //Parte entera del divisor bps //8
    UART1_FBRD_R = (UART1_FBRD_R & 0xFFFFFFC0) | 44;    //Parte decimal divisor bps      //44
    UART1_LCRH_R = (UART1_LCRH_R & 0xFFFFFF00) | 0x70;  //Configuracion 8bits, NoParidad, 1bit de parada, Fifo habilitado
    UART1_CTL_R |= UART_CTL_UARTEN;                     //Activamos UART1

    GPIO_PORTC_AMSEL_R &= ~(0x30);                                  //Desactivamos modo analogico para pin C4 y C5
    GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R&0xFF00FFFF)|0x00220000;  //Conectamos UART1 a C4 y C5
    GPIO_PORTC_AFSEL_R |= 0x30;                                     //Activamos función alternativa
    GPIO_PORTC_DEN_R |= 0x30;                                       //Configuramos como pin digital

    GPIO_PORTF_LOCK_R = 0x4c4f434b;     //Código para quitar la protección en el puerto
    GPIO_PORTF_CR_R |= 0X01;            //Habilitar modificaciones en registros GPIO, pin PF0.
    GPIO_PORTF_DIR_R &= ~(0x11);        //Se configura como entrada
    GPIO_PORTF_AFSEL_R &= ~(0x11);      //Se desactiva la función alternativa
    GPIO_PORTF_PUR_R |= 0x11;           //Se configura resistencia pull up a los pines
    GPIO_PORTF_DEN_R |= 0x1F;           //Se configura como pin digital


    GPIO_PORTF_DIR_R |= BLUE|RED|GREEN;
    GPIO_PORTF_DR8R_R |= BLUE|RED|GREEN;
    GPIO_PORTF_DEN_R |= BLUE|RED|GREEN;
    GPIO_PORTF_DATA_R &= BLUE|RED|GREEN;

    GPIO_PORTA_DIR_R &= ~(0x3C);        //Se configura como entrada
    GPIO_PORTA_AFSEL_R &= ~(0x3C);      //Se desactiva la función alternativa
    GPIO_PORTA_PUR_R |= 0x3C;           //Se configura resistencia pull up a los pines
    GPIO_PORTA_DEN_R |= 0x3C;           //Se configura como pin digital
}

void Send (unsigned char datoTx)
{
    while(UART1_FR_R & UART_FR_TXFF);   //Se espera a que no esté lleno el Tx
    UART1_DR_R = datoTx;
}

// PROGRAMA DE CONTROL1:
void ControlBrazo1(void)
{
    unsigned char datoTx;
    ConfigTx();
    while(1)
    {
         datoTx=LecturaPulsadores1();
         Send(datoTx);
    }
}

//-------------------------------------------------Funciones para mas de un motor:


uint8_t LecturaPulsadores2(void)
{
    uint32_t temp=0,n;
    for(n=0;n<10000;n++);
    if(!(GPIO_PORTA_DATA_R & SW1))
    {
        temp|=(1<<0);
    }
    else if(!(GPIO_PORTA_DATA_R & SW2))
    {
        temp|=(1<<1);
    }
    if(!(GPIO_PORTB_DATA_R & SW3))
    {
        temp|=(1<<2);
    }
    else if(!(GPIO_PORTA_DATA_R & SW4))
    {
        temp|=(1<<3);
    }
    if(!(GPIO_PORTF_DATA_R & (1<<0)))
    {
        temp|=(1<<4);
    }
    else if(!(GPIO_PORTF_DATA_R & (1<<4)))
    {
        temp|=(1<<5);
    }

    return temp;
}

//--

void accionarMotor(uint8_t num, uint32_t *DC_temp1, uint32_t *DC_temp2, uint32_t *DC_temp3)
{
    uint8_t temp1, temp2, temp3;
    temp1= num  & 0x03;
    temp2= (num  & 0x0C)>>2;
    temp3= (num  & 0x30)>>4;

    if(temp1 == 1)
    {
        (*DC_temp1)+=2;
        if((*DC_temp1)>DC_PINZA_MIN)
        {
            *DC_temp1=DC_PINZA_MIN;
        }
        new_DC_PD0(*DC_temp1);
        turn_on_led(RED);
    }
    else if(temp1 == 2)
    {
        (*DC_temp1)-=2;
        if((*DC_temp1)<DC_PINZA_MAX)
        {
            *DC_temp1=DC_PINZA_MAX;
        }
        new_DC_PD0(*DC_temp1);
        turn_on_led(GREEN);
    }
    if(temp2 == 1)
    {
        (*DC_temp2)+=2;
        if((*DC_temp2)>DC_ELEVACION_MIN)
        {
            *DC_temp2=DC_ELEVACION_MIN;
        }
        new_DC_PD1(*DC_temp2);
        turn_on_led(BLUE);
    }
    else if(temp2 == 2)
    {
        (*DC_temp2)-=2;
        if((*DC_temp2)<DC_ELEVACION_MAX)
        {
            *DC_temp2=DC_ELEVACION_MAX;
        }
        new_DC_PD1(*DC_temp2);
        turn_on_led(YELLOW);
    }
    if(temp3 == 1)
    {
        (*DC_temp3)+=2;
        if((*DC_temp3)>DC_ROTACION_MIN)
        {
            *DC_temp3=DC_ROTACION_MIN;
        }
        new_DC_PC4(*DC_temp3);
        turn_on_led(CYAN);
    }
    else if(temp3 == 2)
    {
        (*DC_temp3)-=2;
        if((*DC_temp3)<DC_ROTACION_MAX)
        {
            *DC_temp3=DC_ROTACION_MAX;
        }
        new_DC_PC4(*DC_temp3);
        turn_on_led(MAGENTA);
    }
    else //if(datoRx == 'W')
    {
        turn_on_led(WHITE);
    }
}


void ControlBrazo2(void)
{
    //PARA LA TRANSMISION--
    char Cod[43]={'@','A','B',0,'C','D','E',0,'F','G','H',0,0,0,0,0,'I','J','K',0,'L','M','N',0,'O',
                           'P','Q',0,0,0,0,0,'R','S','T',0,'U','V','W',0,'X','Y','Z'};
    ConfigTx();
    uint8_t num;
    while(1)
    {
        num=Cod[LecturaPulsadores2()];
        Send(num);
    }
}




