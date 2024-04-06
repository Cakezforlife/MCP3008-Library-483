/*
* Class for using the PI's SPI interface 
* with the MCP3008 10-bit ADC
*
*
*
*
*
*
*/

#ifndef MCP3008_H
#define MCP3008_H

#include <cstdint>

#define BIT_RESOLUTION 10

// SPEEDS BASED ON INPUT VOLTAGE
// VDD >= 4V
#define SPI_CLOCK_GE4V 3600000
// VDD >= 3.3V
#define SPI_CLOCK_GE33V 2340000
// VDD >= 2.7V
#define SPI_CLOCK_GE27V 1350000

// temporary
#define SAMPLING_DELAY 0;

typedef void (*MCPCB_t)(uint32_t);

class MCP3008 {
  private:

  // SPI
  int32_t spi_handle;

  // VDD
  double input_voltage;
  // Vref
  double reference_voltage;  

  uint32_t sample_rate;

  public:

  // Constructor
  MCP3008(double input_voltage, double reference_voltage);

  ~MCP3008();

  uint32_t read_raw_from(uint8_t channel);
  double read_from(uint8_t channel);
};

#endif
