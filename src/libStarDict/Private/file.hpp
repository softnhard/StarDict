/*************************************************************************
 * libStarDict - A C++ library to read stardict files
 * Copyright (C) 2013  S.M.Mohammadzadeh <mehran.m@aut.ac.ir>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *************************************************************************/
/**
 @author S.Mohammad M. Ziabary <mehran.m@aut.ac.ir>
 */
/*
 * Copyright 2011 kubtek <kubtek@mail.com>
 *
 * This file is part of StarDict.
 *
 * StarDict is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * StarDict is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with StarDict.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _FILE_HPP_
#define _FILE_HPP_

#include <algorithm>
#include <glib.h>
#include <list>
#include <string>


typedef std::list<std::string> List;

template<typename Function>
void __for_each_file(const std::string& dirname, const std::string& suff,
                     const List& order_list, const List& disable_list,
                     Function f)
{
    GDir *dir = g_dir_open(dirname.c_str(), 0, NULL);
    if (dir) {
        const gchar *filename;

        while ((filename = g_dir_read_name(dir))!=NULL) {
            std::string fullfilename(dirname+G_DIR_SEPARATOR_S+filename);
            if (g_file_test(fullfilename.c_str(), G_FILE_TEST_IS_DIR))
                __for_each_file(fullfilename, suff, order_list, disable_list, f);
            else if (g_str_has_suffix(filename, suff.c_str()) &&
                     std::find(order_list.begin(), order_list.end(),
                               fullfilename)==order_list.end()) {
                bool disable=std::find(disable_list.begin(),
                                       disable_list.end(),
                                       fullfilename)!=disable_list.end();
                f(fullfilename, disable);
            }
        }
        g_dir_close(dir);
    }
}

template<typename Function>
void for_each_file(const List& dirs_list, const std::string& suff,
                   const List& order_list, const List& disable_list,
                   Function f)
{
    List::const_iterator it;
    for (it=order_list.begin(); it!=order_list.end(); ++it) {
        bool disable=std::find(disable_list.begin(), disable_list.end(),
                               *it)!=disable_list.end();
        f(*it, disable);
    }
    for (it=dirs_list.begin(); it!=dirs_list.end(); ++it)
        __for_each_file(*it, suff, order_list, disable_list, f);
}

#endif//!_FILE_HPP_
