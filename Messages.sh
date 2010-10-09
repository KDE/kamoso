#! /bin/sh
$EXTRACTRC `find . -name \*.rc -o -name \*.ui -o -name \*.kcfg` >> rc.cpp
$XGETTEXT rc.cpp src/*.cpp src/plugins/*/*.cpp  -o $podir/kamoso.pot

