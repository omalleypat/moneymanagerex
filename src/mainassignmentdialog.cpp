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

#include "mainassignmentdialog.h"
#include "assignmentdialog.h"
#include "constants.h"
//~ #include "util.h"
//~ #include "defs.h"
#include "paths.h"
//~ #include "mmCalculator.h"
//~ #include "validators.h"
#include "model/Model_Assignment.h"
#include "model/Model_Field.h"
#include "model/Model_Condition.h"
#include "model/Model_Payee.h"
#include "model/Model_Subcategory.h"
#include "model/Model_Category.h"
//~ #include "model/Model_Currency.h"
//~ #include "model/Model_Infotable.h"
//~ #include "model/Model_Setting.h"

#include <wx/log.h> 

#include "../resources/checkupdate.xpm"

wxIMPLEMENT_DYNAMIC_CLASS(mmMainAssignmentDialog, wxDialog);

wxBEGIN_EVENT_TABLE(mmMainAssignmentDialog, wxDialog)
    EVT_BUTTON(wxID_ADD, mmMainAssignmentDialog::OnBtnAdd)
    EVT_BUTTON(wxID_EDIT, mmMainAssignmentDialog::OnBtnEdit)
    EVT_BUTTON(wxID_DELETE, mmMainAssignmentDialog::OnBtnDelete)
    
    EVT_BUTTON(wxID_APPLY, mmMainAssignmentDialog::OnBtnExecuteSelected)
    EVT_BUTTON(wxID_EXECUTE, mmMainAssignmentDialog::OnBtnExecuteAll)
    
    EVT_DATAVIEW_SELECTION_CHANGED(wxID_ANY, mmMainAssignmentDialog::OnListItemSelected)
wxEND_EVENT_TABLE()

mmMainAssignmentDialog::mmMainAssignmentDialog(wxWindow* parent, wxSQLite3Database* db): m_db(db)
{
    ColName_[ASSIG_NAME]   = _("Assignment");
    ColName_[ASSIG_FIELD]  = _("Field");
    ColName_[ASSIG_COND]   = _("Condition");
    ColName_[ASSIG_CRIT]   = _("Criterion");
    ColName_[ASSIG_PAYEE]  = _("Payee");
    ColName_[ASSIG_CAT]    = _("Category");
    

    long style = wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX;
    Create(parent, wxID_ANY, _("Assignment Dialog"), wxDefaultPosition, wxDefaultSize, style);
}

bool mmMainAssignmentDialog::Create(wxWindow* parent
    , wxWindowID id
    , const wxString& caption
    , const wxPoint& pos
    , const wxSize& size
    , long style)
{
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create(parent, id, caption, pos, size, style);

    CreateControls();
    SetIcon(mmex::getProgramIcon());
    fillControls();
    //~ Centre();

    return TRUE;
}

void mmMainAssignmentDialog::fillControls()
{
    assignmentListBox_->DeleteAllItems();

    for (const auto& assignment : Model_Assignment::instance().all())
    {
        wxLogDebug("assignment id: " + assignment.ASSIGNMENTID );
        wxLogDebug("assignment name: " + assignment.ASSIGNMENTNAME);

        wxVector<wxVariant> data;
        data.push_back(wxVariant(assignment.ASSIGNMENTNAME)); 
        if(assignment.FIELDID>0){       
          data.push_back(wxVariant(Model_Field::instance().get(assignment.FIELDID)->FIELDNAME));
        }else{
          data.push_back(wxVariant(""));
        }
        if(assignment.CONDITIONID>0){
          data.push_back(wxVariant(Model_Condition::instance().get(assignment.CONDITIONID)->CONDITIONNAME));
        }else{
          data.push_back(wxVariant(""));
        }
        data.push_back(wxVariant(assignment.CRITERION));
        
        if(assignment.PAYEEID>0){
          data.push_back(wxVariant(Model_Payee::instance().get_payee_name(assignment.PAYEEID)));
        }else{
          data.push_back(wxVariant(""));
        }
        
        if(assignment.SUBCATEGID>0){
          Model_Subcategory::Data * subcategory = Model_Subcategory::instance().get(assignment.SUBCATEGID);
          Model_Category::Data * category = Model_Category::instance().get(subcategory->CATEGID);       
        data.push_back(wxVariant(category->CATEGNAME + ":" + subcategory->SUBCATEGNAME));
        }else{
          data.push_back(wxVariant(""));
        }
        
        assignmentListBox_->AppendItem(data, (wxUIntPtr)assignment.ASSIGNMENTID);
    }
}

