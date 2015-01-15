#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include "../../../deps/libsdnb/include/sdnb/gap_vector.hh"

#ifndef FLEAUX_EDITOR_H_
#define FLEAUX_EDITOR_H_

using namespace std;
using namespace SDNB;

namespace Fleaux
{
    class Cursor;

    class Editor
    {
        friend class Cursor;

        public:
            /* data members */
            size_t size;   
            size_t numLines;
            Cursor* cursor;

            /* constructor(s) & destructor(s) */
            Editor(void);
            ~Editor(void);

            /* operator overloads */
            friend ostream& operator<<(ostream& os, const Editor& ed)
            {
                os << string(ed._data->begin(), ed._data->end());
                return os;
            };
            friend istream& operator>>(istream& is, const Editor& ed)
            {
                string str;
                is >> str;
                ed._data->insert(str.begin(), str.end());
                return is;
            };
            
        protected:
            /* data members */
            GapVector<char>* _data;
    };

    class Cursor
    {
        public:
            /* constructor(s) & destructor(s) */
            Cursor(Editor* ed) : _index(0), \
                                 _x(0), \
                                 _y(0), \
                                 _editor(ed) {};
            
            /* member functions */
            void insert(const string& input);
            void remove(int length);
            inline size_t getIndex(void) { return _index; };
            inline size_t getX(void) { return _x; };
            inline size_t getY(void) { return _y; };
            void moveV(int offset);
            void moveH(int offset);

        protected:
            /* data members */
            size_t _index;
            size_t _x;
            size_t _y;
            Editor* _editor;

            /* member functions */
            void _getXY(void);
            void _getIndex(void);
    };
}
#endif
