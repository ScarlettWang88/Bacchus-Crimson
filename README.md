## Bacchus-Crimson

## CSSE4010 project: Barcode Scanner and card logging for invertory management - TEAM A

### Introduction

The project aims to design and implement a warehouse management system that integrates barcode scanning, NFC technology, PC data processing, a graphical user interface (GUI), and a Bluetooth Mesh network. Through this integration, the system can track and record the check-in and check-out status of items in the warehouse in real time, thereby enhancing the transparency and accuracy of logistics data. The system is designed so that the entire warehouse management and control can be completed through a single computer. The system mainly consists of nine hardware components, all of which communicate via Bluetooth Mesh, and the number of display monitors receiving information can be increased indefinitely, with the Bluetooth Mesh network covering every corner of the warehouse.

In terms of software, a computer-based GUI application is used to receive, display, and query scanned item data and status, providing a user-friendly interface. The PC acts as a local server, processing and verifying the data obtained through scanning, and sending updated information to various displays via Bluetooth Mesh. The displays receive updated information sent by the PC through Bluetooth Mesh, and they show user operation records in real time, including check-in/check-out times, user IDs, and item information.

The implementation of this system will greatly improve the efficiency and accuracy of warehouse management. The application of the Bluetooth Mesh network not only extends the communication range, ensuring stable connections and data synchronization within a large warehouse area, but also provides real-time tracking and clear accountability, helping to improve management quality and operational transparency. Through this project, every step of warehouse operations can be accurately recorded and verified, providing strong data support and query capabilities for management and operational personnel, thereby optimizing overall productivity and management levels.

**** 
### Team member:

|Member|Student Number 
|---|---
|Junyi Zhang|  46273473  
|Jianpeng Teng|  44594370  
|Yuying Wang|  44952532 

Junyi Zhang - Bluetooth Mesh Functionality Implementation

   * Junyi Zhang, with his extensive knowledge in software engineering, is responsible for implementing the Bluetooth Mesh functionality in this project. With a keen eye for detail and a deep understanding of system architecture, Junyi ensures smooth communication between all system components. His involvement is crucial to ensuring the system's reliability and efficiency. Junyi's rich experience with similar systems makes him an indispensable member of our team.

Jianpeng Teng - GUI Development Expert

   * Jianpeng Teng is responsible for creating and implementing the graphical user interface (GUI), enabling it to receive, display, and query scanned data and status while providing a user-friendly interface. His role is vital in ensuring the system's usability and ease of use, offering users an intuitive operating experience. Jianpeng's expertise in GUI design and development will ensure the interface's functionality and user experience are optimized.

Yuying Wang - Barcode Scanning Coordinator

   * Yuying Wang oversees the barcode scanning components of this project, focusing on accurately and efficiently capturing package IDs. With a background in electronics and sensor technology, she is responsible for setting up and calibrating barcode scanners and integrating them with the rest of the system. Her role is crucial for ensuring the speed and accuracy of our inventory management system. Yuying's successful experience with similar scanning solutions makes her the ideal candidate for this role.
