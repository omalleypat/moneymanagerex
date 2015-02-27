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

#include "Model_Assignment.h"

Model_Assignment::Model_Assignment()
: Model<DB_Table_ASSIGNMENT_V1>()
{
}

Model_Assignment::~Model_Assignment()
{
}

/**
* Initialize the global Model_Assignment table.
* Reset the Model_Assignment table or create the table if it does not exist.
*/
Model_Assignment& Model_Assignment::instance(wxSQLite3Database* db)
{
    Model_Assignment& ins = Singleton<Model_Assignment>::instance();
    ins.db_ = db;
    ins.destroy_cache();
    ins.ensure(db);
    ins.preload();

    return ins;
}

/** Return the static instance of Model_Assignment table */
Model_Assignment& Model_Assignment::instance()
{
    return Singleton<Model_Assignment>::instance();
}

/** Get the Data record instance in memory. */
Model_Assignment::Data* Model_Assignment::get(const wxString& name)
{
    Data* assignment = this->get_one(ASSIGNMENTNAME(name));
    if (assignment) return assignment;

    Data_Set items = this->find(ASSIGNMENTNAME(name));
    if (!items.empty()) assignment = this->get(items[0].ASSIGNMENTID, this->db_);
    return assignment;
}

wxString Model_Assignment::get_account_name(int assignment_id)
{
    Data* assignment = instance().get(assignment_id);
    if (assignment)
        return assignment->ASSIGNMENTNAME;
    else
        return _("Assignment Error");
}

/** Remove the Data record instance from memory and the database. */
bool Model_Assignment::remove(int id)
{
    return this->remove(id, db_);
}
