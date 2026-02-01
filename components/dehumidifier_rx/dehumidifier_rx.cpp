#include "dehumidifier_rx.h"

namespace esphome
{
	namespace dehumidifier_rx
	{

		static inline uint8_t checksum_sum_0_8(const uint8_t buf[10])
		{
			uint16_t sum = 0;
			for (int i = 0; i <= 8; i++)
				sum += buf[i];
			return (uint8_t)(sum & 0xFF);
		}

		bool DehumidifierRxComponent::valid_checksum_(const uint8_t buf[10]) const
		{
			return checksum_sum_0_8(buf) == buf[9];
		}

		// Finds AA 55 then reads 8 more bytes (total 10 bytes)
		bool DehumidifierRxComponent::read_frame_(uint8_t out[10])
		{
			while (available() >= 2)
			{
				int b0 = read();
				if (b0 < 0)
					return false;
				if ((uint8_t)b0 != 0xAA)
					continue;

				int b1 = read();
				if (b1 < 0)
					return false;
				if ((uint8_t)b1 != 0x55)
					continue;

				out[0] = 0xAA;
				out[1] = 0x55;

				for (int i = 2; i < 10; i++)
				{
					int v = read();
					if (v < 0)
						return false;
					out[i] = (uint8_t)v;
				}
				return true;
			}
			return false;
		}

		void DehumidifierRxComponent::loop()
		{
			uint8_t buf[10];

			// Drain any complete frames in the UART buffer
			while (available() >= 10)
			{
				if (!read_frame_(buf))
					return;
				if (!valid_checksum_(buf))
					continue;

				// buf[2] = humidity %, buf[3] = running (0/1)
				if (humidity_ != nullptr)
					humidity_->publish_state(buf[2]);
				if (running_ != nullptr)
					running_->publish_state(buf[3] == 1);
			}
		}

	} // namespace dehumidifier_rx
} // namespace esphome
