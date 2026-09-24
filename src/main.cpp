// skynet-voice: SkyNetwork radio voice server.
#include <algorithm>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "voice_server.h"

int main(int argc, char** argv) {
    std::string db = "skynetwork.db", host = "0.0.0.0";
    uint16_t port = 3782;
    int account_check_s = 10;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (i + 1 < argc && a == "--db") db = argv[++i];
        else if (i + 1 < argc && a == "--host") host = argv[++i];
        else if (i + 1 < argc && a == "--port") port = static_cast<uint16_t>(std::atoi(argv[++i]));
        else if (i + 1 < argc && a == "--account-check") account_check_s = std::max(1, std::atoi(argv[++i]));
        else {
            std::fprintf(stderr, "usage: skynet-voice [--db FILE] [--host ADDR] [--port 3782] [--account-check SECONDS]\n");
            return 2;
        }
    }
    try {
        skynet::Accounts accounts(db);
        skynet::VoiceServer server(accounts, host, port, account_check_s * 1000);
        server.bind();
        std::fprintf(stderr, "SkyNetwork voice listening on %s:%u/udp\n", host.c_str(), server.port());
        std::fflush(stderr);
        server.run();
    } catch (const std::exception& e) {
        std::fprintf(stderr, "fatal: %s\n", e.what());
        return 1;
    }
}
