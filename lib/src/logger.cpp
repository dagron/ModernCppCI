/*
 *  Distributed under the MIT License (See accompanying file /LICENSE )
 */
#include "logger.h"
#include <spdlog/sinks/dist_sink.h>

#ifdef _WIN32
#include <spdlog/sinks/wincolor_sink.h>
#else
#include <spdlog/sinks/ansicolor_sink.h>
#endif

#if defined(_DEBUG) && defined(_MSC_VER)
#include <spdlog/sinks/msvc_sink.h>
#endif  // _DEBUG && _MSC_VER

namespace ModernCppCI {

Logger::Logger(const std::string &section) noexcept : section_{section} {}

auto create_spdlog() {
#ifdef _WIN32
  auto color_sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
#else
  auto color_sink = std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>();
#endif

  auto dist_sink = std::make_shared<spdlog::sinks::dist_sink_st>();
  dist_sink->add_sink(color_sink);

#if defined(_DEBUG) && defined(_MSC_VER)
  auto debug_sink = std::make_shared<spdlog::sinks::msvc_sink_st>();
  dist_sink->add_sink(debug_sink);
#endif  // _DEBUG && _MSC_VER

  return spdlog::details::registry::instance().create("console", dist_sink);
}

std::shared_ptr<spdlog::logger> Logger::spdlog() {
  auto logger = spdlog::get("console");

  if (logger == nullptr) {
    logger = create_spdlog();
  }

  return logger;
}

}  // namespace ModernCppCI
