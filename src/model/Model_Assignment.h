/*******************************************************
 Copyright (C) 2013,2014 Guan Lisheng (guanlisheng@gmail.com)

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ********************************************************/

#ifndef MODEL_ASSIGNMENT_H
#define MODEL_ASSIGNMENT_H

#include "Model.h"
#include "db/DB_Table_Assignment_V1.h"
#include "db/DB_Table_Field_V1.h"
#include "db/DB_Table_Condition_V1.h"
#include "util.h"

class Model_Assignment : public Model<DB_Table_ASSIGNMENT_V1>
{
public:
    using Model<DB_Table_ASSIGNMENT_V1>::remove;
    using Model<DB_Table_ASSIGNMENT_V1>::get;

public:
    Model_Assignment();
    ~Model_Assignment();

public:
    /**
    Initialize the global Model_Assignment table on initial call.
    Resets the global table on subsequent calls.
    * Return the static instance address for Model_Assignment table
    * Note: Assigning the address to a local variable can destroy the instance.
    */
    static Model_Assignment& instance(wxSQLite3Database* db);

    /**
    * Return the static instance address for Model_Assignment table
    * Note: Assigning the address to a local variable can destroy the instance.
    */
    static Model_Assignment& instance();

public:
    /** Return the Data record for the given account name */
    Data* get(const wxString& name);

    static wxString get_account_name(int account_id);

    /** Remove the Data record from memory and the database. */
    bool remove(int id);

public:

};

#endif // 
