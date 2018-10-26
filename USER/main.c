#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"

/* start task */
#define START_TASK_PRIO        10
#define START_STK_SIZE         128
OS_STK START_TASK_STK[START_STK_SIZE];

void start_task(void *pdata);

/* LED0 task */
#define LED0_TASK_PRIO        7
#define LED0_STK_SIZE         64
OS_STK LED0_TASK_STK[LED0_STK_SIZE];

void led0_task(void *pdata);

/* LED1 task */
#define LED1_TASK_PRIO        6
#define LED1_STK_SIZE         128
OS_STK LED1_TASK_STK[LED1_STK_SIZE];

void led1_task(void *pdata);

/* float test task */
#define FLOAT_TASK_PRIO        5
#define FLOAT_STK_SIZE         128
__align(8) OS_STK FLOAT_TASK_STK[FLOAT_STK_SIZE];

void float_task(void *pdata);


 int main(void)
 {
    delay_init();  
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  	 
	  uart_init(115200);	     
    LED_Init();		  	
	  OSInit();
	  OSTaskCreate(start_task,(void *)0,(OS_STK *)&START_TASK_STK[START_STK_SIZE-1],START_TASK_PRIO );
    OSStart();
 }
 
 
 
/* start task */
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OSStatInit();
	/* Entering The Critical Section */
	OS_ENTER_CRITICAL(); 
 	OSTaskCreate(led0_task,(void *)0,(OS_STK*)&LED0_TASK_STK[LED0_STK_SIZE-1],LED0_TASK_PRIO);						   
 	OSTaskCreate(led1_task,(void *)0,(OS_STK*)&LED1_TASK_STK[LED1_STK_SIZE-1],LED1_TASK_PRIO);	
	OSTaskCreate(float_task,(void *)0,(OS_STK*)&FLOAT_TASK_STK[FLOAT_STK_SIZE-1],FLOAT_TASK_PRIO);
	OSTaskSuspend(START_TASK_PRIO);
	OS_EXIT_CRITICAL();
}

/* led0 task */
void led0_task(void *pdata)
{	 	
	while(1)
	{
		LED0=0;
		delay_ms(80);
		LED0=1;
		delay_ms(400);
	};
}

/* led1 task */
void led1_task(void *pdata)
{	  
	while(1)
	{
		LED1=0;
		delay_ms(300);
		LED1=1;
		delay_ms(300);
	};
}

/* float test task */
void float_task(void *pdata)
{
    OS_CPU_SR cpu_sr=0;
    static float float_num=0.01;	
	  while(1)
		{
		    float_num+=0.01f;
        OS_ENTER_CRITICAL();
        printf("float_num µÄÖµÎª£º%4f\r\n",float_num);
       	OS_EXIT_CRITICAL();		
			  delay_ms(500);

		}
}





