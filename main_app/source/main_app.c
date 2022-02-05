#include "main_app.h"

#define uCSHELL_STACKSIZE 512
extern UART_HandleTypeDef huart2;
CL_cli_type ucshell;
// protoypes
static void blinkLed(void);
static void led_task(void *pvParameters);
static void uCShell_task(void *pvParameters);
void cmd_ok_handler(uint8_t num, char *values[]);
void cmd_cls_handler(uint8_t num, char *values[]);
void cmd_up_hanlder(uint8_t num, char *values[]);
void cmd_add_handler(uint8_t num, char *values[]);
void cmd_test_handler(uint8_t num, char *values[]);
void cmd_eeprom_handler(uint8_t num, char *values[]);
void CL_printMsg(char *msg, ...);

void main_app_init(void)
{
	int8_t status = 0;
	CL_printMsg("Main App init\r\n");
	status = xTaskCreate(led_task, (signed char *)"LED", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
	if (status == -1)
		CL_printMsg("error creating LED task task\n");
	status = xTaskCreate(uCShell_task, (signed char *)"uCShell", uCSHELL_STACKSIZE, NULL, tskIDLE_PRIORITY, NULL);
	if (status == -1)
		CL_printMsg("error creating UCShell task\n");
	vTaskStartScheduler();
}
static void led_task(void *pvParameters)
{
	while (1)
	{
		blinkLed();
		vTaskDelay(500);
	}
}
void uCShell_task(void *pvParameters)
{
	// enalbe RX inteerupt for USART2
	USART2->CR1 |= USART_CR1_RXNEIE;
	// uCShell init
	CL_cli_init(&ucshell, "uCSHell:>", CL_printMsg);

	ucshell.registerCommand("ok", ' ', cmd_ok_handler, "Prints 'ok' if cli is ok", false);
	ucshell.registerCommand("cls", ' ', cmd_cls_handler, "Clears screen\000", false);
	ucshell.registerCommand("up", ' ', cmd_up_hanlder, "up Arrow", false);
	ucshell.registerCommand("add", '+', cmd_add_handler, "Adds numbers", false);
	ucshell.registerCommand("down", ' ', cmd_up_hanlder, "down Arrow", false);
	ucshell.registerCommand("rand", ' ', cmd_up_hanlder, "Random", false);
	ucshell.registerCommand("Wilbert", ' ', cmd_up_hanlder, "Wilbert", false);
	ucshell.registerCommand("test", ' ', cmd_test_handler, "Prints 'ok' if cli is ok", true);

	while (1)
	{
		uCShell_run(&ucshell);
		vTaskDelay(50);
	}
}
static void blinkLed(void)
{
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}
void cmd_ok_handler(uint8_t num, char *values[])
{

	CL_printMsg("System ok! \r\n");
}
void cmd_test_handler(uint8_t num, char *values[])
{
	static uint32_t x = 0;
	CL_printMsg("%d \r\n", x++);
}
void cmd_add_handler(uint8_t num, char *values[])
{

	//'3' '4' '7' '8'
	// CL_printMsg("%d %d %d\n",atoi(values[0]),atoi(values[1]),atoi(values[2]));
	uint32_t total = 0;
	for (int i = 0; i < num; i++)
	{
		total += atoi(values[i]);
	}
	CL_printMsg("Total: %d \r\n", total);
}
void cmd_cls_handler(uint8_t num, char *values[])
{

	CL_printMsg("\033c");
}
void cmd_up_hanlder(uint8_t num, char *values[])
{
	// CL_printMsg("%c%c%c",0x1b, 0x5b, 0x41);
	int i, j, n;

	for (i = 0; i < 11; i++)
	{
		for (j = 0; j < 10; j++)
		{
			n = 10 * i + j;
			if (n > 108)
				break;
			CL_printMsg("\033[%dm %3d\033[m", n, n);
		}
		CL_printMsg("\n");
	}
}
void CL_printMsg(char *msg, ...)
{
	char buff[250];
	va_list args;
	va_start(args, msg);
	vsprintf(buff, msg, args);

	for (int i = 0; i < strlen(buff); i++)
	{
		USART2->DR = buff[i];
		while (!(USART2->SR & USART_SR_TXE))
			;
	}

	while (!(USART2->SR & USART_SR_TC))
		;
}
void USART2_callback(void)
{
	/* USER CODE BEGIN USART2_IRQn 0 */

	/* USER CODE BEGIN USART2_IRQn 1 */
	// fetch data
	ucshell.charReceived = USART2->DR;

	// if the character receieved is not the delimeter then echo the character
	if (ucshell.charReceived != ucshell.delimeter || ucshell.charReceived != '[' || ucshell.charReceived != ']')
	{
		USART2->DR = ucshell.charReceived;
	}
	ucshell.parseChar(&ucshell);
	/* USER CODE END USART2_IRQn 1 */
}