#include "LoggerWrapper.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <algorithm>
#include <vector>
#include <QString>
#include <QByteArray>

void LoggerWrapper::qtMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // Convert QString to std::string
    QByteArray localMsg = msg.toUtf8();
    std::string txt = localMsg.constData();

    // --- Noise Filtering ---
    // Filter out common Linux/Qt desktop environment noise that is not relevant to the app logic
    if (msg.contains("qt.qpa.wayland") || 
        msg.contains("KServiceTypeTrader") || 
        msg.contains("ThumbCreator") ||
        msg.contains("kdirmodel") || 
        msg.contains("Wayland does not support QWindow::requestActivate")) 
    {
        // Downgrade these to trace (effectively ignored unless level is trace)
        // or just return to suppress completely.
        return; 
    }

    // Map Qt log levels to spdlog levels
    switch (type) {
    case QtDebugMsg:
        spdlog::debug("[Qt] {}", txt);
        break;
    case QtInfoMsg:
        spdlog::info("[Qt] {}", txt);
        break;
    case QtWarningMsg:
        spdlog::warn("[Qt] {}", txt);
        break;
    case QtCriticalMsg:
        spdlog::error("[Qt] {}", txt);
        break;
    case QtFatalMsg:
        spdlog::critical("[Qt] {}", txt);
        // QtFatalMsg usually causes an abort shortly after
        break;
    }
}

void LoggerWrapper::init(const std::string& logToFile, const std::string& logLevel)
{
    try
    {
        spdlog::sink_ptr sink;

        if (!logToFile.empty()) {
            // Create a file sink (overwrite mode = true)
            sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logToFile, true);
        } else {
            // Create a console sink
            sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        }

        auto logger = std::make_shared<spdlog::logger>("app_logger", sink);
        spdlog::set_default_logger(logger);

        // Set Level
        spdlog::level::level_enum levelEnum = spdlog::level::info;
        std::string level = logLevel;
        std::transform(level.begin(), level.end(), level.begin(), ::tolower);

        if (level == "debug") levelEnum = spdlog::level::debug;
        else if (level == "warn") levelEnum = spdlog::level::warn;
        else if (level == "error") levelEnum = spdlog::level::err;
        else if (level == "info") levelEnum = spdlog::level::info;

        spdlog::set_level(levelEnum);
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

        // Install the Qt Message Handler to redirect qDebug, qWarning, etc. to spdlog
        // This ensures system logs go to the configured sink (File or Console)
        qInstallMessageHandler(LoggerWrapper::qtMessageHandler);

        spdlog::info("Logger initialized. Level: {}, Output: {}", level, logToFile.empty() ? "Console" : logToFile);
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        fprintf(stderr, "Log initialization failed: %s\n", ex.what());
    }
}