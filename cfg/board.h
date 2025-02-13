/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#pragma once

/*
 * Board identifier.
 */
#define BOARD_CAN
#define BOARD_NAME                  "Base roulante CAN"

/*
 * Board oscillators-related settings.
 */

#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768
#endif

#define STM32_LSEDRV                (3U << 3U)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000U
#define STM32_HSE_BYPASS            TRUE
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   300U

/*
 * MCU type as defined in the ST header.
 */
#define STM32G484xx

/*
 * IO pins assignments.
 */
#define	PA00                           0U
#define	PA01                           1U
#define	PA02_UART2TX                   2U
#define	PA03_UART2RX                   3U
#define	PA04_MEAS_VBAT                 4U
#define	PA05                           5U
#define	PA06                           6U
#define	PA07                           7U
#define	PA08                           8U
#define	PA09                           9U
#define	PA10                           10U
#define	PA11_USB_DM                    11U
#define	PA12_USB_DP                    12U
#define	PA13_SWDIO                     13U
#define	PA14_SWCLK                     14U
#define	PA15                           15U

#define	PB00                           0U
#define	PB01                           1U
#define	PB02                           2U
#define	PB03_SPI1_CSK                  3U
#define	PB04_SPI1_MISO                 4U
#define	PB05_SPI1_MOSI                 5U
#define	PB06_SPI1_CS                   6U
#define	PB07                           7U
#define	PB08                           8U
#define	PB09                           9U
#define	PB10                           10U
#define	PB11                           11U
#define	PB12_CAN_RX                    12U
#define	PB13_CAN_TX                    13U
#define	PB14                           14U
#define	PB15                           15U

#define	PC00                           0U
#define	PC01_LED1                      1U
#define	PC02_LED2                      2U
#define	PC03                           3U
#define	PC04                           4U
#define	PC05                           5U
#define	PC06                           6U
#define	PC07                           7U
#define	PC08                           8U
#define	PC09                           9U
#define	PC10                           10U
#define	PC11                           11U
#define	PC12                           12U
#define	PC13                           13U
#define	PC14                           14U
#define	PC15                           15U

#define	PD00                           0U
#define	PD01                           1U
#define	PD02_IMU_INT1                  2U
#define	PD03                           3U
#define	PD04                           4U
#define	PD05                           5U
#define	PD06                           6U
#define	PD07                           7U
#define	PD08                           8U
#define	PD09                           9U
#define	PD10                           10U
#define	PD11                           11U
#define	PD12                           12U
#define	PD13                           13U
#define	PD14                           14U
#define	PD15                           15U

#define	PE00                           0U
#define	PE01                           1U
#define	PE02                           2U
#define	PE03                           3U
#define	PE04                           4U
#define	PE05                           5U
#define	PE06                           6U
#define	PE07                           7U
#define	PE08                           8U
#define	PE09                           9U
#define	PE10                           10U
#define	PE11                           11U
#define	PE12                           12U
#define	PE13                           13U
#define	PE14                           14U
#define	PE15                           15U

#define	PF00_OSC_IN                    0U
#define	PF01                           1U
#define	PF02                           2U
#define	PF03                           3U
#define	PF04                           4U
#define	PF05                           5U
#define	PF06                           6U
#define	PF07                           7U
#define	PF08                           8U
#define	PF09                           9U
#define	PF10                           10U
#define	PF11                           11U
#define	PF12                           12U
#define	PF13                           13U
#define	PF14                           14U
#define	PF15                           15U

#define	PG00                           0U
#define	PG01                           1U
#define	PG02                           2U
#define	PG03                           3U
#define	PG04                           4U
#define	PG05                           5U
#define	PG06                           6U
#define	PG07                           7U
#define	PG08                           8U
#define	PG09                           9U
#define	PG10                           10U
#define	PG11                           11U
#define	PG12                           12U
#define	PG13                           13U
#define	PG14                           14U
#define	PG15                           15U

/*
 * IO lines assignments.
 */
#define	LINE_UART2TX                   PAL_LINE(GPIOA, 2U)
#define	LINE_UART2RX                   PAL_LINE(GPIOA, 3U)
#define	LINE_MEAS_VBAT                 PAL_LINE(GPIOA, 4U)
#define	LINE_USB_DM                    PAL_LINE(GPIOA, 11U)
#define	LINE_USB_DP                    PAL_LINE(GPIOA, 12U)
#define	LINE_SWDIO                     PAL_LINE(GPIOA, 13U)
#define	LINE_SWCLK                     PAL_LINE(GPIOA, 14U)

#define	LINE_SPI1_CSK                  PAL_LINE(GPIOB, 3U)
#define	LINE_SPI1_MISO                 PAL_LINE(GPIOB, 4U)
#define	LINE_SPI1_MOSI                 PAL_LINE(GPIOB, 5U)
#define	LINE_SPI1_CS                   PAL_LINE(GPIOB, 6U)
#define	LINE_CAN_RX                    PAL_LINE(GPIOB, 12U)
#define	LINE_CAN_TX                    PAL_LINE(GPIOB, 13U)

#define	LINE_LED1                      PAL_LINE(GPIOC, 1U)
#define	LINE_LED2                      PAL_LINE(GPIOC, 2U)

#define	LINE_IMU_INT1                  PAL_LINE(GPIOD, 2U)

#define	LINE_OSC_IN                    PAL_LINE(GPIOF, 0U)


/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LEVEL_LOW(n)        (0U << (n))
#define PIN_ODR_LEVEL_HIGH(n)       (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_SPEED_VERYLOW(n) (0U << ((n) * 2U))
#define PIN_OSPEED_SPEED_LOW(n)     (1U << ((n) * 2U))
#define PIN_OSPEED_SPEED_MEDIUM(n)  (2U << ((n) * 2U))
#define PIN_OSPEED_SPEED_HIGH(n)    (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

#define VAL_GPIOA_MODER                 (PIN_MODE_INPUT(PA00) | \
					 PIN_MODE_INPUT(PA01) | \
					 PIN_MODE_ALTERNATE(PA02_UART2TX) | \
					 PIN_MODE_ALTERNATE(PA03_UART2RX) | \
					 PIN_MODE_ANALOG(PA04_MEAS_VBAT) | \
					 PIN_MODE_INPUT(PA05) | \
					 PIN_MODE_INPUT(PA06) | \
					 PIN_MODE_INPUT(PA07) | \
					 PIN_MODE_INPUT(PA08) | \
					 PIN_MODE_INPUT(PA09) | \
					 PIN_MODE_INPUT(PA10) | \
					 PIN_MODE_ALTERNATE(PA11_USB_DM) | \
					 PIN_MODE_ALTERNATE(PA12_USB_DP) | \
					 PIN_MODE_ALTERNATE(PA13_SWDIO) | \
					 PIN_MODE_ALTERNATE(PA14_SWCLK) | \
					 PIN_MODE_INPUT(PA15))

