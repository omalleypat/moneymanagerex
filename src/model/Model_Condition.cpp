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

#include "Model_Condition.h"

Model_Condition::Model_Condition()
: Model<DB_Table_CONDITION_V1>()
{
}

Model_Condition::~Model_Condition()
{
}

/**
* Initialize the global Model_Condition table.
* Reset the Model_Condition table or create the table if it does not exist.
*/
Model_Condition& Model_Condition::instance(wxSQLite3Database* db)
{
    Model_Condition& ins = Singleton<Model_Condition>::instance();
    ins.db_ = db;
    ins.destroy_cache();
    ins.ensure(db);
    ins.preload();

    return ins;
}

/** Return the static instance of Model_Condition table */
Model_Condition& Model_Condition::instance()
{
    return Singleton<Model_Condition>::instance();
}

/** Remove the Data record instance from memory and the database. */
bool Model_Condition::remove(int id)
{
    return this->remove(id, db_);
}
