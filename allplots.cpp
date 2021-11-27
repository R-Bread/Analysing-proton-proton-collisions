#pragma once

template<typename d_type>
class read_from_tree_variable
{
private:
    const char* variable_to_read;
    TTree* m_tree_ptr;
    int m_maxsize;
    d_type* m_var_data;
public:
    read_from_tree_variable(const char* variable_name, const char* tree_name, TFile* file, int max_size=200)
        : variable_to_read(variable_name), m_tree_ptr(file->Get<TTree>(tree_name))
    {
        m_maxsize = max_size;
        m_var_data = new d_type[max_size];
        m_tree_ptr->SetBranchAddress(variable_name, m_var_data);
    }

    ~read_from_tree_variable()
    {
        delete[] m_var_data;
    }

    d_type* get(int index)
    {
        m_tree_ptr->GetEntry(index);
        return m_var_data;
    }

};

class read_from_tree_ntrack
{
private:
    const char* variable_to_read;
    TTree* m_tree_ptr;
    int ntrack;
public:
    read_from_tree_ntrack(const char* tree_name, TFile* file)
        : m_tree_ptr(file->Get<TTree>(tree_name))
    {
        m_tree_ptr->SetBranchAddress("ntrack", &ntrack);
    }

    int get(int index)
    {
        m_tree_ptr->GetEntry(index);
        return ntrack;
    }

    int max_index()
    {
        return m_tree_ptr->GetEntries();
    }
};

