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

    ifstream inputFile(path.c_str());
    stringstream fileContents;
    fileContents << inputFile.rdbuf();
    _cursor->insert(fileContents.str());
    inputFile.close();

    size = fileContents.str().size();
}

Fleaux::Editor::~Editor(void)
{
    delete _data;
    delete _cursor;
}

/* Fleaux::Cursor */
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
void
Fleaux::Cursor::insert(const string& input)
{
    _editor->_data->insert(input.begin(), input.end());
    _index += input.size();
    _editor->size += input.size();
    _setXY();
}

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

void
Fleaux::Cursor::replace(int length, const string& replacement)
{
    remove(length);
    insert(replacement);
}

void
Fleaux::Cursor::moveY(int offset)
{
    size_t oldIndex = _index;
    if (offset < 0) {
        offset = max(offset, -(int)_y);
    }
    _y += offset;
    _setIndex();
    _editor->_data->moveGap((int)_index - (int)oldIndex);
}

void
Fleaux::Cursor::moveX(int offset)
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

/* Just start at the beginning and count forward.
 * This is a brute force approach, but I just want to 
 * get this working. Once I do, I can come back and
 * optimize these
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
