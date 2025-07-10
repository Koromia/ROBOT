/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>  // 用于 sprintf 函数
#include <string.h> // 用于 strlen 函数
#include <../../SCSLib/SCServo.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
//FT舵机串口指令发送函数
void ftUart_Send(uint8_t *nDat , int nLen)
{
	HAL_UART_Transmit(&huart2, nDat, nLen, 100);
}

//FT舵机串口指令应答接收函数
int ftUart_Read(uint8_t *nDat, int nLen)
{
	if(HAL_OK!=HAL_UART_Receive(&huart2, nDat, nLen, 100)){
		return 0;
	}else{
		return nLen;
	}
}

//FT舵机总线切换延时，时间大于10us
void ftBus_Delay(void)
{
	HAL_Delay(1);
}
void setup(void)
{
	setEnd(1);//SCSCL舵机为大端存储结构
}
void ServoInit(uint8_t ID) 
{
    // 解锁EPROM
    unLockEpromEx(ID);
    
    // 设置位置模式
    WheelMode(ID);
    
    // 校准零位
    CalibrationOfs(ID);
    
    // 锁定EPROM
    LockEpromEx(ID);
}
//所有总线舵机初始化，目前测试出来舵机中值是510，3的话是630,2的话310
uint8_t servo_flag=0; //为什么加这个flag主要是用来初始化舵机的
uint16_t leg1_all_duty=0; //ID，1,4,7,10的duty值
uint16_t leg1_left_duty=0; //ID，1,4的duty值
uint16_t leg1_right_duty=0; //ID，7,10的duty值
uint16_t leg2_all_duty=0; //ID，2,5,8,11的duty值
uint16_t leg2_left_duty=0; //ID，2,5的duty值
uint16_t leg2_right_duty=0; //ID，8,11的duty值
uint16_t leg3_all_duty=0; //ID，3,6,9,12的duty值
uint16_t leg3_left_duty=0; //ID，3,6的duty值
uint16_t leg3_right_duty=0; //ID，9,12的duty值
void servo_all_init(void)
{
	ServoInit(1);
	ServoInit(2);
	ServoInit(3);
	ServoInit(4);
	ServoInit(5);
	ServoInit(6);
	ServoInit(7);
	ServoInit(8);
	ServoInit(9);
	ServoInit(10);
	ServoInit(11);
	ServoInit(12);
	setup();
}
//主要是用来初始化角度的,初始化一个初始角度也可以是当成初始化的角度
void servo_angle_init(void)
{
	WritePos(1, 450, 0, 1500); //对ID是2的舵机进行一个初始化
	WritePos(2, 260, 0, 1500); //对ID是2的舵机进行一个初始化
	WritePos(3, 665, 0, 1500); //对ID是3的舵机进行一个初始化
	WritePos(4, 540, 0, 1500); //对ID是3的舵机进行一个初始化
	WritePos(5, 750, 0, 1500); //对ID是5的舵机进行一个初始化
	WritePos(6, 370, 0, 1500); //对ID是6的舵机进行一个初始化
	WritePos(7, 540, 0, 1500); //对ID是6的舵机进行一个初始化
	WritePos(8, 250, 0, 1500); //对ID是8的舵机进行一个初始化
	WritePos(9, 645, 0, 1500); //对ID是9的舵机进行一个初始化
	WritePos(10, 480, 0, 1500); //对ID是9的舵机进行一个初始化
	WritePos(11, 770, 0, 1500); //对ID是11的舵机进行一个初始化
	WritePos(12, 400, 0, 1500); //对ID是12的舵机进行一个初始化
	HAL_Delay(250);
}
void servo_angle_curve(void)
{
	WritePos(3, 765, 0, 1500); //对ID是3的舵机进行一个弯曲
	WritePos(6, 270, 0, 1500); //对ID是6的舵机进行一个弯曲
	WritePos(9, 745, 0, 1500); //对ID是9的舵机进行一个弯曲
	WritePos(12, 300, 0, 1500); //对ID是12的舵机进行一个弯曲
	HAL_Delay(200);
}
//对ID1,4,7,10同步控制
void leg1_all(uint16_t duty)
{
	WritePos(1, duty, 0, 1500); //对ID是2的舵机进行一个弯曲
	WritePos(4, duty, 0, 1500); //对ID是3的舵机进行一个弯曲
	WritePos(7, duty, 0, 1500); //对ID是5的舵机进行一个弯曲
	WritePos(10,duty, 0, 1500); //对ID是6的舵机进行一个弯曲
}
//对ID1,4同步控制
void leg1_left(uint16_t duty)
{
	WritePos(1, duty, 0, 1500); //对ID是2的舵机进行一个弯曲
	WritePos(4, duty, 0, 1500); //对ID是3的舵机进行一个弯曲
}
//对ID7,10同步控制
void leg1_right(uint16_t duty)
{
	WritePos(7, duty, 0, 1500); //对ID是2的舵机进行一个弯曲
	WritePos(10, duty, 0, 1500); //对ID是3的舵机进行一个弯曲
}
//对ID2,5,8,11同步控制
void leg2_all(uint16_t duty)
{
	WritePos(2, duty, 0, 1500); //对ID是2的舵机进行一个弯曲
	WritePos(5, duty, 0, 1500); //对ID是3的舵机进行一个弯曲
	WritePos(8, duty, 0, 1500); //对ID是5的舵机进行一个弯曲
	WritePos(11, duty, 0, 1500); //对ID是6的舵机进行一个弯曲
}
//对ID2,5同步控制
void leg2_left(uint16_t duty)
{
	WritePos(2, duty, 0, 1500); //对ID是2的舵机进行一个弯曲
	WritePos(5, duty, 0, 1500); //对ID是3的舵机进行一个弯曲
}
//对ID8,11同步控制
void leg2_right(uint16_t duty)
{
	WritePos(8, duty, 0, 1500); //对ID是2的舵机进行一个弯曲
	WritePos(11, duty, 0, 1500); //对ID是3的舵机进行一个弯曲
}
//对ID3,6,9,12同步控制
void leg3_all(uint16_t duty)
{
	WritePos(3, duty, 0, 1500); //对ID是2的舵机进行一个弯曲
	WritePos(6, duty, 0, 1500); //对ID是3的舵机进行一个弯曲
	WritePos(9, duty, 0, 1500); //对ID是5的舵机进行一个弯曲
	WritePos(12, duty, 0, 1500); //对ID是6的舵机进行一个弯曲
}
//对ID3,6同步控制
void leg3_left(uint16_t duty)
{
	WritePos(3, duty, 0, 1500); //对ID是2的舵机进行一个弯曲
	WritePos(6, duty, 0, 1500); //对ID是3的舵机进行一个弯曲
}
//对ID9,12同步控制
void leg3_right(uint16_t duty)
{
	WritePos(9, duty, 0, 1500); //对ID是2的舵机进行一个弯曲
	WritePos(12, duty, 0, 1500); //对ID是3的舵机进行一个弯曲
}
//电机方向引脚初始化
void motor_init(void)
{
	  //左前轮
		HAL_GPIO_WritePin(GPIOD,MotorA_l_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,A_l_Pin, GPIO_PIN_RESET);
	  //右前轮
		HAL_GPIO_WritePin(GPIOD,MotorA_r_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,A_r_Pin, GPIO_PIN_RESET);
//		//左后轮
//		HAL_GPIO_WritePin(GPIOD,MotorB_l_Pin, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOC,B_l_Pin, GPIO_PIN_RESET);
//	  //右后轮
//		HAL_GPIO_WritePin(GPIOD,MotorB_r_Pin, GPIO_PIN_SET);
//		HAL_GPIO_WritePin(GPIOC,B_r_Pin, GPIO_PIN_RESET);
}
//电机pwm设置
void motorA_ld(int16_t duty)
{
		//左前轮
		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_1,duty);
}
void motorA_rd(int16_t duty)
{
		//右前轮
		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_2,duty);
}
void motorB_ld(int16_t duty)
{
		//左后轮
		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_4,duty);
}
void motorB_rd(int16_t duty)
{
		//右后轮
		__HAL_TIM_SET_COMPARE(&htim8,TIM_CHANNEL_3,duty);
}
//电机的正反转设置
void motorA_lt(void)
{
		//左前轮
		HAL_GPIO_WritePin(GPIOD,MotorA_l_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,A_l_Pin, GPIO_PIN_SET);
}
void motorA_rt(void)
{
		//右前轮
		HAL_GPIO_WritePin(GPIOD,MotorA_r_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,A_r_Pin, GPIO_PIN_SET);
}
//
void motorB_lt(void)
{
		//右后轮
		HAL_GPIO_WritePin(GPIOD,MotorB_r_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,B_r_Pin, GPIO_PIN_SET);
}
//
void motorB_rt(void)
{
		//左后轮
		HAL_GPIO_WritePin(GPIOD,MotorB_l_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,B_l_Pin, GPIO_PIN_SET);
}
void motorA_lf(void)
{
		//左前轮
		HAL_GPIO_WritePin(GPIOD,MotorA_l_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,A_l_Pin, GPIO_PIN_SET);
}
void motorA_rf(void)
{
		//右前轮
		HAL_GPIO_WritePin(GPIOD,MotorA_r_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,A_r_Pin, GPIO_PIN_SET);
}
//
void motorB_lf(void)
{
		//右后轮
		HAL_GPIO_WritePin(GPIOD,MotorB_r_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,B_r_Pin, GPIO_PIN_SET);
}
//
void motorB_rf(void)
{
		//左后轮
		HAL_GPIO_WritePin(GPIOD,MotorB_l_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,B_l_Pin, GPIO_PIN_SET);
}
//编码器的读取
int8_t A_ls=0;
int8_t A_rs=0;
int8_t B_ls=0;
int8_t B_rs=0;
int8_t A_lmc=0;
int8_t A_rmc=0;
int8_t B_lmc=0;
int8_t B_rmc=0;
int8_t motorA_le(void)
{
	int8_t count=0;
	count=__HAL_TIM_GET_COUNTER(&htim2);
	__HAL_TIM_SET_COUNTER(&htim2, 0);
	return count;
}
int8_t motorA_re(void)
{
	int8_t count=0;
	count=__HAL_TIM_GET_COUNTER(&htim3);
	__HAL_TIM_SET_COUNTER(&htim3, 0);
	return count;
}
int8_t motorB_le(void)
{
	int8_t count=0;
	count=__HAL_TIM_GET_COUNTER(&htim4);
	__HAL_TIM_SET_COUNTER(&htim4, 0);
	return count;
}
int8_t motorB_re(void)
{
	int8_t count=0;
	count=__HAL_TIM_GET_COUNTER(&htim5);
	__HAL_TIM_SET_COUNTER(&htim5, 0);
	return count;
}
//四个电机的PID
int A_l_this_error=0;
int A_l_last_error=0;
int A_l_increase=0;
int A_l_out=0;
int A_l_duty=0;
void motorA_lp(int8_t target,int8_t now)
{
	float kp=0;
	float ki=0;
//	if(A_ls == 0)
//	{
//		kp=120;
//		ki=96;
//	}
//	else
//	{
		kp=48; //96
		ki=36; //72
//	}
	A_l_this_error=target-now;
	A_l_increase=kp*(A_l_this_error-A_l_last_error)+ki*A_l_this_error;
	A_l_last_error=A_l_this_error;
	A_l_out+=A_l_increase;
	A_l_duty=A_l_out/100;
	if(A_l_duty >= 99) A_l_duty=99;
	if(A_l_duty <= -99) A_l_duty=-99;
	if(A_l_duty >= 0)
	{
		motorA_lt();
		motorA_ld(A_l_duty);
	}
	else
	{
		motorA_lf();
		motorA_ld(-A_l_duty);
	}
}
int A_r_this_error=0;
int A_r_last_error=0;
int A_r_increase=0;
int A_r_out=0;
int A_r_duty=0;
void motorA_rp(int8_t target,int8_t now)
{
	float kp=0;
	float ki=0;
//	if(A_ls == 0)
//	{
//		kp=120;
//		ki=96;
//	}
//	else
//	{
		kp=48;
		ki=36;
//	}
	A_r_this_error=target-now;
	A_r_increase=kp*(A_r_this_error-A_r_last_error)+ki*A_r_this_error;
	A_r_last_error=A_r_this_error;
	A_r_out+=A_r_increase;
	A_r_duty=A_r_out/100;
	if(A_r_duty >= 99) A_r_duty=99;
	if(A_r_duty <= -99) A_r_duty=-99;
	if(A_r_duty >= 0)
	{
		motorA_rt();
		motorA_rd(A_r_duty);
	}
	else
	{
		motorA_rf();
		motorA_rd(-A_r_duty);
	}
}
int B_l_this_error=0;
int B_l_last_error=0;
int B_l_increase=0;
int B_l_out=0;
int B_l_duty=0;
void motorB_lp(int8_t target,int8_t now)
{
	float kp=0;
	float ki=0;
//	if(A_ls == 0)
//	{
//		kp=120;
//		ki=96;
//	}
//	else
//	{
		kp=30;
		ki=25;
//	}
	B_l_this_error=target-now;
	B_l_increase=kp*(B_l_this_error-B_l_last_error)+ki*B_l_this_error;
	B_l_last_error=B_l_this_error;
	B_l_out+=B_l_increase;
	B_l_duty=B_l_out/100;
	if(B_l_duty >= 99) B_l_duty=99;
	if(B_l_duty <= -99) B_l_duty=-99;
	if(B_l_duty >= 0)
	{
		motorB_lt();
		motorB_ld(B_l_duty);
	}
	else
	{
		motorB_lf();
		motorB_ld(-B_l_duty);
	}
}
int B_r_this_error=0;
int B_r_last_error=0;
int B_r_increase=0;
int B_r_out=0;
int B_r_duty=0;
void motorB_rp(int8_t target,int8_t now)
{
	float kp=0;
	float ki=0;
//	if(A_ls == 0)
//	{
//		kp=120;
//		ki=96;
//	}
//	else
//	{
		kp=30;
		ki=25;
//	}
	B_r_this_error=target-now;
	B_r_increase=kp*(B_r_this_error-B_r_last_error)+ki*B_r_this_error;
	B_r_last_error=B_r_this_error;
	B_r_out+=B_r_increase;
	B_r_duty=B_r_out/100;
	if(B_r_duty >= 99) B_r_duty=99;
	if(B_r_duty <= -99) B_r_duty=-99;
	if(B_r_duty >= 0)
	{
		motorB_rt();
		motorB_rd(B_r_duty);
	}
	else
	{
		motorB_rf();
		motorB_rd(-B_r_duty);
	}
}
//这边主要是负责蓝牙的，1对应的是49,0对应的是48
uint8_t buletooth_mode_flag=0;
uint8_t buletooth_duty_flag=0;
uint8_t buletooth_ID_flag=0;
uint8_t buletooth_tx=0; //作为发送信息的
uint8_t buletooth_rx=0; //作为接收信息的
uint8_t buletooth_flag=0; //作为发送信息的
uint8_t buletooth_count=0; //作为接收信息的
uint8_t buletooth_leg_count=0; //作为接收信息的
//前进是0，后退是1，左转是2，右转是3，停止是4
//模式切换的是5，模式开启按键是6，腿升高7，腿降低8
//总共需要9个按键
void happy_mode(void)
{
	WritePos(3, 565, 0, 500); //对ID是2的舵机进行一个弯曲
	WritePos(6, 470, 0, 500); //对ID是3的舵机进行一个弯曲
	WritePos(9, 545, 0, 500); //对ID是5的舵机进行一个弯曲
	WritePos(12, 500, 0, 500); //对ID是6的舵机进行一个弯曲
	HAL_Delay(300);
	WritePos(3, 765, 0, 500); //对ID是2的舵机进行一个弯曲
	WritePos(6, 270, 0, 500); //对ID是3的舵机进行一个弯曲
	WritePos(9, 745, 0, 500); //对ID是5的舵机进行一个弯曲
	WritePos(12, 300, 0, 500); //对ID是6的舵机进行一个弯曲
	HAL_Delay(500);
}
void bluetooth_ct(void)
{
	if(buletooth_rx == 106)
	{
		buletooth_rx=0;
	}
	//首先这边需要包含一个简单的对狗的控制，主要包括前进，左转，右转，停止以及后退，左右转目前的思路是一个原地打转，转到合适的角度然后进行转弯（这里需要五个按键）
	//前进
	if(buletooth_rx == 48)
	{
		A_ls=5;
		A_rs=5;
		B_ls=-5;
//		B_rs=2;
		buletooth_rx=0;
	}
	//后退
	if(buletooth_rx == 49)
	{
		A_ls=-5;
		A_rs=-5;
		B_ls=-5;
//		B_rs=-2;
		buletooth_rx=0;
	}
	//左转
	if(buletooth_rx == 50)
	{
		A_ls=-10;
		A_rs=15;
		B_ls=10;
//		B_rs=2;
		buletooth_rx=0;
	}
	//右转
	if(buletooth_rx == 51)
	{
		A_ls=15;
		A_rs=-10;
		B_ls=-15;
//		B_rs=-2;
		buletooth_rx=0;
	}
	//停止
	if(buletooth_rx == 52)
	{
		A_ls=0;
		A_rs=0;
		B_ls=0;
//		B_rs=0;
		buletooth_rx=0;
	}
	//接着就是一个模式选择（这里需要一个按键，娱乐模式（主要是包含上蹦下跳）和救援模式）
	//模式选择
	if(buletooth_rx == 53)
	{
		buletooth_count+=1;
		if(buletooth_count > 2) buletooth_count=0;
		//需要人为清零一下
		HAL_UART_Transmit_IT(&huart4, &buletooth_count, sizeof(buletooth_count));
//		HAL_UART_Transmit(&huart1, &buletooth_count, sizeof(buletooth_count), HAL_MAX_DELAY);
		buletooth_rx=0;
	}
	//娱乐模式的时候就加入一个按键，开启娱乐和关闭娱乐，就用同一个按键（这个按键和下面的选择共用）
	if(buletooth_rx == 54)
	{
		//娱乐模式开启
		if(buletooth_rx == 54 && buletooth_count == 1 && buletooth_flag ==0)
		{
			buletooth_flag=1;
			buletooth_rx=0;
//			HAL_UART_Transmit(&huart1, &buletooth_flag, sizeof(buletooth_flag), HAL_MAX_DELAY);
		}
		//娱乐模式关闭
		if(buletooth_rx == 54 && buletooth_count == 1 && buletooth_flag ==1)
		{
			buletooth_flag=0;
			buletooth_rx=0;
//			HAL_UART_Transmit(&huart1, &buletooth_flag, sizeof(buletooth_flag), HAL_MAX_DELAY);
		}
		//救援模式
		if(buletooth_rx == 54 && buletooth_count == 2)
		{
			buletooth_leg_count+=1;
			if(buletooth_leg_count > 3) buletooth_leg_count=0;
			HAL_UART_Transmit(&huart4, &buletooth_leg_count, sizeof(buletooth_leg_count), HAL_MAX_DELAY);
			buletooth_rx=0;
		}
	}
	//升高
	if(buletooth_rx == 55)
	{
		//左边
		if(buletooth_rx == 55 && buletooth_leg_count == 1)
		{
			leg3_left_duty-=50;
			WritePos(3, 665+leg3_left_duty, 0, 1500); //对ID是2的舵机进行一个弯曲
			WritePos(12, 400-leg3_left_duty, 0, 1500); //对ID是6的舵机进行一个弯曲
			HAL_Delay(200);
			buletooth_rx=0;
		}
		//右边
		if(buletooth_rx == 55 && buletooth_leg_count == 2)
		{
			leg3_right_duty-=50;
			WritePos(6, 370-leg3_right_duty, 0, 1500); //对ID是3的舵机进行一个弯曲
			WritePos(9, 645+leg3_right_duty, 0, 1500); //对ID是5的舵机进行一个弯曲
			HAL_Delay(200);
			buletooth_rx=0;
		}
		//全部
		if(buletooth_rx == 55 && buletooth_leg_count == 3)
		{
			leg3_all_duty-=50;
			WritePos(3, 665+leg3_all_duty, 0, 1500); //对ID是2的舵机进行一个弯曲
			WritePos(6, 370-leg3_all_duty, 0, 1500); //对ID是3的舵机进行一个弯曲
			WritePos(9, 645+leg3_all_duty, 0, 1500); //对ID是5的舵机进行一个弯曲
			WritePos(12, 400-leg3_all_duty, 0, 1500); //对ID是6的舵机进行一个弯曲
			HAL_Delay(200);
			buletooth_rx=0;
		}
	}
	//降低
	if(buletooth_rx == 56)
	{
		//左边
		if(buletooth_rx == 56 && buletooth_leg_count == 1)
		{
			leg3_left_duty+=50;
			WritePos(3, 665+leg3_left_duty, 0, 1500); //对ID是2的舵机进行一个弯曲
			WritePos(12, 400-leg3_left_duty, 0, 1500); //对ID是6的舵机进行一个弯曲
			HAL_Delay(200);
			buletooth_rx=0;
		}
		//右边
		if(buletooth_rx == 56 && buletooth_leg_count == 2)
		{
			leg3_right_duty+=50;
			WritePos(6, 370-leg3_right_duty, 0, 1500); //对ID是3的舵机进行一个弯曲
			WritePos(9, 645+leg3_right_duty, 0, 1500); //对ID是5的舵机进行一个弯曲
			HAL_Delay(200);
			buletooth_rx=0;
		}
		//全部
		if(buletooth_rx == 56 && buletooth_leg_count == 3)
		{
			leg3_all_duty+=50;
			WritePos(3, 665+leg3_all_duty, 0, 1500); //对ID是2的舵机进行一个弯曲
			WritePos(6, 370-leg3_all_duty, 0, 1500); //对ID是3的舵机进行一个弯曲
			WritePos(9, 645+leg3_all_duty, 0, 1500); //对ID是5的舵机进行一个弯曲
			WritePos(12, 400-leg3_all_duty, 0, 1500); //对ID是6的舵机进行一个弯曲
			HAL_Delay(200);
			buletooth_rx=0;
		}
	}
	if(buletooth_flag == 1) happy_mode();
		else if(buletooth_flag == 0 && buletooth_count == 1) 
		{
			servo_angle_init();
			HAL_Delay(250);
		}
	//救援模式对每两条腿的选择，第一次左边，第二次右边，第三次全部，两个按键来升高或者下降
	
	//下面这里主要是涉及发送数据，包括呼救的在手机显示，以及ID的显示。
}
//这边主要是负责语音模块的，语音这边接收到的数据和那边发过来是一样的
uint8_t voice_tx=0; //作为发送信息的
uint8_t voice_rx=0; //作为接收信息的
uint8_t voice_flag=0; //作为语音这边的标志位
uint8_t voice_count=0; //作为语音这边时间的计数，当时间超过多少时，对接收数据定时清零
void voice_ct(void)
{
		//当有人呼救时，就收到对应的数据，然后通过蓝牙发出去,1表示有人进行呼救的
		if(voice_rx == 1)
		{
			voice_flag=1;
			HAL_UART_Transmit(&huart4, &voice_flag, sizeof(voice_flag), HAL_MAX_DELAY);
			voice_rx=0;
		}
		//2表示叫机器狗趴下
		if(voice_rx == 2)
		{		
			servo_angle_curve();
			voice_rx=0;
		}
		//3表示叫机器狗站立
		if(voice_rx == 3)
		{
			servo_angle_init();
			voice_rx=0;
		}
		else
		{
//			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		}
		
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int8_t count=0;
// 定义缓冲区存储转换后的字符串
char uart_buf[64]; // 根据数据长度调整大小
//这里注意下，使用串口四作为蓝牙的串口，串口五作为语音模块的
//同时也注意一下优先级问题，蓝牙模块的优先级最高，其次是中断时钟7，最后就是语音模块的，总线舵机只要发送数据，不涉及中断接收，忽略
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
	
  /* USER CODE BEGIN SysInit */
	
  /* USER CODE END SysInit */
	
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_USART2_UART_Init();
  MX_TIM7_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
	//串口中断接收配置
	HAL_UART_Receive_IT(&huart4, &buletooth_rx, 1); //这个用来蓝牙串口中断接收的
	HAL_UART_Receive_IT(&huart5, &voice_rx, 1); //这个用来蓝牙串口中断接收的
	//编码器部分初始化，现在换一个思路，换成两电机进行控制
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim5,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim5,TIM_CHANNEL_2);
	//电机部分初始化,现在换一个思路，换成两电机进行控制
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_4);
	//电机转向初始化
	motor_init();
	//舵机初始化部分
	servo_all_init();
	setup();
