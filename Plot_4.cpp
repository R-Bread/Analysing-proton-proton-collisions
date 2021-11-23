class tree
{
private:
    TTree* m_tree = nullptr;
    unsigned int m_size;
    int m_ntrack;
    int MAX_SIZE;// = 192;// max(ntrack)=192

    double *m_eta;
    double *m_pT;//[MAX_SIZE];
    int *m_pid;//[MAX_SIZE];
    
public:
    tree(const char* tree_name, TFile* ptr_to_file, int max_size=200)
    {
        //MAX_SIZE = max_size;

        /*
        max sizes are:
            pytree          =   192
            pytree2040      =   40
            pytree4060      =   60
            pytree020       =   20
            pytree6080      =   80
            pytree80100     =   100
            pytree100       =   192
        */

        m_eta = new double[max_size];
        m_pid = new int[max_size];
        m_pT = new double[max_size];

        MAX_SIZE = max_size;

        m_tree = ptr_to_file->Get<TTree>(tree_name);
        m_size = m_tree->GetEntries();
        m_tree->SetBranchAddress("ntrack", &m_ntrack);
        m_tree->SetBranchAddress("eta", m_eta);
        m_tree->SetBranchAddress("pid", m_pid);
        m_tree->SetBranchAddress("pT", m_pT);
    }
    ~tree()
    {
        delete[] m_eta;
        delete[] m_pid;
        delete[] m_pT;
    }

    void get_eta(unsigned int index, std::vector<double> &eta)
    {
        if (index < m_size)
        {
            m_tree->GetEntry(index);
            eta.reserve(m_ntrack);
            for (int i = 0; i < m_ntrack; i++)
            {
                eta.push_back(m_eta[i]);
            }
        }
    }

    void get_pT(unsigned int index, std::vector<double> &pT)
    {
        if (index < m_size)
        {
            m_tree->GetEntry(index);
            pT.reserve(m_ntrack);
            for (int i = 0; i < m_ntrack; i++)
            {
                pT.push_back(m_pT[i]);
            }
        }
    }
    
    void get_pid(unsigned int index, std::vector<int> &pid)
    {
        if (index < m_size)
        {
            m_tree->GetEntry(index);
            pid.reserve(m_ntrack);
            for (int i = 0; i < m_ntrack; i++)
            {
                pid.push_back(m_pid[i]);
            }
        }
    }

    int get_ntrack(unsigned int index)
    {
        m_tree->GetEntry(index);
        return m_ntrack;
    }

    int max_index()
    {
        return m_size;
    }
};

void Plot_4()
{
    TFile *dataset = TFile::Open("13TeV_CR0_RHoff.root");
    TH2F *plot = new TH2F("plot", "Plot 4", 140, 0, 140, 80, -40, 40);

    TCanvas *c1 = new TCanvas();
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
        tree t(tree_names[i], dataset, tree_sizes[i]);

        for (int j = 0; j < t.max_index(); j++)
        {
            int ntrack = t.get_ntrack(j);
            
            std::vector<double> eta = {};
            t.get_eta(j, eta);

            std::vector<double> pT = {};
            t.get_pT(j, pT);

            std::vector<int> pid = {};
            t.get_pid(j, pid);

            int net = 0;
            int mult = 0;   //multiplicity
            int counter = 0;    //to make sure we're not counting the particles that don't satisfy the pT condition

            for (int k = 0; k < ntrack; k++)
            {
                if (pT[k] > 0.2 && pT[k] < 2.0)
                {
                    if (eta[k] < 1.0 && eta[k] > -1.0) 
                    {
                        if (pid[k] > 0) net = net+1;
                        else if (pid[k] < 0) net = net-1;
                    }
                    else mult++;

                    counter++;
                }
            }

            if (counter != 0) plot->Fill(mult,net);
                            
        }
    }

    plot->SetContour(1000);
    plot->SetStats(0);

    plot->GetXaxis()->SetTitle("Multiplicity");
    plot->GetYaxis()->SetTitle("Net Charge");
    plot->GetZaxis()->SetTitle("Entries");

    c1->SetLogz();
    c1->SetGridx();
    c1->SetGridy();
    c1->SetTickx();
    c1->SetTicky();

    //Some manual adjustments of position and size of some objects in the plot will be required

    plot->Draw("colz");

}