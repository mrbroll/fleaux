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

    TEST_F(SDNBEditorTest, InsertTest)
    {
        const char *testString0 = "1\n22\n333\n4444\n55555";
        sdnb_editor_insertAtIndex(ed0, "1\n\n4444", 0);
        sdnb_editor_insertAtIndex(ed0, "22\n333", 2);
        sdnb_editor_insertAtIndex(ed0, "\n55555", 999);
        char dataString0[32] = "";
        sdnb_editor_getData(ed0, dataString0,  0, sdnb_editor_getLength(ed0));
        ASSERT_STREQ(testString0, dataString0);

        const char *testString1 = "001\n2112\n33332\n4444993\n555559999";
        sdnb_editor_insertAtXY(ed0, "00", 0, 0);
        sdnb_editor_insertAtXY(ed0, "11", 1, 1);
        sdnb_editor_insertAtXY(ed0, "32", 3, 2);
        sdnb_editor_insertAtXY(ed0, "993", 99, 3);
        sdnb_editor_insertAtXY(ed0, "9999", 99, 99);
        char dataString1[64] = "";
        sdnb_editor_getData(ed0, dataString1, 0, sdnb_editor_getLength(ed0));
        ASSERT_STREQ(testString1, dataString1);
    }

    TEST_F(SDNBEditorTest, RemoveTest)
    {
        const char *testString0 = "\n22\n4444";
        sdnb_editor_insertAtIndex(ed0, "1\n22\n333\n4444\n55555", 0);
        sdnb_editor_removeAtIndex(ed0, 0, 1); //"\n22\n333\n4444\n55555"
        sdnb_editor_removeAtIndex(ed0, 3, 4); //"\n22\n4444\n55555"
        sdnb_editor_removeAtIndex(ed0, 13, 1); //"\n22\n4444\n5555"
        sdnb_editor_removeAtIndex(ed0, 8, 99);
        char dataString0[16] = "";
        sdnb_editor_getData(ed0, dataString0, 0, sdnb_editor_getLength(ed0));
        ASSERT_STREQ(testString0, dataString0);

        sdnb_editor_removeAtIndex(ed0, 0, 99);
        const char *testString1 = "\n22\n4444";
        sdnb_editor_insertAtIndex(ed0, "1\n22\n333\n4444\n55555", 0);
        sdnb_editor_removeAtXY(ed0, 0, 0, 1);
        sdnb_editor_removeAtXY(ed0, 2, 1, 4);
        sdnb_editor_removeAtXY(ed0, 4, 3, 1);
        sdnb_editor_removeAtXY(ed0, 4, 2, 99);
        char dataString1[16] = "";
        sdnb_editor_getData(ed0, dataString1, 0, sdnb_editor_getLength(ed0));
        ASSERT_STREQ(testString1, dataString1);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
