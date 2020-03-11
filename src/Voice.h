#ifndef VOICE_H_
#define VOICE_H_

class Voice {
  private:
    std::deque< std::vector< unsigned char > >
     zMessages{};
    std::vector< int > zInChannels{};
    int zPedal = 0;
    std::vector< int > zOutChannels{};
    std::vector< unsigned char > zMidiCode{};
    std::vector< int > zOutPressed{};
    std::vector< int > zOutPlaying{};
    std::vector< int > zOffSince{};
  public:
    Voice();
    ~Voice();
    void set_in_channels(
      std::vector< std::string > channels
    );
    void set_out_channels(
      std::vector< std::string > channels
    );
    std::vector< int > get_in_channels();
    std::vector< int > get_out_channels();
    int index_longest_off();
    void push(std::vector< unsigned char > message);
    int something_to_pull();
    std::vector< unsigned char > pull();
};

#endif
