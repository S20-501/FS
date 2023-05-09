
include(FetchContent)
FetchContent_Declare(
  ConsoleMonitor
  GIT_REPOSITORY https://github.com/evezers/ConsoleMonitor.git
  GIT_TAG        b675234c01dfb898806aaa97c5543a5985db00ed
)

FetchContent_MakeAvailable(ConsoleMonitor)