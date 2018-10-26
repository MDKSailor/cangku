/****************************************************************************
Filename    : project_line_draw.c
Author      : lzy
Date        : 24/10/2018
Licence     : none
Description : The project made for pratice.
****************************************************************************/

/* Includes *******************************************************************/
#define __HDR_DEFS__
#include "stm32f10x.h"
#include "math.h"
#undef __HDR_DEFS__

/* Private include */
#include "lcd.h"
#include "usart1.h"
#include "TIM2.h"
#include "UltrasonicWave.h"
/* End Includes ***************************************************************/

/* Global *********************************************************************/
volatile rmp_u8_t RMP_Test_Angle=1;
volatile rmp_u16_t RMP_Test_Display_Voltage = 0;     
volatile rmp_u32_t RMP_Test_TimingDelay = 0;
volatile rmp_u32_t RMP_Test_Text[8];
volatile rmp_u32_t RMP_Test_String[10];
volatile rmp_f32_t RMP_Test_Distance[10] = 150.0;
volatile rmp_f32_t RMP_Test_Vehicle_dx;
volatile rmp_f32_t RMP_Test_Vehicle_dy;
volatile rmp_f32_t RMP_Test_Distance[10] = 150.0;
volatile rmp_f32_t RMP_Test_Pixel_dx;
volatile rmp_f32_t RMP_Test_Pixel_dy;
/* End Globals ***************************************************************/

/* Begin Function: System_Delay ***********************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Delay_Ms(rmp_u32_t nTime)
{
    RMP_Test_TimingDelay = nTime;
    while(RMP_Test_TimingDelay != 0);    
}   

/* Begin Function: LCD_Init ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void LCD_Init(void)
{
    /* Set the system clock crossover */
    SysTick_Config(SystemCoreClock/1000); 
    Delay_Ms(200);
    STM3210B_LCD_Init();
    LCD_Clear(Blue);
    LCD_SetBackColor(Blue);
    LCD_SetTextColor(White);    
}

/* Begin Function: USART1_Config ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            
    NVIC_Init(&NVIC_InitStructure);    

    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;    

    USART_Init(USART1, &USART_InitStructure); 
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);          
}

/* Begin Function: ADC_Init ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void ADC_Init(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div4); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_28Cycles5);    
    ADC_Cmd(ADC1, ENABLE);
    
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));    
}

/* Begin Function: ADC_Read ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void ADC_Read(void)
{
    volatile rmp_f32_t RMP_Temp_Value;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    Delay_Ms(5);
    /* Convert analog to voltage */
    RMP_Temp_Value= ADC_GetConversionValue(ADC1)*3.30/0xfff;
    RMP_Test_Display voltage=RMP_Temp_Value;
}

/* Begin Function: KEY_Init ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void KEY_Init(void)
{
    
    
    
    
    
}

/* Begin Function: KEY_Scan ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void KEY_Scan(void)
{
    
    
    
    
    
    
}

/* Begin Function: LCD_SetPoint ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void LCD_SetPoint(rmp_u16_t Point_x,rmp_u16_t Point_y,rmp_u16_t Point_Color)
{
    /* Prepare to write GRAM */    
    LCD_WriteRAM_Prepare();     
    LCD_WriteRAM(Point_Color);   
    LCD_SetCursor(Point_x,Point_y); 
}

