#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include "../headers/editor.hh"
#include "../../../deps/libsdnb/include/sdnb/gap_vector.hh"

using namespace std;
using namespace SDNB;

/* Fleaux::Editor */

/**
 * Default constructor
 */
Fleaux::Editor::Editor(void) : __size(0)
{
    __data = new GapVector<char>();
    if (__data == NULL) {
        cerr << "ERROR: new failed for SDNB::GapVector<char>" << endl;
        exit(1);
    }
    __cursor = NULL;
    __cursor = new Cursor(this);
    if (__cursor == NULL) {
        cerr << "ERROR: new failed for Fleaux::Cursor" << endl;
        exit(1);
    }
}

/**
 * This constructor overload acts the same as the default constructor, except
 * it also fills the editor's buffer with the contents of the file at 'path'.
 * This is the same as calling the default constructor followed by readFromFile(path).
 */
Fleaux::Editor::Editor(const string& path)
{
    __data = new GapVector<char>();
    if (__data == NULL) {
        cerr << "ERROR: new failed for SDNB::GapVector<char>" << endl;
        exit(1);
    }
    __cursor = NULL;
    __cursor = new Cursor(this);
    if (__cursor == NULL) {
        cerr << "ERROR: new failed for Fleaux::Cursor" << endl;
        exit(1);
    }
    readFromFile(path);
}

/**
 * Copy Constructor
 */
Fleaux::Editor::Editor(const Fleaux::Editor& ed)
{
    __size = ed.__size;
    __cursor = new Fleaux::Cursor(*ed.__cursor);
    __data = new GapVector<char>(*ed.__data);
}

/**
 * Deallocates the editor's internal data and cursor members
 */
Fleaux::Editor::~Editor(void)
{
    delete __data;
    delete __cursor;
    __size = 0;
}

/**
 * Sends the Editor's buffer content to the outputstream os
 */
ostream&
Fleaux::operator<<(ostream& os, const Fleaux::Editor& ed)
{
    string* editorContent = new string(ed.__data->begin(), ed.__data->end());
    os << *editorContent;
    delete editorContent;
    return os;
}

/**
 * Reads content from the input stream is and inserts it at the current cursor
 * position.
 */
istream&
Fleaux::operator>>(istream& is, Fleaux::Editor& ed)
{
    istreambuf_iterator<char> begin(is);
    istreambuf_iterator<char> end;
    string* input = new string(begin, end);
    ed.__cursor->insert(*input);
    delete input;
    return is;
}

/**
 * Replaces the contents of the editor's buffer with the contents of the file
 * at 'path'
 */
void
Fleaux::Editor::readFromFile(const string& path)
{
    if (__data != NULL) {
        delete __data;
        __size = 0;
        __data = NULL;
    }
    __data = new GapVector<char>();
    if (__data == NULL) {
        cerr << "ERROR: new failed for SDNB::GapVector<char>" << endl;
        exit(1);
    }
    ifstream inputFile(path.c_str());
    inputFile >> *this;
    inputFile.close();
}

/**
 * Writes the editor's buffer contents to the file at 'path'
 */
void
Fleaux::Editor::writeToFile(const string& path)
{
    ofstream outputFile(path.c_str());
    outputFile << *this;
    outputFile.close();
}

/* Fleaux::Cursor */

/**
 * Constructor creates a new Cursor instance that belongs to the editor pointed
 * to by ed. The optional callerIsEditor parameter is used to determine if this
 * is being called from an Editor's constructor, or by another entity. If the
 * call is being made outside of an editor's constructor, then the new cursor is
 * merely a copy of ed's
 */
Fleaux::Cursor::Cursor(Fleaux::Editor* ed) : __editor(ed)
{
    if (ed->__cursor == NULL) {
        __index = 0;
        __x = 0;
        __y = 0;
        ed->__cursor = this;
    } else {
        const Cursor* curs = ed->getCursor();
        __index = curs->getIndex();
        __x = curs->getX();
        __y = curs->getY();
    }
}

