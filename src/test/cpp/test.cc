#include "../../editor/headers/editor.hh"
#include "gtest/gtest.h"
#include <uv.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;
using namespace Fleaux;


namespace {
    class SDNBEditorTest : public ::testing::Test
    {
        protected:
            SDNBEditorTest()
            {
                ed0 = new Editor();
                ed1 = new Editor();
            }

            ~SDNBEditorTest()
            {
                delete ed0;
                delete ed1;
            }

            Editor* ed0;
            Editor* ed1;
    };

    TEST_F(SDNBEditorTest, InsertTest)
    {
        ifstream file("test/editor/insert_test_result.txt");
        stringstream str_buf;
        stringstream test_buf;
        str_buf << file.rdbuf();
        ed0->cursor->insert("this is the first line\n");
        ed0->cursor->insert("this is the second line\n");
        ed0->cursor->insert("this is the third line\nthis is the fourth line\nthis is the fifth line\n");
        test_buf << *ed0;
        ASSERT_STREQ(str_buf.str().c_str(), test_buf.str().c_str());
    }

    TEST_F(SDNBEditorTest, RemoveTest)
    {
        ifstream result_file("test/editor/remove_test_result.txt");
        stringstream expected_result;
        expected_result << result_file.rdbuf();
        ifstream input_file("test/editor/remove_test_input.txt");
        stringstream input;
        input << input_file.rdbuf();
        ed0->cursor->insert(input.str());
        ed0->cursor->remove(-23);
        stringstream result;
        result << *ed0;
        ASSERT_STREQ(expected_result.str().c_str(), result.str().c_str());
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
