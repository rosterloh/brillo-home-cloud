#include <string>
#include <unistd.h>

#include "include/peripherals/gpio/gpio.h"

void TestGPIO();


/**
 * Test application code.
 */
int main(int argc, char** argv) {
  if (argc <= 1) {
    printf("Usage: hc-test test1 [... testN]\n");
    printf(" Options:\n");
    printf("  g - test GPIO\n");
    printf(" Example: \n");
    printf("  hc-test g \n");
  }

  for (int i=1; i < argc; i++) {
    char mode = argv[i][0];

    // Run the tests as issued
    if (mode == 'g')
      TestGPIO();
  }
}

/**
 * Tests GPIO functions.
 */
void TestGPIO() {
  SetupGPIO();
  bool isOn = true;

  fprintf(stderr, "Setting GPIO\n");
  for (int i=0; i < 10; i++) {
    WriteGPIO(GPIO::PIN_A, isOn);
    WriteGPIO(GPIO::PIN_B, isOn);
    WriteGPIO(GPIO::PIN_C, isOn);
    WriteGPIO(GPIO::PIN_D, isOn);
    sleep(1);
    isOn = !isOn;
  }
}
