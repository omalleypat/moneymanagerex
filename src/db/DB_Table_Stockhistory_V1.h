// -*- C++ -*-
//=============================================================================
/**
 *      Copyright (c) 2013 - 2016 Guan Lisheng (guanlisheng@gmail.com)
 *
 *      @file
 *
 *      @author [sqlite2cpp.py]
 *
 *      @brief
 *
 *      Revision History:
 *          AUTO GENERATED at 2016-02-15 20:32:31.113355.
 *          DO NOT EDIT!
 */
//=============================================================================

#ifndef DB_TABLE_STOCKHISTORY_V1_H
#define DB_TABLE_STOCKHISTORY_V1_H

#include "DB_Table.h"

struct DB_Table_STOCKHISTORY_V1 : public DB_Table
{
    struct Data;
    typedef DB_Table_STOCKHISTORY_V1 Self;
    /** A container to hold list of Data records for the table*/
    struct Data_Set : public std::vector<Self::Data>
    {
        std::wstring to_json() const
        {
            json::Array a;
            for (const auto & item: *this)
            {
                json::Object o;
                item.to_json(o);
                a.Insert(o);
            }
            std::wstringstream ss;
            json::Writer::Write(a, ss);
            return ss.str();
        }
    };
    /** A container to hold a list of Data record pointers for the table in memory*/
    typedef std::vector<Self::Data*> Cache;
    typedef std::map<int, Self::Data*> Index_By_Id;
    Cache cache_;
    Index_By_Id index_by_id_;
    Data* fake_; // in case the entity not found

    /** Destructor: clears any data records stored in memory */
    ~DB_Table_STOCKHISTORY_V1() 
    {
        delete this->fake_;
        destroy_cache();
    }
     
    /** Removes all records stored in memory (cache) for the table*/ 
    void destroy_cache()
    {
        std::for_each(cache_.begin(), cache_.end(), std::mem_fun(&Data::destroy));
        cache_.clear();
        index_by_id_.clear(); // no memory release since it just stores pointer and the according objects are in cache
    }

    /** Creates the database table if the table does not exist*/
    bool ensure(wxSQLite3Database* db)
    {
        if (!exists(db))
        {
            try
            {
                db->ExecuteUpdate("CREATE TABLE STOCKHISTORY_V1(HISTID integer primary key, SYMBOL TEXT NOT NULL, DATE TEXT NOT NULL, VALUE numeric NOT NULL, UPDTYPE integer, UNIQUE(SYMBOL, DATE))");
                this->ensure_data(db);
            }
            catch(const wxSQLite3Exception &e) 
            { 
                wxLogError("STOCKHISTORY_V1: Exception %s", e.GetMessage().c_str());
                return false;
            }
        }

        this->ensure_index(db);

        return true;
    }

    bool ensure_index(wxSQLite3Database* db)
    {
        try
        {
            db->ExecuteUpdate("CREATE INDEX IF NOT EXISTS IDX_STOCKHISTORY_SYMBOL ON STOCKHISTORY_V1(SYMBOL)");
        }
        catch(const wxSQLite3Exception &e) 
        { 
            wxLogError("STOCKHISTORY_V1: Exception %s", e.GetMessage().c_str());
            return false;
        }

        return true;
    }

    void ensure_data(wxSQLite3Database* db)
    {
    }
    
    struct HISTID : public DB_Column<int>
    { 
        static wxString name() { return "HISTID"; } 
        explicit HISTID(const int &v, OP op = EQUAL): DB_Column<int>(v, op) {}
    };
    struct SYMBOL : public DB_Column<wxString>
    { 
        static wxString name() { return "SYMBOL"; } 
        explicit SYMBOL(const wxString &v, OP op = EQUAL): DB_Column<wxString>(v, op) {}
    };
    struct DATE : public DB_Column<wxString>
    { 
        static wxString name() { return "DATE"; } 
        explicit DATE(const wxString &v, OP op = EQUAL): DB_Column<wxString>(v, op) {}
    };
    struct VALUE : public DB_Column<double>
    { 
        static wxString name() { return "VALUE"; } 
        explicit VALUE(const double &v, OP op = EQUAL): DB_Column<double>(v, op) {}
    };
    struct UPDTYPE : public DB_Column<int>
    { 
        static wxString name() { return "UPDTYPE"; } 
        explicit UPDTYPE(const int &v, OP op = EQUAL): DB_Column<int>(v, op) {}
    };
    typedef HISTID PRIMARY;
    enum COLUMN
    {
        COL_HISTID = 0
        , COL_SYMBOL = 1
        , COL_DATE = 2
        , COL_VALUE = 3
        , COL_UPDTYPE = 4
    };

