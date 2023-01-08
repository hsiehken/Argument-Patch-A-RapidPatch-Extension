cmake_minimum_required(VERSION 3.23)
project(SWIntGen C)

set(CMAKE_C_STANDARD 99)

add_executable(SWIntGen main.c FixedPointPatching.c FixedPatchTrigger.h FirmwarePatches.c FirmwarePatches.h RapidPatch.h)
set_target_properties(SWIntGen PROPERTIES COMPILE_FLAGS "" LINK_FLAGS "")