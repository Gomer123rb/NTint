#include "../include/DirectMod.h"

namespace boff {

void classesTOStore::kip(const std::vector<DirectX::XMFLOAT3>& dep,
                         const std::string& deckup)
{
    storedDep    = dep;
    storedDeckup = deckup;

    std::cout << "Storing dep vector with " << storedDep.size() << " elements.\n";
    for (size_t i = 0; i < storedDep.size(); ++i) {
        const auto& v = storedDep[i];
        std::cout << "dep[" << i << "] = (" << v.x << ", " << v.y << ", " << v.z << ")\n";
    }

    std::cout << "Deckup: " << storedDeckup << "\n";
}

void glop::kip(HWND h, const std::string& deckup)
{
    if (deckup == "GLOP") {
        std::cout << "GLOP is private! use '@GLOP' to use it.\n";
        return;
    }

    std::cout << "Kipping with deckup: " << deckup << "\n";

    std::vector<DirectX::XMFLOAT3> dep = {
        {0,0,0},
        {1,0,0},
        {0,1,0},
        {0,0,1}
    };

    if (store)
        store->kip(dep, deckup);
    else
        std::cout << "[glop] No classesTOStore instance attached.\n";

    if (deckup.rfind("@GLOP", 0) == 0) {
        size_t start = deckup.find('(');
        size_t end   = deckup.find(')', start);

        if (start != std::string::npos &&
            end   != std::string::npos &&
            end > start + 1)
        {
            std::string payload = deckup.substr(start + 1, end - start - 1);
            std::cout << "[@GLOP] " << payload << "\n";
        }
        else {
            std::cout << "[@GLOP] (no payload found)\n";
        }
    }
}

}
