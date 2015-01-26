#include "../../fleaux/headers/editor.hh"
#include "../../fleaux/headers/ieditor.hh"
#include "gtest/gtest.h"
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
                ed1 = new Editor("test/editor/move_test_input.txt");
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
        ICursor* curs0 = ed0->getCursor();
        curs0->insert("this is the first line\n");
        curs0->insert("this is the second line\n");
        curs0->insert("this is the third line\nthis is the fourth line\nthis is the fifth line\n");
        test_buf << *ed0;
        ASSERT_STREQ(str_buf.str().c_str(), test_buf.str().c_str());
    }

    TEST_F(FleauxEditorTest, RemoveTest)
    {
        ifstream result_file("test/editor/remove_test_result.txt");
        stringstream expected_result;
        expected_result << result_file.rdbuf();
        ICursor* curs0 = ed0->getCursor();
        ifstream input_file("test/editor/remove_test_input.txt");
        stringstream input;
        input << input_file.rdbuf();
        curs0->insert(input.str());
        curs0->remove(-23);
        stringstream result;
        result << *ed0;
        ASSERT_STREQ(expected_result.str().c_str(), result.str().c_str());
    }

    TEST_F(FleauxEditorTest, MoveTest)
    {
        ifstream result_file("test/editor/move_test_result.txt");
        stringstream sResult;
        sResult << result_file.rdbuf();
        /* go through the maze and leave breadcrumbs(.) */
        /* start at beginning of buffer */
        ICursor* curs0 = ed1->getCursor();
        curs0->remove(-2);
        curs0->insert(".\n");
        curs0->move(0, -1);
        /* up 5 */
        for (int i = 0; i < 5; i++) { 
            curs0->move(0, -1);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }
        /* right 5 */
        curs0->remove(6);
        curs0->insert("......");
        curs0->move(-1, 0);
        /* up 2 */
        for (int i = 0; i < 2; i++) {
            curs0->move(0, -1);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }
        /* 4x {     */
            /* left 1   */
            /* up 1     */
        /* }        */
        for (int i = 0; i < 4; i++) {
            curs0->remove(-1);
            curs0->insert(".");
            curs0->move(-1, 0);
            curs0->move(0, -1);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }
        /* 4x {     */
            /* right 1  */
            /* up 1     */
        /* }        */
        for (int i = 0; i < 4; i++) {
            curs0->move(1, 0);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
            curs0->move(0, -1);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }
        /* right 5  */
        curs0->remove(6);
        curs0->insert("......");
        curs0->move(-1, 0);
        /* down 2   */
        for (int i = 0; i < 2; i++) {
            curs0->move(0, 1);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }
        /* right 3  */
        curs0->remove(4);
        curs0->insert("....");
        curs0->move(-1, 0);
        /* up-right 3 */
        for (int i = 0; i < 3; i++) {
            curs0->move(0, -1);
            curs0->move(1, 0);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }
        /* up 10    */
        for (int i = 0; i < 10; i++) {
            curs0->move(0, -1);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }
        /* left 4   */
        curs0->remove(-4);
        curs0->insert("....");
        curs0->move(-4, 0);
        /* down 8   */
        for (int i = 0; i < 8; i++) {
            curs0->move(0, 1);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }
        /* left 12  */
        curs0->remove(-12);
        curs0->insert("............");
        curs0->move(-12, 0);
        /* up 2     */
        for (int i = 0; i < 2; i++) {
            curs0->move(0, -1);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }
        /* right 9  */
        curs0->remove(10);
        curs0->insert("..........");
        curs0->move(-1, 0);
        /* up 8     */
        for (int i = 0; i < 8; i++) {
            curs0->move(0, -1);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }
        /* left 3   */
        curs0->remove(-3);
        curs0->insert("...");
        curs0->move(-3, 0);
        /* 2x {     */
            /* down 1   */
            /* left 1   */
        /* }        */
        for (int i = 0; i < 2; i++) {
            curs0->move(0, 1);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
            curs0->remove(-1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }
        /* down 2   */
        for (int i = 0; i < 2; i++) {
            curs0->move(0, 1);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }
        /* left 4   */
        curs0->remove(-4);
        curs0->insert("....");
        curs0->move(-4, 0);
        /* up 4     */
        for (int i = 0; i < 4; i++) {
            curs0->move(0, -1);
            curs0->remove(1);
            curs0->insert(".");
            curs0->move(-1, 0);
        }

        /* reverse adding stars(*) at turns and endpoints */
        Cursor curs1(ed1);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* down 4   */
        curs1.move(0, 4);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* right 4  */
        curs1.move(4, 0);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* up 2     */
        curs1.move(0, -2);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* 2x {     */
            /* right 1 */
            /* up 1    */
        /* }        */
        curs1.move(2, 0);
        curs1.move(0, -2);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* right 3 */
        curs1.move(3, 0);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* down 8 */
        curs1.move(0, 8);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* left 9 */
        curs1.move(-9, 0);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* down 2 */
        curs1.move(0, 2);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* right 12 */
        curs1.move(12, 0);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* up 8 */
        curs1.move(0, -8);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* right 4 */
        curs1.move(4, 0);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* down 10 */
        curs1.move(0, 10);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* down-left 3 */
        curs1.move(-3, 0);
        curs1.move(0, 3);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* left 3 */
        curs1.move(-3, 0);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* up 2 */
        curs1.move(0, -2);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* left 5 */
        curs1.move(-5, 0);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* 4x {     */
            /* down 1   */
            /* left 1   */
        /* }        */
        /* 4x {     */
            /* down 1   */
            /* right 1   */
        /* }        */
        curs1.move(0, 8);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* down 2   */
        curs1.move(0, 2);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* left 5   */
        curs1.move(-5, 0);
        curs1.replace(1, "*");
        curs1.move(-1, 0);
        /* down 5   */
        curs1.move(0, 5);
        curs1.replace(1, "*");

        stringstream actual;
        actual << *ed1;
        ASSERT_STREQ(sResult.str().c_str(), actual.str().c_str());
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
