/* This file is part of ktalkd

    Copyright (C) 1997 David Faure (david.faure@ifhamy.insa-lyon.fr)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.  */


#ifdef __cplusplus
extern "C" 
{
#endif

  /**
        * Initiate user-config-file reading.
        *
        * @param l_name User name
        */
int init_user_config(char * l_name);

  /**
        * Read one entry in user-config-file.
        *
        * @param key Key, such as "Mail", "Subj", "Head", "Msg1".."Msg3"
        */
int read_user_config(char * key, char * result, int max);

  /**
        * Read a boolean entry in user-config-file.
        *
        * @param key Key, such as "Sound"
        */
int read_bool_user_config(char * key, int * result);

  /**
        * Close user-config-file and destroys objects used.
        */
void end_user_config();

  /**
        * Read all site-wide configuration in one pass
        */
int process_config_file(void);

 /**
        * Set $KDEBINDIR, which might be used by ktalkdrc files
        */
void setenv_kdebindir(void);

 /**
        * Get KDE bin dir
        */
void get_kdebindir(char * buffer, int max);
    
#ifdef __cplusplus
}
#endif
