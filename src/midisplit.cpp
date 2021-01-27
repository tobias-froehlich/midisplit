#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <chrono>
#include <unistd.h>
#include <Stk.h>
#include <RtMidi.h>
#include "const.h"
#include "utils.h"
#include "Parameters.h"
#include "Voice.h"
#include "Splitter.h"

int main(int argc, char** argv) {
  RtMidiIn* midiin = 0;
  RtMidiOut* midiout = 0;
 
  Splitter* splitter;

  splitter = new Splitter();

  splitter->read_parameter_file("parameters.txt");

  try {
    midiin = new RtMidiIn(RtMidi::UNSPECIFIED, "midisplit in");
  } catch (RtMidiError &error) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }

  try {
    midiout = new RtMidiOut(RtMidi::UNSPECIFIED, "midisplit out");
  } catch (RtMidiError &error) {
    error.printMessage();
    exit( EXIT_FAILURE );
  }

  midiin->openVirtualPort();
  midiin->ignoreTypes(true, true, true);
  midiout->openVirtualPort();


  int running = 1;
  std::vector<unsigned char> message;
  int nBytes;
  while (running) {
    usleep(1000);
    message.clear();
    midiin->getMessage(&message);
    nBytes = message.size();
    if (nBytes >= 3) {
      std::cout << " [";
      for (unsigned char byte : message) {
        std::cout << (int)byte << ", ";
      }
      std::cout << "]->\n";
    
      if ((message[1] == cNoteForExit) && (message[2] == 0)) {
        running = 0;
        for (int i=0; i<128; i++) {
          message.clear();
          message.push_back(128);
          message.push_back((unsigned char)i);
          std::cout << i << " ";
          message.push_back(0);
          midiout->sendMessage(&message);
          usleep(1000);
        }
      }
      if (message[2] == 0) {
        if ((144 <= message[0]) && (message[0] < 160)){
          message[0] = message[0] - 16;
        }
      }
      splitter->push(message);
    }
    if ( splitter->something_to_pull() ) {
      message = splitter->pull();
      std::cout << "               ->[";
      for (unsigned char byte : message) {
        std::cout << (int)byte << ", ";
      }
      std::cout <<"]\n";
      midiout->sendMessage(&message);
    }
    usleep(1000);
  }

  midiout->closePort();
  midiin->closePort();

  delete midiout;
  delete midiin;
}
