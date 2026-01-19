#include <gtest/gtest.h>
#include <QApplication>
#include <QtGlobal>

int main(int argc, char *argv[])
{
    // Force offscreen mode to prevent GUI initialization errors during test discovery
    // or when running in a headless environment (like CTest often assumes).
    qputenv("QT_QPA_PLATFORM", "offscreen");

    // QApplication is required because Vertex/Edge inherit from QGraphicsItem.
    QApplication app(argc, argv);

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}