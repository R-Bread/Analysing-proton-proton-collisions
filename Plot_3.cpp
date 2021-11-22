class tree
{
private:
    TTree* m_tree = nullptr;
    unsigned int m_size;
    int m_ntrack;
    int MAX_SIZE;// = 192;// max(ntrack)=192

    double *m_eta;
    double *m_pT;//[MAX_SIZE];
    double *m_pid;//[MAX_SIZE];
    double *m_rap;//[MAX_SIZE];
    double *m_phi;//[MAX_SIZE];
    
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
        m_pid = new double[max_size];
        m_pT = new double[max_size];
        m_rap = new double[max_size];
        m_phi = new double[max_size];

        MAX_SIZE = max_size;

        m_tree = ptr_to_file->Get<TTree>(tree_name);
        m_size = m_tree->GetEntries();
        m_tree->SetBranchAddress("ntrack", &m_ntrack);
        m_tree->SetBranchAddress("eta", m_eta);
        m_tree->SetBranchAddress("pT", m_pT);
        m_tree->SetBranchAddress("rap", m_rap);
        m_tree->SetBranchAddress("phi", m_phi);
    }
    ~tree()
    {
        delete[] m_eta;
        delete[] m_pid;
        delete[] m_rap;
        delete[] m_phi;
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
        else 
        {
            std::cout << "Error: Index Out of Range! Got " << index << " but maximum = " << m_size << '\n';
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
        else 
        {
            std::cout << "Error: Index Out of Range! Got " << index << " but maximum = " << m_size << '\n';
        }
    }
    
    void get_pid(unsigned int index, std::vector<double> &pid)
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
        else 
        {
            std::cout << "Error: Index Out of Range! Got " << index << " but maximum = " << m_size << '\n';
        }
    }
    
    void get_rap(unsigned int index, std::vector<double> &rap)
    {
        if (index < m_size)
        {
            m_tree->GetEntry(index);
            rap.reserve(m_ntrack);
            for (int i = 0; i < m_ntrack; i++)
            {
                rap.push_back(m_rap[i]);
            }
        }
        else 
        {
            std::cout << "Error: Index Out of Range! Got " << index << " but maximum = " << m_size << '\n';
        }

    }
    
    void get_phi(unsigned int index, std::vector<double> &phi)
    {
        if (index < m_size)
        {
            m_tree->GetEntry(index);
            phi.reserve(m_ntrack);
            for (int i = 0; i < m_ntrack; i++)
            {
                phi.push_back(m_phi[i]);
            }
        }
        else 
        {
            std::cout << "Error: Index Out of Range! Got " << index << " but maximum = " << m_size << '\n';
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

void Hist2D()
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
        tree t(tree_names[i], dataset, tree_sizes[i]);

        for (int j = 0; j < t.max_index(); j++)
        {
            int ntrack = t.get_ntrack(j);
            
            std::vector<double> eta = {};
            t.get_eta(j, eta);

            std::vector<double> pT = {};
            t.get_pT(j, pT);

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

    Hist2D->Draw("colz");
}