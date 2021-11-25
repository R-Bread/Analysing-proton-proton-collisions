void Plot_3()
{
    TFile *dataset = TFile::Open("13TeV_CR0_RHoff.root");
    TH2I *Hist2D = new TH2I("Hist2D", " ", 130, 0, 130, 100, 0, 100);

    TCanvas *c1 = new TCanvas();
    c1->Range(-17.15105,-12.63158,156.348,112.3684);
    c1->SetBorderSize(2);
    c1->SetRightMargin(0.1518625);

    gStyle->SetPalette(kRainBow);

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
            int counter = 0;

            for (int k = 0; k < ntrack; k++)
            {
                if (pT[k] > 0.5)
                {
                    if (eta[k] < 1.0 && eta[k] > -1.0) y++;
                    else x++;

                    counter++;
                }
            }

            if (counter != 0) Hist2D->Fill(x,y);
        }
    }

    Hist2D->GetXaxis()->SetTitle("Multiplicity for |#eta| > 1");
    Hist2D->GetYaxis()->SetTitle("Mulitplicity for |#eta| < 1");
    Hist2D->GetZaxis()->SetTitle("No. of Events");

    Hist2D->SetStats(0);

    Hist2D->SetContour(1000);

    TPaletteAxis *palette = new TPaletteAxis(134.2256,-1.666577e-07,142.1797,100,Hist2D);
    Hist2D->GetListOfFunctions()->Add(palette,"br");
    
    c1->SetLogz();
    c1->SetTickx();
    c1->SetTicky();
    c1->SetGridx();
    c1->SetGridy();

    Hist2D->Draw("colz");
}
