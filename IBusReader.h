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

#ifndef IBUSREADER_H
#define IBUSREADER_H

#include "Arduino.h"

class IBusReader {
    public:
        IBusReader();
        void update();
        bool available();
        uint16_t readChannel(uint8_t channel);
    private:
        static const uint8_t BUFFERSIZE = 32;
        static const uint8_t MAXCHANNEL = 14;

        uint8_t currentChannel = 0;
        uint8_t rawData[BUFFERSIZE] = {0};
        uint16_t cnlData[MAXCHANNEL];

        bool gotFrame;

        uint16_t computeChecksum();
        uint16_t flipBytes(uint8_t input);
};

extern IBusReader IBus;

#endif
