/*******************************************************
 Copyright (C) 2006 Madhan Kanagavel

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

#ifndef MM_EX_MAINASSIGNMENT_DIALOG_H_
#define MM_EX_MAINASSIGNMENT_DIALOG_H_

#include "defs.h"
#include <wx/dataview.h>
#include <map>

class mmMainAssignmentDialog: public wxDialog
{
    wxDECLARE_DYNAMIC_CLASS(mmMainAssignmentDialog);
    wxDECLARE_EVENT_TABLE();

public:
    /// Constructors
    mmMainAssignmentDialog( ) {}

    mmMainAssignmentDialog(wxWindow* parent, wxSQLite3Database* db);

private:
    wxSQLite3Database* m_db;
    
    enum cols
    {
        ASSIG_NAME = 0,
        ASSIG_FIELD,
        ASSIG_COND,
        ASSIG_CRIT,
        ASSIG_PAYEE,
        ASSIG_CAT,
    };

    bool Create(wxWindow* parent
        , wxWindowID id
        , const wxString& caption
        , const wxPoint& pos
        , const wxSize& size
        , long style);

    /// Creates the controls and sizers

    void CreateControls();
    void fillControls();
    
    void OnBtnAdd(wxCommandEvent& event);
    void OnBtnEdit(wxCommandEvent& event);
    void OnBtnDelete(wxCommandEvent& event);
    void OnBtnExecuteSelected(wxCommandEvent& event);
    void OnBtnExecuteAll(wxCommandEvent& event);
    
    void OnListItemSelected(wxDataViewEvent& event);

    wxDataViewListCtrl* assignmentListBox_;
    std::map<int, wxString> ColName_;
    wxButton* itemButtonEdit_;
    wxButton* itemButtonDelete_;
    
    wxButton* itemButtonExecuteSelected_;
    wxButton* itemButtonExecuteAll_;
    
    int selectDbID_;
    int selectedIndex_;
};

#endif // MM_EX_MAINCURRENCY_DIALOG_H_