void mmMainAssignmentDialog::CreateControls()
{
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer3, wxSizerFlags(g_flagsExpand).Border(wxALL, 10));

    assignmentListBox_ = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(20, 400), wxDV_MULTIPLE | wxDV_ROW_LINES);

    assignmentListBox_->AppendTextColumn(ColName_[ASSIG_NAME], wxDATAVIEW_CELL_INERT, wxLIST_AUTOSIZE_USEHEADER);
    assignmentListBox_->AppendTextColumn(ColName_[ASSIG_FIELD], wxDATAVIEW_CELL_INERT, wxLIST_AUTOSIZE_USEHEADER);
    assignmentListBox_->AppendTextColumn(ColName_[ASSIG_COND], wxDATAVIEW_CELL_EDITABLE, wxLIST_AUTOSIZE_USEHEADER);
    assignmentListBox_->AppendTextColumn(ColName_[ASSIG_CRIT], wxDATAVIEW_CELL_EDITABLE, wxLIST_AUTOSIZE_USEHEADER);
    assignmentListBox_->AppendTextColumn(ColName_[ASSIG_PAYEE], wxDATAVIEW_CELL_EDITABLE, wxLIST_AUTOSIZE_USEHEADER);
    assignmentListBox_->AppendTextColumn(ColName_[ASSIG_CAT], wxDATAVIEW_CELL_EDITABLE, wxLIST_AUTOSIZE_USEHEADER);

    itemBoxSizer3->Add(assignmentListBox_, wxSizerFlags(g_flagsExpand).Border(wxALL, 10));
    
    wxPanel* buttonsPanel = new wxPanel(this, wxID_ANY);
    itemBoxSizer2->Add(buttonsPanel, wxSizerFlags(g_flags).Center());
    
    //~ itemButtonExecuteSelected_
    //~ itemButtonExecuteAll_
    

    wxBoxSizer* buttonsSizer = new wxBoxSizer(wxVERTICAL);
    buttonsPanel->SetSizer(buttonsSizer);
    
    wxStdDialogButtonSizer* executeButtonsSizer = new wxStdDialogButtonSizer;
    buttonsSizer->Add(executeButtonsSizer);
    
    itemButtonExecuteSelected_ = new wxButton(buttonsPanel, wxID_APPLY, _("&Execute selected"));
    executeButtonsSizer->Add(itemButtonExecuteSelected_, g_flags);
    itemButtonExecuteSelected_->Disable();

    itemButtonExecuteAll_ = new wxButton(buttonsPanel, wxID_EXECUTE, _("&Execute all"));
    executeButtonsSizer->Add(itemButtonExecuteAll_, g_flags);
    
    wxStdDialogButtonSizer* itemBoxSizer66 = new wxStdDialogButtonSizer;
    buttonsSizer->Add(itemBoxSizer66);
  
    wxButton* itemButton7 = new wxButton(buttonsPanel, wxID_ADD, _("&Add"));
    itemBoxSizer66->Add(itemButton7, g_flags);

    itemButtonEdit_ = new wxButton(buttonsPanel, wxID_EDIT, _("&Edit"));
    itemBoxSizer66->Add(itemButtonEdit_, g_flags);
    itemButtonEdit_->Disable();

    itemButtonDelete_ = new wxButton(buttonsPanel, wxID_DELETE, _("&Delete"));
    itemBoxSizer66->Add(itemButtonDelete_, g_flags);
    itemButtonDelete_->Disable();

    

    this->SetMinSize(wxSize(600,550));
    this->Fit();
}

void mmMainAssignmentDialog::OnListItemSelected(wxDataViewEvent& event)
{
    wxDataViewItem item = event.GetItem();
    selectedIndex_ = assignmentListBox_->ItemToRow(item);
    if (selectedIndex_ >= 0)
    {
        selectDbID_ = (int)assignmentListBox_->GetItemData(item);
        itemButtonEdit_->Enable();
        itemButtonDelete_->Enable();
        itemButtonExecuteSelected_->Enable();
    }
}

void mmMainAssignmentDialog::OnBtnAdd(wxCommandEvent& /*event*/)
{
    mmAssignmentDialog dlg(this, m_db, 0);
    if (dlg.ShowModal() == wxID_OK)
    {
        selectDbID_ = dlg.getselectDbID();
        selectedIndex_ = -1;
    }

    fillControls();
}