#define VAL_GPIOA_OTYPER                (PIN_OTYPE_PUSHPULL(PA00) | \
					 PIN_OTYPE_PUSHPULL(PA01) | \
					 PIN_OTYPE_PUSHPULL(PA02_UART2TX) | \
					 PIN_OTYPE_PUSHPULL(PA03_UART2RX) | \
					 PIN_OTYPE_PUSHPULL(PA04_MEAS_VBAT) | \
					 PIN_OTYPE_PUSHPULL(PA05) | \
					 PIN_OTYPE_PUSHPULL(PA06) | \
					 PIN_OTYPE_PUSHPULL(PA07) | \
					 PIN_OTYPE_PUSHPULL(PA08) | \
					 PIN_OTYPE_PUSHPULL(PA09) | \
					 PIN_OTYPE_PUSHPULL(PA10) | \
					 PIN_OTYPE_PUSHPULL(PA11_USB_DM) | \
					 PIN_OTYPE_PUSHPULL(PA12_USB_DP) | \
					 PIN_OTYPE_PUSHPULL(PA13_SWDIO) | \
					 PIN_OTYPE_PUSHPULL(PA14_SWCLK) | \
					 PIN_OTYPE_PUSHPULL(PA15))

#define VAL_GPIOA_OSPEEDR               (PIN_OSPEED_SPEED_VERYLOW(PA00) | \
					 PIN_OSPEED_SPEED_VERYLOW(PA01) | \
					 PIN_OSPEED_SPEED_HIGH(PA02_UART2TX) | \
					 PIN_OSPEED_SPEED_HIGH(PA03_UART2RX) | \
					 PIN_OSPEED_SPEED_VERYLOW(PA04_MEAS_VBAT) | \
					 PIN_OSPEED_SPEED_VERYLOW(PA05) | \
					 PIN_OSPEED_SPEED_VERYLOW(PA06) | \
					 PIN_OSPEED_SPEED_VERYLOW(PA07) | \
					 PIN_OSPEED_SPEED_VERYLOW(PA08) | \
					 PIN_OSPEED_SPEED_VERYLOW(PA09) | \
					 PIN_OSPEED_SPEED_VERYLOW(PA10) | \
					 PIN_OSPEED_SPEED_HIGH(PA11_USB_DM) | \
					 PIN_OSPEED_SPEED_HIGH(PA12_USB_DP) | \
					 PIN_OSPEED_SPEED_HIGH(PA13_SWDIO) | \
					 PIN_OSPEED_SPEED_HIGH(PA14_SWCLK) | \
					 PIN_OSPEED_SPEED_VERYLOW(PA15))

#define VAL_GPIOA_PUPDR                 (PIN_PUPDR_PULLDOWN(PA00) | \
					 PIN_PUPDR_PULLDOWN(PA01) | \
					 PIN_PUPDR_FLOATING(PA02_UART2TX) | \
					 PIN_PUPDR_FLOATING(PA03_UART2RX) | \
					 PIN_PUPDR_FLOATING(PA04_MEAS_VBAT) | \
					 PIN_PUPDR_PULLDOWN(PA05) | \
					 PIN_PUPDR_PULLDOWN(PA06) | \
					 PIN_PUPDR_PULLDOWN(PA07) | \
					 PIN_PUPDR_PULLDOWN(PA08) | \
					 PIN_PUPDR_PULLDOWN(PA09) | \
					 PIN_PUPDR_PULLDOWN(PA10) | \
					 PIN_PUPDR_FLOATING(PA11_USB_DM) | \
					 PIN_PUPDR_FLOATING(PA12_USB_DP) | \
					 PIN_PUPDR_PULLUP(PA13_SWDIO) | \
					 PIN_PUPDR_PULLUP(PA14_SWCLK) | \
					 PIN_PUPDR_PULLDOWN(PA15))

#define VAL_GPIOA_ODR                   (PIN_ODR_LEVEL_LOW(PA00) | \
					 PIN_ODR_LEVEL_LOW(PA01) | \
					 PIN_ODR_LEVEL_HIGH(PA02_UART2TX) | \
					 PIN_ODR_LEVEL_HIGH(PA03_UART2RX) | \
					 PIN_ODR_LEVEL_LOW(PA04_MEAS_VBAT) | \
					 PIN_ODR_LEVEL_LOW(PA05) | \
					 PIN_ODR_LEVEL_LOW(PA06) | \
					 PIN_ODR_LEVEL_LOW(PA07) | \
					 PIN_ODR_LEVEL_LOW(PA08) | \
					 PIN_ODR_LEVEL_LOW(PA09) | \
					 PIN_ODR_LEVEL_LOW(PA10) | \
					 PIN_ODR_LEVEL_HIGH(PA11_USB_DM) | \
					 PIN_ODR_LEVEL_HIGH(PA12_USB_DP) | \
					 PIN_ODR_LEVEL_HIGH(PA13_SWDIO) | \
					 PIN_ODR_LEVEL_HIGH(PA14_SWCLK) | \
					 PIN_ODR_LEVEL_LOW(PA15))

#define VAL_GPIOA_AFRL			(PIN_AFIO_AF(PA00, 0) | \
					 PIN_AFIO_AF(PA01, 0) | \
					 PIN_AFIO_AF(PA02_UART2TX, 7) | \
					 PIN_AFIO_AF(PA03_UART2RX, 7) | \
					 PIN_AFIO_AF(PA04_MEAS_VBAT, 0) | \
					 PIN_AFIO_AF(PA05, 0) | \
					 PIN_AFIO_AF(PA06, 0) | \
					 PIN_AFIO_AF(PA07, 0))

