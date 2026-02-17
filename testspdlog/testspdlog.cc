#include <iostream>
#include <spdlog/spdlog.h>

int main(void)
{
  /*默认logger模式 */
  spdlog::info("Welcome to spdlog");

  // 占位符
  spdlog::error("Some error message witharg:{}", 1);
  // 格式控制
  spdlog::warn("Easy padding in numbers like{:08d}", 12);
  spdlog::critical("Support for int: {0:d}; hex:{0:x}; oct:{0:o}; bin:{0:b}",
                   41);
  spdlog::info("Support for floats {:03.2f}", 1.23456);

  // 参数位置交换 std::bind
  spdlog::info("Positional args are {1} {0}..", "too", "Support");
  // 左对齐
  spdlog::info("{:<30}", "left aligned ");

  //因为默认的日志级别为INFO，所以这条debug日志不会打印出来
  spdlog::debug("This message should be displayed..");
  // 设置全局日志级别; 之前的日志级别为INFO; trace < debug < info < warn < error < critical < off
  spdlog::set_level(spdlog::level::debug);
  spdlog::debug("This message should be displayed..");

  // change log pattern
  spdlog::set_pattern("[%H:%M%S %z] [%n] [%^---%L---%$][thread %t] %v");

  // Compile time log levels;编译器日志裁剪。以下的是宏函数，行为收SPDLOG_ACTIVE_LEVEL影响
  // Note that this does not change the current log level, it will only
  // remove (depending on SPDLOG_ACTIVE_LEVEL) the call on the release code
  SPDLOG_TRACE("Some trace message with param {}", 42);
  SPDLOG_DEBUG("Some debug message");

  return 0;
}