    /** Returns the column name as a string*/
    static wxString column_to_name(COLUMN col)
    {
        switch(col)
        {
            case COL_HISTID: return "HISTID";
            case COL_SYMBOL: return "SYMBOL";
            case COL_DATE: return "DATE";
            case COL_VALUE: return "VALUE";
            case COL_UPDTYPE: return "UPDTYPE";
            default: break;
        }
        
        return "UNKNOWN";
    }

    /** Returns the column number from the given column name*/
    static COLUMN name_to_column(const wxString& name)
    {
        if ("HISTID" == name) return COL_HISTID;
        else if ("SYMBOL" == name) return COL_SYMBOL;
        else if ("DATE" == name) return COL_DATE;
        else if ("VALUE" == name) return COL_VALUE;
        else if ("UPDTYPE" == name) return COL_UPDTYPE;

        return COLUMN(-1);
    }
    
    /** Data is a single record in the database table*/
    struct Data
    {
        friend struct DB_Table_STOCKHISTORY_V1;
        /** This is a instance pointer to itself in memory. */
        Self* table_;
    
        int HISTID;//  primary key
        wxString SYMBOL;
        wxString DATE;
        double VALUE;
        int UPDTYPE;
        int id() const { return HISTID; }
        void id(int id) { HISTID = id; }
        bool operator < (const Data& r) const
        {
            return this->id() < r.id();
        }
        bool operator < (const Data* r) const
        {
            return this->id() < r->id();
        }

        explicit Data(Self* table = 0) 
        {
            table_ = table;
        
            HISTID = -1;
            VALUE = 0.0;
            UPDTYPE = -1;
        }

        explicit Data(wxSQLite3ResultSet& q, Self* table = 0)
        {
            table_ = table;
        
            HISTID = q.GetInt(0); // HISTID
            SYMBOL = q.GetString(1); // SYMBOL
            DATE = q.GetString(2); // DATE
            VALUE = q.GetDouble(3); // VALUE
            UPDTYPE = q.GetInt(4); // UPDTYPE
        }

        Data& operator=(const Data& other)
        {
            if (this == &other) return *this;

            HISTID = other.HISTID;
            SYMBOL = other.SYMBOL;
            DATE = other.DATE;
            VALUE = other.VALUE;
            UPDTYPE = other.UPDTYPE;
            return *this;
        }

        template<typename C>
        bool match(const C &c) const
        {
            return false;
        }
        bool match(const Self::HISTID &in) const
        {
            return this->HISTID == in.v_;
        }
        bool match(const Self::SYMBOL &in) const
        {
            return this->SYMBOL.CmpNoCase(in.v_) == 0;
        }
        bool match(const Self::DATE &in) const
        {
            return this->DATE.CmpNoCase(in.v_) == 0;
        }
        bool match(const Self::VALUE &in) const
        {
            return this->VALUE == in.v_;
        }
        bool match(const Self::UPDTYPE &in) const
        {
            return this->UPDTYPE == in.v_;
        }
        wxString to_json() const
        {
            json::Object o;
            this->to_json(o);
            std::wstringstream ss;
            json::Writer::Write(o, ss);
            return ss.str();
        }
        