#define VAL_GPIOA_AFRH			(PIN_AFIO_AF(PA08, 0) | \
					 PIN_AFIO_AF(PA09, 0) | \
					 PIN_AFIO_AF(PA10, 0) | \
					 PIN_AFIO_AF(PA11_USB_DM, 0) | \
					 PIN_AFIO_AF(PA12_USB_DP, 0) | \
					 PIN_AFIO_AF(PA13_SWDIO, 0) | \
					 PIN_AFIO_AF(PA14_SWCLK, 0) | \
					 PIN_AFIO_AF(PA15, 0))

#define VAL_GPIOB_MODER                 (PIN_MODE_INPUT(PB00) | \
					 PIN_MODE_INPUT(PB01) | \
					 PIN_MODE_INPUT(PB02) | \
					 PIN_MODE_ALTERNATE(PB03_SPI1_CSK) | \
					 PIN_MODE_ALTERNATE(PB04_SPI1_MISO) | \
					 PIN_MODE_ALTERNATE(PB05_SPI1_MOSI) | \
					 PIN_MODE_OUTPUT(PB06_SPI1_CS) | \
					 PIN_MODE_INPUT(PB07) | \
					 PIN_MODE_INPUT(PB08) | \
					 PIN_MODE_INPUT(PB09) | \
					 PIN_MODE_INPUT(PB10) | \
					 PIN_MODE_INPUT(PB11) | \
					 PIN_MODE_ALTERNATE(PB12_CAN_RX) | \
					 PIN_MODE_ALTERNATE(PB13_CAN_TX) | \
					 PIN_MODE_INPUT(PB14) | \
					 PIN_MODE_INPUT(PB15))

#define VAL_GPIOB_OTYPER                (PIN_OTYPE_PUSHPULL(PB00) | \
					 PIN_OTYPE_PUSHPULL(PB01) | \
					 PIN_OTYPE_PUSHPULL(PB02) | \
					 PIN_OTYPE_PUSHPULL(PB03_SPI1_CSK) | \
					 PIN_OTYPE_PUSHPULL(PB04_SPI1_MISO) | \
					 PIN_OTYPE_PUSHPULL(PB05_SPI1_MOSI) | \
					 PIN_OTYPE_PUSHPULL(PB06_SPI1_CS) | \
					 PIN_OTYPE_PUSHPULL(PB07) | \
					 PIN_OTYPE_PUSHPULL(PB08) | \
					 PIN_OTYPE_PUSHPULL(PB09) | \
					 PIN_OTYPE_PUSHPULL(PB10) | \
					 PIN_OTYPE_PUSHPULL(PB11) | \
					 PIN_OTYPE_PUSHPULL(PB12_CAN_RX) | \
					 PIN_OTYPE_PUSHPULL(PB13_CAN_TX) | \
					 PIN_OTYPE_PUSHPULL(PB14) | \
					 PIN_OTYPE_PUSHPULL(PB15))

#define VAL_GPIOB_OSPEEDR               (PIN_OSPEED_SPEED_VERYLOW(PB00) | \
					 PIN_OSPEED_SPEED_VERYLOW(PB01) | \
					 PIN_OSPEED_SPEED_VERYLOW(PB02) | \
					 PIN_OSPEED_SPEED_HIGH(PB03_SPI1_CSK) | \
					 PIN_OSPEED_SPEED_HIGH(PB04_SPI1_MISO) | \
					 PIN_OSPEED_SPEED_HIGH(PB05_SPI1_MOSI) | \
					 PIN_OSPEED_SPEED_VERYLOW(PB06_SPI1_CS) | \
					 PIN_OSPEED_SPEED_VERYLOW(PB07) | \
					 PIN_OSPEED_SPEED_VERYLOW(PB08) | \
					 PIN_OSPEED_SPEED_VERYLOW(PB09) | \
					 PIN_OSPEED_SPEED_VERYLOW(PB10) | \
					 PIN_OSPEED_SPEED_VERYLOW(PB11) | \
					 PIN_OSPEED_SPEED_HIGH(PB12_CAN_RX) | \
					 PIN_OSPEED_SPEED_HIGH(PB13_CAN_TX) | \
					 PIN_OSPEED_SPEED_VERYLOW(PB14) | \
					 PIN_OSPEED_SPEED_VERYLOW(PB15))

#define VAL_GPIOB_PUPDR                 (PIN_PUPDR_PULLDOWN(PB00) | \
					 PIN_PUPDR_PULLDOWN(PB01) | \
					 PIN_PUPDR_PULLDOWN(PB02) | \
					 PIN_PUPDR_FLOATING(PB03_SPI1_CSK) | \
					 PIN_PUPDR_FLOATING(PB04_SPI1_MISO) | \
					 PIN_PUPDR_FLOATING(PB05_SPI1_MOSI) | \
					 PIN_PUPDR_FLOATING(PB06_SPI1_CS) | \
					 PIN_PUPDR_PULLDOWN(PB07) | \
					 PIN_PUPDR_PULLDOWN(PB08) | \
					 PIN_PUPDR_PULLDOWN(PB09) | \
					 PIN_PUPDR_PULLDOWN(PB10) | \
					 PIN_PUPDR_PULLDOWN(PB11) | \
					 PIN_PUPDR_FLOATING(PB12_CAN_RX) | \
					 PIN_PUPDR_FLOATING(PB13_CAN_TX) | \
					 PIN_PUPDR_PULLDOWN(PB14) | \
					 PIN_PUPDR_PULLDOWN(PB15))

#define VAL_GPIOB_ODR                   (PIN_ODR_LEVEL_LOW(PB00) | \
					 PIN_ODR_LEVEL_LOW(PB01) | \
					 PIN_ODR_LEVEL_LOW(PB02) | \
					 PIN_ODR_LEVEL_HIGH(PB03_SPI1_CSK) | \
					 PIN_ODR_LEVEL_HIGH(PB04_SPI1_MISO) | \
					 PIN_ODR_LEVEL_HIGH(PB05_SPI1_MOSI) | \
					 PIN_ODR_LEVEL_LOW(PB06_SPI1_CS) | \
					 PIN_ODR_LEVEL_LOW(PB07) | \
					 PIN_ODR_LEVEL_LOW(PB08) | \
					 PIN_ODR_LEVEL_LOW(PB09) | \
					 PIN_ODR_LEVEL_LOW(PB10) | \
					 PIN_ODR_LEVEL_LOW(PB11) | \
					 PIN_ODR_LEVEL_HIGH(PB12_CAN_RX) | \
					 PIN_ODR_LEVEL_HIGH(PB13_CAN_TX) | \
					 PIN_ODR_LEVEL_LOW(PB14) | \
					 PIN_ODR_LEVEL_LOW(PB15))

