/*
An abstraction on ROOT's TTree class, to make it easier to read
values from the diffrent events
*/


       
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

class read_from_tree_variable
{
private:
    const char* variable_to_read;
    TTree* m_tree_ptr;
    int m_maxsize;
    double* m_var_data;
public:
    read_from_tree_variable(const char* variable_name, const char* tree_name, TFile* file, int max_size=200)
        : variable_to_read(variable_name), m_tree_ptr(file->Get<TTree>(tree_name))
    {
        m_maxsize = max_size;
        m_var_data = new double[max_size];
        m_tree_ptr->SetBranchAddress(variable_name, m_var_data);
    }

    ~read_from_tree_variable()
    {
        delete[] m_var_data;
    }

    double* get(int index)
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