**** 
### Table of contents
* [Tool Module](#Tool-module)
* [Task List](#Task-List)
* [Block Diagram](#Block-Diagram)
* [DIKW](#DIKW)
* [Wireless Network Communications](#Wireless-Network-Communications)
* [Algorithms schemes](#Algorithms-schemes)
* [KPIs](#KPIs)
* [Intruction](#Intruction)
* [Folder tree](#Folder-tree)
* [Reference](#Reference)
***
### Tool Module：
* nRF52840 DK 
* thingy52
* OLED Display - M5 core2
* Barcode Scanner
* NFC sensor
***
### Task List
* Task1: Barcode Scanner scan package ID
    * Use an light sensor to scan barcodes and retrieve package IDs.
    * Develop an algorithm to recognize barcodes and convert them into numerical or textual ID forms.
    * Switch the barcode scanner mode (check-in or check-out) by scanning a barcode.
* Task2 :NFC Sensor to Identify User IDs
    * Configure an NFC reader to read NFC cards
    * Design the system to extract user IDs from NFC cards, ensuring authentication and logging.
* Task3: OLED Display show the current state of the system
    * Integrate an OLED display to show real-time system information

        eg：Scanned out item X, at time Y
    * Program the display to dynamically update the content reflecting the latest scanning and user activities.
* Task4:  Develop a Data Receiver to Receive Data via Bluetooth
    * Combine Barcode and NFC Data for Transmission to nRF52840 DK Using JSON Protocol.
* Task5: Develop Modules to Transmit Information via Bluetooth Mesh
    * Implement the Bluetooth module to transmit the obtained package ID, status, and user ID from the PC to each node.
* Task6: Data Processing
    * Bluetooth Mesh Environment Setup
        * Define Data Structure: Include package barcode, timestamp, operator NFC identifier, and item status (check-in, check-out).
        * Design Data Transmission Logic: Define the logic for data input, update, and
        * Build Bluetooth Mesh Environment: Set up the Bluetooth Mesh network, including configuring and deploying nodes.
    * Adding Data to the Bluetooth Mesh Network
        * Create Data Packet: The data packet includes all necessary information such as the package barcode, status, timestamp, and operator information.
        * Encrypt Data Packet: The data creator uses encryption methods to encrypt the data. This encryption ensures the integrity of the data and authenticates the sender, preventing tampering during transmission.
        * Broadcast Data Packet: The encrypted data packet is broadcast to all nodes in the Bluetooth Mesh network. This step ensures that each node receives the latest data packet information for verification.
        * Verify Data Packet: Upon receiving the data packet, nodes in the network verify it. This includes checking the validity of the encryption and ensuring the data packet complies with network rules.
        * Store Data: Once the data packet is verified as valid, it is stored on the nodes within the network.
        * Data Synchronization: Through the self-healing feature of the Bluetooth Mesh network, ensure that all nodes synchronize the latest data information.
    * Transaction Confirmation
        * When a data packet is successfully stored and synchronized across multiple nodes in the network, the data is considered confirmed. This process ensures the security, transparency, and immutability of the data.
        * This procedure ensures the security and reliability of data and information within the Bluetooth Mesh network.
* Task7: Create a Graphical User Interface (GUI)
    * Design and implement a GUI for displaying and managing records of package check-ins and check-outs, as well as user information.
    * Integrate real-time data updates and control functionalities to allow intuitive operation and monitoring of the system status.
***
### Block Diagram
![image](https://github.com/ScarlettWang88/Bacchus-Crimson/blob/project/Diagram/flowchart.png)

figure 2: Flow chart
***
### DIKW
![image](https://github.com/ScarlettWang88/Bacchus-Crimson/blob/project/Diagram/DIKW.jpg)

figure 2: DIKW Pyramid Abstraction
***
### Wireless Network Communications
In this project, there are three main wireless communication nodes: the LED display, MeshNode, and BaseNode. They interact with each other through specific wireless transmission protocols, ensuring efficient and secure data transmission.

* LED Display Communication:
    * The LED Display Node acts as the final presentation node, using the Bluetooth Mesh protocol to transmit and receive data. It receives information added to the Bluetooth Mesh network and displays the timestamp, user ID, and item status on the screen.

* Mesh Node Processing:
    * The Mesh Node acts as a relay node, using Bluetooth Mesh to receive and transmit data from the barcode scanner and NFC reader. It needs to receive JSON files sent by the Base Node and forward these files to all surrounding nodes, integrating them into the entire Mesh network. Additionally, the Mesh Node is responsible for monitoring the communication status of the network to ensure efficient and reliable data transmission. If any node encounters connectivity issues, the Mesh Node can automatically adjust the transmission path and reroute the data to ensure that the information reaches its destination smoothly.

* BaseNode Central Processing:
    * The Base Node acts as the central processing node, using BLE technology to receive data from the barcode scanner and NFC reader. The Base Node then integrates this data into a JSON file, structuring it with timestamps, user IDs, and barcode data. Subsequently, the Base Node transmits this JSON file to the PC GUI and local database for further processing. Finally, this data is sent to the Mesh Node.

Each node's wireless communication is implemented through Bluetooth Mesh, as it provides a high-energy-efficient, low-cost, and sufficiently flexible way to send small data packets. This allows the barcode scanner and NFC reader to operate for extended periods without frequent recharging or battery replacement. By applying these protocols and wireless communication methods, the project achieves efficient data capture, secure operator verification, and rapid data aggregation and upload.
![image](https://github.com/ScarlettWang88/Bacchus-Crimson/blob/project/Diagram/masssge_protocol_diagram.JPG)

figure 3: message protocol diagram
***
### Algorithms schemes
#### Wireless Communication and Data Transmission
* Bluetooth Mesh Broadcasting
    * Each node in the network uses the Bluetooth Mesh protocol for wireless communication and data transmission. This protocol allows for efficient, low-energy, and flexible broadcasting of small data packets. Barcode scanners and NFC readers can operate for extended periods without frequent recharging or battery replacement, ensuring continuous operation and data capture. The wide coverage of Bluetooth Mesh can span the entire warehouse, ensuring reliable connection and communication for devices in every corner.
#### Data Structure
* BLE Protocol
    * The Bluetooth Low Energy (BLE) protocol is used for efficient data transmission and reception within our system. BLE is known for its low energy consumption and efficient data transfer capabilities, making it ideal for devices that require long battery life and frequent data transmissions. Through BLE, the system can quickly update information, ensuring real-time data display.
* JSON Files
    * Data from barcode scanners and NFC readers are structured into JSON files. These files include timestamps, user IDs, and item status, providing a standardized format for data exchange and processing. This structured approach simplifies data handling and analysis.
#### Real-time Data Processing
* PC GUI and Local Database
    * The Base Node aggregates data and transmits it to the PC GUI and local database for further processing. This setup allows for real-time monitoring, data querying, and status updates, ensuring that all relevant information is current and accessible. Data can be synchronized across multiple terminals, ensuring transparency and traceability of operations.
    
By integrating these algorithmic solutions, your project can fully leverage the advantages of the Bluetooth Mesh network, providing a secure, transparent, and efficient logistics and warehousing management system. Each algorithm plays a critical role in ensuring the system's reliability, security, and efficiency. The selection and implementation of these algorithms should be carefully considered based on your specific needs and goals.
***
### KPIs
* Sensors: Barcode Scanner and NFC
    * Barcode Scanner
        * Scanning product barcodes to read item information
        * Scanning specific barcodes to control the barcode scanner's status (check-in/check-out)
    * NFC Sensor
        * Reading user ID and information by tapping a user card
* Actuator: OLED Display
    * Displaying item information and status at specific times
    e.g. Item X checked out at time Y
    * The farthest LED display shows information within 5 seconds
* Wireless Networking: Bluetooth Mesh 
    * Each Mesh Node can receive and transmit information, and display it on the LED screen
    * Maintain stable communication in open spaces
* Dashboard for Viewing Package Inventory: GUI
    * Display user information
    * Display scanned item status
    * Display item check-in/check-out records
    * GUI loads in under 2 seconds on the target hardware
***
### Folder Structure
    Bacchus-Crimson/
    ├── Diagram
    │   ├── DIKW.jpg
    │   └── masssge_protocol_diagram.jpg
    ├── Mycode
    │   ├── apps
    │   │   ├── NFC
    │   │   │   ├── CMakeLists.txt
    │   │   │   ├── prj.conf
    │   │   │   └── src
    │   │   │       └── main.c
    │   │   ├── Scanner
    │   │   │   ├── CMakeLists.txt
    │   │   │   ├── prj.conf
    │   │   │   └── src
    │   │   │       └── main.c
    │   │   └── BaseNode
    │   │       ├── CMakeLists.txt
    │   │       ├── prj.conf
    │   │       └── src
    │   │           └── main.c  
    │   └── include
    │       └── dummy.h
    ├── README.md
    └── UI
        └── gui.py



### Reference
NFC Basics
https://developer.android.com/develop/connectivity/nfc/nfc?hl=zh-cn