#define VAL_GPIOB_AFRL			(PIN_AFIO_AF(PB00, 0) | \
					 PIN_AFIO_AF(PB01, 0) | \
					 PIN_AFIO_AF(PB02, 0) | \
					 PIN_AFIO_AF(PB03_SPI1_CSK, 5) | \
					 PIN_AFIO_AF(PB04_SPI1_MISO, 5) | \
					 PIN_AFIO_AF(PB05_SPI1_MOSI, 5) | \
					 PIN_AFIO_AF(PB06_SPI1_CS, 0) | \
					 PIN_AFIO_AF(PB07, 0))

#define VAL_GPIOB_AFRH			(PIN_AFIO_AF(PB08, 0) | \
					 PIN_AFIO_AF(PB09, 0) | \
					 PIN_AFIO_AF(PB10, 0) | \
					 PIN_AFIO_AF(PB11, 0) | \
					 PIN_AFIO_AF(PB12_CAN_RX, 9) | \
					 PIN_AFIO_AF(PB13_CAN_TX, 9) | \
					 PIN_AFIO_AF(PB14, 0) | \
					 PIN_AFIO_AF(PB15, 0))

#define VAL_GPIOC_MODER                 (PIN_MODE_INPUT(PC00) | \
					 PIN_MODE_OUTPUT(PC01_LED1) | \
					 PIN_MODE_OUTPUT(PC02_LED2) | \
					 PIN_MODE_INPUT(PC03) | \
					 PIN_MODE_INPUT(PC04) | \
					 PIN_MODE_INPUT(PC05) | \
					 PIN_MODE_INPUT(PC06) | \
					 PIN_MODE_INPUT(PC07) | \
					 PIN_MODE_INPUT(PC08) | \
					 PIN_MODE_INPUT(PC09) | \
					 PIN_MODE_INPUT(PC10) | \
					 PIN_MODE_INPUT(PC11) | \
					 PIN_MODE_INPUT(PC12) | \
					 PIN_MODE_INPUT(PC13) | \
					 PIN_MODE_INPUT(PC14) | \
					 PIN_MODE_INPUT(PC15))

#define VAL_GPIOC_OTYPER                (PIN_OTYPE_PUSHPULL(PC00) | \
					 PIN_OTYPE_PUSHPULL(PC01_LED1) | \
					 PIN_OTYPE_PUSHPULL(PC02_LED2) | \
					 PIN_OTYPE_PUSHPULL(PC03) | \
					 PIN_OTYPE_PUSHPULL(PC04) | \
					 PIN_OTYPE_PUSHPULL(PC05) | \
					 PIN_OTYPE_PUSHPULL(PC06) | \
					 PIN_OTYPE_PUSHPULL(PC07) | \
					 PIN_OTYPE_PUSHPULL(PC08) | \
					 PIN_OTYPE_PUSHPULL(PC09) | \
					 PIN_OTYPE_PUSHPULL(PC10) | \
					 PIN_OTYPE_PUSHPULL(PC11) | \
					 PIN_OTYPE_PUSHPULL(PC12) | \
					 PIN_OTYPE_PUSHPULL(PC13) | \
					 PIN_OTYPE_PUSHPULL(PC14) | \
					 PIN_OTYPE_PUSHPULL(PC15))

#define VAL_GPIOC_OSPEEDR               (PIN_OSPEED_SPEED_VERYLOW(PC00) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC01_LED1) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC02_LED2) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC03) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC04) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC05) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC06) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC07) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC08) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC09) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC10) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC11) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC12) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC13) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC14) | \
					 PIN_OSPEED_SPEED_VERYLOW(PC15))

#define VAL_GPIOC_PUPDR                 (PIN_PUPDR_PULLDOWN(PC00) | \
					 PIN_PUPDR_FLOATING(PC01_LED1) | \
					 PIN_PUPDR_FLOATING(PC02_LED2) | \
					 PIN_PUPDR_PULLDOWN(PC03) | \
					 PIN_PUPDR_PULLDOWN(PC04) | \
					 PIN_PUPDR_PULLDOWN(PC05) | \
					 PIN_PUPDR_PULLDOWN(PC06) | \
					 PIN_PUPDR_PULLDOWN(PC07) | \
					 PIN_PUPDR_PULLDOWN(PC08) | \
					 PIN_PUPDR_PULLDOWN(PC09) | \
					 PIN_PUPDR_PULLDOWN(PC10) | \
					 PIN_PUPDR_PULLDOWN(PC11) | \
					 PIN_PUPDR_PULLDOWN(PC12) | \
					 PIN_PUPDR_PULLDOWN(PC13) | \
					 PIN_PUPDR_PULLDOWN(PC14) | \
					 PIN_PUPDR_PULLDOWN(PC15))

#define VAL_GPIOC_ODR                   (PIN_ODR_LEVEL_LOW(PC00) | \
					 PIN_ODR_LEVEL_LOW(PC01_LED1) | \
					 PIN_ODR_LEVEL_LOW(PC02_LED2) | \
					 PIN_ODR_LEVEL_LOW(PC03) | \
					 PIN_ODR_LEVEL_LOW(PC04) | \
					 PIN_ODR_LEVEL_LOW(PC05) | \
					 PIN_ODR_LEVEL_LOW(PC06) | \
					 PIN_ODR_LEVEL_LOW(PC07) | \
					 PIN_ODR_LEVEL_LOW(PC08) | \
					 PIN_ODR_LEVEL_LOW(PC09) | \
					 PIN_ODR_LEVEL_LOW(PC10) | \
					 PIN_ODR_LEVEL_LOW(PC11) | \
					 PIN_ODR_LEVEL_LOW(PC12) | \
					 PIN_ODR_LEVEL_LOW(PC13) | \
					 PIN_ODR_LEVEL_LOW(PC14) | \
					 PIN_ODR_LEVEL_LOW(PC15))

#define VAL_GPIOC_AFRL			(PIN_AFIO_AF(PC00, 0) | \
					 PIN_AFIO_AF(PC01_LED1, 0) | \
					 PIN_AFIO_AF(PC02_LED2, 0) | \
					 PIN_AFIO_AF(PC03, 0) | \
					 PIN_AFIO_AF(PC04, 0) | \
					 PIN_AFIO_AF(PC05, 0) | \
					 PIN_AFIO_AF(PC06, 0) | \
					 PIN_AFIO_AF(PC07, 0))

