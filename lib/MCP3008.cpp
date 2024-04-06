
#include "MCP3008.h"
#include <pigpio.h>
#include <stdio.h>
#include <stdexcept>

/**
 * Constructor for MCP3008 
 * Sets up GPIO and SPI interface
 * Also sets speed according to MCP datasheet
 * 
 * @brief MCP constructor
 * 
 * @param input_volatage: double, voltage given to VDD on the chip
 * 
 * @param channel: double, voltage given to VREF on the chip
 * 
 * Constructs a MCP object. Sets up GPIO and the SPI interface
*/
MCP3008::MCP3008(double input_voltage, double reference_voltage){
    
    this->input_voltage = input_voltage;
    this->reference_voltage = reference_voltage;

    // Set speed
    if(input_voltage >= 4.0) {
        sample_rate = SPI_CLOCK_GE4V;
    } else if(input_voltage >= 3.3) {
        sample_rate = SPI_CLOCK_GE33V;
    } else if(input_voltage >= 2.7) {
        sample_rate = SPI_CLOCK_GE27V;
    } else {
        printf("Input voltage is not within 2.7 - 5 Volts");
        throw std::invalid_argument("input voltage not within 2.7 - 5 volts");
    }

    // Initialize GPIOs
    if (gpioInitialise() < 0){
        throw std::runtime_error("GPIO failed to initialize");
    }

    // Open SPI Interface
    spi_handle = spiOpen(0, sample_rate, PI_SPI_FLAGS_MODE(1));
    if(spi_handle < 0){
        switch(spi_handle){
            case PI_BAD_SPI_CHANNEL:
            throw std::invalid_argument("bad SPI channel");
            break;

            case PI_BAD_SPI_SPEED:
            throw std::invalid_argument("bad SPI speed");
            break;

            case PI_BAD_SPI_BAUD:
            throw std::invalid_argument("bad SPI baud rate");
            break;

            case PI_NO_AUX_SPI:
            throw std::invalid_argument("no AUX SPI");
            break;

            case PI_SPI_OPEN_FAILED:
            throw std::invalid_argument("SPI device failed to open");
            break;

            case PI_BAD_FLAGS:
            throw std::invalid_argument("bad SPI flags");
            break;

            default:
            throw std::runtime_error("failed due to unknown error");
            break;
        }
    }
}

/**
 * Deconstructor cleans up by terminating GPIO
*/
MCP3008::~MCP3008(){
    gpioTerminate();
}

/**
 * @brief Reads digital output from SPI interface
 * 
 * @param channel: uint8_t, Channel on MCP3008 chip to read from
 * 
 * @return uint32_t: Digital output of MCP3008
 * 
 * Read raw bits through SPI channel from MCP
*/
uint32_t MCP3008::read_raw_from(uint8_t channel){
    // the hardware only supports 8-bit spi
    // according to datasheet, need to send 3 8-bit buffers

    if(channel  > 7) {
        throw std::invalid_argument("channel must be between 0-7");
    }

    uint8_t buffer[3];

    // start bit
    buffer[0] = 1;
    // set to single and set bits D2 D1 and D0 accordingly
    //((8+channel) < 4)
    buffer[1] = 0x80 | (channel << 4);
    // all zeros, ignored but needed
    buffer[2] = 0;

    // need to read and write and same time
    // can use same buffer
    int32_t bytes = spiXfer(spi_handle, (char*)buffer, (char*)buffer, 3);
    switch(bytes){
        case  PI_BAD_HANDLE:
        throw std::invalid_argument("bad SPI handle");
        break;
        case  PI_BAD_SPI_COUNT:
        throw std::invalid_argument("bad SPI count");
        break;
        case  PI_SPI_XFER_FAILED:
        throw std::runtime_error("transfer failed");
        break;
        default:
        break;
    }

    uint32_t output;
    
    // set output according to MCP datasheet
    output = ((buffer[1]&3)<<8) | buffer[2];
    return output;
}

/**
 * @brief Reads voltage output from SPI interface
 * 
 * @param channel: uint8_t, Channel on MCP3008 chip to read from
 * 
 * @return double: double output from MCP
 * 
 * Reads from SPI and converts digital output to voltage
*/
double MCP3008::read_from(uint8_t channel){
    uint32_t raw_out = read_raw_from(channel);

    return (raw_out / 1024.0 * 3.3);
}