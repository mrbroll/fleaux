#ifndef FLEAUX_EDITOR_HH_
#define FLEAUX_EDITOR_HH_

#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include "../../../deps/libsdnb/include/sdnb/gap_vector.hh"

using namespace std;
using namespace SDNB;

namespace Fleaux
{
    class Editor;

    ostream& operator<<(ostream& os, const Editor& ed);
    istream& operator>>(istream& is, Editor& ed);

    class Cursor;

    class Editor
    {
        friend class Cursor;

        public:
            /* constructor(s) & destructor(s) */
            Editor(void);
            Editor(const string& path);
            Editor(const Editor& ed);
            ~Editor(void);

            /* member functions */
            friend ostream& operator<<(ostream& os, const Editor& ed);
            friend istream& operator>>(istream& is, Editor& ed);
            void readFromFile(const string& path);
            void writeToFile(const string& path);
            size_t getSize(void) const { return __size; };
            const void* getData(void) const { return static_cast<void*>(__data); };
            Cursor* getCursor(void) const { return __cursor; };
            
        private:
            /* data members */
            size_t __size;
            Cursor* __cursor;
            GapVector<char>* __data;
    };

    class Cursor
    {
        public:
            /* constructor(s) & destructor(s) */
            Cursor(void) : __index(0), __x(0), __y(0), __editor(NULL) {};
            Cursor(Editor* ed);
            Cursor(const Cursor& curs);
            
            /* member functions */
            void insert(const string& input);
            void remove(int length);
            void replace(int length, const string& replacement);
            const Editor* getEditor(void) const { return __editor; };
            size_t getIndex(void) const { return __index; };
            size_t getX(void) const { return __x; };
            size_t getY(void) const { return __y; };
            void move(int offsetX, int offsetY);



        private:
            /* data members */
            size_t __index;
            size_t __x;
            size_t __y;
            Editor* __editor;

            /* member functions */
            void __setXY(void);
            void __setIndex(void);
            void __moveX(int offset);
            void __moveY(int offset);
    };
}
#endif
