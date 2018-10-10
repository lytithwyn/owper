/*
 * sam.h - known structures in the SAM hive of NT registry
 *
 *****
 *
 * NTREG - Window registry file reader / writer library
 * Copyright (c) 1997-2011 Petter Nordahl-Hagen.
 *
 * Edits by Matthew Morgan, as marked
 * Copyright (c) 2010-2018 Matthew Morgan
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef _INCLUDE_SAM_H
#define _INCLUDE_SAM_H 1

#ifdef __cplusplus
namespace ntreg {  //namespace wrapper added by Matthew Morgan
    extern "C" {
#endif

/* This contains some policy settings for the account database */

#define ACCOUNTDB_F_PATH "\\SAM\\Domains\\Account\\F"


struct accountdb_F {
  char unknown1[8]; /* 0 */
  char unknown2[8]; /* 8 */
  unsigned long updatecnt;   /* 10 Number of times policy data updated */
  char unknown3[4]; /* 14 */
  char t_maxpwage[8];  /* 18 Maximum password age, GUI shows only days */
  char t_minpwage[8];  /* 20 Minimum password age, GUI shows only days */
  char unknown4[8];    /* 28 */
  char t_lockdur[8];  /*  30 Account lockout duration, GUI shows minutes */
  char t_lockrel[8];  /*  38 Release account lockout after, GUI show minutes */
  char unknown5[8];   /*  40 */
  unsigned long rid;  /*  48 RID of user doing last edit? */
  unsigned long flags; /* 4c Some flags & options, see below */
  unsigned short minpwlen; /* 50 Minimum password lenght */
  unsigned short numhist;  /* 52 How many passwords to keep in history */
  unsigned short locklimit; /*54  How many tries before lockout */
  char unknown6[0x9a];    /* Rest is unknown */
};                         /* Total size 0xF0 bytes, seems to be constant */

/* Known bits in flags field */

#define ACF_COMPLEX    0x0001  /* Pass must meet complexity req. */
#define ACF_REVERSIBLE 0x0010  /* Store password using reversible encryption */


/* This is users F value, contains account type & state etc */

#define USER_F_PATH "\\SAM\\Domains\\Account\\Users\\%08X\\F"

struct user_F {
  char unknown1[8];
  char t_lockout[8];  /* Time of lockout */
  char unknown2[8];
  char t_creation[8]; /* Time of account creation */
  char unknown3[8];
  char t_login[8];    /* Time of last login */
  uint32_t rid;
  char unknown4[4];
  uint16_t ACB_bits;  /* Account type and status flags */
  char unknown5[6];
  uint16_t failedcnt; /* Count of failed logins, if > than policy it is locked */
  uint16_t logins;    /* Total logins since creation */
  char unknown6 [0xc];
  };

#define ACB_DISABLED   0x0001  /* Act disabled */
#define ACB_HOMDIRREQ  0x0002  /* Home directory required */
#define ACB_PWNOTREQ   0x0004  /* User password not req */
#define ACB_TEMPDUP    0x0008  /* Temporary duplicate account?? */
#define ACB_NORMAL     0x0010  /* Normal user account */
#define ACB_MNS        0x0020  /* MNS logon user account */
#define ACB_DOMTRUST   0x0040  /* Interdomain trust account */
#define ACB_WSTRUST    0x0080  /* Workstation trust account */

#define ACB_SVRTRUST   0x0100  /*  Server trust account */
#define ACB_PWNOEXP    0x0200  /* User password does not expire */
/* Seems not to be used on failed console logins at least */
#define ACB_AUTOLOCK   0x0400  /* Account auto locked */

//char *acb_fields[16] = { /* explicit casts to char* added by Matthew Morgan */
//   (char *)"Disabled" ,
//   (char *)"Homedir req." ,
//   (char *)"Passwd not req." ,
//   (char *)"Temp. duplicate" ,
//   (char *)"Normal account" ,
//   (char *)"NMS account" ,
//   (char *)"Domain trust act." ,
//   (char *)"Wks trust act." ,
//   (char *)"Srv trust act" ,
//   (char *)"Pwd don't expire" ,
//   (char *)"Auto lockout" ,
//   (char *)"(unknown 0x08)" ,
//   (char *)"(unknown 0x10)" ,
//   (char *)"(unknown 0x20)" ,
//   (char *)"(unknown 0x40)" ,
//   (char *)"(unknown 0x80)" ,
//};

/* Users V data struct */
/* First 0xCC bytes is pointer & len table, rest is data which
 * the table points to
 * String data is unicode, not zero terminated (must use len)
 */

