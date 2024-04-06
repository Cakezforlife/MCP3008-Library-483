#include <MCP3008.h>
#include <cstdint>
#include <stdio.h>
#include <unistd.h>

int main() {

    MCP3008 mcp(5.0,5.0);

    for(;;){
        uint32_t raw = mcp.read_raw_from(7);
        double volt = mcp.read_from(7);

        printf("Raw reading: %u\n", raw);
        printf("Voltage reading: %f\n", volt);

        usleep(100000);
    }

    return 0;
}