#ifndef LOGGERWRAPPER_H
#define LOGGERWRAPPER_H

#include <spdlog/spdlog.h>
#include <string>
#include <QtGlobal>

/**
 * @brief Static wrapper class to configure and access the application logger.
 */
class LoggerWrapper
{
public:
    /**
     * @brief Initializes the global logger based on command line arguments.
     * @param logToFile If empty, logs to console. If set, logs to this filename.
     * @param logLevel The string representation of the log level (debug, info, warn, error).
     */
    static void init(const std::string& logToFile, const std::string& logLevel);

private:
    /**
     * @brief Intercepts Qt internal messages (qDebug, qWarning, etc.) and forwards them to spdlog.
     */
    static void qtMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif // LOGGERWRAPPER_H