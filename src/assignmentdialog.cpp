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

#include "assignmentdialog.h"
#include "constants.h"
#include "util.h"
#include "model/Model_Currency.h"
#include "defs.h"
#include "paths.h"
#include "validators.h"
#include "mmtextctrl.h"
#include "mmSimpleDialogs.h"

#include <wx/combobox.h>
#include <wx/valnum.h>

wxIMPLEMENT_DYNAMIC_CLASS(mmAssignmentDialog, wxDialog);

wxBEGIN_EVENT_TABLE( mmAssignmentDialog, wxDialog )
    EVT_BUTTON(wxID_REFRESH, mmAssignmentDialog::OnSave)
wxEND_EVENT_TABLE()

mmAssignmentDialog::mmAssignmentDialog()
{
}

mmAssignmentDialog::~mmAssignmentDialog()
{
}

mmAssignmentDialog::mmAssignmentDialog(wxWindow* parent, wxSQLite3Database* db, Model_Assignment::Data * assignment)
    : m_db(db)
    , m_assignment(assignment)
{
    long style = wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX;
    Create(parent, wxID_STATIC, _("Currency Manager"), wxDefaultPosition, wxSize(500, 300), style);
}

bool mmAssignmentDialog::Create(wxWindow* parent, wxWindowID id
    , const wxString& caption, const wxPoint& pos
    , const wxSize& size, long style)
{
    SetExtraStyle(GetExtraStyle()|wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create(parent, id, caption, pos, size, style);

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);

    SetIcon(mmex::getProgramIcon());

    fillControls();

    Centre();
    return TRUE;
}

void mmAssignmentDialog::fillControls()
{
    if (m_assignment)
    {
        m_assignmentName->SetValue(m_assignment->ASSIGNMENTNAME);
        //~ m_assignmentField->SetValue(m_assignment->CURRENCY_SYMBOL);
        
        if (m_assignment->FIELDID>0){
          Model_Field::Data* field = Model_Field::instance().get(m_assignment->FIELDID);
          cbField_->ChangeValue(field->FIELDNAME);
        }
        
        if (m_assignment->CONDITIONID>0){
          Model_Condition::Data* condition = Model_Condition::instance().get(m_assignment->CONDITIONID);
          cbCondition_->ChangeValue(condition->CONDITIONNAME);
        }
        
        m_assignmentCriterion->SetValue(m_assignment->CRITERION);
        
        if (m_assignment->PAYEEID>0){
          Model_Payee::Data* payee = Model_Payee::instance().get(m_assignment->PAYEEID);
          cbPayee_->ChangeValue(payee->PAYEENAME);
        }
        
        if (m_assignment->SUBCATEGID>0){
          wxString select;
          select <<
          "SELECT " <<
          "  CASE " <<
          "    WHEN SUBCATEGNAME = \"\" THEN CATEGNAME " <<
          "    ELSE CATEGNAME || \" : \" || SUBCATEGNAME " <<
          "  END, " <<
          "  SUBCATEGID " <<
          "FROM category_v1 c JOIN subcategory_v1 s ON c.categid = s.categid " <<
          "WHERE s.subcategid = '" << m_assignment->SUBCATEGID << "' "
          "ORDER BY CATEGNAME ASC, SUBCATEGNAME ASC ";
          wxSQLite3ResultSet set = m_db->ExecuteQuery(select);
          wxArrayString autoCompleteSubcateg;
          wxString s;
          while (set.NextRow())
          {
            s = set.GetAsString(0);
          }
          set.Finalize(); 
          cbSubcat_->ChangeValue(s);
        }
        
        if(m_assignment->PAYEE_OVERWRITE_FLAG==1){
          cbOverwritePayee_->SetValue(true);
        }else{
          cbOverwritePayee_->SetValue(false);
        }
        
        if(m_assignment->SUBCATEG_OVERWRITE_FLAG==1){
          cbOverwriteCateg_->SetValue(true);
        }else{
          cbOverwriteCateg_->SetValue(false);
        }
        
        wxString select;
        select << 
        "SELECT * FROM V_ASSIGNMENT_SQL WHERE ASSIGNMENTID='" << m_assignment->ASSIGNMENTID << "'";
        wxSQLite3ResultSet set = m_db->ExecuteQuery(select);
        wxString s;
        while (set.NextRow())
        {
          s << set.GetAsString(3) << "\n";
        }
        set.Finalize(); 
        SqlCodeSample_->SetLabelText(s);
        int w,h;
        this->GetSize(&w,&h);
        SqlCodeSample_->Wrap(w-20);
    }

    // resize the dialog window
    Fit();
}

