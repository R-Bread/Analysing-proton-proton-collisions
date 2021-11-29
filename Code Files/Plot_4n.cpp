void Plot_4n()
{
    //Normalized version of Net Charge vs Multiplicity plot

    TFile *dataset = TFile::Open("13TeV_CR0_RHoff.root");
    TH2F *plot = new TH2F("plot", " ", 140, 0, 140, 80, -40, 40);

    TCanvas *c1 = new TCanvas("c1", "Plot 4 - Normalized",18,41,700,500);
    c1->Range(-18.78508,-50.10526,169.0657,50);
    c1->SetRightMargin(0.1547278);
    c1->SetBorderSize(2);

    gStyle->SetPalette(kBird);

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
        read_from_tree_variable<int32_t> v_pid("pid", tree_names[i], dataset, tree_sizes[i]);

        int max_index = v_ntrack.max_index();
        double factor = (1e10)/max_index;

        TH2F *hist = new TH2F ("hist", " ", 140, 0, 140, 80, -40, 40);

        for (int j = 0; j < max_index; j++)
        {
            int ntrack = v_ntrack.get(j);
            
            double *eta = v_eta.get(j);
            double *pT = v_pT.get(j);
            int *pid = v_pid.get(j);

            int net = 0;
            int mult = 0;   //multiplicity
            int counter = 0;    //to make sure we're not counting the particles that don't satisfy the pT condition

            for (int k = 0; k < ntrack; k++)
            {
                if (pT[k] > 0.5)
                {
                    if (eta[k] < 1.0 && eta[k] > -1.0)  //net charge from acceptance region, |eta| < 1.0
                    {
                        if (pid[k] > 0) net = net+1;
                        else if (pid[k] < 0) net = net-1;
                    }
                    else mult++;

                    counter++;
                }
            }

            if (counter != 0) hist->Fill(mult,net);
                            
        }

        hist->Scale(factor/hist->Integral());
        plot->Add(hist);

        delete hist;

    }

    plot->SetContour(1000);
    plot->SetStats(0);

    TPaletteAxis *palette = new TPaletteAxis(145.6517,-39.89474,153.9946,40.10526,plot);
    
    plot->GetListOfFunctions()->Add(palette,"br");
    plot->GetXaxis()->SetTitle("Multiplicity ( |#eta| > 1 )");
    plot->GetYaxis()->SetTitle("Net Charge");
    plot->GetZaxis()->SetTitle("Normalized no. of Events");

    c1->SetLogz();
    c1->SetGridx();
    c1->SetGridy();
    c1->SetTickx();
    c1->SetTicky();

    plot->Draw("colz");

}
