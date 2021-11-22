#include "read.cpp"

void plot1and2()
{
    TFile *dataset = TFile::Open("13TeV_CR0_RHoff.root");
 
    TH1 *plot1 = new TH1F("h", "Plot - 2", 192, 0, 192);
    TH1 *plot2 = new TH1F("h", "Plot - 1", 192, 0, 192);

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
        std::cout << "processing tree: " << i+1 << " of 7 " << '\n';
        read_from_tree_variable<double> v_eta("eta", tree_names[i], dataset, tree_sizes[i]);
        read_from_tree_ntrack v_ntrack(tree_names[i], dataset);
        
        int size = v_ntrack.max_index();
        for (int j = 0; j < size; j++)
        {
            double* eta = v_eta.get(j);
            int ntrack = v_ntrack.get(j);

            for (int k = 0; k < ntrack; k++)
            {
                if (eta[k] < -1 || eta[k] > 1)
                {
                    plot1->Fill(ntrack);
                   // if (ntrack > 120)
                        //std::cout << ntrack << '\n';
                }
                else
                {
                    plot2->Fill(ntrack);
                }
            }
        }
    }
    auto c1 = new TCanvas("c1","Canvas",800,600);
    c1->Divide(2,1);

    plot1->SetFillColor(kYellow);
    plot1->GetYaxis()->SetTitle("Number of events in acceptance region");
    plot1->GetXaxis()->SetTitle("Multiplicity");
    c1->cd(1);
    plot1->Draw();

    plot2->SetFillColor(kYellow);
    plot2->GetYaxis()->SetTitle("Number of events not in acceptance region");
    plot2->GetXaxis()->SetTitle("Multiplicity");
    c1->cd(2);
    plot2->Draw();

    
}