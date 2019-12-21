TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

#

SOURCES += \
        main.cpp \
    gameObj.cpp \
    global.cpp \
    playerObj.cpp
INCLUDEPATH += \
    $$PWD/sdl2/2.0.9/include/SDL2/ \
    $$PWD/sdl2_image/2.0.4/include/SDL2/ \

# sdl2 library inclusion

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/sdl2/2.0.9/lib/release/ -lSDL2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/sdl2/2.0.9/lib/debug/ -lSDL2
else:unix: LIBS += -L$$PWD/sdl2/2.0.9/lib/ -lSDL2

INCLUDEPATH += $$PWD/sdl2/2.0.9/include
DEPENDPATH += $$PWD/sdl2/2.0.9/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/sdl2/2.0.9/lib/release/libSDL2.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/sdl2/2.0.9/lib/debug/libSDL2.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/sdl2/2.0.9/lib/release/SDL2.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/sdl2/2.0.9/lib/debug/SDL2.lib
else:unix: PRE_TARGETDEPS += $$PWD/sdl2/2.0.9/lib/libSDL2.a

# sdl2_image library inclusion

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/sdl2_image/2.0.4/lib/release/ -lSDL2_image
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/sdl2_image/2.0.4/lib/debug/ -lSDL2_image
else:unix: LIBS += -L$$PWD/sdl2_image/2.0.4/lib/ -lSDL2_image

INCLUDEPATH += $$PWD/sdl2_image/2.0.4/lib
DEPENDPATH += $$PWD/sdl2_image/2.0.4/lib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/sdl2_image/2.0.4/lib/release/libSDL2_image.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/sdl2_image/2.0.4/lib/debug/libSDL2_image.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/sdl2_image/2.0.4/lib/release/SDL2_image.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/sdl2_image/2.0.4/lib/debug/SDL2_image.lib
else:unix: PRE_TARGETDEPS += $$PWD/sdl2_image/2.0.4/lib/libSDL2_image.a

HEADERS += \
    debug.h \
    gameObj.h \
    global.h \
    playerObj.h
