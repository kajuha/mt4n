#include <stdio.h>
#include <modbus.h>
#include <errno.h>
#include <unistd.h>

#define SLAVE_NUM 11

int main(void) {
  modbus_t *ctx;
  uint16_t input_register = 0;

  ctx = modbus_new_rtu("/dev/ttyUSB9", 9600, 'N', 8, 1);

  // modbus_new_rtu return
  // pointer : successful
  // NULL : error, set errno
  if (ctx == NULL) {
  	printf("Unable to allocate libmodbus context: %s \n", modbus_strerror(errno));

  	return -1;
  }

  // modbus_set_slave return
  // 0: successful
  // -1 : error, set errno
  if (modbus_set_slave(ctx, SLAVE_NUM) == -1) {
  	printf("Unable to set the slave ID in context: %s \n", modbus_strerror(errno));

  	return -1;
  }

  #if 0
  // modbus_set_debug no return
  modbus_set_debug(ctx, TRUE);
  #endif

  // modbus_connect return
  // 0: successful
  // -1 : error, set errno
  if (modbus_connect(ctx) == -1) {
  	printf("Connection failed: %s \n", modbus_strerror(errno));

  	return -1;
  }

  int count = 0;

  while (TRUE) {
  // modbus_read_input_registers return
  // the number of read input registers
  // -1 : error, set errno
    if (modbus_read_input_registers(ctx, 0, 1, &input_register)) {
      printf("%8d : %6.2f V \n", count++, input_register/100.0);
    }
  }

  // modbus_close no return
  modbus_close(ctx);
  // modbus_free no return
  modbus_free(ctx);

  return 0;
}