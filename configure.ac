AC_INIT([owper], [0.1], [matthew@lifandi.org])
AM_INIT_AUTOMAKE([])
AC_PROG_CXX
LT_INIT
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_FILES([Makefile libowper/Makefile gui/base/Makefile gui/gtk2/Makefile gui/gtk3/Makefile])
AX_CXX_COMPILE_STDCXX(11)
PKG_CHECK_MODULES(OpenSSL, openssl)
PKG_CHECK_MODULES(gtk2, gtk+-2.0)
PKG_CHECK_MODULES(gtk3, gtk+-3.0)
AC_OUTPUT

