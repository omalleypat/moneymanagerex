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

#ifndef MM_EX_ASSIGNMENTDIALOG_H_
#define MM_EX_ASSIGNMENTDIALOG_H_

#include "model/Model_Field.h"
#include "model/Model_Condition.h"
#include "model/Model_Payee.h"
#include "model/Model_Subcategory.h"
#include "model/Model_Assignment.h"
#include <wx/log.h> 

class mmTextCtrl;

enum
{
    ID_DIALOG_ASSIGNMENT = wxID_HIGHEST + 400,
    ID_DIALOG_ASSIGNMENT_BUTTON_ADD,
    ID_DIALOG_ASSIGNMENT_BUTTON_SELECT,
    ID_DIALOG_ASSIGNMENT_PAYEECOMBO,
    ID_DIALOG_CURRENCY_CHOICE,
    ID_DIALOG_CURRENCY_TEXT_PFX,
    ID_DIALOG_CURRENCY_TEXT_SFX,
    ID_DIALOG_CURRENCY_TEXT_DECIMAL,
    ID_DIALOG_CURRENCY_TEXT_GROUP,
    ID_DIALOG_CURRENCY_TEXT_UNIT,
    ID_DIALOG_CURRENCY_TEXT_CENTS,
    ID_DIALOG_CURRENCY_TEXT_SCALE,
    ID_DIALOG_CURRENCY_TEXT_BASECONVRATE,
    ID_DIALOG_CURRENCY_BUTTON_CANCEL,
    ID_DIALOG_CURRENCY_TEXT_SYMBOL,
};

class mmAssignmentDialog : public wxDialog
{
    wxDECLARE_DYNAMIC_CLASS(mmAssignmentDialog);
    wxDECLARE_EVENT_TABLE();

public:
    mmAssignmentDialog();
    ~mmAssignmentDialog();
    mmAssignmentDialog(wxWindow* parent, wxSQLite3Database* db, Model_Assignment::Data * currency);

    int getselectDbID() { return m_assignment->ASSIGNMENTID; };

private:
    bool Create(wxWindow* parent, wxWindowID id,
        const wxString& caption,
        const wxPoint& pos,
        const wxSize& size,
        long style);

    void CreateControls();

    // utility functions
    void OnSave(wxCommandEvent& event);
    void fillControls();

    Model_Assignment::Data * m_assignment; //Model Assignment var

    mmTextCtrl* m_assignmentName;
    wxComboBox* cbField_;
    wxStaticText*  field_label_;
    wxComboBox* cbCondition_;
    wxStaticText*  condition_label_;
    mmTextCtrl* m_assignmentCriterion;
    wxComboBox* cbPayee_;
    wxStaticText*  payee_label_;
    wxComboBox* cbSubcat_;
    wxStaticText*  subcat_label_;
    wxStaticText *SqlCodeSample_;
    
    wxCheckBox* cbOverwritePayee_;
    wxCheckBox* cbOverwriteCateg_;
    
    wxSQLite3Database* m_db;

};

#endif
