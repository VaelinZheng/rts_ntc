TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES    +=  \
    RTS_NTC_ENABLE=1

SOURCES += \
        rts_ntc.c \
        test.c

HEADERS += \
    rts_ntc.h
