/*
 * This file is part of owper - a graphical version of the well known chntpw password changer
 * Copyright (C) 2010 Matthew Morgan
 *
 * Some code was borrowed/modified from the chntpw project
 * Copyright (c) 1997-2007 Petter Nordahl-Hagen
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
 */

#include "include/hive.h"
#include "include/owpException.h"

using std::cerr;
using std::endl;

namespace owper {
    hive::hive(const char* fileName, int hiveMode/* = HMODE_RW*/) {
        int fileMode;
        //struct stat statBuff;
        //unsigned long pageOffset;
        int curHiveMode;

        this->fileName = fileName;
        this->state = 0;
        this->size = 0;
        this->buffer = NULL;

        if(hiveMode & HMODE_RO)
            fileMode = O_RDONLY;
        else
            fileMode = O_RDWR;

        this->fileDesc = open(this->fileName.c_str(), fileMode);

        if(this->fileDesc < 0) {
            if(fileMode == O_RDONLY) {
                string errorMessage = "Could not open hive file: ";
                errorMessage += this->fileName;
                throw(owpException(errorMessage));
            }else {
                cerr << "Failed to open hive [" << this->fileName << "] in RW mode, attempting RO" << endl;
                this->fileDesc = open(this->fileName.c_str(), O_RDONLY);
                if(this->fileDesc < 0) {
                    string errorMessage = "Could not open hive file: ";
                    errorMessage += this->fileName;
                    throw(owpException(errorMessage));
                }

                cerr << "Opened in RO mode." << endl;
                curHiveMode |= HMODE_RO;
            }
        }

        this->state |= (curHiveMode|HMODE_OPEN);
        cerr << "Opened hive: " << this->fileName << endl;
    }

