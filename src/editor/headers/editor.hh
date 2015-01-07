#include <cstdlib>
#include <iostream>
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
            /* constructor(s) & destructor(s) */
            Editor(void);
            ~Editor(void);

            /* operator overloads */
            Editor& operator=(const Editor& arg);
            friend ostream& operator<<(ostream& os, const Editor& ed);
            friend istream& operator>>(istream& is, const Editor& ed);

            /* member functions */
            void insert(Cursor curs, string input);
            void remove(Cursor curs, int length);
            Cursor& begin(void);
            Cursor& end(void);
            Cursor& getCursor(size_t index = 0);
            Cursor& getCursor(size_t x = 0, size_t y = 0);
            
        private:
            /* data members */
            GapVector<char> __data;
            Cursor __curs;

    };

    class Cursor
    {
        public:
            /* data members */
            size_t index;
            size_t x;
            size_t y;

            /* constructor(s) & destructor(s) */
            Cursor(void) : index(0), \
                           x(0), \
                           y(0), \
                           __parent(NULL) {};

            Cursor(const Cursor& curs) : index(curs.index), \
                                         x(curs.x), \
                                         y(curs.y), \
                                         __parent(curs.__parent){};

            Cursor(Editor* ed, size_t index = 0);
            Cursor(Editor* ed, size_t x = 0, size_t y = 0);
            
            /* operator overloads */
            char operator*(void);
            Cursor& operator=(const Cursor& curs);
            Cursor& operator+=(const Cursor& curs);
            Cursor& operator+=(int offset);
            Cursor& operator-=(const Cursor& curs);
            Cursor& operator-=(int offset);
            Cursor& operator++(void); //post
            Cursor operator++(int); //pre
            Cursor& operator--(void); //post
            Cursor operator--(int); //pre

            friend Cursor operator+(Cursor lhs, const Cursor& rhs);
            friend Cursor operator+(Cursor curs, int offset);
            friend Cursor operator+(int offset, const Cursor& curs);
            friend Cursor operator-(Cursor lhs, const Cursor& rhs);
            friend Cursor operator-(Cursor curs, int offset);
            friend Cursor operator-(int offset, const Cursor& curs);
            bool operator<(const Cursor& curs);
            bool operator>(const Cursor& curs);
            bool operator<=(const Cursor& curs);
            bool operator>=(const Cursor& curs);
            bool operator==(const Cursor& curs);
            bool operator!=(const Cursor& curs);

            
            /* member functions */
            void move(size_t index);
            void move(size_t x, size_t y);

        private:
            /* data members */
            Editor* __parent;

            /* member functions */
    };

}
#endif
