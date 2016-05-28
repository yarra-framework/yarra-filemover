TARGET = filemover

# Define identifier for Ubuntu Linux version (UBUNTU_1204 / UBUNTU_1404)
BUILD_OS=UBUNTU_1204

equals( BUILD_OS, "UBUNTU_1404" ) {
    message( "Configuring for Ubuntu 14.04" )
    QMAKE_CXXFLAGS += -DUBUNTU_1404
    ICU_PATH=/usr/lib/x86_64-linux-gnu
    BOOST_PATH=/usr/lib/x86_64-linux-gnu
}

equals( BUILD_OS, "UBUNTU_1204" ) {
    message( "Configuring for Ubuntu 12.04" )
    QMAKE_CXXFLAGS += -DUBUNTU_1204
    ICU_PATH=/usr/lib
    BOOST_PATH=/usr/local/lib
}

QMAKE_CXXFLAGS += -DNDEBUG -std=c++11 -DWT_DEPRECATED_3_0_0

# Statically link dependencies
LIBS += $$BOOST_PATH/libboost_filesystem.a
LIBS += $$BOOST_PATH/libboost_date_time.a
LIBS += $$BOOST_PATH/libboost_system.a

LIBS += -lrt
LIBS += -ldl

TEMPLATE = app

SOURCES += main.cpp \
    yfm_notification.cpp \
    yfm_configuration.cpp \
    yfm_log.cpp \
    yfm_process.cpp

HEADERS += \ 
    yfm_notification.h \
    yfm_configuration.h \
    yfm_log.h \
    yfm_process.h


