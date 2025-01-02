namespace Hydragon {
namespace Logging {

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

#define HD_LOG(level, category, format, ...) \
    LogSystem::Get().Log(level, category, format, ##__VA_ARGS__)

#define HD_LOG_ERROR(category, format, ...) \
    HD_LOG(LogLevel::Error, category, format, ##__VA_ARGS__)

}} // namespace Hydragon::Logging 