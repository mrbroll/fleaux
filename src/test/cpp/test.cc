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

    TEST_F(SDNBEditorTest, AddTest)
    {
        sdnb_editor_addChar(ed0, 'a', 0);
        sdnb_editor_addChar(ed0, 'b', 1);
        sdnb_editor_addChar(ed0, 'c', 2);
        sdnb_editor_addStr(ed0, " This is a test string", 3);
        const char *testStr0 = "abc This is a test string";
        char dataStr0[25] = "";
        sdnb_editor_getData(ed0, dataStr0, 0, sdnb_editor_getLength(ed0) - 1);
        ASSERT_STREQ(testStr0, dataStr0);
    }
}




int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
