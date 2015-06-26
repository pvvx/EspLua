// Common code for all backends

#include "platform.h"
#include "common.h"
#include "c_string.h"
#include "c_stdio.h"

void cmn_platform_init(void)
{

}

// ****************************************************************************
// GPIO functions

int platform_gpio_exists( unsigned pin )
{
  return pin < NUM_GPIO;
}

// ****************************************************************************
// CAN functions

int platform_can_exists( unsigned id )
{
  return id < NUM_CAN;
}

// ****************************************************************************
// SPI functions


int platform_spi_exists( unsigned id )
{
  return id < NUM_SPI;
}

// ****************************************************************************
// PWM functions

int platform_pwm_exists( unsigned id )
{
  return ((id < NUM_PWM) && (id > 0));
}

// ****************************************************************************
// ADC functions

int platform_adc_exists( unsigned id )
{
  return id < NUM_ADC;
}

// ****************************************************************************
// UART functions

int platform_uart_exists( unsigned id )
{
  return id < NUM_UART;
}

// ****************************************************************************
// OneWire functions

int platform_ow_exists( unsigned id )
{
  return ((id < NUM_OW) && (id > 0));
}

// ****************************************************************************
// Timer functions

int platform_tmr_exists( unsigned id )
{
  return id < NUM_TMR;
}

// I2C support
int platform_i2c_exists( unsigned id )
{
#ifndef NUM_I2C
  return 0;
#else
  return id < NUM_I2C;
#endif
}

