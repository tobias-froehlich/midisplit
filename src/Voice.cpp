#include <iostream>
#include <vector>
#include <queue>
#include "const.h"
#include "utils.h"
#include "Voice.h"

Voice::Voice() {

}

Voice::~Voice() {

}


void Voice::set_mode(std::string mode) {
  if (mode.compare("piano") == 0) {
      std::cout << "piano\n";
      zMode = piano;
  }
  else if (mode.compare("piano_bending") == 0) {
      std::cout << "piano_bending\n";
      zMode = piano_bending;
  }
  else if (mode.compare("legato_bending") == 0) {
      std::cout << "legato_bending\n";
      zMode = legato_bending;
  }
  else {
    std::cout << "Warning: No valid mode!\n";
  }
}


void Voice::set_in_channels(
 std::vector< std::string > channels) {
  zInChannels.clear();
  for ( std::string channel : channels ) {
    zInChannels.push_back(std::stoi(channel) - 1);
  }
}

void Voice::set_out_channels(
 std::vector< std::string > channels) {
  zOutChannels.clear();
  for ( std::string channel : channels ) {
    zOutChannels.push_back(std::stoi(channel) - 1);
    zMidiCode.push_back(0);
    zOutPressed.push_back(0);
    zOutPlaying.push_back(0);
    zOffSince.push_back(1000);
  }
}

std::vector< int > Voice::get_in_channels() {
  return zInChannels;
}

std::vector< int > Voice::get_out_channels() {
  return zOutChannels;
}

int Voice::index_longest_off() {
  int max = 0;
  unsigned int index = 0;
  for ( unsigned int i=0; i<zOffSince.size(); i++ ) {
    if (zOffSince[i] > max) {
      max = zOffSince[i];
      index = i;
    }
  }
  return index;
}

void Voice::push(
  std::vector< unsigned char > message) {
  switch ( zMode ) {
    case piano:
      push_piano(message);
      break;
    case piano_bending:
      push_piano_bending(message);
      break;
    case legato_bending:
      push_legato_bending(message);
      break;
  }
}

void Voice::push_piano(
 std::vector< unsigned char > message) {
  int channel = utils::channel(message);
  for (int order : zOffSince) {
    std::cout << order << " ";
  }
  std::cout << "\n";
  for (int inchannel : zInChannels) {
    if (inchannel == channel) {
      StatusByteType type = utils::status_byte_type(message);
      if ( type == note_on ) {
        zIndex = index_longest_off();
        zMidiCode[zIndex] = message[1];
        zOutPressed[zIndex] = 1;
        zOutPlaying[zIndex] = 1;
        zOffSince[zIndex] = -1;
        message[0] = 144 + zOutChannels[zIndex];
        zMessages.push_back(message);
      }
      if ( type == note_off ) {
        for (unsigned int i=0; i<zMidiCode.size(); i++) {
          if ((zMidiCode[i] == message[1]) && (zOutPlaying[i])) {
            zOutPressed[i] = 0;
            if ( ! (zPedal) ) {
              zOutPlaying[i] = 0;
              zOffSince[i] = 0;
              message[0] = 128 + zOutChannels[i];
              zMessages.push_back(message);
              for(unsigned int j=0; j<zOffSince.size(); j++) {
                if (zOutPlaying[j] == 0) {
                  zOffSince[j] = zOffSince[j] + 1;
                }
              }
            }
          }
        }
      }
      if (type == pedal_pressed) {
        std::cout << "pedal pressed\n";
        zPedal = 1;
      }
      if (type == pedal_released) {
        std::cout << "pedal released\n";
        zPedal = 0;
        for (unsigned int i=0; i<zOutPressed.size(); i++) {
          if ( (!(zOutPressed[i])) && (zOutPlaying[i]) ) {
            zOutPlaying[i] = 0;
            zOffSince[i] = 0;
            message[0] = 128 + zOutChannels[i];
            message[1] = zMidiCode[i];
            message[2] = 0;
            zMessages.push_back(message);
            for(unsigned int j=0; j<zOffSince.size(); j++) {
              zOffSince[j] = zOffSince[j] + 1;
            }
          }
        }
      }
    }
  }
}


void Voice::push_piano_bending(
 std::vector< unsigned char > message) {
  int channel = utils::channel(message);
  for (int order : zOffSince) {
    std::cout << order << " ";
  }
  std::cout << "\n";
  for (int inchannel : zInChannels) {
    if (inchannel == channel) {
      StatusByteType type = utils::status_byte_type(message);
      if ( type == bending ) {
          zIndex = index_longest_off();
          message[0] = 224 + zOutChannels[zIndex];
          zMessages.push_back(message);
      }
      if ( type == note_on ) {
        zMidiCode[zIndex] = message[1];
        zOutPressed[zIndex] = 1;
        zOutPlaying[zIndex] = 1;
        zOffSince[zIndex] = -1;
        message[0] = 144 + zOutChannels[zIndex];
        zMessages.push_back(message);
      }
      if ( type == note_off ) {
        for (unsigned int i=0; i<zMidiCode.size(); i++) {
          if ((zMidiCode[i] == message[1]) && (zOutPlaying[i])) {
            zOutPressed[i] = 0;
            if ( ! (zPedal) ) {
              zOutPlaying[i] = 0;
              zOffSince[i] = 0;
              message[0] = 128 + zOutChannels[i];
              zMessages.push_back(message);
              for(unsigned int j=0; j<zOffSince.size(); j++) {
                if (zOutPlaying[j] == 0) {
                  zOffSince[j] = zOffSince[j] + 1;
                }
              }
            }
          }
        }
      }
      if (type == pedal_pressed) {
        std::cout << "pedal pressed\n";
        zPedal = 1;
      }
      if (type == pedal_released) {
        std::cout << "pedal released\n";
        zPedal = 0;
        for (unsigned int i=0; i<zOutPressed.size(); i++) {
          if ( (!(zOutPressed[i])) && (zOutPlaying[i]) ) {
            zOutPlaying[i] = 0;
            zOffSince[i] = 0;
            message[0] = 128 + zOutChannels[i];
            message[1] = zMidiCode[i];
            message[2] = 0;
            zMessages.push_back(message);
            for(unsigned int j=0; j<zOffSince.size(); j++) {
              zOffSince[j] = zOffSince[j] + 1;
            }
          }
        }
      }
    }
  }
}


void Voice::push_legato_bending(
 std::vector< unsigned char > message) {
  int channel = utils::channel(message);
  for (int order : zOffSince) {
    std::cout << order << " ";
  }
  std::cout << "\n";
  for (int inchannel : zInChannels) {
    if (inchannel == channel) {
      StatusByteType type = utils::status_byte_type(message);
      if ( type == bending ) {
        if (zNumberOfNotesPlaying == 0) {
            zIndex += 1;
            zIndex = zIndex % zMidiCode.size();
        }
        
        message[0] = 224 + zOutChannels[zIndex];
        zMessages.push_back(message);
      }
      if ( type == note_on ) {
        zNumberOfNotesPlaying += 1;
        zMidiCode[zIndex] = message[1];
        message[0] = 144 + zOutChannels[zIndex];
        zMessages.push_back(message);
      }
      if ( type == note_off ) {
            zNumberOfNotesPlaying -= 1;
            message[0] = 128 + zOutChannels[zIndex];
            zMessages.push_back(message);
      }
    }
  }
}

int Voice::something_to_pull() {
  return (zMessages.size());
}

std::vector< unsigned char > Voice::pull() {
  std::vector< unsigned char > result =
   zMessages.front();
  zMessages.pop_front();
  return result;
}