void allplots()
{
    TFile *dataset = TFile::Open("13TeV_CR0_RHoff.root");

    TH2F *plot4 = new TH2F("plot4", " ", 140, 0, 140, 80, -40, 40);
    TH2I *Hist2D = new TH2I("Hist2D", "Plot 3", 150, 0, 150, 150, 0, 150);

    auto c1 = new TCanvas("c1", "Plot-1", 800, 600);
    auto c2 = new TCanvas("c2", "Plot-2", 800, 600);
    c1->Divide(3, 2);
    c2->Divide(3, 2);

    TH1 *plot1[6];
    TH1 *plot2[6];

    const char *tree_plot_names[] = {
        "pytree020",
        "pytree2040",
        "pytree4060",
        "pytree6080",
        "pytree80100",
        "pytree100"};

    const int tree_sizes[] = {20, 40, 60, 80, 100, 192};

    double nsum[6], mult[6], meanmult[6], netcharge[6], ncmean[6], var[6], std[6];
    // nsum,mult,meanmult,ncmean represent toatal no of particles,multiplicity,mean multiplicity, mean of netcharge respectively

    for (int i = 0; i < 6; i++)
    {
        read_from_tree_variable<double> v_eta("eta", tree_plot_names[i], dataset, tree_sizes[i]);
        read_from_tree_variable<double> v_pT("pT", tree_plot_names[i], dataset, tree_sizes[i]);
        read_from_tree_variable<int> v_pid("pid", tree_plot_names[i], dataset, tree_sizes[i]);
        read_from_tree_ntrack v_ntrack(tree_plot_names[i], dataset);

        int max_index = v_ntrack.max_index();

        char plot_name[256];

        if (i != 5)
        {
            sprintf(plot_name, "%d to %d", i * 20, (i + 1) * 20);
            plot1[i] = new TH1F(plot_name, "", 20, i * 20, (i + 1) * 20);

            sprintf(plot_name, " %d to %d ", i * 20, (i + 1) * 20);
            plot2[i] = new TH1F(plot_name, "", 20, i * 20, (i + 1) * 20);
        }
        else
        {
            sprintf(plot_name, "%d to %d", 192 - 101, 192);
            plot1[i] = new TH1F(plot_name, "", 192 - 101, 101, 192);

            sprintf(plot_name, " %d to %d ", 192 - 101, 192);
            plot2[i] = new TH1F(plot_name, "", 192 - 101, 101, 192);
        }
        for (int j = 0; j < max_index; j++)
        {

            double *eta = v_eta.get(j);
            double *pT = v_pT.get(j);
            int *pid = v_pid.get(j);
            int ntrack = v_ntrack.get(j);

            int net = 0;
            int multiplicity = 0; // multiplicity
            int counter = 0;      // to make sure we're not counting the particles that don't satisfy the pT condition

            int x = 0;
            int y = 0;

            for (int k = 0; k < ntrack; k++)
            {
                if (pT[k] > 0.2 && pT[k] < 2.0)
                {
                    if (eta[k] < 1.0 && eta[k] > -1.0)
                        y++;
                    else
                        x++;
                }

                if (pT[k] > 0.05)
                {
                    if (eta[k] < 1.0 && eta[k] > -1.0) // net charge from acceptance region, |eta| < 1.0
                    {
                        if (pid[k] > 0)
                            net = net + 1;
                        else if (pid[k] < 0)
                            net = net - 1;
                    }
                    else
                        multiplicity++;

                    counter++;
                }

                if (eta[k] < 1 && eta[k] > -1 && pT[k] > 0.5)
                {
                    nsum[i] += 1;
                    if (pid[k] > 0)
                        netcharge[i] += 1;
                    if (pid[k] < 0)
                        netcharge[i] -= 1;
                    if (pid[k] == 0)
                        netcharge[i] += 0;
                }

                if (eta[k] > 1 || eta[k] < -1)
                {
                    mult[i] += 1;
                }
                if (eta[k] < -1 || eta[k] > 1)
                {
                    plot1[i]->Fill(ntrack);
                }
                else
                {
                    plot2[i]->Fill(ntrack);
                }
            }
            if (counter != 0)
                plot4->Fill(multiplicity, net);

            Hist2D->Fill(x, y);
        }
        plot1[i]->SetFillColor(kYellow);
        c1->cd(i + 1);
        plot1[i]->Draw();

        plot2[i]->SetFillColor(kYellow);
        c2->cd(i + 1);
        plot2[i]->Draw();

        meanmult[i] = mult[i] / max_index;
        ncmean[i] = netcharge[i] / max_index;
        var[i] = nsum[i] / max_index - (ncmean[i] * ncmean[i]); // As the charge of particles is either +1 or -1 , the sum of squares of charges of particles is the no of particles.
        std[i] = sqrt(var[i]);
    }

    double indexes[6] = {10, 30, 50, 70, 90, 110};
    double var_scaledby_Mult[6], var_scaledby_Multsqr[6];

    for (int i = 0; i < 6; i++)
    {
        var_scaledby_Mult[i] = var[i] / meanmult[i];
        var_scaledby_Multsqr[i] = var[i] / (meanmult[i] * meanmult[i]);
    }
    plot1[0]->GetYaxis()->SetTitle("Frequency");
    plot1[0]->GetYaxis()->SetTitleSize(0.06);
    plot1[5]->GetXaxis()->SetTitle("Multiplicity (|\\eta| > 1)  ");
    plot1[5]->GetXaxis()->SetTitleSize(0.06);

    plot2[0]->GetYaxis()->SetTitle("Frequency");
    plot2[0]->GetYaxis()->SetTitleSize(0.06);
    plot2[5]->GetXaxis()->SetTitle("Multiplicity (|\\eta| < 1)  ");
    plot2[5]->GetXaxis()->SetTitleSize(0.06);

    TCanvas *c3 = new TCanvas();

    Hist2D->GetXaxis()->SetTitle("Multiplicity in Non-Accepting Region");
    Hist2D->GetYaxis()->SetTitle("Mulitplicity in Accepting Region");
    Hist2D->GetZaxis()->SetTitle("Entries");

    Hist2D->SetStats(0);

    Hist2D->SetContour(1000);

    c3->SetLogz();
    c3->SetTickx();
    c3->SetTicky();
    c3->SetGridx();
    c3->SetGridy();

    // Some manual adjustments of position and size of some objects in the plot will be required

    Hist2D->Draw("colz");

    TCanvas *c4 = new TCanvas("c4", "Plot 4", 18, 41, 700, 500);
    c4->Range(-18.78508, -50.10526, 169.0657, 50);
    c4->SetRightMargin(0.1547278);
    c4->SetBorderSize(2);

    gStyle->SetPalette(kRainBow);
    plot4->SetContour(1000);
    plot4->SetStats(0);

    TPaletteAxis *palette = new TPaletteAxis(145.6517, -39.89474, 153.9946, 40.10526, plot4);

    plot4->GetListOfFunctions()->Add(palette, "br");
    plot4->GetXaxis()->SetTitle("multiplicityiplicity");
    plot4->GetYaxis()->SetTitle("Net Charge");
    plot4->GetZaxis()->SetTitle("Entries");

    c4->SetLogz();
    c4->SetGridx();
    c4->SetGridy();
    c4->SetTickx();
    c4->SetTicky();

    plot4->Draw("colz");

    TCanvas *C1 = new TCanvas();
    TGraph *gr1type1 = new TGraph(6, indexes, ncmean);
    gr1type1->GetYaxis()->SetTitle("Mean(M)");
    gr1type1->GetXaxis()->SetTitle("Multiplicity bins");
    gr1type1->GetXaxis()->SetLimits(0, 120);
    gr1type1->SetMarkerStyle(22);
    gr1type1->SetMarkerColor(2);
    gr1type1->SetMarkerSize(2);
    gr1type1->Draw("AP");

    TCanvas *C2 = new TCanvas();
    TGraph *gr1type2 = new TGraph(6, meanmult, ncmean);
    gr1type2->GetYaxis()->SetTitle("Mean(M)");
    gr1type2->GetXaxis()->SetTitle("Mean Multiplicities");
    gr1type2->GetXaxis()->SetLimits(0, 120);
    gr1type2->SetMarkerStyle(22);
    gr1type2->SetMarkerColor(2);
    gr1type2->SetMarkerSize(2);
    gr1type2->Draw("AP");

    TCanvas *C3 = new TCanvas();
    TGraph *gr2type1 = new TGraph(6, meanmult, var);
    gr2type1->GetYaxis()->SetTitle("#sigma^{ 2}                         ");
    gr2type1->GetXaxis()->SetLimits(0, 120);
    gr2type1->GetXaxis()->SetTitle("Multiplicity bins");
    gr2type1->SetMarkerStyle(33);
    gr2type1->SetMarkerColor(4);
    gr2type1->SetMarkerSize(2);
    gr2type1->Draw("AP");

    TCanvas *C4 = new TCanvas();
    TGraph *gr2type2 = new TGraph(6, meanmult, var);
    gr2type2->GetYaxis()->SetTitle("#sigma^{ 2}                         ");
    gr2type2->GetXaxis()->SetLimits(0, 120);
    gr2type2->GetXaxis()->SetTitle("Mean Multiplicities");
    gr2type2->SetMarkerStyle(33);
    gr2type2->SetMarkerColor(4);
    gr2type2->SetMarkerSize(2);
    gr2type2->Draw("AP");

    TCanvas *C5 = new TCanvas();
    TGraph *gr3type1 = new TGraph(6, indexes, var_scaledby_Mult);
    gr3type1->GetYaxis()->SetTitle("#frac{#sigma^{ 2}}{Mean Multiplicity}                    ");
    gr3type1->GetXaxis()->SetLimits(0, 120);
    gr3type1->GetXaxis()->SetTitle("Multiplicity bins");
    gr3type1->SetMarkerStyle(22);
    gr3type1->SetMarkerColor(6);
    gr3type1->SetMarkerSize(2);
    gr3type1->Draw("AP");

    TCanvas *C6 = new TCanvas();
    TGraph *gr3type2 = new TGraph(6, meanmult, var_scaledby_Mult);
    gr3type2->GetYaxis()->SetTitle("#frac{#sigma^{ 2}}{Mean Multiplicity}                    ");
    gr3type2->GetXaxis()->SetLimits(0, 120);
    gr3type2->GetXaxis()->SetTitle("Mean Multiplicities");
    gr3type2->SetMarkerStyle(22);
    gr3type2->SetMarkerColor(6);
    gr3type2->SetMarkerSize(2);
    gr3type2->Draw("AP");

    TCanvas *C7 = new TCanvas();
    TGraph *gr4type1 = new TGraph(6, indexes, var_scaledby_Multsqr);
    gr4type1->GetYaxis()->SetTitle("#frac{#sigma^{ 2}}{square of mean multiplicity}                ");
    gr4type1->GetXaxis()->SetLimits(0, 120);
    gr4type1->GetXaxis()->SetTitle("Multiplicity bins");
    gr4type1->SetMarkerStyle(33);
    gr4type1->SetMarkerColor(4);
    gr4type1->SetMarkerSize(2);
    gr4type1->Draw("AP");

    TCanvas *C8 = new TCanvas();
    TGraph *gr4type2 = new TGraph(6, meanmult, var_scaledby_Multsqr);
    gr4type2->GetYaxis()->SetTitle("#frac{#sigma^{ 2}}{square of mean multiplicity}                ");
    gr4type2->GetXaxis()->SetLimits(0, 120);
    gr4type2->GetXaxis()->SetTitle("Mean Multiplicities");
    gr4type2->SetMarkerStyle(33);
    gr4type2->SetMarkerColor(4);
    gr4type2->SetMarkerSize(2);
    gr4type2->Draw("AP");

    TCanvas *C9 = new TCanvas();
    TGraph *gr5type1 = new TGraph(6, indexes, std);
    gr5type1->GetYaxis()->SetTitle("#sigma                                    ");
    gr5type1->GetXaxis()->SetTitle("Multiplicity bins");
    gr5type1->GetXaxis()->SetLimits(0, 120);
    gr5type1->SetMarkerStyle(22);
    gr5type1->SetMarkerColor(2);
    gr5type1->SetMarkerSize(2);
    gr5type1->Draw("AP");

    TCanvas *C10 = new TCanvas();
    TGraph *gr5type2 = new TGraph(6, meanmult, std);
    gr5type2->GetYaxis()->SetTitle("#sigma                                    ");
    gr5type2->GetXaxis()->SetTitle("Mean Multiplicities");
    gr5type2->GetXaxis()->SetLimits(0, 120);
    gr5type2->SetMarkerStyle(22);
    gr5type2->SetMarkerColor(2);
    gr5type2->SetMarkerSize(2);
    gr5type2->Draw("AP");
}
