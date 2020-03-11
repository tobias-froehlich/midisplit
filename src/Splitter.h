#ifndef SPLITTER_H_
#define SPLITTER_H_

class Splitter {
  private:
    std::vector< Voice > zVoices{};
    std::deque< std::vector < unsigned char > >
     zMessages{};
  public:
    Splitter();
    ~Splitter();
    void read_parameter_file(std::string filename);
    int get_num_of_voices();
    void push(std::vector< unsigned char > message);
    int something_to_pull();
    std::vector< unsigned char > pull();
};

#endif
