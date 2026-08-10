// test_data/StressTestApp/stress_register_map.c
// Heavy focus on register maps, bitfields, unions, and peripheral configuration.
// Very common pattern in real embedded drivers.

#include <stdint.h>
#include <stdbool.h>

#define __IO volatile
#define __IOM volatile
#define __IM volatile const

// === Typical peripheral register block (similar to STM32 / NXP style) ===
typedef struct {
    __IOM uint32_t CR;           /* Control Register */
    __IOM uint32_t SR;           /* Status Register */
    __IOM uint32_t DR;           /* Data Register */
    __IOM uint32_t BRR;          /* Baud Rate Register */
    __IM  uint32_t RESERVED[4];
    __IOM uint32_t CR2;          /* Control Register 2 */
} USART_TypeDef;

// === Bitfield definitions for CR register ===
typedef union {
    uint32_t word;
    struct {
        uint32_t UE     : 1;     /* USART Enable */
        uint32_t RE     : 1;     /* Receiver Enable */
        uint32_t TE     : 1;     /* Transmitter Enable */
        uint32_t IDLEIE : 1;     /* IDLE Interrupt Enable */
        uint32_t RXNEIE : 1;     /* RXNE Interrupt Enable */
        uint32_t TCIE   : 1;     /* Transmission Complete Interrupt Enable */
        uint32_t TXEIE  : 1;     /* TXE Interrupt Enable */
        uint32_t PEIE   : 1;     /* PE Interrupt Enable */
        uint32_t PS     : 1;     /* Parity Selection */
        uint32_t PCE    : 1;     /* Parity Control Enable */
        uint32_t WAKE   : 1;     /* Wakeup method */
        uint32_t M      : 1;     /* Word length */
        uint32_t UEIE   : 1;     /* USART Error Interrupt Enable */
        uint32_t        : 19;    /* Reserved */
    } bits;
} USART_CR_t;

// === Status register bitfield union ===
typedef union {
    uint32_t word;
    struct {
        uint32_t PE   : 1;       /* Parity Error */
        uint32_t FE   : 1;       /* Framing Error */
        uint32_t NE   : 1;       /* Noise Error */
        uint32_t ORE  : 1;       /* Overrun Error */
        uint32_t IDLE : 1;       /* IDLE line detected */
        uint32_t RXNE : 1;       /* Read Data Register Not Empty */
        uint32_t TC   : 1;       /* Transmission Complete */
        uint32_t TXE  : 1;       /* Transmit Data Register Empty */
        uint32_t      : 24;
    } bits;
} USART_SR_t;

// === Peripheral instance with multiple access methods ===
typedef struct {
    USART_TypeDef*   regs;           /* Pointer to hardware registers */
    USART_CR_t       cr_shadow;      /* Shadow copy for safe modification */
    USART_SR_t       sr;             /* Status register shadow */
    uint32_t         baudrate;
    bool             initialized;
} USART_Handle_t;

// === Common register access macros ===
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))
#define MODIFY_REG(REG, CLEARMASK, SETMASK)  ((REG) = (((REG) & (~(CLEARMASK))) | (SETMASK)))

// === USART Initialization with heavy bitfield usage ===
bool USART_Init(USART_Handle_t* husart, uint32_t baud) {
    if (husart == NULL || husart->regs == NULL) {
        return false;
    }

    // Use shadow register for safe configuration
    husart->cr_shadow.word = 0;

    // Configure using bitfields
    husart->cr_shadow.bits.UE  = 0;     // Disable first
    husart->cr_shadow.bits.RE  = 1;
    husart->cr_shadow.bits.TE  = 1;
    husart->cr_shadow.bits.RXNEIE = 1;  // Enable RX interrupt
    husart->cr_shadow.bits.TXEIE  = 0;

    // Write to hardware
    husart->regs->CR = husart->cr_shadow.word;

    // Baud rate configuration (simplified)
    husart->regs->BRR = (16000000U / baud);
    husart->baudrate = baud;

    // Enable peripheral
    husart->cr_shadow.bits.UE = 1;
    husart->regs->CR = husart->cr_shadow.word;

    husart->initialized = true;
    return true;
}

// === Transmit function with status checking ===
bool USART_Transmit(USART_Handle_t* husart, uint8_t data) {
    if (husart == NULL || !husart->initialized) {
        return false;
    }

    // Wait for TXE using bitfield
    while (!READ_BIT(husart->regs->SR, (1U << 7))) {
        // Timeout could be added here
    }

    husart->regs->DR = data;

    // Wait for transmission complete
    while (!READ_BIT(husart->regs->SR, (1U << 6))) {}

    return true;
}

// === Receive with error checking via bitfields ===
bool USART_Receive(USART_Handle_t* husart, uint8_t* data) {
    if (husart == NULL || data == NULL || !husart->initialized) {
        return false;
    }

    // Check for errors using bitfield union
    husart->sr.word = husart->regs->SR;

    if (husart->sr.bits.PE || husart->sr.bits.FE || husart->sr.bits.ORE) {
        // Clear errors
        husart->regs->SR = 0;
        return false;
    }

    if (husart->sr.bits.RXNE) {
        *data = (uint8_t)husart->regs->DR;
        return true;
    }

    return false;
}