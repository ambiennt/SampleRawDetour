#pragma once

#include <Windows.h>
#include <Psapi.h>

#include <hook.h>
#include "../../ModLoader/hook_init.h"
#include <base/base.h>
#include <base/playerdb.h>
#include <base/log.h>
#include <Actor/ServerPlayer.h>

using signature_t = std::vector<std::optional<uint8_t>>;
using func_t = void(__cdecl*)(ServerPlayer*, Packet &);
extern func_t original_ServerPlayer_sendNetworkPacket;
void detour_TestFunc(ServerPlayer *self, Packet &pkt);

namespace Memory {

extern uintptr_t MODULE_START;
extern uintptr_t MODULE_END;

void initialize();
uintptr_t findSignature(const signature_t &signature);

} // namespace Memory






DEF_LOGGER("SampleRawDetour");