/*
Filename:       MEMS.c
Revised:        $Date: 2018-11-11  $
Revision:       $Revision: ck $
*/


#include "mems.h"


#ifdef USE_ICALL
#include <icall.h>
#endif

uint8_t transBuf; 

/*********************Mems functions ******************************************/
void usDelay(volatile uint16_t delay);
static bool Mems_ReadReg(uint8_t Reg, uint8_t* Data);
static bool Mems_WriteReg(uint8_t Reg, uint8_t Data);
/********************************************
* Mems functions *
********************************************/
bool MemsOpen(void)
{	
  //	IIC_Init();
  GPIO_InitTypeDef GPIO_Init_Structure;
//  RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
  
  GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init_Structure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_Init_Structure);
  usDelay(1);
  
    //read chip id
    
    Mems_ReadReg(0x01, &transBuf);
    if(transBuf != 0x13)
      return FALSE;

  //Mode: normal
  transBuf = 0x35;
  Mems_WriteReg(0x11, transBuf);
  
  //ODR=125HZ
  transBuf = 0x07;
  Mems_WriteReg(0x10, transBuf);	
  
  //Set active_ths default:g_Rang +/-2g 
  //threshold of active interrupt=Active_th*K(mg)
  //K = 3.91(2g range)
  //K = 7.81(4g range)
  //K = 15.625(8g range)
  //K = 31.25(16g range)
  transBuf = 50;
  Mems_WriteReg(0x28, transBuf);
  
  //Enable active interrupt
  transBuf = 0x07;
  Mems_WriteReg(0x16, transBuf);			
  
  //mapping active interrupt to INT1
  transBuf = 0x04;
  Mems_WriteReg(0x19, transBuf);	
  
  return TRUE;
}

void MemsLowPwMode(void)
{
  //Mode: suspend
  transBuf = 0x35 | (1<<3);
  Mems_WriteReg(0x11, transBuf);
}

/********************************************
* IIC Functions * 
********************************************/
void IIC_Init(void)
{
  GPIO_InitTypeDef GPIO_Init_Structure;
  RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
  
  GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init_Structure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_Init_Structure);
  GPIO_SetBits(GPIOA,GPIO_Pin_6|GPIO_Pin_7);
}

void usDelay(volatile uint16_t delay)
{
  volatile uint16_t i;
  while(delay--)
  {
    i = 500;
    for(;i>0;i--);
  }
}

static void SDA_In(void)
{ 
  //	GPIOA->MODER&=~(3<<11);
  //	GPIOA->MODER|=0<<11;
  GPIO_InitTypeDef GPIO_Init_Structure;
  
  GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_Init_Structure);	
}

static void SDA_Out(void)
{	
  //	GPIOA->MODER&=~(3<<11);
  //	GPIOA->MODER|=1<<11;
  GPIO_InitTypeDef GPIO_Init_Structure;
  
  GPIO_Init_Structure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init_Structure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init_Structure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init_Structure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA,&GPIO_Init_Structure);
}

static void SDA_OutPutBit(BitAction bit)
{
  if(bit == Bit_RESET)
    //        GPIOA->BRR = GPIO_Pin_7;
    GPIO_ResetBits(GPIOA,GPIO_Pin_7);
  else
    //        GPIOA->BSRR = GPIO_Pin_7; 
    GPIO_SetBits(GPIOA,GPIO_Pin_7);		
  
}

static void SCL_OutPutBit(BitAction bit)
{ 
  if(bit == Bit_RESET)
    GPIO_ResetBits(GPIOA,GPIO_Pin_6);
  //        GPIOA->BRR = GPIO_Pin_6;
  else
    GPIO_SetBits(GPIOA,GPIO_Pin_6);
  //        GPIOA->BSRR = GPIO_Pin_6;  
}

static uint8_t Read_SDA(void)
{
  //    return (uint8_t)(GPIOA->IDR & GPIO_Pin_7);//maybe not IDR!   
  return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7);
}

void IIC_Start(void)
{
  SDA_OutPutBit(Bit_SET);
  usDelay(1);
  SCL_OutPutBit(Bit_SET);
  usDelay(2);
  
  SDA_OutPutBit(Bit_RESET);
  usDelay(2);
  
  /* 钳住I2C总线，准备发送或接收数据 */
  SCL_OutPutBit(Bit_RESET);
  usDelay(2);
}

