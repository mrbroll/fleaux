#include "../../fleaux/headers/fleaux.h"
#include "../../sdnb_editor/headers/editor.h"
#include "../../sdnb_editor/headers/fs_module.h"
#include "gtest/gtest.h"
#include <uv.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;



namespace {
    class SDNBEditorTest : public ::testing::Test
    {
        protected:
            SDNBEditorTest()
            {
                ed0 = sdnb_editor_create("test/editor/input.txt");
                ed1 = sdnb_editor_create("test/editor/input.txt");
            }

            ~SDNBEditorTest()
            {
                sdnb_editor_destroy(ed0);
                sdnb_editor_destroy(ed1);
            }

            fl_editor_t *ed0;
            fl_editor_t *ed1;
    };

    TEST_F(SDNBEditorTest, InsertTest)
    {
        ifstream file("test/editor/insert_test.txt");
        stringstream str_buf;
        str_buf << file.rdbuf();
        sdnb_editor_insertAtIndex(ed0, "adding something before the first line\n", 0);
        sdnb_editor_insertAtIndex(ed0, " adding something after the second line", 85);
        sdnb_editor_insertAtIndex(ed0, "the middle of ",  133);
        sdnb_editor_insertAtIndex(ed0, "and this is the end of the buffer\n", 999);
        char dataString0[sdnb_editor_getLength(ed0)];
        sdnb_editor_getData(ed0, dataString0,  0, sdnb_editor_getLength(ed0));
        ASSERT_STREQ(str_buf.str().c_str(), dataString0);

        sdnb_editor_insertAtXY(ed1, "adding something before the first line\n", 0, 0);
        sdnb_editor_insertAtXY(ed1, " adding something after the second line", 23, 2);
        sdnb_editor_insertAtXY(ed1, "the middle of ", 8, 3);
        sdnb_editor_insertAtXY(ed1, "and this is the end of the buffer\n", 99, 99);
        char dataString1[sdnb_editor_getLength(ed1)];
        sdnb_editor_getData(ed1, dataString1, 0, sdnb_editor_getLength(ed1));
        ASSERT_STREQ(str_buf.str().c_str(), dataString1);
    }

    TEST_F(SDNBEditorTest, RemoveTest)
    {
        ifstream file("test/editor/remove_test.txt");
        stringstream str_buf;
        str_buf << file.rdbuf();
        sdnb_editor_removeAtIndex(ed0, 0, 5);
        sdnb_editor_removeAtIndex(ed0, 30, 7);
        sdnb_editor_removeAtIndex(ed0, 52, 5);
        sdnb_editor_removeAtIndex(ed0, 54, 24);
        sdnb_editor_removeAtIndex(ed0, 67, 8);
        char dataString0[sdnb_editor_getLength(ed0)];
        sdnb_editor_getData(ed0, dataString0, 0, sdnb_editor_getLength(ed0));
        ASSERT_STREQ(str_buf.str().c_str(), dataString0);

        sdnb_editor_removeAtXY(ed1, 0, 0, 5);
        sdnb_editor_removeAtXY(ed1, 12, 1, 7);
        sdnb_editor_removeAtXY(ed1, 17, 2, 5);
        sdnb_editor_removeAtXY(ed1, 0, 3, 24);
        sdnb_editor_removeAtXY(ed1, 14, 3, 8);
        char dataString1[sdnb_editor_getLength(ed1)];
        sdnb_editor_getData(ed1, dataString1, 0, sdnb_editor_getLength(ed1));
        ASSERT_STREQ(str_buf.str().c_str(), dataString1);
    }

    TEST_F(SDNBEditorTest, MoveTest)
    {
        ifstream file("test/editor/move_test.txt");
        stringstream str_buf;
        str_buf << file.rdbuf();
        sdnb_editor_moveCursorToIndex(ed0, 0);
        sdnb_editor_insertAtCursor(ed0, "adding something before the first line\n", sdnb_editor_getCursor(ed0));
        sdnb_editor_moveCursorToXY(ed0, 0, 1);
        sdnb_editor_removeAtCursor(ed0, sdnb_editor_getCursor(ed0), 5);
        sdnb_editor_moveCursorToXY(ed0, 16, 2);
        sdnb_editor_removeAtIndex(ed0, sdnb_editor_getCursor(ed0).index - 4, 7);
        sdnb_editor_moveCursorToXY(ed0, 99, 3);
        sdnb_editor_removeAtXY(ed0, 11, 3, 11);
        sdnb_editor_moveCursorToXY(ed0, 999, 4);
        sdnb_editor_removeAtXY(ed0, 0, 4, 24);
        sdnb_editor_insertAtCursor(ed0, "replacing line four\n", sdnb_editor_getCursor(ed0));
        sdnb_editor_moveCursorToIndex(ed0, 999);
        sdnb_editor_insertAtCursor(ed0, "and this is the end of the buffer\n", sdnb_editor_getCursor(ed0));
        char testString0[sdnb_editor_getLength(ed0)];
        sdnb_editor_getData(ed0, testString0, 0, sdnb_editor_getLength(ed0));
        ASSERT_STREQ(str_buf.str().c_str(), testString0);

        sdnb_editor_writeFile(ed0, "test/editor/output.txt");
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
