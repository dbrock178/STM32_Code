#include "usart.h"

#define GPIOAEN		(1<<0)
#define	GPIOCEN		(1<<2)
#define USART1EN	(1<<4)
#define USART2EN	(1<<17)
#define USART6EN	(1<<5)
#define TE			(1<<3)
#define RE			(1<<2)
#define UE			(1<<13)
#define SR_TXE		(1<<7)
#define SR_RXNE		(1<<5)
#define PERIPH_CLK	(16000000) //16 Mhz

static void usart_set_buad_rate(USART_TypeDef *usart,uint32_t periph_clk,uint32_t buad_rate);
static uint16_t usart_buad_rate_convert(uint32_t periph_clk, uint32_t buad_rate);

void usart1_tx_init(uint32_t buad_rate)
{
	/*
	 * Configure GPOIA
	 * 		Enable clock for GPOIA
	 * 		Set PA9 as alt function
	 * 		Configure alt function as AF7
	 */
	/*
	 * Configure USART
	 * 		Enable clock for USART
	 * 		Configure buad rate
	 * 		Disable Module (if enabled)
	 * 		Configure transfer direction
	 * 		Enable Module
	 */

	// Enable clock for GPOIA
	RCC->AHB1ENR|=GPIOAEN;

	// Set PA9 as alt function
	GPIOA->MODER|=(1<<19);
	GPIOA->MODER&=~(1<<18);

	// Configure alt function as AF7
	GPIOA->AFR[1]&=~(1<<7);
	GPIOA->AFR[1]|=(1<<6);
	GPIOA->AFR[1]|=(1<<5);
	GPIOA->AFR[1]|=(1<<4);

	// Enable clock for USART
	RCC->APB2ENR|=USART1EN;

	// Configure buad rate
	usart_set_buad_rate(USART1, PERIPH_CLK, buad_rate);

	// Disable Module
	USART1->CR1&=~UE;

	// Configure transfer direction
	USART1->CR1|=TE;

	// Enable Module
	USART1->CR1|=UE;
}

void usart1_rx_init(uint32_t buad_rate)
{
	/*
	 * Configure GPOIA
	 * 		Enable clock for GPOIA
	 * 		Set PA10 as alt function
	 * 		Configure alt function as AF7
	 */
	/*
	 * Configure USART
	 * 		Enable clock for USART
	 * 		Configure buad rate
	 * 		Disable Module (if enabled)
	 * 		Configure transfer direction
	 * 		Enable Module
	 */

	// Enable clock for GPOIA
	RCC->AHB1ENR|=GPIOAEN;

	// Set PA10 as alt function
	GPIOA->MODER|=(1<<21);
	GPIOA->MODER&=~(1<<20);

	// Configure alt function as AF7
	GPIOA->AFR[1]&=~(1<<11);
	GPIOA->AFR[1]|=(1<<10);
	GPIOA->AFR[1]|=(1<<9);
	GPIOA->AFR[1]|=(1<<8);

	// Enable clock for USART
	RCC->APB2ENR|=USART1EN;

	// Configure buad rate
	usart_set_buad_rate(USART1, PERIPH_CLK, buad_rate);

	// Disable Module
	USART1->CR1&=~UE;

	// Configure transfer direction
	USART1->CR1|=RE;

	// Enable Module
	USART1->CR1|=UE;
}

void usart1_write(uint8_t ch)
{
	/* Make sure the transmit data registry is empty
	 * Write to transmit data register
	 */

	// Make sure the transmit data registry is empty
	while(!(USART1->SR & SR_TXE));

	// Write to transmit data register
	USART1->DR = ch;
}

uint8_t usart1_read(void)
{
	while(!(USART1->SR & SR_RXNE));
	return USART1->DR;
}

/*
 * USART2 code not tested. May contain bugs.
 */
void usart2_tx_init(uint32_t buad_rate)
{
	/*
	 * Configure GPOIA
	 * 		Enable clock for GPOIA
	 * 		Set PA2 as alt function
	 * 		Configure alt function as AF7
	 */
	/*
	 * Configure USART
	 * 		Enable clock for USART
	 * 		Configure buad rate
	 * 		Disable Module (if enabled)
	 * 		Configure transfer direction
	 * 		Enable Module
	 */

	// Enable clock for GPOIA
	RCC->AHB1ENR|=GPIOAEN;

	// Set PA2 as alt function
	GPIOA->MODER|=(1<<5);
	GPIOA->MODER&=~(1<<4);

	// Configure alt function as AF7
	GPIOA->AFR[0]&=~(1<<11);
	GPIOA->AFR[0]|=(1<<10);
	GPIOA->AFR[0]|=(1<<9);
	GPIOA->AFR[0]|=(1<<8);

	// Enable clock for USART
	RCC->APB1ENR|=USART2EN;

	// Configure buad rate
	usart_set_buad_rate(USART2, PERIPH_CLK, buad_rate);

	// Disable Module
	USART2->CR1&=~UE;

	// Configure transfer direction
	USART2->CR1|=TE;

	// Enable Module
	USART2->CR1|=UE;
}

