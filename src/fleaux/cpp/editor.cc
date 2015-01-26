#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include "../headers/editor.hh"
#include "../headers/ieditor.hh"
#include "../../../deps/libsdnb/include/sdnb/gap_vector.hh"

using namespace std;
using namespace SDNB;

/* Fleaux::Editor */

/**
 * Default constructor
 */
Fleaux::Editor::Editor(void) : size(0)
{
    _data = new GapVector<char>();
    if (_data == NULL) {
        cerr << "ERROR: new failed for SDNB::GapVector<char>" << endl;
        exit(1);
    }

    _cursor = new Cursor(this, true);
    if (_cursor == NULL) {
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
    _data = new GapVector<char>();
    if (_data == NULL) {
        cerr << "ERROR: new failed for SDNB::GapVector<char>" << endl;
        exit(1);
    }
    _cursor = new Cursor(this, true);
    if (_cursor == NULL) {
        cerr << "ERROR: new failed for Fleaux::Cursor" << endl;
        exit(1);
    }
    readFromFile(path);
}

/**
 * Deallocates the editor's internal data and cursor members
 */
Fleaux::Editor::~Editor(void)
{
    delete _data;
    delete _cursor;
    size = 0;
}

/**
 * Sends the Editor's buffer content to the outputstream os
 */
ostream&
Fleaux::operator<<(ostream& os, const Fleaux::Editor& ed)
{
    string* editorContent = new string(ed._data->begin(), ed._data->end());
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
    ed._cursor->insert(*input);
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
    if (_data != NULL) {
        delete _data;
        size = 0;
        _data = NULL;
    }
    _data = new GapVector<char>();
    if (_data == NULL) {
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
Fleaux::Cursor::Cursor(Editor* ed, bool callerIsEditor) : _editor(ed)
{
    if (callerIsEditor) {
        _index = 0;
        _x = 0;
        _y = 0;
    } else {
        _index = ed->_cursor->_index;
        _x = ed->_cursor->_x;
        _y = ed->_cursor->_y;
    }
}

/**
 * Inserts the input string at the cursor's current position in the editor's
 * buffer.
 */
void
Fleaux::Cursor::insert(const string& input)
{
    _editor->_data->insert(input.begin(), input.end());
    _index += input.size();
    _editor->size += input.size();
    _setXY();
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
    length = _editor->_data->remove(length);
    if (length < 0) {
        _index += length;
        _setXY();
    }
    _editor->size -= abs(length);
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
    size_t oldIndex = _index;
    if (offset < 0) {
        offset = max(offset, -(int)_x);
    } else if (offset > 0) {
        size_t rIndex = _x;
        GapVector<char>::iterator it = _editor->_data->begin() + _index;
        GapVector<char>::iterator end = _editor->_data->end();
        while (it != end && *(it++) != '\n') {
            rIndex++;
        }
        offset = min(offset, (int)rIndex);
    }
    _x += offset;
    _index += offset;
    _editor->_data->moveGap((int)_index - (int)oldIndex);
}

/**
 * Moves the Cursor vertically by n characters, where n = |length|.
 * The direction is determined by the sign of offset: (-) moves up, (+) moves down.
 */
void
Fleaux::Cursor::__moveY(int offset)
{
    size_t oldIndex = _index;
    if (offset < 0) {
        offset = max(offset, -(int)_y);
    }
    _y += offset;
    _setIndex();
    _editor->_data->moveGap((int)_index - (int)oldIndex);
}

/**
 * Calculates the cursor's new x and/or y value(s) after updating its index.
 */
void
Fleaux::Cursor::_setXY(void)
{
    if (_editor->size > 0) {
        GapVector<char>::iterator it = _editor->_data->begin();
        GapVector<char>::iterator end = it + _index;
        _x = 0;
        _y = 0;
        while (it++ != end) {
            if (*(it - 1) == '\n') {
                _x = 0;
                _y++;
            } else {
                _x++;
            }
        }
    }
}

/**
 * Calculates the cursor's new index after updating it's x and/or y value(s).
 */
void
Fleaux::Cursor::_setIndex(void)
{
    if (_editor->size > 0) {
        _index = 0;
        GapVector<char>::iterator it = _editor->_data->begin();
        GapVector<char>::iterator end = _editor->_data->end();
        for (size_t y = 0; y < _y; y++) {
            for (;it != end && *it != '\n'; it++) {
                _index++;
            }
            it++;
            _index++;
        }

        for (size_t x = 0; x < _x && it != end && *(it++) != '\n'; x++) {
            _index++;
        }
    }
}
