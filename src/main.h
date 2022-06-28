#pragma once

#include <Windows.h>
#include <Psapi.h>

#include <hook.h>
#include "../../ModLoader/hook_init.h"
#include <base/base.h>
#include <base/playerdb.h>
#include <base/log.h>

#include <Actor/ServerPlayer.h>

namespace mem {

extern uintptr_t MODULE_BASE;
extern uintptr_t MODULE_END;

void initialize();
uintptr_t findSignatureDefault(const std::vector<int16_t> &bytes);

} // namespace mem

void detour_TestFunc(ServerPlayer *self, Packet &pkt);






DEF_LOGGER("SampleRawDetour");