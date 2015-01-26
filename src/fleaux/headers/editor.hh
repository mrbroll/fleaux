#ifndef FLEAUX_EDITOR_HH_
#define FLEAUX_EDITOR_HH_

#include <cstdlib>
#include "ieditor.hh"
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

    class Editor : public IEditor
    {
        friend class Cursor;

        public:
            /* data members */
            size_t size;   

            /* constructor(s) & destructor(s) */
            Editor(void);
            Editor(const string& path);
            ~Editor(void);

            /* member functions */
            friend ostream& operator<<(ostream& os, const Editor& ed);
            friend istream& operator>>(istream& is, Editor& ed);
            void readFromFile(const string& path);
            void writeToFile(const string& path);
            ICursor* getCursor(void) { return (ICursor*)_cursor; };
            
        protected:
            /* data members */
            Cursor* _cursor;
            GapVector<char>* _data;
    };

    class Cursor : public ICursor
    {
        public:
            /* constructor(s) & destructor(s) */
            Cursor(Editor* ed);
            Cursor(const ICursor& curs);
            
            /* member functions */
            void insert(const string& input);
            void remove(int length);
            void replace(int length, const string& replacement);
            IEditor* getEditor(void) const { return (IEditor*)_editor; };
            size_t getIndex(void) const { return _index; };
            size_t getX(void) const { return _x; };
            size_t getY(void) const { return _y; };
            void move(int offsetX, int offsetY);

        protected:
            /* data members */
            size_t _index;
            size_t _x;
            size_t _y;
            Editor* _editor;

            /* member functions */
            void _setXY(void);
            void _setIndex(void);

        private:
            void __moveX(int offset);
            void __moveY(int offset);
    };
}
#endif
