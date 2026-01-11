TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += \
        /usr/include/ImageMagick-7 \
        /usr/include/podofo-0.9 \
        /usr/include/libxml2 \
        /usr/include/webkitgtk-4.1 \
        /usr/include/glib-2.0 \
        /usr/include/gtk-3.0 \
        /usr/include/pango-1.0 \
        /usr/lib/glib-2.0/include \
        /usr/include/harfbuzz \
        /usr/include/cairo \
        /usr/include/gdk-pixbuf-2.0 \
        /usr/include/atk-1.0 \
        /usr/include/libsoup-3.0

SOURCES += \
        src/cli++/main.cpp \
        src/log++/ic_printerlog++.cpp \
        src/wkgtkprinter++/ichtmltopdf++.cpp \
        src/wkgtkprinter++/iclog.cpp \
        src/wkgtkprinter++/pretty_html.cpp

DISTFILES += \
        src/cli++/icprint-cli \
        src/wkgtkprinter++/makefile

HEADERS += \
        src/log++/ic_printerlog++.h \
        src/wkgtkprinter++/ichtmltopdf++.h \
        src/wkgtkprinter++/iclog.h \
        src/wkgtkprinter++/pretty_html.h
