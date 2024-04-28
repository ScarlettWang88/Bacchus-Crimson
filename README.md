# Bacchus-Crimson

## CSSE4010 project: Barcode Scanner and card logging for invertory management - TEAM A
This project aims to design and implement a blockchain-based warehouse logistics management system that integrates barcode scanning, NFC technology, and Bluetooth communication. Through this integration, the system can track and record real-time entries and exits of goods in the warehouse, thus enhancing the transparency and security of logistics data. The system primarily consists of three hardware components, all of which communicate via Bluetooth: a barcode scanner equipped with an OLED display to show item information and scan times in real-time; an NFC scanner to identify and log operator user IDs; and a base transmission device responsible for integrating data and transferring it to a computer.

On the software side, a computer-based GUI application is used to receive, display, and query scanned item data and status, providing a user-friendly interface. Additionally, the computer acts as a node in the blockchain network, processing and verifying data obtained through scanning. Once data is verified and confirmed through the network's consensus mechanism, related transactions are packaged into new blocks and added to the blockchain. This process ensures the data's immutability and the traceability of history.

The implementation of this system will greatly enhance the efficiency and accuracy of warehouse management. The application of blockchain technology not only ensures the security and integrity of all logistics data but also provides real-time tracking and clear accountability, helping to improve management quality and operational transparency. Through this project, every step of warehouse operations can be precisely recorded and verified, providing management and operators with robust data support and query capabilities, as well as secure protection of the entire database.

**** 
### Team member:

|Member|Student Number | Roles
|---|---|---
|Junyi Zhang|  46273473  | 
|Jianpeng Teng|  44594370  |
|Yuying Wang|  44952532 |