#define VAL_GPIOC_AFRH			(PIN_AFIO_AF(PC08, 0) | \
					 PIN_AFIO_AF(PC09, 0) | \
					 PIN_AFIO_AF(PC10, 0) | \
					 PIN_AFIO_AF(PC11, 0) | \
					 PIN_AFIO_AF(PC12, 0) | \
					 PIN_AFIO_AF(PC13, 0) | \
					 PIN_AFIO_AF(PC14, 0) | \
					 PIN_AFIO_AF(PC15, 0))

#define VAL_GPIOD_MODER                 (PIN_MODE_INPUT(PD00) | \
					 PIN_MODE_INPUT(PD01) | \
					 PIN_MODE_INPUT(PD02_IMU_INT1) | \
					 PIN_MODE_INPUT(PD03) | \
					 PIN_MODE_INPUT(PD04) | \
					 PIN_MODE_INPUT(PD05) | \
					 PIN_MODE_INPUT(PD06) | \
					 PIN_MODE_INPUT(PD07) | \
					 PIN_MODE_INPUT(PD08) | \
					 PIN_MODE_INPUT(PD09) | \
					 PIN_MODE_INPUT(PD10) | \
					 PIN_MODE_INPUT(PD11) | \
					 PIN_MODE_INPUT(PD12) | \
					 PIN_MODE_INPUT(PD13) | \
					 PIN_MODE_INPUT(PD14) | \
					 PIN_MODE_INPUT(PD15))

#define VAL_GPIOD_OTYPER                (PIN_OTYPE_PUSHPULL(PD00) | \
					 PIN_OTYPE_PUSHPULL(PD01) | \
					 PIN_OTYPE_OPENDRAIN(PD02_IMU_INT1) | \
					 PIN_OTYPE_PUSHPULL(PD03) | \
					 PIN_OTYPE_PUSHPULL(PD04) | \
					 PIN_OTYPE_PUSHPULL(PD05) | \
					 PIN_OTYPE_PUSHPULL(PD06) | \
					 PIN_OTYPE_PUSHPULL(PD07) | \
					 PIN_OTYPE_PUSHPULL(PD08) | \
					 PIN_OTYPE_PUSHPULL(PD09) | \
					 PIN_OTYPE_PUSHPULL(PD10) | \
					 PIN_OTYPE_PUSHPULL(PD11) | \
					 PIN_OTYPE_PUSHPULL(PD12) | \
					 PIN_OTYPE_PUSHPULL(PD13) | \
					 PIN_OTYPE_PUSHPULL(PD14) | \
					 PIN_OTYPE_PUSHPULL(PD15))

#define VAL_GPIOD_OSPEEDR               (PIN_OSPEED_SPEED_VERYLOW(PD00) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD01) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD02_IMU_INT1) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD03) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD04) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD05) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD06) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD07) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD08) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD09) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD10) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD11) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD12) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD13) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD14) | \
					 PIN_OSPEED_SPEED_VERYLOW(PD15))

#define VAL_GPIOD_PUPDR                 (PIN_PUPDR_PULLDOWN(PD00) | \
					 PIN_PUPDR_PULLDOWN(PD01) | \
					 PIN_PUPDR_FLOATING(PD02_IMU_INT1) | \
					 PIN_PUPDR_PULLDOWN(PD03) | \
					 PIN_PUPDR_PULLDOWN(PD04) | \
					 PIN_PUPDR_PULLDOWN(PD05) | \
					 PIN_PUPDR_PULLDOWN(PD06) | \
					 PIN_PUPDR_PULLDOWN(PD07) | \
					 PIN_PUPDR_PULLDOWN(PD08) | \
					 PIN_PUPDR_PULLDOWN(PD09) | \
					 PIN_PUPDR_PULLDOWN(PD10) | \
					 PIN_PUPDR_PULLDOWN(PD11) | \
					 PIN_PUPDR_PULLDOWN(PD12) | \
					 PIN_PUPDR_PULLDOWN(PD13) | \
					 PIN_PUPDR_PULLDOWN(PD14) | \
					 PIN_PUPDR_PULLDOWN(PD15))

#define VAL_GPIOD_ODR                   (PIN_ODR_LEVEL_LOW(PD00) | \
					 PIN_ODR_LEVEL_LOW(PD01) | \
					 PIN_ODR_LEVEL_LOW(PD02_IMU_INT1) | \
					 PIN_ODR_LEVEL_LOW(PD03) | \
					 PIN_ODR_LEVEL_LOW(PD04) | \
					 PIN_ODR_LEVEL_LOW(PD05) | \
					 PIN_ODR_LEVEL_LOW(PD06) | \
					 PIN_ODR_LEVEL_LOW(PD07) | \
					 PIN_ODR_LEVEL_LOW(PD08) | \
					 PIN_ODR_LEVEL_LOW(PD09) | \
					 PIN_ODR_LEVEL_LOW(PD10) | \
					 PIN_ODR_LEVEL_LOW(PD11) | \
					 PIN_ODR_LEVEL_LOW(PD12) | \
					 PIN_ODR_LEVEL_LOW(PD13) | \
					 PIN_ODR_LEVEL_LOW(PD14) | \
					 PIN_ODR_LEVEL_LOW(PD15))

#define VAL_GPIOD_AFRL			(PIN_AFIO_AF(PD00, 0) | \
					 PIN_AFIO_AF(PD01, 0) | \
					 PIN_AFIO_AF(PD02_IMU_INT1, 0) | \
					 PIN_AFIO_AF(PD03, 0) | \
					 PIN_AFIO_AF(PD04, 0) | \
					 PIN_AFIO_AF(PD05, 0) | \
					 PIN_AFIO_AF(PD06, 0) | \
					 PIN_AFIO_AF(PD07, 0))

#define VAL_GPIOD_AFRH			(PIN_AFIO_AF(PD08, 0) | \
					 PIN_AFIO_AF(PD09, 0) | \
					 PIN_AFIO_AF(PD10, 0) | \
					 PIN_AFIO_AF(PD11, 0) | \
					 PIN_AFIO_AF(PD12, 0) | \
					 PIN_AFIO_AF(PD13, 0) | \
					 PIN_AFIO_AF(PD14, 0) | \
					 PIN_AFIO_AF(PD15, 0))

