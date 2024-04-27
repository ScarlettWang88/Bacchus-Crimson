# CSSE4010 project: Barcode Scanner and card logging for invertory management - TEAM A
This project involves using a Bluetooth barcode scanner and an NFC card reader. The barcode scanner is used to scan package IDs and and NFC reader is used to log a user's ID card.

**** 
#### Team member:

|Member|Student Number|
|---|---
|Junyi Zhang|  46273473
|Jianpeng Teng|  44594370
|Yuying Wang|  44952532
**** 
## Table of contents
* [Task List](#Task-List)
* [KPIs](#KPIs)
* [Intruction](#Intruction)
* [Folder tree](#Folder-tree)
* [Reference](#Reference)
***
## 工具模块：
* nrf
* OLED Display
* light sensor
* NFC Reader
***
## Task List
* Task1: Barcode Scanner scan package ID
    * Use an light sensor to scan barcodes and retrieve package IDs.
    * Develop an algorithm to recognize barcodes and convert them into numerical or textual ID forms.
* Task2 :NFC Sensor to Identify User IDs
    * Configure an NFC reader to read NFC cards
    * Design the system to extract user IDs from NFC cards, ensuring authentication and logging.
* Task3: OLED Display show the current state of the system
    * Integrate an OLED display to show real-time system information
        Scanned out item X, at time Y
    * Program the display to dynamically update the content reflecting the latest scanning and user activities.
* Task4: Transmit Data via Bluetooth from barcode and NFC Scanners
    * Implement a Bluetooth module to send recognized package IDs and user IDs to a central processor or server.
* Task5: Develop a Data Receiver to Receive Data via Bluetooth
    * DK(Now)
* Task6: Data Processing
    * IoT BlockChain(Now)
    * Develop backend logic to process the received data, including user authentication and package tracking.
    * Design a database or data storage solution to securely store all relevant data.
* Task7: Create a Graphical User Interface (GUI)
    * Design and implement a GUI for displaying and managing records of package check-ins and check-outs, as well as user information.
    * Integrate real-time data updates and control functionalities to allow intuitive operation and monitoring of the system status.
***
## KPIs
* Sensor: Light and NFC
    * Light sensor（Barcode Scanner）
        * 99% accuracy in barcode scanning
        * Processing time of no more than 1 second
    * NFC Sensor
        * Read success rate of 99% or higher
        * Processing time of no more than 1 second
        * Device response time of no more than 100 milliseconds.
* Actuator: OLED Display
    * Less than 1 second delay in OLED update after scanning
* Use non-trivial wireless networking: Bluetooth. 
    * Data packet transmission success rate above 99.5%
    * Average connection establishment time less than 5 seconds
    * Communication latency within 10 milliseconds
    * Maintain stable communication at least 30 meters in open space
* Algorithms：IoT BlockChain
    * Blockchain node uptime of 99.9%
    * Average block confirmation time of less than 10 seconds
    * Data integrity error rate of less than 1%
* Creae a dashboard to view the package inventory：GUI
    * Response time of under 100 milliseconds for any action
    * GUI loads in under 2 seconds on the target hardware
***
## Intruction

***
## Folder tree
Bacchus-Crimson/
├── Diagram/
│ ├── block_diagram.pdf
│ ├── DIKW.pdf
│ └── flow_chart.pdf
├── Mycode/
│ ├── Bluetooth/
│ │ └── main.c
│ └── Test/
│ └── main.c
├── UI/
│ └── gui.py
└── README.md



## Reference