**** 
### Table of contents
* [Tool Module](#Tool-module)
* [Task List](#Task-List)
* [Block Diagram](#Block-Diagram)
* [DIKW](#DIKW)
* [KPIs](#KPIs)
* [Intruction](#Intruction)
* [Folder tree](#Folder-tree)
* [Reference](#Reference)
***
### Tool Module：
* nRF52840 DK
* OLED Display
* Light sensor(Barcode Scanner)
    * Light Sensors
        * Barcode scanners utilize light sensors, typically employing laser diodes or LEDs to illuminate barcodes. The pattern of light reflected back is captured by either photodiodes or CCDs (Charge-Coupled Devices).
    * Optical Data
        * The scanner needs to capture the varying patterns of light and dark bars from the barcode which encode data such as product IDs, prices, or inventory information.
    * Integrated
        * Optical Assembly: To focus and collect the reflected light from the barcode.
        * Signal Decoder: Converts the optical signal from the light sensor into digital data.
        * Data Interface: The digital data is processed and encoded into a format suitable for Bluetooth transmission.
* NFC sensor
    * NFC Modules
        * These sensors are capable of reading and writing to NFC tags using RF (Radio Frequency) communication. They generate a small electromagnetic field that powers the tag and allows data transfer.
    * Electromagnetic Data
        * NFC sensors interact with NFC tags to retrieve or write data, such as links, transaction details, or authentication tokens.
    * Integrated
        * Antenna: To create and manage the electromagnetic field necessary for communication with NFC tags.
        * Transceiver: Manages data sending and receiving operations.
        * Data Processor: Handles data processing tasks such as reading, writing, and possibly encrypting data before it is sent over Bluetooth.
        * System Interface: NFC sensors typically connect to a main system controller via communication protocols like SPI, I2C, or UART, which then manages Bluetooth data transmission.
***
### Task List
* Task1: Barcode Scanner scan package ID
    * Use an light sensor to scan barcodes and retrieve package IDs.
    * Develop an algorithm to recognize barcodes and convert them into numerical or textual ID forms.
* Task2 :NFC Sensor to Identify User IDs
    * Configure an NFC reader to read NFC cards
    * Design the system to extract user IDs from NFC cards, ensuring authentication and logging.
* Task3: OLED Display show the current state of the system
    * Integrate an OLED display to show real-time system information

        eg：Scanned out item X, at time Y
    * Program the display to dynamically update the content reflecting the latest scanning and user activities.
* Task4: Transmit Data via Bluetooth from barcode and NFC Scanners
    * Implement a Bluetooth module to send recognized package IDs and user IDs to a central processor or server.
* Task5: Develop a Data Receiver to Receive Data via Bluetooth
    * Combining Barcode and NFC Data for Transmission to PC Using JSON Protocol
* Task6: Data Processing
    * Blockchain Environment Setup
        * Define Data Structures: This includes package barcodes, timestamps, operator NFC identifiers, and item status (inbound, outbound).
        * Design Smart Contracts: Smart contracts define the logic for data entry, updates, and queries.
        * Build Blockchain Environment: Set up the blockchain network, including configuring and deploying nodes.
    * Adding Data to the Blockchain
        * Create Transaction: The transaction includes all necessary information, such as the package's barcode, status, timestamp, and operator information.
        * Transaction Signature: The transaction creator uses their private key to sign the transaction. This signature ensures the integrity of the transaction and the identity verification of the initiator, preventing the content from being tampered with during transmission.
        * Broadcast Transaction: The signed transaction is broadcast to all nodes in the blockchain network. This step ensures that every node in the network receives the latest transaction information for verification.
        * Transaction Verification: After receiving the transaction, nodes in the network verify it. This includes checking whether the transaction's signature is valid and whether the transaction complies with network rules.
        * Block Packaging: Once a transaction is verified as valid, it is added to a new block. The block also includes other verified transactions and a reference to the hash value of the previous block.
        * Block Mining: New blocks are mined through a proof-of-work system or other consensus mechanisms. Successful mining means the block will be added to the blockchain.
        * Block Confirmation: The newly mined block is again verified by other nodes in the network. Once the majority of nodes confirm the validity of the block, it is officially added to the blockchain, and each node updates its copy of the blockchain.
        * Transaction Confirmation: A transaction is considered confirmed once it is included in a block that has been confirmed by the network.

     This process ensures the security, transparency, and immutability of transactions and data.
* Task7: Create a Graphical User Interface (GUI)
    * Design and implement a GUI for displaying and managing records of package check-ins and check-outs, as well as user information.
    * Integrate real-time data updates and control functionalities to allow intuitive operation and monitoring of the system status.
***
### Block Diagram

***
### DIKW
![image](https://github.com/ScarlettWang88/Bacchus-Crimson/blob/project/Diagram/DIKW.jpg)

figure 2: DIKW Pyramid Abstraction
***
### Wireless Network Communications
In this project, there are three main wireless communication nodes: the barcode scanner, NFC reader, and BaseNode. They interact with each other through specific wireless transmission protocols, ensuring efficient and secure data transmission.

* Barcode Scanner Communication:
    * The barcode scanner transmits data using the Bluetooth Low Energy (BLE) protocol. After scanning the barcode of a package, the device packages the scanned barcode data, along with the device's unique identifier (UUID) and signal strength (RSSI) into the iBeacon broadcast format, and then sends it out via BLE wireless signal. The iBeacon protocol, designed for proximity communication, is suitable for rapid and energy-efficient data transfer between devices.

* NFC Reader Communication:
    * The NFC reader uses Near Field Communication (NFC) technology to capture the identity information of the operator. When an operator approaches the reader with an NFC tag, the reader quickly identifies the user ID on the NFC tag and transmits this information using the same BLE iBeacon advertisement. The very short working distance of NFC technology ensures the security of transactions, as only users close to the reader can be identified and recorded.

* BaseNode Central Processing:
    * BaseNode acts as a central processing node, using BLE technology to receive data from both the barcode scanner and NFC reader. BaseNode then integrates this data into a JSON file, which structures timestamps, user IDs, and barcode data. Thereafter, BaseNode transmits this JSON file to a PC GUI and local database for further processing. This data is eventually packaged into blocks and added to the entire blockchain network.

Each node's wireless communication is implemented through BLE, as BLE provides a high-energy-efficient, low-cost, and sufficiently flexible way to send small data packets. This allows the barcode scanner and NFC reader to operate for extended periods without frequent recharging or battery replacement. Through the application of these protocols and wireless communication methods, the project achieves efficient data capture, secure operator verification, and rapid data aggregation and upload.
![image](https://github.com/ScarlettWang88/Bacchus-Crimson/blob/project/Diagram/masssge_protocol_diagram.jpg)
figure 3: message protocol diagram
***
### Algorithms schemes
* Hash Function for Adding Blocks
    * Use of SHA-256: This widely used secure hash algorithm encrypts data and adds blocks. It generates a 256-bit hash value, ensuring the uniqueness and integrity of the block data.
* Consensus Algorithm
    * Proof of Stake (PoS): This method is used for block validation, employing defined authority metrics and algorithmic mechanisms to create new blocks and validate them through other nodes.
* Chain Data Structure
    * Linked List: In blockchain, each block contains a link to the hash value of the previous block, forming a continuous chain. This structure ensures data integrity from any block back to the original block (genesis block).
* Creation of Smart Contracts
    * Design and deploy smart contracts to handle logistics data, such as recording the inbound and outbound movements of packages, validating operator actions, and possibly triggering further actions such as notifying recipients or updating inventory records.
* Transaction Signature Verification
    * ECDSA (Elliptic Curve Digital Signature Algorithm): This is a signature algorithm based on elliptic curve cryptography, used to generate and verify digital signatures. ECDSA is extensively used in blockchain to ensure that transactions are initiated by legitimate and correct parties and have not been tampered with.

By integrating these algorithmic solutions, your project can fully leverage the advantages of blockchain technology, providing a secure, transparent, and efficient logistics and warehousing management system. Each algorithm plays a critical role in your system, ensuring the legality of all operations and the security of the data. The selection and implementation of these algorithms should be carefully considered based on your specific needs and goals.
***
### KPIs
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
* Algorithms schemes：IoT BlockChain
    * Blockchain node uptime of 99.9%
    * Average block confirmation time of less than 10 seconds
    * Data integrity error rate of less than 1%
* Creae a dashboard to view the package inventory：GUI
    * Response time of under 100 milliseconds for any action
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





