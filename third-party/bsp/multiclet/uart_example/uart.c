 /**
   ******************************************************************************
   *  _ _ _    _ _ _   _ _
   * |     \  /     |/     \
   * |      \/           _ _\
   * |                  /
   * |         MultiClet
   * |                  \ _ _
   * |                      /
   * |_ _ _|\/|_ _ _|\ _ _ /
   *
   * @file    uart.c 
   * @author  ������� ������������� ����������
   * @date    13-07-2015
   * @brief   ������ ������ c UART. 
   *          
   ******************************************************************************
   * @attention
   *
   * <h2><center>&copy; COPYRIGHT(c) 2015 Multiclet corp.</center></h2>
   *
   *  ������ ������ ������� ��� �������� ������������ ������������ �
   *  ������������� ���������������� ����������. �������� ���������� �� ��Ѩ�
   *  ��������������� �� ��������� ������ ��� ��������� �����, �����ͨ���� 
   *  �������������� � ���������������� ��������� ����������, ������������
   *  � ������ �������.
   * 
   ******************************************************************************
   */

//���������� ����������� ����������
#include <R1_ccf.h>
#include <uart.h>

/**
������������ �����������: 
LED0 - GPIOB(1<<25), LED1 - GPIOB(1<<24), LED2 - GPIOB(1<<23)
LED3 - GPIOB(1<<27), LED4 - GPIOC(1<<26), LED5 - GPIOB(1<<29)
LED6 - GPIOB(1<<28), LED7 - GPIOA(1<<25)

������������ UART: 
UART3[TXD] - GPIOF(1<<14), UART3[RXD] - GPIOF(1<<13)
UART1[TXD] - GPIOA(1<<23), UART1[RXD] - GPIOA(1<<22), UART1[CTS] - GPIOA(1<<24), UART1[RTS] - GPIOA(1<<25)
UART2[TXD] - GPIOC(1<<11), UART2[RXD] - GPIOC(1<<10)
UART0[TXD] - GPIOC(1<<22), UART0[RXD] - GPIOC(1<<21)
*/

char test[5]={'t', 'e', 's', 't', '\n'};

void main()
{
	
	UART_InitTypeDef UART_InitStructure;
    
	UART_InitStructure.System_freq = 8000000; //��������� ��������� ������� (8 ���)
	UART_InitStructure.BaudRate = 115200; //������������� ������� ������ UART
	UART_InitStructure.TypeParity = 0x00000000; //��������� ��� �������� ��������
	UART_InitStructure.Parity = 0x00000000; //��������� �������� ��������
	UART_InitStructure.FlowControl = 0x00000000; //��������� cts/rts
	UART_InitStructure.Mode = 0x00000003; //��������� ����� � ��������

	GPIOC->BPS = (1<<10)|(1<<11);		 //����������� ���� F �� �������������� ������� ��� UART3
	uart_init(UART0, &UART_InitStructure); //�������������� UART3 � ������� ���������

	DM2UART(UART0, (int)test, 5); //���������� �� UART3 ������ �� 5-�� ���������
#if 0
	uart_send_int(0xABCDEF12, UART3); //���������� 32-� ������ ����� �� UART3
	*(int*)0x40000 = 0x12345678; //���������� �������� � ������ �� ������ 0x40000
	while(UART_FIFO_TX_FULL(UART3) == 1); //���������, ��� � ������ FIFO �� �������� ���� �����
	UART_SEND_BYTE(*(char*)0x40001, UART3); //���������� ���� � ����� FIFO �� �������� UART3
	while(UART_FIFO_TX_FULL(UART3) == 1); 
	UART_SEND_BYTE(0xCD, UART3);
	while(UART_FIFO_TX_FULL(UART3) == 1); 
	UART_SEND_BYTE(0xEF, UART3);
	while(UART_FIFO_TX_FULL(UART3) == 1); 
	UART_SEND_BYTE(0x12, UART3);
#endif
	//led    
	GPIOB->DIR |= 1<<25; //����������� ���� B ��� ���������� LED0 �� �����
	GPIOB->OUT |= 1<<25; //������������� "1" � �������� ������������ ������ ����� ��� LED0
}