        int to_json(json::Object& o) const
        {
            o[L"HISTID"] = json::Number(this->HISTID);
            o[L"SYMBOL"] = json::String(this->SYMBOL.ToStdWstring());
            o[L"DATE"] = json::String(this->DATE.ToStdWstring());
            o[L"VALUE"] = json::Number(this->VALUE);
            o[L"UPDTYPE"] = json::Number(this->UPDTYPE);
            return 0;
        }
        row_t to_row_t() const
        {
            row_t row;
            row(L"HISTID") = HISTID;
            row(L"SYMBOL") = SYMBOL;
            row(L"DATE") = DATE;
            row(L"VALUE") = VALUE;
            row(L"UPDTYPE") = UPDTYPE;
            return row;
        }
        void to_template(html_template& t) const
        {
            t(L"HISTID") = HISTID;
            t(L"SYMBOL") = SYMBOL;
            t(L"DATE") = DATE;
            t(L"VALUE") = VALUE;
            t(L"UPDTYPE") = UPDTYPE;
        }

        /** Save the record instance in memory to the database. */
        bool save(wxSQLite3Database* db)
        {
            if (db && db->IsReadOnly()) return false;
            if (!table_ || !db) 
            {
                wxLogError("can not save STOCKHISTORY_V1");
                return false;
            }

            return table_->save(this, db);
        }

        /** Remove the record instance from memory and the database. */
        bool remove(wxSQLite3Database* db)
        {
            if (!table_ || !db) 
            {
                wxLogError("can not remove STOCKHISTORY_V1");
                return false;
            }
            
            return table_->remove(this, db);
        }

        void destroy()
        {
            //if (this->id() < 0)
            //    wxSafeShowMessage("unsaved object", this->to_json());
            delete this;
        }
    };

    enum
    {
        NUM_COLUMNS = 5
    };

    size_t num_columns() const { return NUM_COLUMNS; }

    /** Name of the table*/    
    wxString name() const { return "STOCKHISTORY_V1"; }

    DB_Table_STOCKHISTORY_V1() : fake_(new Data())
    {
        query_ = "SELECT * FROM STOCKHISTORY_V1 ";
    }

    /** Create a new Data record and add to memory table (cache)*/
    Self::Data* create()
    {
        Self::Data* entity = new Self::Data(this);
        cache_.push_back(entity);
        return entity;
    }
    
    /** Create a copy of the Data record and add to memory table (cache)*/
    Self::Data* clone(const Data* e)
    {
        Self::Data* entity = create();
        *entity = *e;
        entity->id(-1);
        return entity;
    }

    /**
    * Saves the Data record to the database table.
    * Either create a new record or update the existing record.
    * Remove old record from the memory table (cache)
    */
    bool save(Self::Data* entity, wxSQLite3Database* db)
    {
        wxString sql = wxEmptyString;
        if (entity->id() <= 0) //  new & insert
        {
            sql = "INSERT INTO STOCKHISTORY_V1(SYMBOL, DATE, VALUE, UPDTYPE) VALUES(?, ?, ?, ?)";
        }
        else
        {
            sql = "UPDATE STOCKHISTORY_V1 SET SYMBOL = ?, DATE = ?, VALUE = ?, UPDTYPE = ? WHERE HISTID = ?";
        }

        try
        {
            wxSQLite3Statement stmt = db->PrepareStatement(sql);

            stmt.Bind(1, entity->SYMBOL);
            stmt.Bind(2, entity->DATE);
            stmt.Bind(3, entity->VALUE);
            stmt.Bind(4, entity->UPDTYPE);
            if (entity->id() > 0)
                stmt.Bind(5, entity->HISTID);

            stmt.ExecuteUpdate();
            stmt.Finalize();

            if (entity->id() > 0) // existent
            {
                for(Cache::iterator it = cache_.begin(); it != cache_.end(); ++ it)
                {
                    Self::Data* e = *it;
                    if (e->id() == entity->id()) 
                        *e = *entity;  // in-place update
                }
            }
        }
        catch(const wxSQLite3Exception &e) 
        { 
            wxLogError("STOCKHISTORY_V1: Exception %s, %s", e.GetMessage().c_str(), entity->to_json());
            return false;
        }

        if (entity->id() <= 0)
        {
            entity->id((db->GetLastRowId()).ToLong());
            index_by_id_.insert(std::make_pair(entity->id(), entity));
        }
        return true;
    }