#define VAL_GPIOE_MODER                 (PIN_MODE_INPUT(PE00) | \
					 PIN_MODE_INPUT(PE01) | \
					 PIN_MODE_INPUT(PE02) | \
					 PIN_MODE_INPUT(PE03) | \
					 PIN_MODE_INPUT(PE04) | \
					 PIN_MODE_INPUT(PE05) | \
					 PIN_MODE_INPUT(PE06) | \
					 PIN_MODE_INPUT(PE07) | \
					 PIN_MODE_INPUT(PE08) | \
					 PIN_MODE_INPUT(PE09) | \
					 PIN_MODE_INPUT(PE10) | \
					 PIN_MODE_INPUT(PE11) | \
					 PIN_MODE_INPUT(PE12) | \
					 PIN_MODE_INPUT(PE13) | \
					 PIN_MODE_INPUT(PE14) | \
					 PIN_MODE_INPUT(PE15))

#define VAL_GPIOE_OTYPER                (PIN_OTYPE_PUSHPULL(PE00) | \
					 PIN_OTYPE_PUSHPULL(PE01) | \
					 PIN_OTYPE_PUSHPULL(PE02) | \
					 PIN_OTYPE_PUSHPULL(PE03) | \
					 PIN_OTYPE_PUSHPULL(PE04) | \
					 PIN_OTYPE_PUSHPULL(PE05) | \
					 PIN_OTYPE_PUSHPULL(PE06) | \
					 PIN_OTYPE_PUSHPULL(PE07) | \
					 PIN_OTYPE_PUSHPULL(PE08) | \
					 PIN_OTYPE_PUSHPULL(PE09) | \
					 PIN_OTYPE_PUSHPULL(PE10) | \
					 PIN_OTYPE_PUSHPULL(PE11) | \
					 PIN_OTYPE_PUSHPULL(PE12) | \
					 PIN_OTYPE_PUSHPULL(PE13) | \
					 PIN_OTYPE_PUSHPULL(PE14) | \
					 PIN_OTYPE_PUSHPULL(PE15))

#define VAL_GPIOE_OSPEEDR               (PIN_OSPEED_SPEED_VERYLOW(PE00) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE01) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE02) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE03) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE04) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE05) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE06) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE07) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE08) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE09) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE10) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE11) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE12) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE13) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE14) | \
					 PIN_OSPEED_SPEED_VERYLOW(PE15))

#define VAL_GPIOE_PUPDR                 (PIN_PUPDR_PULLDOWN(PE00) | \
					 PIN_PUPDR_PULLDOWN(PE01) | \
					 PIN_PUPDR_PULLDOWN(PE02) | \
					 PIN_PUPDR_PULLDOWN(PE03) | \
					 PIN_PUPDR_PULLDOWN(PE04) | \
					 PIN_PUPDR_PULLDOWN(PE05) | \
					 PIN_PUPDR_PULLDOWN(PE06) | \
					 PIN_PUPDR_PULLDOWN(PE07) | \
					 PIN_PUPDR_PULLDOWN(PE08) | \
					 PIN_PUPDR_PULLDOWN(PE09) | \
					 PIN_PUPDR_PULLDOWN(PE10) | \
					 PIN_PUPDR_PULLDOWN(PE11) | \
					 PIN_PUPDR_PULLDOWN(PE12) | \
					 PIN_PUPDR_PULLDOWN(PE13) | \
					 PIN_PUPDR_PULLDOWN(PE14) | \
					 PIN_PUPDR_PULLDOWN(PE15))

#define VAL_GPIOE_ODR                   (PIN_ODR_LEVEL_LOW(PE00) | \
					 PIN_ODR_LEVEL_LOW(PE01) | \
					 PIN_ODR_LEVEL_LOW(PE02) | \
					 PIN_ODR_LEVEL_LOW(PE03) | \
					 PIN_ODR_LEVEL_LOW(PE04) | \
					 PIN_ODR_LEVEL_LOW(PE05) | \
					 PIN_ODR_LEVEL_LOW(PE06) | \
					 PIN_ODR_LEVEL_LOW(PE07) | \
					 PIN_ODR_LEVEL_LOW(PE08) | \
					 PIN_ODR_LEVEL_LOW(PE09) | \
					 PIN_ODR_LEVEL_LOW(PE10) | \
					 PIN_ODR_LEVEL_LOW(PE11) | \
					 PIN_ODR_LEVEL_LOW(PE12) | \
					 PIN_ODR_LEVEL_LOW(PE13) | \
					 PIN_ODR_LEVEL_LOW(PE14) | \
					 PIN_ODR_LEVEL_LOW(PE15))

#define VAL_GPIOE_AFRL			(PIN_AFIO_AF(PE00, 0) | \
					 PIN_AFIO_AF(PE01, 0) | \
					 PIN_AFIO_AF(PE02, 0) | \
					 PIN_AFIO_AF(PE03, 0) | \
					 PIN_AFIO_AF(PE04, 0) | \
					 PIN_AFIO_AF(PE05, 0) | \
					 PIN_AFIO_AF(PE06, 0) | \
					 PIN_AFIO_AF(PE07, 0))

#define VAL_GPIOE_AFRH			(PIN_AFIO_AF(PE08, 0) | \
					 PIN_AFIO_AF(PE09, 0) | \
					 PIN_AFIO_AF(PE10, 0) | \
					 PIN_AFIO_AF(PE11, 0) | \
					 PIN_AFIO_AF(PE12, 0) | \
					 PIN_AFIO_AF(PE13, 0) | \
					 PIN_AFIO_AF(PE14, 0) | \
					 PIN_AFIO_AF(PE15, 0))

#define VAL_GPIOF_MODER                 (PIN_MODE_ALTERNATE(PF00_OSC_IN) | \
					 PIN_MODE_INPUT(PF01) | \
					 PIN_MODE_INPUT(PF02) | \
					 PIN_MODE_INPUT(PF03) | \
					 PIN_MODE_INPUT(PF04) | \
					 PIN_MODE_INPUT(PF05) | \
					 PIN_MODE_INPUT(PF06) | \
					 PIN_MODE_INPUT(PF07) | \
					 PIN_MODE_INPUT(PF08) | \
					 PIN_MODE_INPUT(PF09) | \
					 PIN_MODE_INPUT(PF10) | \
					 PIN_MODE_INPUT(PF11) | \
					 PIN_MODE_INPUT(PF12) | \
					 PIN_MODE_INPUT(PF13) | \
					 PIN_MODE_INPUT(PF14) | \
					 PIN_MODE_INPUT(PF15))