    hive::~hive() {
        if(this->state & HMODE_OPEN) {
            cerr << "Closing hive: " << this->fileName << endl;
            close(this->fileDesc);
        } else {
            cerr << "Not closing hive...not open" << endl;
        }

        if(this->buffer)
            delete[] buffer;
    }



//        int fmode,r,vofs;
//        struct stat sbuf;
//        unsigned long pofs;
//        /* off_t l; */
//        char *c;
//        struct hbin_page *p;
//        struct regf_header *hdr;
//        struct nk_key *nk;
//        struct ri_key *rikey;
//        int verbose = (mode & HMODE_VERBOSE);
//        int trace   = (mode & HMODE_TRACE);
//
//        CREATE(hdesc,struct hive,1);
//
//        hdesc->filename = str_dup(filename);
//        hdesc->state = 0;
//        hdesc->size = 0;
//        hdesc->buffer = NULL;
//
//        if ( (mode & HMODE_RO) ) {
//          fmode = O_RDONLY;
//        } else {
//          fmode = O_RDWR;
//        }
//        hdesc->filedesc = open(hdesc->filename,fmode);
//        if (hdesc->filedesc < 0) {
//          fprintf(stderr,"openHive(%s) failed: %s, trying read-only\n",hdesc->filename,strerror(errno));
//          fmode = O_RDONLY;
//          mode |= HMODE_RO;
//          hdesc->filedesc = open(hdesc->filename,fmode);
//          if (hdesc->filedesc < 0) {
//            fprintf(stderr,"openHive(%s) in fallback RO-mode failed: %s\n",hdesc->filename,strerror(errno));
//            closeHive(hdesc);
//            return(NULL);
//          }
//        }
//
//
//        if ( fstat(hdesc->filedesc,&sbuf) ) {
//          perror("stat()");
//          exit(1);
//        }
//
//        hdesc->size = sbuf.st_size;
//        hdesc->state = mode | HMODE_OPEN;
//        /*  fprintf(stderr,"hiveOpen(%s) successful\n",hdesc->filename); */
//
//        /* Read the whole file */
//
//        ALLOC(hdesc->buffer,1,hdesc->size);
//
//        r = read(hdesc->filedesc,hdesc->buffer,hdesc->size);
//        if (r < hdesc->size) {
//          fprintf(stderr,"Could not read file, got %d bytes while expecting %d\n",
//              r, hdesc->size);
//          closeHive(hdesc);
//          return(NULL);
//        }
//
//        /* Now run through file, tallying all pages */
//        /* NOTE/KLUDGE: Assume first page starts at offset 0x1000 */
//
//         pofs = 0x1000;
//
//         hdr = (struct regf_header *)hdesc->buffer;
//         if (hdr->id != 0x66676572) {
//           printf("openHive(%s): File does not seem to be a registry hive!\n",filename);
//           return(hdesc);
//         }
//         printf("Hive <%s> name (from header): <",filename);
//         for (c = hdr->name; *c && (c < hdr->name + 64); c += 2) putchar(*c);
//
//         hdesc->rootofs = hdr->ofs_rootkey + 0x1000;
//         printf(">\nROOT KEY at offset: 0x%06x * ",hdesc->rootofs);
//
//         /* Cache the roots subkey index type (li,lf,lh) so we can use the correct
//          * one when creating the first subkey in a key */
//
//         nk = (struct nk_key *)(hdesc->buffer + hdesc->rootofs + 4);
//         if (nk->id == 0x6b6e) {
//           rikey = (struct ri_key *)(hdesc->buffer + nk->ofs_lf + 0x1004);
//           hdesc->nkindextype = rikey->id;
//           if (hdesc->nkindextype == 0x6972) {  /* Gee, big root, must check indirectly */
//             printf("load_hive: DEBUG: BIG ROOT!\n");
//             rikey = (struct ri_key *)(hdesc->buffer + rikey->hash[0].ofs_li + 0x1004);
//             hdesc->nkindextype = rikey->id;
//           }
//           if (hdesc->nkindextype != 0x666c &&
//           hdesc->nkindextype != 0x686c &&
//           hdesc->nkindextype != 0x696c) {
//             hdesc->nkindextype = 0x666c;
//           }
//
//           printf("Subkey indexing type is: %04x <%c%c>\n",
//              hdesc->nkindextype,
//              hdesc->nkindextype & 0xff,
//              hdesc->nkindextype >> 8);
//         } else {
//           printf("load_hive: WARNING: ROOT key does not seem to be a key! (not type == nk)\n");
//         }
//
//
//
//         while (pofs < hdesc->size) {
//      #ifdef LOAD_DEBUG
//                if (trace) hexdump(hdesc->buffer,pofs,pofs+0x20,1);
//      #endif
//           p = (struct hbin_page *)(hdesc->buffer + pofs);
//           if (p->id != 0x6E696268) {
//             printf("Page at 0x%lx is not 'hbin', assuming file contains garbage at end\n",pofs);
//             break;
//           }
//           hdesc->pages++;
//      #ifdef LOAD_DEBUG
//           if (trace) printf("\n###### Page at 0x%0lx has size 0x%0lx, next at 0x%0lx ######\n",pofs,p->len_page,p->ofs_next);
//      #endif
//           if (p->ofs_next == 0) {
//      #ifdef LOAD_DEBUG
//             if (trace) printf("openhive debug: bailing out.. pagesize zero!\n");
//      #endif
//             return(hdesc);
//           }
//      #if 0
//           if (p->len_page != p->ofs_next) {
//      #ifdef LOAD_DEBUG
//             if (trace) printf("openhive debug: len & ofs not same. HASTA!\n");
//      #endif
//             exit(0);
//           }
//      #endif
//
//
//           vofs = pofs + 0x20; /* Skip page header */
//      #if 1
//           while (vofs-pofs < p->ofs_next && vofs < hdesc->size) {
//             vofs += parse_block(hdesc,vofs,trace);
//
//           }
//      #endif
//           pofs += p->ofs_next;
//         }
//         printf("File size %d [%x] bytes, containing %d pages (+ 1 headerpage)\n",hdesc->size,hdesc->size, hdesc->pages);
//         printf("Used for data: %d/%d blocks/bytes, unused: %d/%d blocks/bytes.\n\n",
//            hdesc->useblk,hdesc->usetot,hdesc->unuseblk,hdesc->unusetot);
//
//
//         /* So, let's guess what kind of hive this is, based on keys in its root */
//
//         hdesc->type = HTYPE_UNKNOWN;
//         if (trav_path(hdesc, 0, "\\SAM", 0)) hdesc->type = HTYPE_SAM;
//         else if (trav_path(hdesc, 0, "\\ControlSet", 0)) hdesc->type = HTYPE_SYSTEM;
//         else if (trav_path(hdesc, 0, "\\Policy", 0)) hdesc->type = HTYPE_SECURITY;
//         else if (trav_path(hdesc, 0, "\\Microsoft", 0)) hdesc->type = HTYPE_SOFTWARE;
//         if (verbose) printf("Type of hive guessed to be: %d\n",hdesc->type);
//
//        return(hdesc);



/*  reg_off hive::travPath(reg_off startingOffest, char* path, int type) {

    }*/
}
