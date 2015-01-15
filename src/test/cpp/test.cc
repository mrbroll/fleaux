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
    class FleauxEditorTest : public ::testing::Test
    {
        protected:
            FleauxEditorTest()
            {
                ed0 = new Editor();
                ed1 = new Editor();
            }

            ~FleauxEditorTest()
            {
                delete ed0;
                delete ed1;
            }

            Editor* ed0;
            Editor* ed1;
    };

    TEST_F(FleauxEditorTest, InsertTest)
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

    TEST_F(FleauxEditorTest, RemoveTest)
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

    TEST_F(FleauxEditorTest, MoveTest)
    {
        ifstream result_file("test/editor/move_test_result.txt");
        stringstream sResult;
        sResult << result_file.rdbuf();
        ifstream input_file("test/editor/move_test_input.txt");
        stringstream sInput;
        sInput << input_file.rdbuf();
        /* go through the maze and leave breadcrumbs(.) */
        /* start at beginning of buffer */
        ed0->cursor->insert(sInput.str());
        ed0->cursor->remove(-2);
        ed0->cursor->insert(".\n");
        ed0->cursor->moveV(-1);
        /* up 5 */
        for (int i = 0; i < 5; i++) { 
            ed0->cursor->moveV(-1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }
        /* right 5 */
        ed0->cursor->remove(6);
        ed0->cursor->insert("......");
        ed0->cursor->moveH(-1);
        /* up 2 */
        for (int i = 0; i < 2; i++) {
            ed0->cursor->moveV(-1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }
        /* 4x {     */
            /* left 1   */
            /* up 1     */
        /* }        */
        for (int i = 0; i < 4; i++) {
            ed0->cursor->remove(-1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
            ed0->cursor->moveV(-1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }
        /* 4x {     */
            /* right 1  */
            /* up 1     */
        /* }        */
        for (int i = 0; i < 4; i++) {
            ed0->cursor->moveH(1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
            ed0->cursor->moveV(-1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }
        /* right 5  */
        ed0->cursor->remove(6);
        ed0->cursor->insert("......");
        ed0->cursor->moveH(-1);
        /* down 2   */
        for (int i = 0; i < 2; i++) {
            ed0->cursor->moveV(1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }
        /* right 3  */
        ed0->cursor->remove(4);
        ed0->cursor->insert("....");
        ed0->cursor->moveH(-1);
        /* up-right 3 */
        for (int i = 0; i < 3; i++) {
            ed0->cursor->moveV(-1);
            ed0->cursor->moveH(1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }
        /* up 10    */
        for (int i = 0; i < 10; i++) {
            ed0->cursor->moveV(-1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }
        /* left 4   */
        ed0->cursor->remove(-4);
        ed0->cursor->insert("....");
        ed0->cursor->moveH(-4);
        /* down 8   */
        for (int i = 0; i < 8; i++) {
            ed0->cursor->moveV(1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }
        /* left 12  */
        ed0->cursor->remove(-12);
        ed0->cursor->insert("............");
        ed0->cursor->moveH(-12);
        /* up 2     */
        for (int i = 0; i < 2; i++) {
            ed0->cursor->moveV(-1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }
        /* right 9  */
        ed0->cursor->remove(10);
        ed0->cursor->insert("..........");
        ed0->cursor->moveH(-1);
        /* up 8     */
        for (int i = 0; i < 8; i++) {
            ed0->cursor->moveV(-1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }
        /* left 3   */
        ed0->cursor->remove(-3);
        ed0->cursor->insert("...");
        ed0->cursor->moveH(-3);
        /* 2x {     */
            /* down 1   */
            /* left 1   */
        /* }        */
        for (int i = 0; i < 2; i++) {
            ed0->cursor->moveV(1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
            ed0->cursor->remove(-1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }
        /* down 2   */
        for (int i = 0; i < 2; i++) {
            ed0->cursor->moveV(1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }
        /* left 4   */
        ed0->cursor->remove(-4);
        ed0->cursor->insert("....");
        ed0->cursor->moveH(-4);
        /* up 4     */
        for (int i = 0; i < 4; i++) {
            ed0->cursor->moveV(-1);
            ed0->cursor->remove(1);
            ed0->cursor->insert(".");
            ed0->cursor->moveH(-1);
        }

        stringstream actual;
        actual << *ed0;
        ASSERT_STREQ(sResult.str().c_str(), actual.str().c_str());

        /* reverse adding stars(*) at turns and endpoints */
        /* down 4   */
        /* right 4  */
        /* up 2     */
        /* right 1  */
        /* up 1     */
        /* 2x {     */
            /* right 1 */
            /* up 1    */
        /* }        */
        /* right 3 */
        /* down 8 */
        /* left 9 */
        /* down 2 */
        /* right 12 */
        /* up 8 */
        /* right 4 */
        /* down 10 */
        /* down-left 3 */
        /* left 3 */
        /* up 2 */
        /* left 5 */
        /* 4x {     */
            /* down 1   */
            /* left 1   */
        /* }        */
        /* 4x {     */
            /* down 1   */
            /* right 1   */
        /* }        */
        /* down 2   */
        /* left 5   */
        /* down 5   */
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
