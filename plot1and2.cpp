void plot1and2()
{
    TFile *dataset = TFile::Open("13TeV_CR0_RHoff.root");

    const char* tree_plot_names[] = {
        "pytree020",
        "pytree2040",
        "pytree4060",
        "pytree6080",
        "pytree80100",
        "pytree100"
    };

    const int tree_sizes[] = {20, 40, 60,  80, 100, 192};

    TCanvas* c1 = new TCanvas("c1", "Plot-1", 800, 600);
    TCanvas* c2 = new TCanvas("c2", "Plot-2", 800, 600);
    TCanvas* c3 = new TCanvas("c3", "Plot-1 (combined)", 800, 600);
    TCanvas* c4 = new TCanvas("c4", "Plot-2 (combined)", 800, 600);
    c1->Divide(3, 2);
    c2->Divide(3, 2);
    
    TH1* plot1[6];
    TH1* plot2[6];
    TH1* plot1c = new TH1F("plot1c", "", 192, 0, 192);
    TH1* plot2c = new TH1F("plot2c", "", 192, 0, 192);
    

    for (int i = 0; i < 6; i++)
    {
        char plot_name[256];
        
        if (i != 5)
        {
            sprintf(plot_name, "%d to %d", i*20, (i+1)*20);
            plot1[i] = new TH1F(plot_name, "", 20, i*20, (i+1)*20);

            sprintf(plot_name, " %d to %d ", i*20, (i+1)*20);
            plot2[i] = new TH1F(plot_name, "", 20, i*20, (i+1)*20);
        }
        else
        {
            sprintf(plot_name, "%d to %d", 192-101, 192);
            plot1[i] = new TH1F(plot_name, "", 192-101, 101, 192);

            sprintf(plot_name, " %d to %d ", 192-101, 192);
            plot2[i] = new TH1F(plot_name, "", 192-101, 101, 192);
        }
         

        read_from_tree_variable<double> v_eta("eta", tree_plot_names[i], dataset, tree_sizes[i]);
        read_from_tree_variable<double> v_pT("pT", tree_plot_names[i], dataset, tree_sizes[i]);
        read_from_tree_ntrack v_ntrack(tree_plot_names[i], dataset);
        
        int max_index = v_ntrack.max_index();
        
        for (int j = 0; j < max_index; j++)
        {
            double* eta = v_eta.get(j);
            double* pT = v_pT.get(j);
            int ntrack = v_ntrack.get(j);

            for (int k = 0; k < ntrack; k++)
            {
                if (0.5 < pT[k] || pT[k] < -0.5)
                {
                    if (eta[k] < -1 || eta[k] > 1)
                    {
                        plot1[i]->Fill(ntrack);
                        plot1c->Fill(ntrack);
                    }
                    else
                    {
                        plot2[i]->Fill(ntrack);
                        plot2c->Fill(ntrack);
                    }
                }
            }   
        }
        
        plot1[i]->SetFillColor(kYellow);
        c1->cd(i+1);
        plot1[i]->Draw();

        plot2[i]->SetFillColor(kYellow);
        c2->cd(i+1);
        plot2[i]->Draw();
        
      
    }

    plot1[0]->GetYaxis()->SetTitle("Frequency");
    plot1[0]->GetYaxis()->SetTitleSize(0.06);
    plot1[5]->GetXaxis()->SetTitle("Multiplicity (|\\eta| > 1)");
    plot1[5]->GetXaxis()->SetTitleSize(0.06);

    plot2[0]->GetYaxis()->SetTitle("Frequency");
    plot2[0]->GetYaxis()->SetTitleSize(0.06);
    plot2[5]->GetXaxis()->SetTitle("Multiplicity (|\\eta| < 1)");
    plot2[5]->GetXaxis()->SetTitleSize(0.06);
    
    
    c3->cd();
    plot1c->GetXaxis()->SetTitle("Multiplicity (|\\eta| > 1)");
    plot1c->GetYaxis()->SetTitle("Frequency");
    plot1c->Draw();

    c4->cd();
    plot2c->GetXaxis()->SetTitle("Multiplicity (|\\eta| < 1)");
    plot2c->GetYaxis()->SetTitle("Frequency");
    plot2c->Draw();
    
}
