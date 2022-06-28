#include "main.h"
#include <dllentry.h>

using func_t = void(__cdecl*)(ServerPlayer*, Packet &);
static func_t original_ServerPlayer_sendNetworkPacket = nullptr;

void dllenter() {}
void dllexit() {}
void PreInit() {
	mem::initialize();

	// ServerPlayer::sendNetworkPacket(Packet &)
	// opcode sequence, where -1 is a wildcard. generated with https://github.com/ajkhoury/SigMaker-x64
	auto sampleFuncPtr = (void*)(mem::findSignatureDefault({
		0x48, 0x8B, 0x81, -1, -1, -1, -1, 0x4C, 0x8B, 0xC2, 0x44, 0x0F, 0xB6, 0x89, -1, -1, -1, -1,
		0x48, 0x8D, 0x91, -1, -1, -1, -1, 0x48, 0x8B, 0xC8, 0x4C, 0x8B, 0x10, 0x49, 0xFF, 0x62, 0x18
	}));

	void* tmp;
	THookRegister(sampleFuncPtr, (void*)detour_TestFunc, (void**)&tmp);
	original_ServerPlayer_sendNetworkPacket = reinterpret_cast<func_t>(tmp);
}
void PostInit() {
	LOGV("Sample raw detour loaded");
}

void detour_TestFunc(ServerPlayer *self, Packet &pkt) {
	std::cout << "before\n"; // hook code
	original_ServerPlayer_sendNetworkPacket(self, pkt); // call back to original (optional)
	std::cout << "after\n"; // hook code
}