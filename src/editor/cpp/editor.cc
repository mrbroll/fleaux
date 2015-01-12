#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "../../../deps/libuv/include/uv.h"
#include "../headers/editor.hh"
#include "../../../deps/libsdnb/include/sdnb/gap_vector.hh"

using namespace std;
using namespace SDNB;

/* Fleaux::Editor */
Fleaux::Editor::Editor(void)
{
    __data = new GapVector<char>();
    __cursor = new UserCursor();
}

Fleaux::Editor::~Editor(void)
{
    delete __data;
    delete __cursor;
}

Fleaux::Editor&
Fleaux::Editor::operator=(const Fleaux::Editor& arg)
{
    if (__data != NULL) {
        delete __data;
    }
    
    if (__cursor != NULL) {
        delete __cursor;
    }
    __data = new GapVector<char>(*(arg.__data));
    __cursor = new UserCursor(*(arg.__cursor));
}

ostream&
operator<<(ostream& os, const Fleaux::Editor& ed)
{
    cout << string(ed.__data->begin(), ed.__data->end());
}

istream&
operator>>(istream& is, Fleaux::Editor& ed)
{
    string str;
    cin >> str;
    ed.userCursor().insert(str);
}

Fleaux::Cursor
Fleaux::Editor::begin(void)
{
    Cursor curs;
    return curs;
}

Fleaux::Cursor
Fleaux::Editor::end(void)
{
    Cursor curs;
    curs += __data->size;
    return curs;
}

Fleaux::Cursor
Fleaux::Editor::cursor(void)
{
    return *__cursor;
}

/* Fleaux::Cursor */
char&
Fleaux::Cursor::operator*(void)
{
    GapVector<char>::iterator it = __editor->__data->begin() + __index;
    return *it;
}

Fleaux::Cursor&
Fleaux::Cursor::operator=(const Fleaux::Cursor& curs)
{
    __index = curs.__index;
    __x = curs.__x;
    __y = curs.__y;
    __editor = curs.__editor;
}

Fleaux::Cursor&
Fleaux::Cursor::operator+=(int offset)
{
    if (offset < 0) {
    } else if (offset > 0) {
        
    }

}

Fleaux::Cursor&
Fleaux::Cursor::operator-=(int offset)
{
}

Fleaux::Cursor&
Fleaux::Cursor::operator++(void)
{
}

Fleaux::Cursor&
Fleaux::Cursor::operator--(void)
{
}

Fleaux::Cursor
operator+(Fleaux::Cursor curs, int offset)
{
}

Fleaux::Cursor
operator+(int offset, const Fleaux::Cursor& curs)
{
}

Fleaux::Cursor
operator-(Fleaux::Cursor curs, int offset)
{
}

Fleaux::Cursor
operator-(int offset, const Fleaux::Cursor&)
{
}

void
Fleaux::Cursor::insert(string input)
{
}

void
Fleaux::Cursor::remove(int length)
{
}

void
Fleaux::Cursor::moveVert(int offset)
{
}

void
Fleaux::Cursor::moveHoriz(int offset)
{
}

/* Just start at the beginning and count forward.
 * This is a brute force approach, but I just want to 
 * get this working. Once I do, I can come back and
 * optimize this
 */
void
Fleaux::Cursor::getXY(void)
{
    Fleaux::Cursor curs = __editor->begin();
   size_t i = 0;
   __x = 0;
   __y = 0;
   while (curs < *this) {
        if (*curs == '\n') {
            __y++;
        }

        if (i > 0 && *(curs - 1) == '\n') {
            __x = 0;
        } else {
            __x++;
        }
        curs++;
    }
}
