#include <iostream>
#include <queue>
#include "const.h"
#include "utils.h"
#include "Parameters.h"
#include "Voice.h"
#include "Splitter.h"

Splitter::Splitter() {

}

Splitter::~Splitter() {

}

void Splitter::read_parameter_file(
 std::string filename) {
  Parameters* parameters;
  parameters = new Parameters();

  parameters->read_file(filename);
  
  std::string par_inchannel = "input_channels_1";
  std::string par_outchannel = "output_channels_1";
  std::string par_mode = "mode_1";

  zVoices.clear();

  while (
       (parameters->name_occurs(par_inchannel))
    && (parameters->name_occurs(par_outchannel))
  ) {
    zVoices.push_back(Voice());
    zVoices.back().set_in_channels(
      parameters->get_values(par_inchannel)
    );
    zVoices.back().set_out_channels(
      parameters->get_values(par_outchannel)
    );
    if (parameters->name_occurs(par_mode)) {
      zVoices.back().set_mode(
        parameters->get_values(par_mode)[0]
      );
    }
    par_inchannel =
     utils::inc_ending_number(par_inchannel);
    par_outchannel = 
     utils::inc_ending_number(par_outchannel);
    par_mode = utils::inc_ending_number(par_mode);
  }

  delete parameters;
}

int Splitter::get_num_of_voices() {
  return zVoices.size();
}

void Splitter::push(std::vector< unsigned char > message) {
  for ( Voice &voice : zVoices ) {
    voice.push(message);
  }
  for (Voice &voice : zVoices) {
    while (voice.something_to_pull()) {
      zMessages.push_back(voice.pull());
    }
  }
}

int Splitter::something_to_pull() {
  return (zMessages.size());
}

std::vector< unsigned char > Splitter::pull() {
  std::vector< unsigned char > result =
   zMessages.front();
  zMessages.pop_front();
  return result;
};
