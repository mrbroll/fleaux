#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include "../../../deps/libsdnb/include/sdnb/gap_vector.hh"

#ifndef FLEAUX_EDITOR_H_
#define FLEAUX_EDITOR_H_

using namespace std;
using namespace SDNB;

namespace Fleaux
{
    class Cursor;
    class UserCursor;

    class Editor
    {
        friend class Cursor;
        friend class UserCursor;

        public:
            /* data members */
            size_t size;    

            /* constructor(s) & destructor(s) */
            Editor(void);
            ~Editor(void);

            /* operator overloads */
            Editor& operator=(const Editor& arg);
            friend ostream& operator<<(ostream& os, const Editor& ed);
            friend istream& operator>>(istream& is, const Editor& ed);

            /* member functions */
            Cursor begin(void);
            Cursor end(void);
            Cursor cursor(void);
            UserCursor userCursor(void);
            
        protected:
            /* data members */
            GapVector<char>* __data;
            UserCursor* __cursor;
    };

    class Cursor
    {
        public:
            /* constructor(s) & destructor(s) */
            Cursor(void) : __index(0), \
                           __x(0), \
                           __y(0), \
                           __editor(NULL) {};

            Cursor(const Cursor& curs) : __index(curs.__index), \
                                         __x(curs.__x), \
                                         __y(curs.__y), \
                                         __editor(curs.__editor){};

            /* operator overloads */
            char& operator*(void);
            Cursor& operator=(const Cursor& curs);
            Cursor& operator+=(int offset);
            Cursor& operator-=(int offset);
            Cursor& operator++(void); //post
            Cursor operator++(int)
            {
                Cursor tmp(*this);
                operator++();
                return tmp;
            };
            Cursor& operator--(void); //post
            Cursor operator--(int)
            {
                Cursor tmp(*this);
                operator--();
                return tmp;
            };
            friend Cursor operator+(Cursor curs, int offset);
            friend Cursor operator+(int offset, const Cursor& curs);
            friend Cursor operator-(Cursor curs, int offset);
            friend Cursor operator-(int offset, const Cursor& curs);
            friend int operator-(Cursor lhs, const Cursor& rhs)
            {
                return lhs.__index - rhs.__index;
            };
            inline bool operator<(const Cursor& curs) { return __index < curs.__index; };
            inline bool operator>(const Cursor& curs) { return __index > curs.__index; };
            inline bool operator<=(const Cursor& curs) { return __index <= curs.__index; };
            inline bool operator>=(const Cursor& curs) { return __index >= curs.__index; };
            inline bool operator==(const Cursor& curs) { return __index == curs.__index; };
            inline bool operator!=(const Cursor& curs) { return __index != curs.__index; };
            
            /* member functions */
            void insert(string input);
            void remove(int length);
            inline size_t getIndex(void) { return __index; };
            inline size_t getX(void) { return __x; };
            inline size_t getY(void) { return __y; };
            void moveVert(int offset);
            void moveHoriz(int offset);

        protected:
            /* data members */
            size_t __index;
            size_t __x;
            size_t __y;
            Editor* __editor;

            /* member functions */
            void getXY(void);
    };

    class UserCursor : public Cursor
    {
        public:
            char& operator*(void);
            UserCursor& operator=(const UserCursor& curs);
            UserCursor& operator+=(int offset);
            UserCursor& operator-=(int offset);
            UserCursor& operator++(void); //post
            UserCursor operator++(int)
            {
                UserCursor tmp(*this);
                operator++();
                return tmp;
            };
            UserCursor& operator--(void); //post
            UserCursor operator--(int)
            {
                UserCursor tmp(*this);
                operator--();
                return tmp;
            };
            friend UserCursor operator+(UserCursor curs, int offset);
            friend UserCursor operator+(int offset, const UserCursor& curs);
            friend UserCursor operator-(UserCursor curs, int offset);
            friend UserCursor operator-(int offset, const UserCursor& curs);
            friend int operator-(UserCursor lhs, const UserCursor& rhs)
            {
                return lhs.__index - rhs.__index;
            };
            inline bool operator<(const UserCursor& curs) { return __index < curs.__index; };
            inline bool operator>(const UserCursor& curs) { return __index > curs.__index; };
            inline bool operator<=(const UserCursor& curs) { return __index <= curs.__index; };
            inline bool operator>=(const UserCursor& curs) { return __index >= curs.__index; };
            inline bool operator==(const UserCursor& curs) { return __index == curs.__index; };
            inline bool operator!=(const UserCursor& curs) { return __index != curs.__index; };
    };
}
#endif
