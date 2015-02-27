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

#include "Model_Field.h"

Model_Field::Model_Field()
: Model<DB_Table_FIELD_V1>()
{
}

Model_Field::~Model_Field()
{
}

/**
* Initialize the global Model_Field table.
* Reset the Model_Field table or create the table if it does not exist.
*/
Model_Field& Model_Field::instance(wxSQLite3Database* db)
{
    Model_Field& ins = Singleton<Model_Field>::instance();
    ins.db_ = db;
    ins.destroy_cache();
    ins.ensure(db);
    ins.preload();

    return ins;
}

/** Return the static instance of Model_Field table */
Model_Field& Model_Field::instance()
{
    return Singleton<Model_Field>::instance();
}

/** Remove the Data record instance from memory and the database. */
bool Model_Field::remove(int id)
{
    return this->remove(id, db_);
}
