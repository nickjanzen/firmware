#pragma once
#include "../mesh/generated/telemetry.pb.h"
#include "NodeDB.h"
#include "main.h"

#define DEFAULT_SENSOR_MINIMUM_WAIT_TIME_BETWEEN_READS 1000

class TelemetrySensor
{
  protected:
    TelemetrySensor(TelemetrySensorType sensorType, const char *sensorName)
    {
        this->sensorName = sensorName;
        this->sensorType = sensorType;
        this->status = 0;
    }

    const char *sensorName;
    TelemetrySensorType sensorType;
    unsigned status;

    int32_t initI2CSensor() {
        if (!status) {
            LOG_WARN("Could not connect to detected %s sensor.\n Removing from nodeTelemetrySensorsMap.\n", sensorName);
            nodeTelemetrySensorsMap[sensorType] = 0;
        } else {
            LOG_INFO("Opened %s sensor on default i2c bus\n", sensorName);
            setup();
        }
        return DEFAULT_SENSOR_MINIMUM_WAIT_TIME_BETWEEN_READS;
    }
    virtual void setup();

  public:
    bool hasSensor() { 
        return sensorType < sizeof(nodeTelemetrySensorsMap) && nodeTelemetrySensorsMap[sensorType] > 0; 
    }

    virtual int32_t runOnce() = 0;
    virtual bool getMetrics(Telemetry *measurement) = 0;
};