//	WritePos(2, 510, 0, 1500); //对ID是2的舵机进行一个初始化
	//时钟初始化部分
	HAL_TIM_Base_Start_IT(&htim7);	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
			//灯测试
//		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
//		HAL_Delay(500);
//		HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
//		HAL_Delay(500);
		if(servo_flag == 0)
		{
			servo_angle_init();
			servo_flag=1;
		}
//		motorA_lt();
//		motorA_ld(30);
//		motorA_rt();
//		motorA_rd(10);
//		motorB_lf();
//		motorB_ld(30);
//		motorB_rt();
//		motorB_rd(10);
		//打印出来，上面的是四个编码器的，下面的是一个的
//		sprintf(uart_buf, "E1:%d,E2:%d,E3:%d,E4:%d\n", A_lmc, A_rmc, B_lmc, B_rmc);
		bluetooth_ct();
		voice_ct();
//		//蓝牙和语音模块的
//		sprintf(uart_buf, "b:%d,v:%d", A_lmc, A_rmc);
//		//编码器的
//		sprintf(uart_buf, "E1:%d,E2:%d", A_lmc, A_rmc);
//    //先用串口2来发送
//    HAL_UART_Transmit(&huart4, (uint8_t*)uart_buf, sizeof(uart_buf), HAL_MAX_DELAY);
//		HAL_Delay(200);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
// TIM7中断回调函数（弱定义，用户需重写）
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) 
{
    if (htim->Instance == TIM7) 
		{
				//编码器读取部分
				A_lmc=motorA_le();
				A_rmc=-motorA_re();
//				B_lmc=motorB_le();
//				B_rmc=motorB_re();
////			  //这里主要是一个闭环的控制
				motorA_lp(A_ls,A_lmc);
				motorA_rp(A_rs,A_rmc);
//				motorB_lp(A_ls,B_lmc);
//				motorB_rp(A_rs,B_rmc);
//				motorA_lp(0,A_lmc);
//				motorA_rp(10,A_rmc);
//				motorB_lp(10,B_lmc);
//				motorB_rp(10,B_rmc);
    }
}
//接收完成回调函数



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{
		//蓝牙的
    if (huart->Instance == UART4) 
		{
//        printf("Received: %c\n", buletooth_rx);  // 打印接收的字符
        HAL_UART_Receive_IT(&huart4, &buletooth_rx, 1); // 重新启动接收
    }
		//语音的
		 if (huart->Instance == UART5) 
		{
//        printf("Received: %c\n", voice_rx);  // 打印接收的字符
        HAL_UART_Receive_IT(&huart5, &voice_rx, 1); // 重新启动接收
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
