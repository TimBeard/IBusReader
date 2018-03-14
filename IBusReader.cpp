/**
 *  IBusReader.h - A FlySky IBus signal reader
 *
 *  This library allows the reading of an IBus Stream and
 *  the consumption of its data
 *
 *  @author Timothy Beard <timothy.beard@orange.fr>
 *
 *  @since 2018-02-26
 */

#include "Arduino.h"
#include "IBusReader.h"

IBusReader::IBusReader() {

    Serial.begin(115200);
}

void IBusReader::update() {

    // Avoid early processing
    gotFrame = false;

    // If data is available on serial port...
    while (Serial.available()) {

        // Read it
        uint8_t val = Serial.read();

        // Look for an I-Bus header (0x2040) and restart if we get anything else
        if (currentChannel == 0 && val != 0x20) {

            return;
        }

        if (currentChannel == 1 && val != 0x40) {

            currentChannel = 0;

            return;
        }

        // Store value if incomplete
        if (currentChannel < BUFFERSIZE) {

            rawData[currentChannel] = val;
            currentChannel += 1;

            return;
        }

        // Reset index
        currentChannel = 0;

        // Flip bytes in received checksum
        uint16_t checksum1 = computeChecksum();
        uint16_t checksum2 = flipBytes(16);

        // If the chechk is good, store packet data and allow processing
        if (computeChecksum()) {

            for (int i = 0; i < MAXCHANNEL; i += 1) {

                // Flip bytes
                cnlData[i] = flipBytes(i + 2);
            }

            // Allow processing
            gotFrame = true;
        }
    }
}

bool IBusReader::available() {

    return gotFrame;
}

uint16_t IBusReader::readChannel(uint8_t channel) {

    return cnlData[channel];
}

uint16_t IBusReader::computeChecksum() {

    uint16_t checksum = 0xFFFF;

    for (int i = 0; i < BUFFERSIZE - 2; i += 1) {

        checksum -= rawData[i];
    }

    return checksum;
}

uint16_t IBusReader::flipBytes(uint8_t input) {

    input *= 2;

    uint8_t high = rawData[input - 1];
    uint8_t low = rawData[input - 2];

    return (high << 8) + low;
}

IBusReader IBus;
