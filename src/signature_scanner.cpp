#include "main.h"

uintptr_t mem::MODULE_BASE = 0;
uintptr_t mem::MODULE_END = 0;

void mem::initialize() {
	mem::MODULE_BASE = (uintptr_t)GetModuleHandleA(nullptr);
	MODULEINFO modinfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)mem::MODULE_BASE, &modinfo, sizeof(MODULEINFO));
	mem::MODULE_END = mem::MODULE_BASE + modinfo.SizeOfImage;
}

uintptr_t mem::findSignatureDefault(const std::vector<int16_t>& bytes) {
	
	if (bytes.size() <= 1) { return 0; }

	uint16_t patternSize = (uint16_t)bytes.size();
	uint16_t matchLength = patternSize - 1;
	const int16_t* front = bytes.data();

	for (uintptr_t pCur = mem::MODULE_BASE; pCur < mem::MODULE_END; pCur++) {

		if (*(int8_t*)pCur == (int8_t)front[0]) { // if first opcode matches
			
			if (*(int8_t*)(pCur + matchLength) == (int8_t)front[matchLength]) { // if last opcode matches

				for (uint16_t gamer = 1; gamer < patternSize; gamer++) {

					if (front[gamer] == -1) continue; // -1 in the bytes vector is considered a wildcard

					if (*(int8_t*)(pCur + gamer) == (int8_t)front[gamer]) {
						if (gamer == matchLength) { return pCur; }
						else continue;
					}
					else break;
				}
			}
		}
	}
	return 0;
}