void mmMainAssignmentDialog::OnBtnEdit(wxCommandEvent& /*event*/)
{
    Model_Assignment::Data *assignment = Model_Assignment::instance().get(selectDbID_);
    if (assignment)
        mmAssignmentDialog(this, m_db, assignment).ShowModal();
    fillControls();
}

void mmMainAssignmentDialog::OnBtnDelete(wxCommandEvent& /*event*/)
{
    if (selectedIndex_ < 0) return;

    Model_Assignment::Data* assignment = Model_Assignment::instance().get(selectDbID_);
    if (!assignment) return;
    if (wxMessageBox(_("Do you really want to delete the selected assignment rule?")
                     , _("Assignment Dialog")
                     , wxYES_NO | wxNO_DEFAULT | wxICON_ERROR) == wxYES)
    {
        Model_Assignment::instance().remove(selectDbID_);
        selectedIndex_ = -1;
        selectDbID_ = -1;
        fillControls();
    }
}

void mmMainAssignmentDialog::OnBtnExecuteSelected(wxCommandEvent& /*event*/)
{
  if (selectedIndex_ < 0) return;

  Model_Assignment::Data* assignment = Model_Assignment::instance().get(selectDbID_);
  if (!assignment) return;

  try{
    wxString select;
    wxString s;
    wxSQLite3ResultSet set;
    int nbPayee=0;
    int nbCateg=0;
    
    m_db->Begin();
    select = "";
    select << 
    "SELECT * FROM V_ASSIGNMENT_SQL WHERE TYPEFIELD='PAYEE' AND ASSIGNMENTID='" << assignment->ASSIGNMENTID << "'";
    set = m_db->ExecuteQuery(select);
    while (set.NextRow())
    {
      s="";
      s << set.GetAsString(3);
      wxLogDebug(s);
      nbPayee += m_db->ExecuteUpdate(s);
    }
    set.Finalize();
    
    select = "";
    select << 
    "SELECT * FROM V_ASSIGNMENT_SQL WHERE TYPEFIELD='CATEGORY' AND ASSIGNMENTID='" << assignment->ASSIGNMENTID << "'";
    set = m_db->ExecuteQuery(select);
    while (set.NextRow())
    {
      s="";
      s << set.GetAsString(3);
      wxLogDebug(s);
      nbCateg += m_db->ExecuteUpdate(s);
    }
    set.Finalize();  
    
    if(wxMessageBox(wxString::Format(_("%i Payee updated \n%i Category updated\n Apply ?"),nbPayee,nbCateg)
                       , _("Assignment Dialog")
                       , wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION)== wxYES)
    {
      m_db->Commit();
    }else{
      m_db->Rollback();
    }
  }catch (...){
  m_db->Rollback();
  wxMessageBox(_("Error")
   , _("Assignment Dialog")
   , wxOK | wxICON_ERROR
   );
  }
}

void mmMainAssignmentDialog::OnBtnExecuteAll(wxCommandEvent& /*event*/)
{
  try{
    wxString select;
    wxString s;
    wxSQLite3ResultSet set;
    int nbPayee=0;
    int nbCateg=0;
    
    m_db->Begin();
    select = "";
    select << 
    "SELECT * FROM V_ASSIGNMENT_SQL WHERE TYPEFIELD='PAYEE'";
    set = m_db->ExecuteQuery(select);
    while (set.NextRow())
    {
      s="";
      s << set.GetAsString(3);
      wxLogDebug(s);
      nbPayee += m_db->ExecuteUpdate(s);
    }
    set.Finalize();
    
    select = "";
    select << 
    "SELECT * FROM V_ASSIGNMENT_SQL WHERE TYPEFIELD='CATEGORY'";
    set = m_db->ExecuteQuery(select);
    while (set.NextRow())
    {
      s="";
      s << set.GetAsString(3);
      wxLogDebug(s);
      nbCateg += m_db->ExecuteUpdate(s);
    }
    set.Finalize();  
    
    if(wxMessageBox(wxString::Format(_("%i Payee updated \n%i Category updated\n Apply ?"),nbPayee,nbCateg)
                       , _("Assignment Dialog")
                       , wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION)== wxYES)
    {
      m_db->Commit();
    }else{
      m_db->Rollback();
    }
  }catch (...){
    m_db->Rollback();
    wxMessageBox(_("Error")
     , _("Assignment Dialog")
     , wxOK | wxICON_ERROR
     );
  }
}