/* Begin Function: Draw_Line ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Draw_Line(rmp_u16_t Line_x1,rmp_u16_t Line_y1,rmp_u16_t Line_x2,rmp_u16_t Line_y2,rmp_u16_t Line_color)
{
    rmp_u16_t Line_dx;
    rmp_u16_t Line_dy;
    rmp_u16_t Line_e;
    
    Line_dx=Line_x2-Line_x1;
    Line_dy=Line_y2-Line_y1;
    if(Line_dx>=0)
    {
        if(Line_dy>=0)
        {
            if(Line_dx>=Line_dy)
            {
                Line_e=Line_dy-Line_dx/2;
                while(Line_x1<=Line_x2)
                { 
                LCD_SetPoint(Line_x1,Line_y1,Line_color);
                if(Line_e>0) 
                {
                    Line_y1+=1;
                    Line_e-=Line_dx;
                }  
                Line_x1+=1;
                Line_e+=Line_dy;
                }
            }
            else 
            {
                Line_e=Line_dx-Line_dy/2;
                while(Line_y1<=Line_y2)
                {
                 LCD_SetPoint(Line_x1,Line_y1,Line_color);    
                    if(Line_e>0)
                    {
                        Line_x1+=1;
                        Line_e-=Line_dy;
                    }
                }
            }
        }
        else 
        { 
             Line_dy=-Line_dy;    
            if(Line_dx>=Line_dy)
            {  
                Line_e=Line_dy-Line_dx/2;  
                while(Line_x1<=Line_x2)  
                {  
                    LCD_SetPoint(Line_x1,Line_y1,Line_color);  
                    if(Line_e>0)
                    {
                        Line_y1-=1;
                        Line_e-=Line_dx;
                    }     
                        Line_x1+=1;  
                        Line_e+=Line_dy;  
                }  
            }  
            else       
                Line_e=Line_dx-Line_dy/2;  
                while(Line_y1>=Line_y2)  
                {  
                    LCD_SetPoint(Line_x1,Line_y1,Line_color);  
                    if(Line_e>0)
                    {
                        Line_x1+=1;
                        Line_e-=Line_dy;
                    }     
                        Line_y1-=1;  
                        Line_e+=Line_dx;  
                }  
            }  
        }        
    else 
    {  
        Line_dx=-Line_dx;    
        if(Line_dy >= 0) 
        {  
            if(Line_dx>=Line_dy)
            {  
                Line_e=Line_dy-Line_dx/2;  
                while(Line_x1>=Line_x2)  
                {  
                    LCD_SetPoint(Line_x1,Line_y1,Line_color);  
                    if(Line_e>0)
                    {
                        Line_y1+=1;
                        Line_e-=Line_dx;}     
                        Line_x1-=1;  
                        Line_e+=Line_dy;  
                }  
            }  
            else        
            {  
                Line_e=Line_dx-Line_dy/2;  
                while(Line_y1<=Line_y2)  
                {  
                    LCD_SetPoint(Line_x1,Line_y1,Line_color);  
                    if(Line_e>0)
                    {
                        Line_x1-=1;
                        Line_e-=Line_dy;
                    }     
                    Line_y1+=1;  
                    Line_e+=Line_dx;  
                }  
            }  
        } 
        /* If Line_dy<0 Then */        
        else          
        {  
            Line_dy=-Line_dy;    
            if(Line_dx>=Line_dy) 
            {  
                Line_e=Line_dy-Line_dx/2;  
                while(Line_x1>=Line_x2)  
                {  
                    LCD_SetPoint(Line_x1,Line_y1,Line_color);  
                    if(Line_e>0)
                    {
                        Line_y1-=1;
                        Line_e-=Line_dx;
                    }     
                    Line_x1-=1;  
                    Line_e+=Line_dy;  
                }  
            }  
            else        
            {  
                Line_e=Line_dx-Line_dy/2;  
                while(Line_y1>=Line_y2)  
                {  
                    LCD_SetPoint(Line_x1,Line_y1,Line_color);  
                    if(Line_e>0)
                    {
                        Line_x1-=1;
                        Line_e-=Line_dy;
                    }     
                    Line_y1-=1;  
                    Line_e+=Line_dx;  
                }  
            }  
        }     
    }  
}

/* Begin Function: Angle_Corresponding_Voltage ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Angle_Corresponding_Voltage(void) 
{ 
    const rmp_f64_t RMP_Test_pi=acos(-1.0);
    rmp_f64_t RMP_Test_r;
    rmp_f64_t RMP_Test_s;
    rmp_f64_t RMP_Test_c;
        
    RMP_Test_Angle=(rmp_u16_t)(RMP_Test_Display_Voltage*90/3.3);
    RMP_Test_r=(rmp_f64_t)RMP_Test_Angle*pi/180;
    RMP_Test_s=sin(RMP_Test_r);
    RMP_Test_c=cos(RMP_Test_r);
    
    RMP_Test_Vehicle_dx=RMP_Test_c*distance;
    RMP_Test_Vehicle_dy=RMP_Test_s*distance;
    
    RMP_Test_Pixel_dx=(rmp_f64t)((RMP_Test_Vehicle_dx*70)/200); 
    if((rmp_u16_t)(RMP_Test_Pixel_dx*10)%10>5)
        RMP_Test_Pixel_dx=(rmp_u16_t)RMP_Test_Pixel_dx+1;
    else 
        RMP_Test_Pixel_dx=(rmp_u16_t)RMP_Test_Pixel_dx;
  
    LCD_SetPoint(70-RMP_Test_Pixel_dx,RMP_Test_Pixel_dy+230,Red);
    RMP_Test_Pixel_dy=(rmp_f32_t)((RMP_Test_Vehicle_dy*70)/200); 
    if((rmp_u16_t)(RMP_Test_Pixel_dy*10)%10>5)
        RMP_Test_Pixel_dy=(rmp_u16_t)RMP_Test_Pixel_dy+1;
   else 
       RMP_Test_Pixel_dy=(rmp_u16_t)RMP_Test_Pixel_dy;
       LCD_SetPoint(70-RMP_Test_Pixel_dx,RMP_Test_Pixel_dy+230,Red);
}

/* Begin Function: Graphic_Drawing ******************************************************
Description : The test function group 1.
Input       : None.
Output      : None.
Return      : None.
******************************************************************************/
void Graphic_Drawing(void)
{
    /* Draw rectangle */
    LCD_DrawRect(50,230,140,140); 
    /* Continuous line Drawing */
    Draw_Line(190,230,170,250,White);   
    Draw_Line(50,230,71,250,White);
    Draw_Line(170,250,70,250,White);        
    Draw_Line(190,90,170,70,White);
    Draw_Line(50,90,70,70,White);
    Draw_Line(170,70,68,70,White);
    Draw_Line(70,230,30,270,White);
} 




int main(void)
{
    LCD_Init();
    USART1_Config();
    TIM2_NVIC_Configuration();
    TIM2_Configuration();
    ADC_Init();
    KEY_Init();
    Graphic_Drawing();
    UltrasonicWave_Configuration();
    
    while(1)   
    {      
         ADC_Read();
         Angle_Corresponding_Voltage();
         KEY_Scan();

    }
}
/* End Function:RMP_Init *****************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/







                    
                    
            
            
    
    
    
    
    
    
    
    
    



    

    
    
    
    
    
    
    
    
    

