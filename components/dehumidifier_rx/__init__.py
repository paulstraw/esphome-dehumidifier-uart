import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.components import uart, sensor, binary_sensor
from esphome.const import CONF_ID, CONF_UART_ID

CODEOWNERS = ["@paulstraw"]
DEPENDENCIES = ["uart", "sensor", "binary_sensor"]

dehumidifier_rx_ns = cg.esphome_ns.namespace("dehumidifier_rx")
DehumidifierRxComponent = dehumidifier_rx_ns.class_(
    "DehumidifierRxComponent", cg.Component, uart.UARTDevice)

CONF_HUMIDITY = "humidity"
CONF_RUNNING = "running"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(DehumidifierRxComponent),
        cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
        cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
            unit_of_measurement="%",
            accuracy_decimals=0,
        ),
        cv.Optional(CONF_RUNNING): binary_sensor.binary_sensor_schema(),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    await uart.register_uart_device(var, config)

    if CONF_HUMIDITY in config:
        hum = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(hum))

    if CONF_RUNNING in config:
        run = await binary_sensor.new_binary_sensor(config[CONF_RUNNING])
        cg.add(var.set_running_sensor(run))
