#ifndef GLOP_H
#define GLOP_H

#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <iostream>

namespace boff {

    class classesTOStore {
    public:
        std::vector<DirectX::XMFLOAT3> storedDep;
        std::string storedDeckup;

        void kip(const std::vector<DirectX::XMFLOAT3>& dep,
                 const std::string& deckup);
    };

    class glop {
        friend class classesTOStore;

    public:
        std::string GLOP;
        classesTOStore* store = nullptr;

        void kip(HWND h, const std::string& deckup = "@GLOP");
    };

}

#endif // GLOP_H
