#include <M5Core2.h> // Include M5Core2 library
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

char name[200];
std::string manufacturerData;  // Global variable to store manufacturer data

int scanTime = 10; // Scan time in seconds
BLEScan* pBLEScan;

int lastUserId = -1; // Store the last user ID to detect changes
std::string lastBarcodeData = ""; // Store the last barcode data to detect changes

// Forward declaration of processManufacturerData function
bool processManufacturerData(const std::string& data);

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        // Print device name and manufacturer data on M5Core2 screen
        if (advertisedDevice.haveManufacturerData()) {
          manufacturerData = advertisedDevice.getManufacturerData();
          processManufacturerData(manufacturerData);
          if(manufacturerData[3] == (char)0x11){
            Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
          }
        } 
    }
};

// Definition of processManufacturerData function
bool processManufacturerData(const std::string& data) {
    // Check if the data starts with the specified header 0x4c 0x00 0x40 0x11
    if (data.size() >= 14 && data[0] == (char)0x4c && data[1] == (char)0x00 && data[2] == (char)0x40 && data[3] == (char)0x11) {
        // Data matches the header, process further
        if (data[6] == (char)0x05 && data[7] == (char)0xc2) {
            int user_id = static_cast<unsigned char>(data[8]);
            int scan_type = static_cast<unsigned char>(data[9]);
            String scanType;
            if (scan_type == 0){
              scanType = "scan in item:";
            }else if (scan_type == 1){
              scanType = "scan out item:";
            }else {
              scanType = "Non valid status type";
            }
            int month = static_cast<unsigned char>(data[10]);
            int day = static_cast<unsigned char>(data[11]);
            int hour = static_cast<unsigned char>(data[12]);
            int minute = static_cast<unsigned char>(data[13]);
            std::string barcodeData = data.size() > 14 ? data.substr(14) : "";

            

                uint16_t backgroundColor = M5.Lcd.color565(0xFF, 0xD0, 0xD0); // #FFD0D0
                M5.Lcd.fillScreen(backgroundColor);

                uint16_t userIdColor = M5.Lcd.color565(0x3D, 0x49, 0xA3); // #3D49A3
                uint16_t barcodeColor = M5.Lcd.color565(0x14, 0x82, 0x1A); // #14821A

                // Set text color and size
                M5.Lcd.setTextColor(userIdColor, backgroundColor); // Set text color to userIdColor, background to light pink
                M5.Lcd.setTextSize(3); // Set text size

                // Center display
                int screenWidth = M5.Lcd.width();
                int screenHeight = M5.Lcd.height();
                
                M5.Lcd.setTextDatum(MC_DATUM); // Set text alignment to center

                // Print user ID
                if (user_id == 0){
                  M5.Lcd.drawString("None User ", screenWidth / 2, screenHeight / 2 - 80);
                }else{
                  M5.Lcd.drawString("User " + String(user_id), screenWidth / 2, screenHeight / 2 - 80);
                }
              
                M5.Lcd.drawString(scanType, screenWidth / 2, screenHeight / 2 - 40);
                
                // Set barcode data text color and size
                M5.Lcd.setTextColor(barcodeColor, backgroundColor); // Set text color to barcodeColor, background to light pink
                M5.Lcd.setTextSize(2); // Set barcode data text size

                // Print barcode data and timestamp
                if (!barcodeData.empty() && month != 0) {
                    String timestamp = String(month) + "/" + String(day) + " " + String(hour) + ":" + String(minute);
                    M5.Lcd.drawString(barcodeData.c_str(), screenWidth / 2, screenHeight / 2);
                    M5.Lcd.drawString("in " + timestamp, screenWidth / 2, screenHeight / 2 + 30);
                }
            
            return true;
        }
    }
    return false;
}

void setup() {
    M5.begin(); // Initialize M5Core2
    uint16_t backgroundColor = M5.Lcd.color565(0xFF, 0xD0, 0xD0); // #FFD0D0
    M5.Lcd.fillScreen(backgroundColor); // Set background color to light pink
    M5.Lcd.setTextColor(WHITE, backgroundColor); // Set text color to white, background to light pink
    M5.Lcd.setTextSize(1); // Set text size
    Serial.begin(115200);
    Serial.println("Scanning...");

    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); // Create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setActiveScan(true); // Active scan uses more power, but gets results faster
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);  // Less or equal setInterval value
}

void loop() {
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
    Serial.printf("Devices found: %d\n", foundDevices.getCount());
    pBLEScan->clearResults();   // Delete results from BLEScan buffer to release memory
    delay(500);
}