#define VAL_GPIOF_OTYPER                (PIN_OTYPE_PUSHPULL(PF00_OSC_IN) | \
					 PIN_OTYPE_PUSHPULL(PF01) | \
					 PIN_OTYPE_PUSHPULL(PF02) | \
					 PIN_OTYPE_PUSHPULL(PF03) | \
					 PIN_OTYPE_PUSHPULL(PF04) | \
					 PIN_OTYPE_PUSHPULL(PF05) | \
					 PIN_OTYPE_PUSHPULL(PF06) | \
					 PIN_OTYPE_PUSHPULL(PF07) | \
					 PIN_OTYPE_PUSHPULL(PF08) | \
					 PIN_OTYPE_PUSHPULL(PF09) | \
					 PIN_OTYPE_PUSHPULL(PF10) | \
					 PIN_OTYPE_PUSHPULL(PF11) | \
					 PIN_OTYPE_PUSHPULL(PF12) | \
					 PIN_OTYPE_PUSHPULL(PF13) | \
					 PIN_OTYPE_PUSHPULL(PF14) | \
					 PIN_OTYPE_PUSHPULL(PF15))

#define VAL_GPIOF_OSPEEDR               (PIN_OSPEED_SPEED_HIGH(PF00_OSC_IN) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF01) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF02) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF03) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF04) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF05) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF06) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF07) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF08) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF09) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF10) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF11) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF12) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF13) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF14) | \
					 PIN_OSPEED_SPEED_VERYLOW(PF15))

#define VAL_GPIOF_PUPDR                 (PIN_PUPDR_FLOATING(PF00_OSC_IN) | \
					 PIN_PUPDR_PULLDOWN(PF01) | \
					 PIN_PUPDR_PULLDOWN(PF02) | \
					 PIN_PUPDR_PULLDOWN(PF03) | \
					 PIN_PUPDR_PULLDOWN(PF04) | \
					 PIN_PUPDR_PULLDOWN(PF05) | \
					 PIN_PUPDR_PULLDOWN(PF06) | \
					 PIN_PUPDR_PULLDOWN(PF07) | \
					 PIN_PUPDR_PULLDOWN(PF08) | \
					 PIN_PUPDR_PULLDOWN(PF09) | \
					 PIN_PUPDR_PULLDOWN(PF10) | \
					 PIN_PUPDR_PULLDOWN(PF11) | \
					 PIN_PUPDR_PULLDOWN(PF12) | \
					 PIN_PUPDR_PULLDOWN(PF13) | \
					 PIN_PUPDR_PULLDOWN(PF14) | \
					 PIN_PUPDR_PULLDOWN(PF15))

#define VAL_GPIOF_ODR                   (PIN_ODR_LEVEL_HIGH(PF00_OSC_IN) | \
					 PIN_ODR_LEVEL_LOW(PF01) | \
					 PIN_ODR_LEVEL_LOW(PF02) | \
					 PIN_ODR_LEVEL_LOW(PF03) | \
					 PIN_ODR_LEVEL_LOW(PF04) | \
					 PIN_ODR_LEVEL_LOW(PF05) | \
					 PIN_ODR_LEVEL_LOW(PF06) | \
					 PIN_ODR_LEVEL_LOW(PF07) | \
					 PIN_ODR_LEVEL_LOW(PF08) | \
					 PIN_ODR_LEVEL_LOW(PF09) | \
					 PIN_ODR_LEVEL_LOW(PF10) | \
					 PIN_ODR_LEVEL_LOW(PF11) | \
					 PIN_ODR_LEVEL_LOW(PF12) | \
					 PIN_ODR_LEVEL_LOW(PF13) | \
					 PIN_ODR_LEVEL_LOW(PF14) | \
					 PIN_ODR_LEVEL_LOW(PF15))

#define VAL_GPIOF_AFRL			(PIN_AFIO_AF(PF00_OSC_IN, 0) | \
					 PIN_AFIO_AF(PF01, 0) | \
					 PIN_AFIO_AF(PF02, 0) | \
					 PIN_AFIO_AF(PF03, 0) | \
					 PIN_AFIO_AF(PF04, 0) | \
					 PIN_AFIO_AF(PF05, 0) | \
					 PIN_AFIO_AF(PF06, 0) | \
					 PIN_AFIO_AF(PF07, 0))

#define VAL_GPIOF_AFRH			(PIN_AFIO_AF(PF08, 0) | \
					 PIN_AFIO_AF(PF09, 0) | \
					 PIN_AFIO_AF(PF10, 0) | \
					 PIN_AFIO_AF(PF11, 0) | \
					 PIN_AFIO_AF(PF12, 0) | \
					 PIN_AFIO_AF(PF13, 0) | \
					 PIN_AFIO_AF(PF14, 0) | \
					 PIN_AFIO_AF(PF15, 0))

#define VAL_GPIOG_MODER                 (PIN_MODE_INPUT(PG00) | \
					 PIN_MODE_INPUT(PG01) | \
					 PIN_MODE_INPUT(PG02) | \
					 PIN_MODE_INPUT(PG03) | \
					 PIN_MODE_INPUT(PG04) | \
					 PIN_MODE_INPUT(PG05) | \
					 PIN_MODE_INPUT(PG06) | \
					 PIN_MODE_INPUT(PG07) | \
					 PIN_MODE_INPUT(PG08) | \
					 PIN_MODE_INPUT(PG09) | \
					 PIN_MODE_INPUT(PG10) | \
					 PIN_MODE_INPUT(PG11) | \
					 PIN_MODE_INPUT(PG12) | \
					 PIN_MODE_INPUT(PG13) | \
					 PIN_MODE_INPUT(PG14) | \
					 PIN_MODE_INPUT(PG15))

#define VAL_GPIOG_OTYPER                (PIN_OTYPE_PUSHPULL(PG00) | \
					 PIN_OTYPE_PUSHPULL(PG01) | \
					 PIN_OTYPE_PUSHPULL(PG02) | \
					 PIN_OTYPE_PUSHPULL(PG03) | \
					 PIN_OTYPE_PUSHPULL(PG04) | \
					 PIN_OTYPE_PUSHPULL(PG05) | \
					 PIN_OTYPE_PUSHPULL(PG06) | \
					 PIN_OTYPE_PUSHPULL(PG07) | \
					 PIN_OTYPE_PUSHPULL(PG08) | \
					 PIN_OTYPE_PUSHPULL(PG09) | \
					 PIN_OTYPE_PUSHPULL(PG10) | \
					 PIN_OTYPE_PUSHPULL(PG11) | \
					 PIN_OTYPE_PUSHPULL(PG12) | \
					 PIN_OTYPE_PUSHPULL(PG13) | \
					 PIN_OTYPE_PUSHPULL(PG14) | \
					 PIN_OTYPE_PUSHPULL(PG15))

