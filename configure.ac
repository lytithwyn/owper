AC_INIT([owper], [0.1], [matthew@lifandi.org])
AM_INIT_AUTOMAKE([])
AC_PROG_CXX
LT_INIT
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_FILES([Makefile libowper/Makefile gui/base/Makefile gui/gtk2/Makefile gui/gtk3/Makefile gui/ncurses/Makefile])
AX_CXX_COMPILE_STDCXX(11)
PKG_CHECK_MODULES(OpenSSL, openssl)

# gtk2
PKG_CHECK_MODULES(gtk2, gtk+-2.0, HAVE_GTK2="yes", HAVE_GTK2="no")
AM_CONDITIONAL(HAVE_GTK2, [test "x$HAVE_GTK2" = "xyes"])

# gtk3
PKG_CHECK_MODULES(gtk3, gtk+-3.0, HAVE_GTK3="yes", HAVE_GTK3="no")
AM_CONDITIONAL(HAVE_GTK3, [test "x$HAVE_GTK3" = "xyes"])

# ncurses
PKG_CHECK_MODULES(ncurses,panel menu ncurses, HAVE_NCURSES="yes", HAVE_NCURSES="no")
AM_CONDITIONAL(HAVE_NCURSES, [test "x$HAVE_NCURSES" = "xyes"])

AS_IF([test "x$HAVE_GTK2" = "xno" -a "x$HAVE_GTK3" = "xno" -a "xHAVE_NCURSES" = "xno"], [
       AC_MSG_ERROR([Dev files could not be found for any supported GUI!])
])

AC_OUTPUT