/**
 * Copy constructor - Simple shallow copy.
 */
Fleaux::Cursor::Cursor(const Fleaux::Cursor& curs)
{ 
    __editor = curs.__editor;
    __index = curs.__index;
    __x = curs.__x;
    __y = curs.__y;
}

/**
 * Inserts the input string at the cursor's current position in the editor's
 * buffer.
 */
void
Fleaux::Cursor::insert(const string& input)
{
    __editor->__data->insert(input.begin(), input.end());
    __index += input.size();
    __editor->__size += input.size();
    __setXY();
}

/**
 * Removes n characters, where n is |length|. The sign of
 * length determines the direction from the cursor in which characters are
 * removed: (-) removes backward starting at the character behind the cursor, 
 * (+) removes forward starting at the character under the cursor.
 */
void
Fleaux::Cursor::remove(int length)
{
    length = __editor->__data->remove(length);
    if (length < 0) {
        __index += length;
        __setXY();
    }
    __editor->__size -= abs(length);
}

/**
 * Replaces n characters in where n is |length| with the replacement string.
 * This is the same as Fleaux::Cursor::remove(length) followed by
 * Fleaux::Cursor::insert(replacement).
 */
void
Fleaux::Cursor::replace(int length, const string& replacement)
{
    remove(length);
    insert(replacement);
}

/**
 * Moves the cursor relative to its current x,y position.
 * For example: calling move(-4, 27) will move the cursor left 4 characters
 * and down 27 characters
 */
void
Fleaux::Cursor::move(int offsetX, int offsetY)
{
    if (offsetY != 0) {
        __moveY(offsetY);
    }

    if (offsetX != 0) {
        __moveX(offsetX);
    }
}

/**
 * Moves the Cursor horizontally by n characters, where n = |length|.
 * The direction is determined by the sign of offset: (-) moves left, (+) moves right.
 */
void
Fleaux::Cursor::__moveX(int offset)
{
    size_t oldIndex = __index;
    if (offset < 0) {
        offset = max(offset, -(int)__x);
    } else if (offset > 0) {
        size_t rIndex = __x;
        GapVector<char>::iterator it = __editor->__data->begin() + __index;
        GapVector<char>::iterator end = __editor->__data->end();
        while (it != end && *(it++) != '\n') {
            rIndex++;
        }
        offset = min(offset, (int)rIndex);
    }
    __x += offset;
    __index += offset;
    __editor->__data->moveGap((int)__index - (int)oldIndex);
}

/**
 * Moves the Cursor vertically by n characters, where n = |length|.
 * The direction is determined by the sign of offset: (-) moves up, (+) moves down.
 */
void
Fleaux::Cursor::__moveY(int offset)
{
    size_t oldIndex = __index;
    if (offset < 0) {
        offset = max(offset, -(int)__y);
    }
    __y += offset;
    __setIndex();
    __editor->__data->moveGap((int)__index - (int)oldIndex);
}

/**
 * Calculates the cursor's new x and/or y value(s) after updating its index.
 */
void
Fleaux::Cursor::__setXY(void)
{
    if (__editor->__size > 0) {
        GapVector<char>::iterator it = __editor->__data->begin();
        GapVector<char>::iterator end = it + __index;
        __x = 0;
        __y = 0;
        while (it++ != end) {
            if (*(it - 1) == '\n') {
                __x = 0;
                __y++;
            } else {
                __x++;
            }
        }
    }
}

/**
 * Calculates the cursor's new index after updating it's x and/or y value(s).
 */
void
Fleaux::Cursor::__setIndex(void)
{
    if (__editor->__size > 0) {
        __index = 0;
        GapVector<char>::iterator it = __editor->__data->begin();
        GapVector<char>::iterator end = __editor->__data->end();
        for (size_t y = 0; y < __y; y++) {
            for (;it != end && *it != '\n'; it++) {
                __index++;
            }
            it++;
            __index++;
        }

        for (size_t x = 0; x < __x && it != end && *(it++) != '\n'; x++) {
            __index++;
        }
    }
}
