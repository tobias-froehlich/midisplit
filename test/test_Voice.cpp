#include "../src/const.h"
#include "../src/utils.h"
#include "../src/Voice.h"

TEST ( Voice, create_and_delete ) {
  Voice* voice;
  voice = new Voice();
  delete voice;
}

TEST ( Voice, set_channels ) {
  Voice* voice;
  voice = new Voice();
  std::vector< int > inchannels
   {0, 1, 2, 3};
  std::vector< int > outchannels
   {4, 5, 6};
  std::vector< std::string > inchannels_str
   {"1", "2", "3", "4"};
  std::vector< std::string > outchannels_str
   {"5", "6", "7"};
  std::vector< std::string > invalid_channels_str
    {"bla"};

  ASSERT_THROW(
    voice->set_in_channels(invalid_channels_str),
    std::invalid_argument
  );

  voice->set_in_channels(inchannels_str);
  voice->set_out_channels(outchannels_str);

  ASSERT_EQ(voice->get_in_channels(), inchannels);
  ASSERT_EQ(voice->get_out_channels(), outchannels);

  delete voice;
}

TEST ( Voice, push_and_pull ) {
  Voice* voice;
  voice = new Voice();
  std::vector< unsigned char > message1{144, 2, 3};
  std::vector< unsigned char > message2{128, 2, 0};
  std::vector< unsigned char > message3{129, 5, 6};
  std::vector< std::string > inchannels_str{"1"};
  std::vector< std::string > outchannels_str{"1", "2", "3"};
  voice->set_in_channels(inchannels_str);
  voice->set_out_channels(outchannels_str);

  ASSERT_FALSE(voice->something_to_pull());

  voice->push(message1);
  ASSERT_TRUE(voice->something_to_pull());
  voice->push(message2);
  ASSERT_TRUE(voice->something_to_pull());

  ASSERT_EQ(
    voice->pull(),
    message1
  );

  ASSERT_TRUE(voice->something_to_pull());
  ASSERT_EQ(
    voice->pull(),
    message2
  );

  ASSERT_FALSE(voice->something_to_pull());

  voice->push(message3);

  ASSERT_FALSE(voice->something_to_pull());

  delete voice;
}