void IIC_Stop(void)
{  
  SDA_OutPutBit(Bit_RESET);   
  usDelay(1);
  
  SCL_OutPutBit(Bit_SET); 
  usDelay(2);
  
  SDA_OutPutBit(Bit_SET);	 
  usDelay(2);							   	
}

uint8_t IIC_Wait_Ack(void)
{
  uint8_t ucErrTime = 0;
  
  usDelay(1);
  SDA_OutPutBit(Bit_SET);
  usDelay(1);
  SDA_In();
  
  SCL_OutPutBit(Bit_SET);    
  
  while(Read_SDA())
  {
    ucErrTime++;
    usDelay(1);
    if(ucErrTime > (20))
    {
      IIC_Stop();
      
      return 1;
    }
  }
  
  SCL_OutPutBit(Bit_RESET);
  usDelay(1);
  return 0;  
}

void IIC_Ack(void)
{
  SCL_OutPutBit(Bit_RESET);
  
  SDA_Out();
  
  SDA_OutPutBit(Bit_RESET);	 
  usDelay(2);
  
  SCL_OutPutBit(Bit_SET);
  usDelay(2);
  
  SCL_OutPutBit(Bit_RESET);
}

void IIC_NAck(void)
{
  SCL_OutPutBit(Bit_RESET);
  
  SDA_Out();
  SDA_OutPutBit(Bit_SET);    
  usDelay(2);
  
  SCL_OutPutBit(Bit_SET);   
  usDelay(2);
  
  SCL_OutPutBit(Bit_RESET);
}

void IIC_Send_Byte(uint8_t txd)
{                        
  uint8_t t;
  uint8_t bit;
  
  SCL_OutPutBit(Bit_RESET);
  
  for(t = 0; t < 8; t++)
  {              
    bit =(txd & 0x80) >> 7;
    
    if(bit == Bit_RESET)
      SDA_OutPutBit(Bit_RESET);
    else
      SDA_OutPutBit(Bit_SET);
    
    txd <<= 1; 	  
    //usDelay(2);
    
    SCL_OutPutBit(Bit_SET);
    usDelay(2); 
    
    SCL_OutPutBit(Bit_RESET);
    usDelay(2);
  }
} 

uint8_t IIC_Read_Byte(uint8_t ack)
{
  uint8_t i,receive = 0;
  
  
  SDA_OutPutBit(Bit_SET);
  SDA_In();
  
  SCL_OutPutBit(Bit_RESET);
  
  for(i=0;i<8;i++ )
  {
    SCL_OutPutBit(Bit_SET);
    usDelay(1);
    
    receive <<= 1;
    
    if(Read_SDA())
      receive ++;  
    
    SCL_OutPutBit(Bit_RESET);   
    usDelay(1); 
  }	
  
  if ( !ack )
    IIC_NAck();
  else
    IIC_Ack(); 
  
  return receive;
}

static bool Mems_ReadReg(uint8_t Reg, uint8_t* Data) 
{    
  SDA_Out();
  IIC_Start();  
  /* 写指令 */    
  IIC_Send_Byte(MEMS_SADW);		 
  if( IIC_Wait_Ack() )  
    return FALSE;
  
  SDA_Out();
  IIC_Send_Byte(Reg);   		
  if( IIC_Wait_Ack() ) 
    return FALSE;
  
  SDA_Out();
  IIC_Start();  	 
  /* 读指令 */    
  IIC_Send_Byte(MEMS_SADR);	     	 
  if( IIC_Wait_Ack() )
    return FALSE;
  
  SDA_Out();
  *Data = IIC_Read_Byte(0);		    	   
  IIC_Stop();			        
  
  return TRUE;			 
}

static bool Mems_WriteReg(uint8_t Reg, uint8_t Data) 
{ 
  SDA_Out();
  IIC_Start();  
  /* 写指令 */       
  IIC_Send_Byte(MEMS_SADW);     	
  if( IIC_Wait_Ack() )
    return FALSE; 
  
  SDA_Out();
  IIC_Send_Byte(Reg);           
  if( IIC_Wait_Ack() )
    return FALSE;  
  
  SDA_Out();
  IIC_Send_Byte(Data);     						   
  if( IIC_Wait_Ack() )
    return FALSE;
  
  IIC_Stop();			            
  
  return TRUE;
}
