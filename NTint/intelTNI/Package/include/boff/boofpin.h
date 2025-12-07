#ifndef PIN_H
#define PIN_H

#include <Windows.h>     // For HWND
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <iostream>
#include "ntcredro.h"

namespace boff {
    class classesTOStore; // Forward declaration

    class pin {
        friend class classesTOStore;

    public:
        std::string GLOP;

        void kip(HWND h, const std::string& deckup = "@GLOP") {
            if (deckup == "GLOP") {
                std::cout << "GLOP is private! use '@GLOP' to use it.  (glop just is a string call)" << std::endl;
                return;
            }

            std::cout << "Kipping with deckup: " << deckup << std::endl;

            std::vector<DirectX::XMFLOAT3> dep;
            dep.push_back(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)); // Origin
            dep.push_back(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)); // X-axis
            dep.push_back(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)); // Y-axis
            dep.push_back(DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)); // Z-axis
            // references for games to log

            classesTOStore::kip(dep, deckup);
            // glop ONLY prints, dont try to just call it
            if (deckup.rfind("@GLOP", 0) == 0) {
            size_t start = deckup.find('(');
            size_t end = deckup.find(')', start);
            if (start != std::string::npos && end != std::string::npos && end > start + 1) {
            std::string payload = deckup.substr(start + 1, end - start - 1);
            std::cout << "[@GLOP] " << payload << std::endl;
            } else {
            std::cout << "[@GLOP] (no payload found)" << std::endl;
            }
            return;
            }

           
        }
    };
}

#endif // PIN_H