# Edge AI Predictive Maintenance System

A TinyML-based vibration monitoring device capable of detecting mechanical faults in a Fan in real-time. This project uses an ESP32-S3 and an MPU6050 accelerometer to run on-device inference, distinguishing between normal operation, mechanical imbalance, and unknown anomalies without cloud connectivity.

## Project Overview
Industrial motors and fans often fail unexpectedly, leading to costly downtime. This system demonstrates a **Predictive Maintenance** solution that monitors vibration patterns to detect:
* **Normal Operation:** Smooth rotation.
* **Mechanical Fault (Imbalance):** Simulated using a weighted fan blade imbalence.
* **Idle State:** System is off.
* **Anomalies:** Unknown vibration patterns.

## Hardware & Tech Stack
**Hardware:**
* ESP32-S3 
* MPU6050 
* Desk Fan

**Software & Tools:**
* **Edge Impulse:** For DSP (Digital Signal Processing), model training, and C++ library generation.
* **Arduino IDE:** Firmware development
* **TinyML:** Quantized Neural Network for low-latency inference.

## The AI Model
The system uses a Hybrid Edge AI Architecture ran entirely on the ESP32:

1.  **Data Acquisition:** Samples 3-axis vibration data at **62.5Hz**.
2.  **DSP Pipeline:**
    * **High-Pass Filter (5Hz):** Removes gravity and static bias to isolate rotational vibration.
    * **Spectral Analysis (FFT):** Extracts frequency domain features (Power Spectrum) from time-series data.
3.  **Inference Engine:**
    * **Classification (Neural Network):** Predicts classes (`On`, `Offset`, `Off`).
    * **Anomaly Detection (K-Means):** Calculates a "distance" score from known clusters. If the vibration pattern is unknown (Score > 1.0 or < -1.0), the system flags.

##  Wiring Setup
The MPU6050 communicates via I2C.
* **VCC** -> 3.3V
* **GND** -> GND
* **SDA** -> GPIO 1 (ESP32-S3)
* **SCL** -> GPIO 2 (ESP32-S3)
* The sensor is mounted behind the fan, closest to the motor

![IMG_1156](https://github.com/user-attachments/assets/50afbc5e-72b1-4ecc-9bf6-27346a38deb6)

## How to Run
1.  **Clone the Repository:**
    ```bash
    git clone https://github.com/BrandonMayer1/Embedded-TinyML-Fan-Vibration-Sensor.git
    ```
2.  **Install Dependencies:**
    * Install the ESP32 Board Manager in Arduino IDE.
    * Import the custom Edge Impulse library.
3.  **Upload Firmware:**
    * Open `firmware/main.ino`.
    * Select Board: `ESP32S3 Dev Module`.
    * Upload and open Serial Monitor (Baud: 115200).

## Results
The system achieves **>95.59% accuracy** in distinguishing between balanced and imbalanced states.

![Screenshot 2026-01-19 202613](https://github.com/user-attachments/assets/8e870958-db2c-41de-81b0-342c721a6ea5)
![Screenshot 2026-01-19 202628](https://github.com/user-attachments/assets/f11d315a-7626-4fa1-abab-f164a0b677d9)


## Sample Output

![Screenshot 2026-01-19 201520](https://github.com/user-attachments/assets/35fd5cbe-98f2-4fe9-b49a-a714efda2e93)

