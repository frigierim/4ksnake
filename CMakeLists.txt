cmake_minimum_required(VERSION 2.8.9)
project (4ksnake)

SET (MINIMIZE_COMPILE_FLAGS "/O1 /Os /Oi /GS- /EHa- /EHs- /EHc- /arch:IA32 -arch:IA32 /GR- /wd4725 /fp:fast /QIfist") #FatalWarnings /MultiProcessorCompile /No64BitChecks /NoEditAndContinue /NoBufferSecurityCheck /NoExceptions /NoManifest /GR- /NoRuntimeChecks /wd4725")
SET (MINIMIZE_LINK_FLAGS  "/NODEFAULTLIB /ignore:4254 /SUBSYSTEM:WINDOWS /DYNAMICBASE:NO /SAFESEH:NO /FIXED /OPT:ICF /MANIFEST:NO /MERGE:.rdata=.text winmm.lib")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MINIMIZE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${MINIMIZE_LINK_FLAGS}")

add_definitions("-D_CRT_SECURE_NO_WARNINGS -DWIN32")
add_subdirectory(startup)

add_executable(4ksnake main.cpp game.cpp)
target_link_libraries(4ksnake startup)
