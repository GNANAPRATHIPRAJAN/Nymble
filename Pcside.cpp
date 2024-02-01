#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

int main() {
    // Serial port settings
    const string serialPort = "/dev/ttyUSB0"; // Update this with your serial port
    ofstream serialWriter(serialPort);

    if (!serialWriter.is_open()) {
        cerr << "Error: Unable to open serial port." << endl;
        return 1;
    }

    // Text to be transmitted
    string textData = "Finance Minister Arun Jaitley Tuesday ... (Your long text)";

    // Calculate the size of data to be sent
    int dataSize = textData.size();

    // Start time for measuring transmission speed
    auto startTime = chrono::high_resolution_clock::now();

    // Transmit data to MCU
    serialWriter << textData;

    // Stop time for measuring transmission speed
    auto endTime = chrono::high_resolution_clock::now();

    // Calculate transmission speed in bits per second
    double transmissionSpeed = (dataSize * 8) / chrono::duration<double>(endTime - startTime).count();
    cout << "Transmission Speed: " << transmissionSpeed << " bits/second" << endl;

    // Close the serial port
    serialWriter.close();

    // Simulate receiving data on the MCU side
    this_thread::sleep_for(chrono::seconds(1));

    // Read data from MCU (replace with actual read logic)
    ifstream serialReader(serialPort);
    string receivedData;

    if (serialReader.is_open()) {
        getline(serialReader, receivedData);
        cout << "Received Data:" << endl;
        cout << receivedData << endl;
        serialReader.close();
    } else {
        cerr << "Error: Unable to open serial port for reading." << endl;
        return 1;
    }

    return 0;
}
