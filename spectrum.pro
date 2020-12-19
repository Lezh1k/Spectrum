TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += inc

SOURCES += \
        src/commons.cpp \
        src/fft_guard.cpp \
        src/fft_window.cpp \
        src/main.cpp \
        src/sound_processor.cpp \
        src/tone_generator.cpp


LIBS += -lfftw3 \
        -lm

HEADERS += \
    inc/commons.h \
    inc/fft_guard.h \
    inc/fft_window.h \
    inc/pcm_format.h \
    inc/sound_processor.h \
    inc/tone_generator.h
