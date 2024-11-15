# FIMA
## Introduction
The Fridge Intelligent Management Assistant (FIMA) is a smart device project aimed at enhancing the efficiency and health of household food management. FIMA provides users with real-time insights into the contents and condition of their fridge, allowing for optimized food preservation and reducing waste by alerting users to expiring items. The device works as an accessory compatible with various refrigerator models, enabling consumers to adopt this intelligent management system without needing a new fridge.

FIMA uses a Wio Terminal, equipped with an IMU sensor, temperature and humidity sensors, and an RFID reader. The device connects with an app via a server, utilizing MQTT for communication. It tracks the fridge's contents through RFID tags, which are scanned to create a database of items with associated shelf-life and storage condition information. The IMU sensor detects door openings, reducing power consumption and preventing erroneous updates by only updating inventory when the door is open. The app allows users to monitor inventory and receive alerts on expiration and storage conditions.

FIMA addresses sustainability by incorporating software updates to extend device longevity and educating users on energy-efficient practices to minimize food waste. The project emphasizes security measures against common threats, including identity spoofing, data tampering, and denial-of-service attacks, ensuring data integrity, user authentication, and device availability.

Read the PDF for more details, this project is not fully developed and only parts of the codes in this project are uploaded.