void usart2_rx_init(uint32_t buad_rate)
{
	/*
	 * Configure GPOIA
	 * 		Enable clock for GPOIA
	 * 		Set PA3 as alt function
	 * 		Configure alt function as AF7
	 */
	/*
	 * Configure USART
	 * 		Enable clock for USART
	 * 		Configure buad rate
	 * 		Disable Module (if enabled)
	 * 		Configure transfer direction
	 * 		Enable Module
	 */

	// Enable clock for GPOIA
	RCC->AHB1ENR|=GPIOAEN;

	// Set PA3 as alt function
	GPIOA->MODER|=(1<<7);
	GPIOA->MODER&=~(1<<6);

	// Configure alt function as AF7
	GPIOA->AFR[0]&=~(1<<15);
	GPIOA->AFR[0]|=(1<<14);
	GPIOA->AFR[0]|=(1<<13);
	GPIOA->AFR[0]|=(1<<12);

	// Enable clock for USART
	RCC->APB1ENR|=USART2EN;

	// Configure buad rate
	usart_set_buad_rate(USART2, PERIPH_CLK, buad_rate);

	// Disable Module
	USART2->CR1&=~UE;

	// Configure transfer direction
	USART2->CR1|=RE;

	// Enable Module
	USART2->CR1|=UE;
}

void usart2_write(uint8_t ch)
{
	/* Make sure the transmit data registry is empty
	 * Write to transmit data register
	 */

	// Make sure the transmit data registry is empty
	while(!(USART2->SR & SR_TXE));

	// Write to transmit data register
	USART2->DR = ch;
}

uint8_t usart2_read(void)
{
	while(!(USART2->SR & SR_RXNE));
	return USART2->DR;
}

void usart6_tx_init(uint32_t buad_rate)
{
	/*
	 * Configure GPOIC
	 * 		Enable clock for GPOIC
	 * 		Set PC6 as alt function
	 * 		Configure alt function as AF8
	 */
	/*
	 * Configure USART
	 * 		Enable clock for USART
	 * 		Configure buad rate
	 * 		Disable Module (if enabled)
	 * 		Configure transfer direction
	 * 		Enable Module
	 */

	// Enable clock for GPOIC
	RCC->AHB1ENR|=GPIOCEN;

	// Set PC6 as alt function
	GPIOC->MODER|=(1<<13);
	GPIOC->MODER&=~(1<<12);

	// Configure alt function as AF8
	GPIOC->AFR[0]|=(1<<27);
	GPIOC->AFR[0]&=~(1<<26);
	GPIOC->AFR[0]&=~(1<<25);
	GPIOC->AFR[0]&=~(1<<24);

	// Enable clock for USART
	RCC->APB2ENR|=USART6EN;

	// Configure buad rate
	usart_set_buad_rate(USART6, PERIPH_CLK, buad_rate);

	// Disable Module
	USART6->CR1&=~UE;

	// Configure transfer direction
	USART6->CR1|=TE;

	// Enable Module
	USART6->CR1|=UE;
}

void usart6_rx_init(uint32_t buad_rate)
{
	/*
	 * Configure GPOIC
	 * 		Enable clock for GPOIC
	 * 		Set PC7 as alt function
	 * 		Configure alt function as AF8
	 */
	/*
	 * Configure USART
	 * 		Enable clock for USART
	 * 		Configure buad rate
	 * 		Disable Module (if enabled)
	 * 		Configure transfer direction
	 * 		Enable Module
	 */

	// Enable clock for GPOIC
	RCC->AHB1ENR|=GPIOCEN;

	// Set PC7 as alt function
	GPIOC->MODER|=(1<<15);
	GPIOC->MODER&=~(1<<14);

	// Configure alt function as AF8
	GPIOC->AFR[0]|=(1<<31);
	GPIOC->AFR[0]&=~(1<<30);
	GPIOC->AFR[0]&=~(1<<29);
	GPIOC->AFR[0]&=~(1<<28);

	// Enable clock for USART
	RCC->APB2ENR|=USART6EN;

	// Configure buad rate
	usart_set_buad_rate(USART6, PERIPH_CLK, buad_rate);

	// Disable Module
	USART6->CR1&=~UE;

	// Configure transfer direction
	USART6->CR1|=RE;

	// Enable Module
	USART6->CR1|=UE;
}

void usart6_write(uint8_t ch)
{
	/* Make sure the transmit data registry is empty
	 * Write to transmit data register
	 */

	// Make sure the transmit data registry is empty
	while(!(USART6->SR & SR_TXE));

	// Write to transmit data register
	USART6->DR = ch;
}

uint8_t usart6_read(void)
{
	while(!(USART6->SR & SR_RXNE));
	return USART6->DR;
}

static void usart_set_buad_rate(USART_TypeDef *usart,uint32_t periph_clk,uint32_t buad_rate)
{
	usart->BRR=usart_buad_rate_convert(periph_clk,buad_rate);
}

static uint16_t usart_buad_rate_convert(uint32_t periph_clk,uint32_t buad_rate)
{
	return ((periph_clk+(buad_rate/2U))/buad_rate);
}
