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
        sdnb_editor_addStrXY(ed0, "hey", 0, 0);
        sdnb_editor_addStrXY(ed0, "you", 0, 1);
        sdnb_editor_addStrXY(ed0, "there", 0, 2);
        sdnb_editor_addStrXY(ed0, " huge", 9, 3);
        const char *testStr0 = "hey\nyou\nthere\nthis is a huge test string";
        char dataStr0[64] = "";
        sdnb_editor_getData(ed0, dataStr0, 0, sdnb_editor_getLength(ed0) - 1);
        ASSERT_STREQ(testStr0, dataStr0);

        // out of bounds
        sdnb_editor_addStrXY(ed0, "\nsomething crazy", 1000, 3); //should just append to line
        const char *testStr1 = "hey\nyou\nthere\nthis is a huge test string\nsomething crazy";
        char dataStr1[128] = "";
        sdnb_editor_getData(ed0, dataStr1, 0, sdnb_editor_getLength(ed0) - 1);
        ASSERT_STREQ(testStr1, dataStr1);

        //might change this behavior to be more like the XY version
        sdnb_editor_addStr(ed0, "not going to work", 1000); //should fail silently
        char dataStr2[128] = "";
        sdnb_editor_getData(ed0, dataStr2, 0, sdnb_editor_getLength(ed0) - 1);
        ASSERT_STREQ(testStr1, dataStr2);
    }

    TEST_F(SDNBEditorTest, RemoveCharTest)
    {
        sdnb_editor_addStr(ed0, "11223344556677889900", sdnb_editor_getCursor(ed0).index);
        for (size_t i = 0; i < 10; i++) {
            sdnb_editor_removeChar(ed0, i);
        }
        const char *testStr0 = "1234567890";
        char dataStr0[16] = "";
        sdnb_editor_getData(ed0, dataStr0, 0, sdnb_editor_getLength(ed0) - 1);
        ASSERT_STREQ(testStr0, dataStr0);

        sdnb_editor_addStr(ed0, "\n11\n1122\n112233\n11223344",  sdnb_editor_getCursor(ed0).index);
        sdnb_editor_removeCharXY(ed0, 0, 1);
        sdnb_editor_removeCharXY(ed0, 2, 2);
        sdnb_editor_removeCharXY(ed0, 4, 3);
        sdnb_editor_removeCharXY(ed0, 6, 4);
        const char *testStr1 = "1234567890\n1\n112\n11223\n1122334";
        char dataStr1[64] = "";
        sdnb_editor_getData(ed0, dataStr1, 0, sdnb_editor_getLength(ed0) - 1);
        ASSERT_STREQ(testStr1, dataStr1);
    }
}




int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