struct user_V {

  uint32_t unknown1_1;      /* 0x00 - always zero? */
  uint32_t unknown1_2;      /* 0x04 - points to username? */
  uint32_t unknown1_3;      /* 0x08 - always 0x02 0x00 0x01 0x00 ? */

  uint32_t username_ofs;    /* 0x0c */
  uint32_t username_len;    /* 0x10 */

  uint32_t unknown2_1;      /* 0x14 - always zero? */

  uint32_t fullname_ofs;    /* 0x18 */
  uint32_t fullname_len;    /* 0x1c */

  uint32_t unknown3_1;      /* 0x20 - always zero? */

  uint32_t comment_ofs;     /* 0x24 */
  uint32_t comment_len;     /* 0x28 */

  uint32_t unknown4_1;      /* 0x2c - alway zero? */
  uint32_t unknown4_2;      /* 0x30 - points 4 or 8 byte field before hashes */
  uint32_t unknown4_3;      /* 0x34 - zero? or size? */
  uint32_t unknown4_4;      /* 0x38 - zero? */
  uint32_t unknown4_5;      /* 0x3c - to field 8 bytes before hashes */
  uint32_t unknown4_6;      /* 0x40 - zero? or size of above? */
  uint32_t unknown4_7;      /* 0x44 - zero? */

  uint32_t homedir_ofs;     /* 0x48 */
  uint32_t homedir_len;     /* 0x4c */

  uint32_t unknown5_1;      /* 0x50 - zero? */

  uint32_t drvletter_ofs;   /* 0x54 - drive letter for home dir */
  uint32_t drvletter_len;   /* 0x58 - len of above, usually 4   */

  uint32_t unknown6_1;      /* 0x5c - zero? */

  uint32_t logonscr_ofs;    /* 0x60 - users logon script path */
  uint32_t logonscr_len;    /* 0x64 - length of string */

  uint32_t unknown7_1;      /* 0x68 - zero? */

  uint32_t profilep_ofs;    /* 0x6c - profile path string */
  uint32_t profilep_len;    /* 0x70 - profile path stringlen */

  char unknown7[0x90-0x74]; /* 0x74 */

  uint32_t unknown8_1;      /* 0x90 - pointer to some place before hashes, after comments */
  uint32_t unknown8_2;      /* 0x94 - size of above? */
  uint32_t unknown8_3;      /* 0x98 - unknown? always 1? */

  uint32_t lmpw_ofs;        /* 0x9c */
  uint32_t lmpw_len;        /* 0xa0 */

  uint32_t unknown9_1;      /* 0xa4 - zero? */

  uint32_t ntpw_ofs;        /* 0xa8 */
  uint32_t ntpw_len;        /* 0xac */

  uint32_t unknowna_1;      /* 0xb0 */
  uint32_t unknowna_2;      /* 0xb4 - points to field after hashes */
  uint32_t unknowna_3;      /* 0xb8 - size of above field */
  uint32_t unknowna_4;      /* 0xbc - zero? */
  uint32_t unknowna_5;      /* 0xc0 - points to field after that */
  uint32_t unknowna_6;      /* 0xc4 - size of above */
  uint32_t unknowna_7;      /* 0xc8 - zero ? */

  char data[4];        /* Data starts here. All pointers above is relative to this,
              that is V + 0xCC */

};

/* Groups C data struct
 * First 0x34 bytes is pointer & len table, rest is data which
 * the table points to
 * String data is unicode, not zero terminated (must use len)
 */

struct group_C {

  uint32_t groupid;         /* 0x00 - Seems to be the group ID */
  uint32_t unknown1_1;      /* 0x04 - always zero? */
  uint32_t unknown1_2;      /* 0x08 - points to groupname? */
  uint32_t unknown1_3;      /* 0x0c - always 0x02 0x00 0x01 0x00 ? */

  uint32_t grpname_ofs;    /* 0x10 */
  uint32_t grpname_len;    /* 0x14 */

  uint32_t unknown2_1;      /* 0x18 - always zero? */

  uint32_t fullname_ofs;    /* 0x1c */
  uint32_t fullname_len;    /* 0x20 */

  uint32_t unknown3_1;      /* 0x24 - always zero? */
  uint32_t unknown3_2;      /* 0x28 - always zero? */
  uint32_t unknown3_3;      /* 0x2c - always zero? */

  uint32_t grp_members;     /* 0x30 - number of group members */

  /* 0x34 - data starts here. pointers above are offset from this */

  char data[];

};

//close namespace
#ifdef __cplusplus
    }
}
#endif

#endif
