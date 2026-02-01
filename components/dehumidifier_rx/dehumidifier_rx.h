#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome
{
	namespace dehumidifier_rx
	{

		class DehumidifierRxComponent : public Component, public uart::UARTDevice
		{
		public:
			void set_humidity_sensor(sensor::Sensor *s) { humidity_ = s; }
			void set_running_sensor(binary_sensor::BinarySensor *b) { running_ = b; }

			void loop() override;

		protected:
			sensor::Sensor *humidity_{nullptr};
			binary_sensor::BinarySensor *running_{nullptr};

			bool read_frame_(uint8_t out[10]);
			bool valid_checksum_(const uint8_t buf[10]) const;
		};

	} // namespace dehumidifier_rx
} // namespace esphome