#define VAL_GPIOG_OSPEEDR               (PIN_OSPEED_SPEED_VERYLOW(PG00) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG01) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG02) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG03) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG04) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG05) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG06) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG07) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG08) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG09) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG10) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG11) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG12) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG13) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG14) | \
					 PIN_OSPEED_SPEED_VERYLOW(PG15))

#define VAL_GPIOG_PUPDR                 (PIN_PUPDR_PULLDOWN(PG00) | \
					 PIN_PUPDR_PULLDOWN(PG01) | \
					 PIN_PUPDR_PULLDOWN(PG02) | \
					 PIN_PUPDR_PULLDOWN(PG03) | \
					 PIN_PUPDR_PULLDOWN(PG04) | \
					 PIN_PUPDR_PULLDOWN(PG05) | \
					 PIN_PUPDR_PULLDOWN(PG06) | \
					 PIN_PUPDR_PULLDOWN(PG07) | \
					 PIN_PUPDR_PULLDOWN(PG08) | \
					 PIN_PUPDR_PULLDOWN(PG09) | \
					 PIN_PUPDR_PULLDOWN(PG10) | \
					 PIN_PUPDR_PULLDOWN(PG11) | \
					 PIN_PUPDR_PULLDOWN(PG12) | \
					 PIN_PUPDR_PULLDOWN(PG13) | \
					 PIN_PUPDR_PULLDOWN(PG14) | \
					 PIN_PUPDR_PULLDOWN(PG15))

#define VAL_GPIOG_ODR                   (PIN_ODR_LEVEL_LOW(PG00) | \
					 PIN_ODR_LEVEL_LOW(PG01) | \
					 PIN_ODR_LEVEL_LOW(PG02) | \
					 PIN_ODR_LEVEL_LOW(PG03) | \
					 PIN_ODR_LEVEL_LOW(PG04) | \
					 PIN_ODR_LEVEL_LOW(PG05) | \
					 PIN_ODR_LEVEL_LOW(PG06) | \
					 PIN_ODR_LEVEL_LOW(PG07) | \
					 PIN_ODR_LEVEL_LOW(PG08) | \
					 PIN_ODR_LEVEL_LOW(PG09) | \
					 PIN_ODR_LEVEL_LOW(PG10) | \
					 PIN_ODR_LEVEL_LOW(PG11) | \
					 PIN_ODR_LEVEL_LOW(PG12) | \
					 PIN_ODR_LEVEL_LOW(PG13) | \
					 PIN_ODR_LEVEL_LOW(PG14) | \
					 PIN_ODR_LEVEL_LOW(PG15))

#define VAL_GPIOG_AFRL			(PIN_AFIO_AF(PG00, 0) | \
					 PIN_AFIO_AF(PG01, 0) | \
					 PIN_AFIO_AF(PG02, 0) | \
					 PIN_AFIO_AF(PG03, 0) | \
					 PIN_AFIO_AF(PG04, 0) | \
					 PIN_AFIO_AF(PG05, 0) | \
					 PIN_AFIO_AF(PG06, 0) | \
					 PIN_AFIO_AF(PG07, 0))

#define VAL_GPIOG_AFRH			(PIN_AFIO_AF(PG08, 0) | \
					 PIN_AFIO_AF(PG09, 0) | \
					 PIN_AFIO_AF(PG10, 0) | \
					 PIN_AFIO_AF(PG11, 0) | \
					 PIN_AFIO_AF(PG12, 0) | \
					 PIN_AFIO_AF(PG13, 0) | \
					 PIN_AFIO_AF(PG14, 0) | \
					 PIN_AFIO_AF(PG15, 0))

#define AF_PA02_UART2TX                  7U
#define AF_LINE_UART2TX                  7U
#define AF_PA03_UART2RX                  7U
#define AF_LINE_UART2RX                  7U
#define AF_PA11_USB_DM                   0U
#define AF_LINE_USB_DM                   0U
#define AF_PA12_USB_DP                   0U
#define AF_LINE_USB_DP                   0U
#define AF_PA13_SWDIO                    0U
#define AF_LINE_SWDIO                    0U
#define AF_PA14_SWCLK                    0U
#define AF_LINE_SWCLK                    0U
#define AF_PB03_SPI1_CSK                 5U
#define AF_LINE_SPI1_CSK                 5U
#define AF_PB04_SPI1_MISO                5U
#define AF_LINE_SPI1_MISO                5U
#define AF_PB05_SPI1_MOSI                5U
#define AF_LINE_SPI1_MOSI                5U
#define AF_PB12_CAN_RX                   9U
#define AF_LINE_CAN_RX                   9U
#define AF_PB13_CAN_TX                   9U
#define AF_LINE_CAN_TX                   9U
#define AF_PF00_OSC_IN                   0U
#define AF_LINE_OSC_IN                   0U


#define MEAS_VBAT_ADC	 2
#define MEAS_VBAT_ADC_FN	 IN
#define MEAS_VBAT_ADC_IN	 17

#define BOARD_GROUP_DECLFOREACH(line, group) \
  static const ioline_t group ## _ARRAY[] = {group}; \
  for (ioline_t i=0, line =  group ## _ARRAY[i]; (i < group ## _SIZE) && (line = group ## _ARRAY[i]); i++)

#define BOARD_GROUP_FOREACH(line, group) \
  for (ioline_t i=0, line =  group ## _ARRAY[i]; (i < group ## _SIZE) && (line = group ## _ARRAY[i]); i++)


#define BOARD_GROUP_DECLFOR(array, index, group)  \
  static const ioline_t group ## _ARRAY[] = {group};    \
  for (ioline_t index=0, *array =  (ioline_t *) group ## _ARRAY; index < group ## _SIZE; index++)

#define BOARD_GROUP_FOR(array, index, group)  \
  for (ioline_t index=0, *array =  (ioline_t *) group ## _ARRAY; index < group ## _SIZE; index++)

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

