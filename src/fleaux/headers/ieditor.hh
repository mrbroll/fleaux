#ifndef FLEAUX_IEDITOR_HH_
#define FLEAUX_IEDITOR_HH_

#include <string>

using namespace std;

namespace Fleaux
{
    class ICursor;

    class IEditor
    {
        public:
            virtual ~IEditor(void) {};
            virtual void readFromFile(const string& path) = 0;
            virtual void writeToFile(const string& path) = 0;
            virtual ICursor* getCursor(void) = 0;
    };

    class ICursor
    {
        public:
            virtual void insert(const string& input) = 0;
            virtual void remove(int length) = 0;
            virtual void replace(int length, const string& replacement) = 0;
            virtual size_t getIndex(void) = 0;
            virtual size_t getX(void) = 0;
            virtual size_t getY(void) = 0;
            virtual void move(int offsetX, int offsetY) = 0;
    };
}

#endif