    /** Remove the Data record from the database and the memory table (cache) */
    bool remove(int id, wxSQLite3Database* db)
    {
        if (id <= 0) return false;
        try
        {
            wxString sql = "DELETE FROM STOCKHISTORY_V1 WHERE HISTID = ?";
            wxSQLite3Statement stmt = db->PrepareStatement(sql);
            stmt.Bind(1, id);
            stmt.ExecuteUpdate();
            stmt.Finalize();

            Cache c;
            for(Cache::iterator it = cache_.begin(); it != cache_.end(); ++ it)
            {
                Self::Data* entity = *it;
                if (entity->id() == id) 
                {
                    index_by_id_.erase(entity->id());
                    delete entity;
                }
                else 
                {
                    c.push_back(entity);
                }
            }
            cache_.clear();
            cache_.swap(c);
        }
        catch(const wxSQLite3Exception &e) 
        { 
            wxLogError("STOCKHISTORY_V1: Exception %s", e.GetMessage().c_str());
            return false;
        }

        return true;
    }

    /** Remove the Data record from the database and the memory table (cache) */
    bool remove(Self::Data* entity, wxSQLite3Database* db)
    {
        if (remove(entity->id(), db))
        {
            entity->id(-1);
            return true;
        }

        return false;
    }

    template<typename... Args>
    Self::Data* get_one(const Args& ... args)
    {
        for (Index_By_Id::iterator it = index_by_id_.begin(); it != index_by_id_.end(); ++ it)
        {
            Self::Data* item = it->second;
            if (item->id() > 0 && match(item, args...)) 
            {
                ++ hit_;
                return item;
            }
        }

        ++ miss_;

        return 0;
    }
    
    /**
    * Search the memory table (Cache) for the data record.
    * If not found in memory, search the database and update the cache.
    */
    Self::Data* get(int id, wxSQLite3Database* db)
    {
        if (id <= 0) 
        {
            ++ skip_;
            return 0;
        }

        Index_By_Id::iterator it = index_by_id_.find(id);
        if (it != index_by_id_.end())
        {
            ++ hit_;
            return it->second;
        }
        
        ++ miss_;
        Self::Data* entity = 0;
        wxString where = wxString::Format(" WHERE %s = ?", PRIMARY::name().c_str());
        try
        {
            wxSQLite3Statement stmt = db->PrepareStatement(this->query() + where);
            stmt.Bind(1, id);

            wxSQLite3ResultSet q = stmt.ExecuteQuery();
            if(q.NextRow())
            {
                entity = new Self::Data(q, this);
                cache_.push_back(entity);
                index_by_id_.insert(std::make_pair(id, entity));
            }
            stmt.Finalize();
        }
        catch(const wxSQLite3Exception &e) 
        { 
            wxLogError("%s: Exception %s", this->name().c_str(), e.GetMessage().c_str());
        }
        
        if (!entity) 
        {
            entity = this->fake_;
            // wxLogError("%s: %d not found", this->name().c_str(), id);
        }
 
        return entity;
    }

    /**
    * Return a list of Data records (Data_Set) derived directly from the database.
    * The Data_Set is sorted based on the column number.
    */
    const Data_Set all(wxSQLite3Database* db, COLUMN col = COLUMN(0), bool asc = true)
    {
        Data_Set result;
        try
        {
            wxSQLite3ResultSet q = db->ExecuteQuery(col == COLUMN(0) ? this->query() : this->query() + " ORDER BY " + column_to_name(col) + " COLLATE NOCASE " + (asc ? " ASC " : " DESC "));

            while(q.NextRow())
            {
                Self::Data entity(q, this);
                result.push_back(std::move(entity));
            }

            q.Finalize();
        }
        catch(const wxSQLite3Exception &e) 
        { 
            wxLogError("%s: Exception %s", this->name().c_str(), e.GetMessage().c_str());
        }

        return result;
    }
};
#endif //
