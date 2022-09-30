#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <sensor_msgs/Range.h>
#include <sensor_msgs/BatteryState.h>

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <stdlib.h>

#include <iostream>
#include <queue>

#include <modbus.h>

using namespace std;

int main(int argc, char* argv[])
{
	ros::init(argc, argv, "mt4n");
	ros::NodeHandle nh("~");

	std::string serial_port;
	int baud_rate;
	int slave_num;

	// ros::param::get("~serial_port", serial_port);
	// ros::param::get("~baud_rate", baud_rate);
	// ros::param::get("~slave_num", slave_num);
	nh.getParam("serial_port", serial_port);
	nh.getParam("baud_rate", baud_rate);
	nh.getParam("slave_num", slave_num);

	ros::Publisher battery_pub = nh.advertise<sensor_msgs::BatteryState>("battery", 1000, true);

	modbus_t *ctx;
	uint16_t input_register = 0;

	sensor_msgs::BatteryState batteryState;

    char real_name[NAME_MAX] = {'\0', };

	realpath(serial_port.c_str(), real_name);

	printf("%s->%s %d %d\n", serial_port.c_str(), real_name, baud_rate, slave_num);
	
	ctx = modbus_new_rtu(real_name, baud_rate, 'N', 8, 1);

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
	if (modbus_set_slave(ctx, slave_num) == -1) {
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

	ros::Rate r(1000);

	#define STEP_TIME 1.0
	double time_cur = ros::Time::now().toSec();
	double time_pre = time_cur;
	double time_diff;

	while (ros::ok())
	{
		// modbus_read_input_registers return
		// the number of read input registers
		// -1 : error, set errno
		if (modbus_read_input_registers(ctx, 0, 1, &input_register)) {
			batteryState.header.stamp = ros::Time::now();
			batteryState.voltage = input_register/100.0;
			battery_pub.publish(batteryState);
		}

		time_cur = ros::Time::now().toSec();
		time_diff = time_cur - time_pre;
		if ( time_diff > STEP_TIME ) {
			// printf("%lf: %6.2f V \n", time_cur, input_register/100.0);
			time_pre = time_cur;
		}

		ros::spinOnce();

		r.sleep();
	}

	// modbus_close no return
	modbus_close(ctx);
	// modbus_free no return
	modbus_free(ctx);

	return 0;
}
