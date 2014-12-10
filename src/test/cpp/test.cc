#include "../../fleaux/c/plugin_manager/plugin_manager.h"
#include "../../fleaux/headers/fleaux.h"
#include "../../sdnb_editor/headers/sdnb_editor.h"
#include "gtest/gtest.h"

using namespace std;

namespace {
    class SDNBEditorTest : public ::testing::Test
    {
        protected:
            SDNBEditorTest()
            {
                ed0 = (fl_editor_t *)malloc(sizeof(fl_editor_t));
                sdnb_editor_init(ed0, "/no/path");
            }

            ~SDNBEditorTest()
            {
                sdnb_editor_cleanup(ed0);
                free(ed0);
            }

            fl_editor_t *ed0;
    };

    TEST_F(SDNBEditorTest, AddCharTest)
    {
        sdnb_editor_addChar(ed0, 'a', sdnb_editor_getCursor(ed0).index);
        sdnb_editor_addChar(ed0, 'b', sdnb_editor_getCursor(ed0).index);
        sdnb_editor_addChar(ed0, 'c', sdnb_editor_getCursor(ed0).index);
        sdnb_editor_addChar(ed0, 'x', 0);
        sdnb_editor_addChar(ed0, 'y', 2);
        sdnb_editor_addChar(ed0, 'z', 4);
        sdnb_editor_addChar(ed0, '\n', sdnb_editor_getCursor(ed0).index);
        sdnb_editor_addChar(ed0, '\n', sdnb_editor_getCursor(ed0).index);
        sdnb_editor_addCharXY(ed0, 'v', 0, 1);
        sdnb_editor_addCharXY(ed0, 'u', 0, 1);
        sdnb_editor_addCharXY(ed0, 'h', sdnb_editor_getCursor(ed0).x, sdnb_editor_getCursor(ed0).y);
        //sdnb_editor_addStr(ed0, " This is a test string", 3);
        const char *testStr0 = "xaybzc\nuv\nh";
        char dataStr0[25] = "";
        sdnb_editor_getData(ed0, dataStr0, 0, sdnb_editor_getLength(ed0) - 1);
        ASSERT_STREQ(testStr0, dataStr0);
    }

    TEST_F(SDNBEditorTest, AddStrTest)
    {
        sdnb_editor_addStr(ed0, "this is a", sdnb_editor_getCursor(ed0).index);
        sdnb_editor_addStr(ed0, " test string", sdnb_editor_getCursor(ed0).index);
        sdnb_editor_addStr(ed0, "\n\n\n", 0);
        /*sdnb_editor_addStrXY(ed0, "hey", 0, 0);
        sdnb_editor_addStrXY(ed0, "you", 0, 1);
        sdnb_editor_addStrXY(ed0, "there", 0, 2);
        sdnb_editor_addStrXY(ed0, " huge", 9, 3);*/
        //const char *testStr0 = "hey\nyou\nthere\nthis is a huge test string";
        const char *testStr0 = "\n\n\nthis is a test string";
        char dataStr0[64] = "";
        sdnb_editor_getData(ed0, dataStr0, 0, sdnb_editor_getLength(ed0) - 1);
        ASSERT_STREQ(testStr0, dataStr0);
    }
}




int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
