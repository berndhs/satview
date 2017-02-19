#
#
# top level qmake pro file for cloudtrack
#
#

CONFIG += debug_and_release

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets gui core
  CONFIG += c++11
}
UI_DIR = tmp/ui
MOC_DIR = tmp/moc
OBJECTS_DIR = tmp/obj
RCC_DIR = tmp/rcc

SUBDIRS = viewer copier import

TEMPLATE = subdirs

MAKEFILE = Makefile


