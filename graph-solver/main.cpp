#include "mainwindow.h"
#include "LoggerWrapper.h"
#include "graph.h"
#include <QApplication>
#include <QCommandLineParser>
#include <spdlog/spdlog.h>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("GraphSolver");
    a.setApplicationVersion("2.3");

    // Command Line Parser
    QCommandLineParser parser;
    parser.setApplicationDescription("Graph Algorithm Solver.");
    parser.addHelpOption();
    parser.addVersionOption();

    // Log File Path
    QCommandLineOption logFileOption(QStringList() << "log-file",
            QCoreApplication::translate("main", "Path to log file. If not set, logs to console."),
            QCoreApplication::translate("main", "path"));
    parser.addOption(logFileOption);

    // Load File (Optional utility)
    QCommandLineOption loadFileOption(QStringList() << "load",
            QCoreApplication::translate("main", "Load initial graph from text file."),
            QCoreApplication::translate("main", "file"));
    parser.addOption(loadFileOption);

    parser.process(a);

    // --- Configure Logging ---
    // Rule: If --log-file is present, write to file. Otherwise console.
    QString logFilePath = parser.value(logFileOption);
    
    if (!logFilePath.isEmpty()) {
        LoggerWrapper::init(logFilePath.toStdString(), "info"); 
    } else {
        LoggerWrapper::init("", "info"); // Console logger
    }

    spdlog::info("Application starting...");

    // --- Launch Application (Always GUI) ---
    MainWindow w;
    w.show();
    
    // Handle file loading for GUI
    if (parser.isSet(loadFileOption)) {
         spdlog::warn("CLI file loading for GUI is partially supported via Open Dialog, argument ignored for now.");
    }

    int exitCode = a.exec();
    spdlog::info("Application exiting with code {}", exitCode);
    return exitCode;
}