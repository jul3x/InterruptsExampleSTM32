#include <gpio.h>
#include <stm32.h>
#include <string.h>
#include "queue.h"
#include "usart_nvic_dma.h"

// Button functions
typedef struct {
    uint16_t pin;
    GPIO_TypeDef * gpio;
    char *pressed_str;
    char *released_str;
    uint16_t is_reverse_logic;
} Button;

Queue tx_buffer;
Button buttons[7] = {
    {13, GPIOC, "USER PRESSED\r\n", "USER RELEASED\r\n", 0},
    {3, GPIOB, "LEFT PRESSED\r\n", "LEFT RELEASED\r\n", 0},
    {4, GPIOB, "RIGHT PRESSED\r\n", "RIGHT RELEASED\r\n", 0},
    {5, GPIOB, "UP PRESSED\r\n", "UP RELEASED\r\n", 0},
    {6, GPIOB, "DOWN PRESSED\r\n", "DOWN RELEASED\r\n", 0},
    {10, GPIOB, "FIRE PRESSED\r\n", "FIRE RELEASED\r\n", 0},
    {0, GPIOA, "MODE PRESSED\r\n", "MODE RELEASED\r\n", 1}
};

void DMA1_Stream6_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI0_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);

void configure(Button *button);

void interrupt(uint16_t interr, uint16_t exti_line, Button *button);

int main() {
    clear(&tx_buffer);

    // Initializations
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_DMA1EN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    __NOP();

    // Buttons configuration
    for (int i = 0; i < 7; ++i)
    {
        configure(&buttons[i]);
    }

    configureUSART();
    configureDMA();
    configureNVIC();

    // USART Enabling
    USART2->CR1 |= USART_CR1_UE;

    // Main loop
    for (;;) {}

    return 0;
}

void configure(Button *button)
{
    GPIOinConfigure(button->gpio, button->pin, GPIO_PuPd_UP, EXTI_Mode_Interrupt, EXTI_Trigger_Rising_Falling);
}

void interrupt(uint16_t interr, uint16_t exti_line, Button *button)
{
    if (interr & exti_line)
    {
        uint16_t is_released = ((button->gpio->IDR >> button->pin) & 1) ^ button->is_reverse_logic;

        if ((DMA1_Stream6->CR & DMA_SxCR_EN) == 0 &&
            (DMA1->HISR & DMA_HISR_TCIF6) == 0)
        {
            if (is_released) {
                DMA1_Stream6->M0AR = (uint32_t)button->released_str;
                DMA1_Stream6->NDTR = strlen(button->released_str);
            }
            else {
                DMA1_Stream6->M0AR = (uint32_t)button->pressed_str;
                DMA1_Stream6->NDTR = strlen(button->pressed_str);
            }
            DMA1_Stream6->CR |= DMA_SxCR_EN;
        }
        else
        {
            if (is_released) {
                enqueue(&tx_buffer, button->released_str);
            }
            else {
                enqueue(&tx_buffer, button->pressed_str);
            }

        }

        EXTI->PR = exti_line;
    }
}

void DMA1_Stream6_IRQHandler() {
    uint32_t isr = DMA1->HISR;
    if (isr & DMA_HISR_TCIF6)
    {
        DMA1->HIFCR = DMA_HIFCR_CTCIF6;

        if (!empty(&tx_buffer))
        {
            char *text = dequeue(&tx_buffer);
            DMA1_Stream6->M0AR = (uint32_t)text;
            DMA1_Stream6->NDTR = strlen(text);
            DMA1_Stream6->CR |= DMA_SxCR_EN;

            resetIfNeeded(&tx_buffer);
        }
    }
}

void EXTI15_10_IRQHandler(void)
{
    uint16_t interr = EXTI->PR;
    interrupt(interr, EXTI_PR_PR10, &buttons[5]);
    interrupt(interr, EXTI_PR_PR13, &buttons[0]);
}

void EXTI9_5_IRQHandler(void) {
    uint16_t interr = EXTI->PR;
    interrupt(interr, EXTI_PR_PR5, &buttons[3]);
    interrupt(interr, EXTI_PR_PR6, &buttons[4]);
}

void EXTI0_IRQHandler(void) {
    uint16_t interr = EXTI->PR;
    interrupt(interr, EXTI_PR_PR0, &buttons[6]);
}

void EXTI3_IRQHandler(void) {
    uint16_t interr = EXTI->PR;
    interrupt(interr, EXTI_PR_PR3, &buttons[1]);
}

void EXTI4_IRQHandler(void) {
    uint16_t interr = EXTI->PR;
    interrupt(interr, EXTI_PR_PR4, &buttons[2]);
}