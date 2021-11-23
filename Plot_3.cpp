void Plot_3()
{
    TFile *dataset = TFile::Open("13TeV_CR0_RHoff.root");

    TCanvas *c1 = new TCanvas();
    gStyle->SetPalette(kRainBow);

    TH2I *Hist2D = new TH2I("Hist2D", "Plot 3", 150, 0, 150, 150, 0, 150);

    const char* tree_names[] = {
        "pytree",
        "pytree020",
        "pytree2040",
        "pytree4060",
        "pytree6080",
        "pytree80100",
        "pytree100"
    };

    const int tree_sizes[] = {192, 20, 40, 60,  80, 100, 192};

    for (int i = 1; i < 7; i++)
    {
        read_from_tree_ntrack v_ntrack(tree_names[i], dataset);
        read_from_tree_variable<double> v_eta("eta", tree_names[i], dataset, tree_sizes[i]);
        read_from_tree_variable<double> v_pT("pT", tree_names[i], dataset, tree_sizes[i]);

        int max_index = v_ntrack.max_index();
        
        for (int j = 0; j < max_index; j++)
        {
            int ntrack = v_ntrack.get(j);
            
            double *eta = v_eta.get(j);
            double *pT = v_pT.get(j);

            int x = 0;
            int y = 0;

            for (int k = 0; k < ntrack; k++)
            {
                if (pT[k] > 0.2 && pT[k] < 2.0)
                {
                    if (eta[k] < 1.0 && eta[k] > -1.0) y++;
                    else x++;
                }
            }

            Hist2D->Fill(x,y);
        }
    }

    Hist2D->GetXaxis()->SetTitle("Multiplicity in Non-Accepting Region");
    Hist2D->GetYaxis()->SetTitle("Mulitplicity in Accepting Region");
    Hist2D->GetZaxis()->SetTitle("Entries");

    Hist2D->SetStats(0);

    Hist2D->SetContour(1000);
    
    c1->SetLogz();
    c1->SetTickx();
    c1->SetTicky();
    c1->SetGridx();
    c1->SetGridy();
    
    //Some manual adjustments of position and size of some objects in the plot will be required

    Hist2D->Draw("colz");
}
