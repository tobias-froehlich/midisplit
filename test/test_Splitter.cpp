#include "../src/Splitter.h"

TEST ( Splitter, create_and_delete ) {
  Splitter* splitter;
  splitter = new Splitter();
  delete splitter;
}

TEST ( Splitter, read_parameter_file ) {
  Splitter* splitter;
  splitter = new Splitter();

  ASSERT_EQ(splitter->get_num_of_voices(), 0);

  splitter->read_parameter_file(
    "../test/testfiles/parameters.txt"
  );

  ASSERT_EQ(splitter->get_num_of_voices(), 2);

  delete splitter;
}
