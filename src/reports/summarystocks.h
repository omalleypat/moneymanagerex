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

#ifndef _MM_EX_REPORTSUMMARYSTOCKS_H_
#define _MM_EX_REPORTSUMMARYSTOCKS_H_

#include "reportbase.h"
#include <vector>
#include "../reports/mmgraphgenerator.h"
#include "../reports/chart.h"
#include "mmDateRange.h"

class mmHTMLBuilder;

class mmReportSummaryStocks : public mmPrintableBase
{
public:
    mmReportSummaryStocks();
    virtual void RefreshData();
    virtual wxString getHTMLText();
    virtual wxString version();

private:
    void display_header(mmHTMLBuilder& hb);
    // structure for sorting of data
    struct data_holder { wxString name; wxString symbol; wxString date; double qty; double purchase; double current; double commission; double gainloss; double value; };
    struct account_holder { int id; wxString name; std::vector<data_holder> data; double gainloss; double total; };
    std::vector<account_holder> stocks_;
    double gain_loss_sum_total_;
    double stockBalance_;
};
class mmReportChartStocks : public mmPrintableBase 
{
public:
    mmReportChartStocks(mmDateRange* date_range=new mmAllTime()) :
        mmPrintableBase(), dtRange_(date_range) {}
    ~mmReportChartStocks() { delete dtRange_; }
    wxString getHTMLText();

private:
    mmDateRange* dtRange_;
};

class mmGraphHistoryStocks : public mmGraphGenerator
{
public:
    mmGraphHistoryStocks();
    ~mmGraphHistoryStocks();

    void init(std::vector<ChartData> aData, int indColor=0);
    bool Generate(const wxString& chartTitle);

private:
    wxScopedPtr<GraphChart> chart;
    wxString fileName;
};

#endif // _MM_EX_REPORTSUMMARYSTOCKS_H_