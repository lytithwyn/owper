owper - Open Windows Password Eradicator
=========================================

Owper is a graphical tool for clearing the passwords for Microsoft Windows user accounts, including Microsoft Accounts.  It can also enable and disable accounts.  In the case of a Microsoft Account, Owper converts the account to a local account with no password.

Currently all registry editing functionality is handled by (a modified version of) the ntreg library, which is distributed as a part of Petter Nordahl-Hagen's chntpw.

Supported Systems
------------------
Owper is developed on Linux, though it may well work on other Unix-like operating systems.  It should run correctly on either the x86 or x86_64 architectures.  It will NOT work on big-endian systems as of the current version.

Build Requirements
-------------------
 * gtk+-2.0 development libraries
 * openssl development libraries

License
-------
Owper is distributed under version 2 of the GPL License.  See the "licenses" subdirectory for more information.

Owper copyright 2010-2018 Matthew Morgan <matthew@lifandi.org>
ntreg (chntpw) copyright 1997-2011 Petter Nordahl-Hagen