void mmAssignmentDialog::CreateControls()
{
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(itemBoxSizer2);

    wxFlexGridSizer* itemFlexGridSizer3 = new wxFlexGridSizer(0, 2, 0, 0);
    itemFlexGridSizer3->AddGrowableCol(1);
    itemBoxSizer2->Add(itemFlexGridSizer3, g_flags);

    //--------------------------
    itemFlexGridSizer3->Add(new wxStaticText(this, wxID_STATIC, _("Assignment Name")), g_flags);
    m_assignmentName = new mmTextCtrl(this, ID_DIALOG_CURRENCY_CHOICE
        , "", wxDefaultPosition, wxSize(220, -1));
    itemFlexGridSizer3->Add(m_assignmentName, g_flags);
    
    cbField_ = new wxComboBox(this, wxID_ANY, ""
        , wxDefaultPosition, wxSize(230, -1));

    field_label_ = new wxStaticText(this, wxID_STATIC, _("Assignment Field"));
    itemFlexGridSizer3->Add(field_label_, g_flagsExpand);
    itemFlexGridSizer3->Add(cbField_, g_flagsExpand);
    
    auto &fields = Model_Field::instance().all();
    wxArrayString autoCompleteField;
    for (auto &field : fields)
    {
        int *id = new int(field.FIELDID); 
        cbField_->Append(field.FIELDNAME,id);
        autoCompleteField.Add(field.FIELDNAME);
    }
    cbField_->AutoComplete(autoCompleteField);
    
    //***
    cbCondition_ = new wxComboBox(this, wxID_ANY, ""
        , wxDefaultPosition, wxSize(230, -1));
    
    condition_label_ = new wxStaticText(this, wxID_STATIC, _("Assignment Condition"));
    itemFlexGridSizer3->Add(condition_label_, g_flagsExpand);
    itemFlexGridSizer3->Add(cbCondition_, g_flagsExpand);
    
    auto &conditions = Model_Condition::instance().all();
    wxArrayString autoCompleteCondition;
    for (auto &condition : conditions)
    {
        int *id = new int(condition.CONDITIONID); 
        cbCondition_->Append(condition.CONDITIONNAME,id);
        autoCompleteCondition.Add(condition.CONDITIONNAME);
    }
    cbCondition_->AutoComplete(autoCompleteCondition);
    
    //***
    itemFlexGridSizer3->Add(new wxStaticText(this, wxID_STATIC, _("Assignment Criterion")), g_flags);
    m_assignmentCriterion = new mmTextCtrl(this, wxID_ANY
        , "", wxDefaultPosition, wxSize(220, -1));
    itemFlexGridSizer3->Add(m_assignmentCriterion, g_flagsExpand);
    
    cbPayee_ = new wxComboBox(this, ID_DIALOG_ASSIGNMENT_PAYEECOMBO, ""
        , wxDefaultPosition, wxSize(230, -1));

    payee_label_ = new wxStaticText(this, wxID_STATIC, _("Assignment Payee"));
    itemFlexGridSizer3->Add(payee_label_, g_flagsExpand);
    itemFlexGridSizer3->Add(cbPayee_, g_flagsExpand);
    
    auto &payees = Model_Payee::instance().all();
    wxArrayString autoCompletePayee;
    for (auto &payee : payees)
    {
        int *id = new int(payee.PAYEEID); 
        cbPayee_->Append(payee.PAYEENAME,id);
        autoCompletePayee.Add(payee.PAYEENAME);
    }
    cbPayee_->AutoComplete(autoCompletePayee);
    
    //***
    cbSubcat_ = new wxComboBox(this, wxID_ANY, ""
        , wxDefaultPosition, wxSize(230, -1));

    subcat_label_ = new wxStaticText(this, wxID_STATIC, _("Assignment Category"));
    itemFlexGridSizer3->Add(subcat_label_, g_flagsExpand);
    itemFlexGridSizer3->Add(cbSubcat_, g_flagsExpand);
    
    wxString select;
    select <<
    "SELECT " <<
    "  CASE " <<
    "    WHEN SUBCATEGNAME = \"\" THEN CATEGNAME " <<
    "    ELSE CATEGNAME || \" : \" || SUBCATEGNAME " <<
    "  END, " <<
    "  SUBCATEGID " <<
    "FROM category_v1 c JOIN subcategory_v1 s ON c.categid = s.categid " <<
    "ORDER BY CATEGNAME ASC, SUBCATEGNAME ASC ";
    wxSQLite3ResultSet set = m_db->ExecuteQuery(select);
    int count = 0;
    wxArrayString autoCompleteSubcateg;
    while (set.NextRow())
    {
      wxString s = set.GetAsString(0);
      int *id = new int(set.GetInt(1));
      cbSubcat_->Append(s,id);
      autoCompleteSubcateg.Add(s);
      count++;
    }
    set.Finalize(); 
    cbSubcat_->AutoComplete(autoCompleteSubcateg);

    itemFlexGridSizer3->Add(new wxStaticText(this, wxID_STATIC, _("Overwrite Payee")), g_flags);
    cbOverwritePayee_ = new wxCheckBox(this, wxID_STATIC, _("Overwrite Payee"), wxDefaultPosition
        , wxDefaultSize, wxCHK_2STATE);
    itemFlexGridSizer3->Add(cbOverwritePayee_, g_flagsExpand);
    
    itemFlexGridSizer3->Add(new wxStaticText(this, wxID_STATIC, _("Overwrite Category")), g_flags);
    cbOverwriteCateg_ = new wxCheckBox(this, wxID_STATIC, _("Overwrite Category"), wxDefaultPosition
        , wxDefaultSize, wxCHK_2STATE);
    itemFlexGridSizer3->Add(cbOverwriteCateg_, g_flagsExpand);
//~ 
    //~ itemFlexGridSizer3->Add(new wxStaticText(this, wxID_STATIC, _("Unit Name")), g_flags);
    //~ unitTx_ = new wxTextCtrl(this, ID_DIALOG_CURRENCY_TEXT_UNIT, "");
    //~ itemFlexGridSizer3->Add(unitTx_, g_flagsExpand);
//~ 
    //~ itemFlexGridSizer3->Add(new wxStaticText(this, wxID_STATIC, _("Cents Name")), g_flags);
    //~ centTx_ = new wxTextCtrl(this, ID_DIALOG_CURRENCY_TEXT_CENTS, "");
    //~ itemFlexGridSizer3->Add(centTx_, g_flagsExpand);
//~ 
    //~ itemFlexGridSizer3->Add(new wxStaticText(this, wxID_STATIC, _("Prefix Symbol")), g_flags);
    //~ pfxTx_ = new wxTextCtrl(this, ID_DIALOG_CURRENCY_TEXT_PFX, "");
    //~ itemFlexGridSizer3->Add(pfxTx_, g_flagsExpand);
//~ 
    //~ itemFlexGridSizer3->Add(new wxStaticText(this, wxID_STATIC, _("Suffix Symbol")), g_flags);
    //~ sfxTx_ = new wxTextCtrl(this, ID_DIALOG_CURRENCY_TEXT_SFX, "");
    //~ itemFlexGridSizer3->Add(sfxTx_, g_flagsExpand);
//~ 
    //~ itemFlexGridSizer3->Add(new wxStaticText(this, wxID_STATIC, _("Decimal Char")), g_flags);
    //~ decTx_ = new wxTextCtrl(this, ID_DIALOG_CURRENCY_TEXT_DECIMAL, "");
    //~ itemFlexGridSizer3->Add(decTx_, g_flagsExpand);
//~ 
    //~ itemFlexGridSizer3->Add(new wxStaticText(this, wxID_STATIC, _("Grouping Char")), g_flags);
    //~ grpTx_ = new wxTextCtrl(this, ID_DIALOG_CURRENCY_TEXT_GROUP, "");
    //~ itemFlexGridSizer3->Add(grpTx_, g_flagsExpand);
//~ 
    //~ wxIntegerValidator<int> valInt(&scale_,
        //~ wxNUM_VAL_THOUSANDS_SEPARATOR | wxNUM_VAL_ZERO_AS_BLANK);
    //~ valInt.SetMin(0); // Only allow positive numbers
    //~ valInt.SetMax(6);
    //~ itemFlexGridSizer3->Add(new wxStaticText(this, wxID_STATIC, _("Scale")), g_flags);
    //~ scaleTx_ = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize
        //~ , wxALIGN_RIGHT | wxTE_PROCESS_ENTER, valInt);
    //~ itemFlexGridSizer3->Add(scaleTx_, g_flagsExpand);
//~ 
    //~ itemFlexGridSizer3->Add(new wxStaticText(this, wxID_STATIC, _("Conversion to Base Rate")), g_flags);
    //~ baseConvRate_ = new mmTextCtrl(this, ID_DIALOG_CURRENCY_TEXT_BASECONVRATE, ""
        //~ , wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT | wxTE_PROCESS_ENTER
        //~ , mmCalcValidator());
    //~ itemFlexGridSizer3->Add(baseConvRate_, g_flagsExpand);
    //~ baseConvRate_->SetToolTip(_("Other currency conversion rate. Set Base Currency to 1."));

    //--------------------------
    wxStaticBox* itemStaticBox_02 = new wxStaticBox(this, wxID_ANY, _("Générate SQL code :"));
    wxStaticBoxSizer* itemStaticBoxSizer_02 = new wxStaticBoxSizer(itemStaticBox_02, wxHORIZONTAL);
    itemBoxSizer2->Add(itemStaticBoxSizer_02, g_flagsExpand);

    SqlCodeSample_ = new wxStaticText(this, wxID_STATIC, "");
    itemStaticBoxSizer_02->Add(SqlCodeSample_, g_flags);

    //~ //--------------------------
    //~ wxStaticBox* itemStaticBox_01 = new wxStaticBox(this, wxID_STATIC, _("Value Display Sample:"));
    //~ wxStaticBoxSizer* itemStaticBoxSizer_01 = new wxStaticBoxSizer(itemStaticBox_01, wxHORIZONTAL);
    //~ itemBoxSizer2->Add(itemStaticBoxSizer_01, g_flagsExpand);
//~ 
    //~ wxStaticText *sampleText_ = new wxStaticText(this, wxID_STATIC, "");
    //~ itemStaticBoxSizer_01->Add(sampleText_, g_flags);

    //--------------------------
    wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer22, wxSizerFlags(g_flags).Centre());

    wxButton* itemButton24 = new wxButton( this, wxID_REFRESH, _("&Save"));
    itemBoxSizer22->Add(itemButton24, g_flags);
    itemButton24->SetToolTip(_("Save any changes made"));

    wxButton* itemButton25 = new wxButton( this, wxID_CANCEL, _("&Close "));
    itemBoxSizer22->Add(itemButton25, g_flags);
    itemButton25->SetToolTip(_("Any changes will be lost without update"));
}


