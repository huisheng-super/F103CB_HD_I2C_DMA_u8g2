/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "u8g2.h"
	u8g2_t u8g2;
//#define u8         unsigned char  // ?unsigned char ????
#define u32 unsigned int
extern void Delayxus(u32 nus)
{
	u32 temp;
	SysTick->LOAD = 9*nus;//Systickʱ��ԴΪ8��Ƶ 72M/8=9M
	SysTick->VAL = 0x00;//�����Ĵ�������
	SysTick->CTRL = 0x01;//Systick������ʹ�� ʱ��ԴΪHCLK/8
	do
	{
		temp = SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));//�жϱ�־λ��ʹ��λ
	SysTick->CTRL = 0x00;
	SysTick->VAL = 0x00;
}
/********************************************************************
*��    ��:Delay_nms(u32 nms)
*��    ��:��ʱn���� ����?:18642
*��ڲ���?:nms
*���ڲ���:��
*˵    ��:��
*���÷�������
********************************************************************/
extern void Delayxms(u32 nms)
{
	u32 temp;
	SysTick->LOAD = 9000*nms;//Systickʱ��ԴΪ8��Ƶ 72M/8=9M
	SysTick->VAL = 0x00;//�����Ĵ�������
	SysTick->CTRL = 0x01;//Systick���������� ʱ��ԴΪHCLK/8
	do
	{
		temp = SysTick->CTRL;
	}while((temp&0x01)&&(!(temp&(1<<16))));//�жϱ�־λ��ʹ��λ
	SysTick->CTRL = 0x00;
	SysTick->VAL = 0x00;
}

/**/
//#define u8         unsigned char  // ?unsigned char ????
#define MAX_LEN    128  //
#define OLED_ADDRESS  0x78 // oledģ��ӻ����?
#define OLED_CMD   0x00  // д����
#define OLED_DATA  0x40  // д����

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
    /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    static uint8_t buffer[128];
    static uint8_t buf_idx;
    uint8_t *data;

    switch (msg)
    {
    case U8X8_MSG_BYTE_INIT:
    {
        /* add your custom code to init i2c subsystem */
        MX_I2C1_Init(); //I2C��ʼ��
    }
    break;

    case U8X8_MSG_BYTE_START_TRANSFER:
    {
        buf_idx = 0;
    }
    break;

    case U8X8_MSG_BYTE_SEND:
    {
        data = (uint8_t *)arg_ptr;

        while (arg_int > 0)
        {
            buffer[buf_idx++] = *data;
            data++;
            arg_int--;
        }
    }
    break;

    case U8X8_MSG_BYTE_END_TRANSFER:
    {
        if (HAL_I2C_Master_Transmit(&hi2c1, (OLED_ADDRESS), buffer, buf_idx, 1000) != HAL_OK)
            return 0;
    }
    break;

    case U8X8_MSG_BYTE_SET_DC:
        break;

    default:
        return 0;
    }

    return 1;
}

void delay_us(uint32_t time)
{
    uint32_t i = 8 * time;
    while (i--)
        ;
}

uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  static uint8_t buffer[32];		/* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
  static uint8_t buf_idx;
  uint8_t *data;
 
  switch(msg){
		
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;      
      while( arg_int > 0 ){
				buffer[buf_idx++] = *data;
				data++;
				arg_int--;
			}      
    break;
			
    case U8X8_MSG_BYTE_INIT:
      /* add your custom code to init i2c subsystem */
    break;
		
    case U8X8_MSG_BYTE_START_TRANSFER:
      buf_idx = 0;
    break;
		
    case U8X8_MSG_BYTE_END_TRANSFER:
      HAL_I2C_Master_Transmit_DMA(&hi2c1,u8x8_GetI2CAddress(u8x8), buffer, buf_idx);

    break;
		
    default:
      return 0;
  }
  return 1;
}
void u8g2Init(u8g2_t *u8g2)
{
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_gpio_and_delay); // ��ʼ�� u8g2 �ṹ��
	u8g2_InitDisplay(u8g2);                                                                       // ������ѡ��оƬ���г�ʼ����������ʼ����ɺ���ʾ�����ڹر�״�?
	u8g2_SetPowerSave(u8g2, 0);                                                                   // ����ʾ��
	u8g2_ClearBuffer(u8g2);
}


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int state;
short x,x_trg;
short y = 10,y_trg =-64;
int ui_run(short *a,short *a_trg)
{
	if(*a < *a_trg)
	{
		*a +=1;
	}
	else if(*a > *a_trg)
	{
		*a -= 1;
	}
	else
	{
		return 0;
	}
	return 1;
}

typedef struct
{
	char *str;
	
}SETTING_LIST;
SETTING_LIST list[] = 
{
  {"list_a"},
  {"list_b"},
  {"list_c"},
  {"list_1"},
  {"list_2"},
  {"list_3"},
  {"list_4"},
  {"list_5"},
  {"list_6"},
};

void ui_show()
{
	int list_len = sizeof(list) / sizeof(SETTING_LIST);
	u8g2_ClearBuffer(&u8g2);
	for(int i = 0; i < list_len ; i++)
	{
		u8g2_DrawStr(&u8g2,x,y+i*10,list[i].str);
	}
	ui_run(&y,&y_trg);
	u8g2_SendBuffer(&u8g2);
	
}
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
  MX_DMA_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

	u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2,U8G2_R0,u8x8_byte_hw_i2c,u8x8_gpio_and_delay);
	u8g2_InitDisplay(&u8g2); // send init sequence to the display, display is in sleep mode after this,
	u8g2_SetPowerSave(&u8g2, 0); // wake up display
	u8g2_ClearDisplay(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_ncenB08_tr);
	u8g2_SendBuffer(&u8g2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_1) == RESET)
		{
			x = 0;
			y = 10;
		
		
		}
		if(ui_run(&y,&y_trg) == 0)
		{
//				if(state == 1)
//				{
//					state = 2;
//				}
		}
			u8g2_ClearBuffer(&u8g2);
			u8g2_DrawStr(&u8g2,x,y,"world");
			u8g2_DrawStr(&u8g2,x,y+16,"Hello1");
			u8g2_DrawStr(&u8g2,x,y+32,"Hello2");
			Delayxms(1);
		
			u8g2_SendBuffer(&u8g2);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
