#include "main.h"

uintptr_t Memory::MODULE_START = 0x0;
uintptr_t Memory::MODULE_END = 0x0;

void Memory::initialize() {
	Memory::MODULE_START = (uintptr_t)GetModuleHandleA(nullptr); // "bedrock_server_mod.exe"
	if (Memory::MODULE_START > 0x0) {
		MODULEINFO modInfo;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)Memory::MODULE_START, &modInfo, sizeof(modInfo));
		Memory::MODULE_END = (Memory::MODULE_START + modInfo.SizeOfImage);
	}
}

uintptr_t Memory::findSignature(const signature_t& signature) {

	if (signature.empty()) { return 0x0; }

	auto it = reinterpret_cast<uint8_t*>(Memory::MODULE_START);
	const auto scanEnd = reinterpret_cast<uint8_t*>(Memory::MODULE_END);
	size_t sigSize = signature.size();

	while (it < scanEnd) {
		it = std::find(it, scanEnd, signature.front());
		if (it == scanEnd) break;
		auto matchEnd = reinterpret_cast<uint8_t*>((uintptr_t)it + sigSize);
		auto currentPossibleMatch = std::search(
			it, matchEnd,
			signature.cbegin(), signature.cend(),
			[](uint8_t exeOpcode, std::optional<uint8_t> sigOpcode) -> bool {
				return (!sigOpcode.has_value() || (sigOpcode.value() == exeOpcode));
			}
		);
		if (currentPossibleMatch != matchEnd) {
			return (uintptr_t)currentPossibleMatch;
		}
		it++; // we want to search from the byte after the signature.front() match
	}
	return 0x0;
}