#ifndef SBUSFUNCTIONS_HPP
#define SBUSFUNCTIONS_HPP

#include <Arduino.h>

extern uint16_t channels[16];

void SBUSsetup();
void SBUSloop();
bool SBUSOK();
uint16_t ChannelData(uint16_t channelnum);

#endif