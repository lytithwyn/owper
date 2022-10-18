owper - Open Windows Password Eradicator
=========================================

Owper is a graphical/TUI tool for clearing the passwords for Microsoft Windows user accounts, including Microsoft Accounts.  It can also enable and disable accounts.  In the case of a Microsoft Account, Owper converts the account to a local account with no password.

Currently all registry editing functionality is handled by (a modified version of) the ntreg library, which is distributed as a part of Petter Nordahl-Hagen's chntpw.

Supported Systems
------------------
Owper is developed on Linux, though it may well work on other Unix-like operating systems (I am doing some testing on FreeBSD).  It should run correctly on either the x86 or x86_64 architectures.  It will NOT work on big-endian systems as of the current version.

Build Requirements
-------------------
 * openssl development files
 * development files for a supported GUI/TUI toolkit:
     * GTK 2
     * GTK 3
     * ncurses
 * on FreeBSD I had to have autotools and aclocal-archive installed, as it never seemed happy to accept the files generated in Linux (Makefile.in, etc.)

Build Instructions
-------------------
Owper uses autotools, so the typical ./configure, make, make install should suffice, except that I keep having to do autoreconf on FreeBSD.

License
-------
Owper is distributed under version 2 of the GPL License.  See the "licenses" subdirectory for more information.

Owper copyright 2010-2022 Matthew Morgan <matthew@lifandi.org>
ntreg (chntpw) copyright 1997-2011 Petter Nordahl-Hagen

