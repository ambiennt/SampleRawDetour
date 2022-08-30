#include "main.h"
#include <dllentry.h>

func_t original_ServerPlayer_sendNetworkPacket{};

void dllenter() {}
void dllexit() {}
void PreInit() {
	Memory::initialize();

	// ServerPlayer::sendNetworkPacket(Packet &)
	// opcode sequence, where {} is a wildcard. generated with https://github.com/ajkhoury/SigMaker-x64
	auto sampleFuncPtr = (void*)(Memory::findSignature({
		0x48, 0x8B, 0x81, {}, {}, {}, {}, 0x4C, 0x8B, 0xC2, 0x44, 0x0F, 0xB6, 0x89, {}, {}, {}, {},
		0x48, 0x8D, 0x91, {}, {}, {}, {}, 0x48, 0x8B, 0xC8, 0x4C, 0x8B, 0x10, 0x49, 0xFF, 0x62, 0x18
	}));

	THookRegister(sampleFuncPtr, (void*)detour_TestFunc, (void**)&original_ServerPlayer_sendNetworkPacket);
}
void PostInit() {
	LOGV("Sample raw detour loaded");
}

void detour_TestFunc(ServerPlayer *self, Packet &pkt) {
	std::cout << "before\n"; // hook code
	original_ServerPlayer_sendNetworkPacket(self, pkt); // call back to original (optional)
	std::cout << "after\n"; // hook code
}