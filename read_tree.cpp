/*
An abstraction on ROOT's TTree class, to make it easier to read
values from the diffrent events
*/

class tree
{
private:
    TTree* m_tree = nullptr;
    unsigned int m_size;
    int MAX_SIZE;// max(ntrack)=192
    int m_ntrack;
    double m_eta[MAX_SIZE];
    double m_pT[MAX_SIZE];
    double m_pid[MAX_SIZE];
    double m_rap[MAX_SIZE];
    double m_phi[MAX_SIZE];
    
public:
    tree(const char* tree_name, TFile* ptr_to_file = nullptr, int max_size=200)
    {
        if (ptr_to_file){
            MAX_SIZE = max_size;
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


            m_tree = ptr_to_file->Get<TTree>(tree_name);
            m_size = m_tree->GetEntries();
            m_tree->SetBranchAddress("ntrack", &m_ntrack);
            m_tree->SetBranchAddress("eta", &m_eta);
            m_tree->SetBranchAddress("pT", &m_pT);
            m_tree->SetBranchAddress("rap", &m_rap);
            m_tree->SetBranchAddress("phi", &m_phi);
        }
        return;
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

    void get_ntrack(unsigned int index, int &ntrack)
    {
        if (index < m_size)
        {
            ntrack = m_ntrack;
        }
        else 
        {
            std::cout << "Error: Index Out of Range! Got " << index << " but maximum = " << m_size << '\n';
        }
    }

    int max_index()
    {
        return m_size;
    }
};