void mmAssignmentDialog::OnSave(wxCommandEvent& /*event*/)
{
    wxString name = m_assignmentName->GetValue();
    if (name.empty()) return;

    if (!m_assignment)
    {
        m_assignment = Model_Assignment::instance().create();
    }
    else
    {
        const auto &assignments = Model_Assignment::instance().find(Model_Assignment::ASSIGNMENTNAME(name));
        if (!assignments.empty() && m_assignment->ASSIGNMENTID == -1)
        {
            wxMessageBox(_("Assignment with same name exists"), _("Organize Assignment: Add Assignment"), wxOK | wxICON_ERROR);
            return;
        }
    }
    
    wxString cbNotFound;
    bool cbError = false;
    if(wxNOT_FOUND == cbField_->FindString(cbField_->GetValue())){cbError=true;cbNotFound=_("Field");};
    if(wxNOT_FOUND == cbCondition_->FindString(cbCondition_->GetValue())){cbError=true;cbNotFound=_("Condition");};
    if(wxNOT_FOUND == cbPayee_->FindString(cbPayee_->GetValue()) && cbPayee_->GetValue() != ""){cbError=true;cbNotFound=_("Payee");};
    if(wxNOT_FOUND == cbSubcat_->FindString(cbSubcat_->GetValue()) && cbSubcat_->GetValue()!=""){cbError=true;cbNotFound=_("Subcat");};
    if(cbError){
      wxMessageBox( wxString::Format(_("Bad value for combobox %s"),cbNotFound), _("Organize Assignment: Add/Edit Assignment"), wxOK | wxICON_ERROR);
      return;
    }
    
    if(cbPayee_->GetValue()=="" && cbSubcat_->GetValue()==""){
      wxMessageBox( wxString::Format(_("Select at least payee or category"),cbNotFound), _("Organize Assignment: Add/Edit Assignment"), wxOK | wxICON_ERROR);
      return;
    }

    m_assignment->ASSIGNMENTNAME = name;
    m_assignment->FIELDID = *(int *)cbField_->GetClientData(cbField_->FindString(cbField_->GetValue()));
    m_assignment->CONDITIONID = *(int *)cbCondition_->GetClientData(cbCondition_->FindString(cbCondition_->GetValue()));
    m_assignment->CRITERION = m_assignmentCriterion->GetValue();
    if(cbPayee_->GetValue()=="")
      m_assignment->PAYEEID = -1;
    else
      m_assignment->PAYEEID = *(int *)cbPayee_->GetClientData(cbPayee_->FindString(cbPayee_->GetValue()));
    if(cbSubcat_->GetValue()=="")
      m_assignment->SUBCATEGID = -1;
    else
      m_assignment->SUBCATEGID = *(int *)cbSubcat_->GetClientData(cbSubcat_->FindString(cbSubcat_->GetValue()));
    if(cbOverwriteCateg_->IsChecked()){
      m_assignment->SUBCATEG_OVERWRITE_FLAG = 1;
    }else{
      m_assignment->SUBCATEG_OVERWRITE_FLAG = 0;
    }
    if(cbOverwritePayee_->IsChecked()){
      m_assignment->PAYEE_OVERWRITE_FLAG = 1;
    }else{
      m_assignment->PAYEE_OVERWRITE_FLAG = 0;
    }
    //~ m_assignment->SCALE = static_cast<int>(pow(10,scal));
    //~ m_assignment->BASECONVRATE = baseConvRate;
    //~ m_assignment->CURRENCY_SYMBOL = m_assignmentField->GetValue().Trim();
    //~ m_assignment->CURRENCYNAME = m_assignmentName->GetValue();
//~ 
    Model_Assignment::instance().save(m_assignment);
    fillControls();
}
