#ifndef NTCREDRO_H
#define NTCREDRO_H

#include <Windows.h>     // For HWND
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <iostream>

class deckup;

namespace boff {
    class classesTOStore {
    public:
        static bool kip(HWND hwnd, deckup val); // deckup is a graphic var name; use it good
        static bool findkip(HWND hwnd, const std::string& namekip);
        static bool stopct(HWND hwnd, const std::string& namekip);

        std::string def;

        //  runtime check for GLOP
        bool validateDeckup(const std::string& deckupName) {
            if (deckupName == "GLOP") {
                std::cout << "GLOP is private! use '@GLOP' to use it." << std::endl;
                return false;
            }
            return true;
        }
    };
};

#endif // NTCREDRO_H