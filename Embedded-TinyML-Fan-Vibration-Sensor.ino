#include <Fan_Error_detection_inferencing.h>
#include <Wire.h>

#define FREQUENCY_HZ 63
#define INTERVAL_MS (1000 / (FREQUENCY_HZ + 1))
#define SDA_PIN 1
#define SCL_PIN 2
#define MPU_ADDR 0x68

static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

void setup() {
    Serial.begin(115200);
    delay(2000);
    Serial.println("STARTING.....");
    
    Wire.begin(SDA_PIN, SCL_PIN);
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B);
    Wire.write(0);
    Wire.endTransmission(true);

    if (EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME != 3) {
        Serial.println("NOT 3 SENSOR AXIS!");
    }
}

void loop() {
    ei_printf("SAMPLING..\n");


    for (int i = 0; i < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; i += 3) {
        
        uint64_t next_tick = micros() + (INTERVAL_MS * 1000);

        Wire.beginTransmission(MPU_ADDR);
        Wire.write(0x3B);
        Wire.endTransmission(false);
        Wire.requestFrom(MPU_ADDR, 6, true);

        int16_t AcX = Wire.read() << 8 | Wire.read();
        int16_t AcY = Wire.read() << 8 | Wire.read();
        int16_t AcZ = Wire.read() << 8 | Wire.read();

        features[i + 0] = (float)AcX;
        features[i + 1] = (float)AcY;
        features[i + 2] = (float)AcZ;

        while (micros() < next_tick) { 
        }
    } 
 
    ei_impulse_result_t result = {0};
    signal_t signal;
    
    int err = numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
        ei_printf("Failed to create signal (%d)\n", err);
        return;
    }
    err = run_classifier(&signal, &result, false);
    if (err != EI_IMPULSE_OK) {
        ei_printf("Failed to run classifier (%d)\n", err);
        return;
    }

    ei_printf("Predictions: \n");

    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
    }

#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("ANOMALY DETECTED SCORE: %.3f\n", result.anomaly);
    if (result.anomaly > 1.0) {
        Serial.println("!!! UNKNOWN VIBRATION DETECTED !!!");
    }
#endif

    delay(1000);
}