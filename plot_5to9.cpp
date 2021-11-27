#include"read.cpp"
#include<stdio.h>
#include<math.h>
void plot_5to9(){
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
    
    Double_t nsum[6],mult[6],meanmult[6],netcharge[6],ncmean[6],var[6],std[6]; 
    //nsum,mult,meanmult,ncmean represent toatal no of particles,multiplicity,mean multiplicity, mean of netcharge respectively
 
  for(int i=0; i<6; i++)
  {
        read_from_tree_variable<double> v_eta("eta", tree_plot_names[i], dataset, tree_sizes[i]);
        read_from_tree_variable<double> v_pT("pT", tree_plot_names[i], dataset, tree_sizes[i]);
        read_from_tree_variable<int> v_pid("pid", tree_plot_names[i], dataset, tree_sizes[i]);
        read_from_tree_ntrack v_ntrack(tree_plot_names[i], dataset);
        
        int max_index = v_ntrack.max_index();
        
        for(int j=0; j<max_index; j++)
        {
            double* eta = v_eta.get(j);
            double* pT = v_pT.get(j);
            int* pid= v_pid.get(j);
            int ntrack = v_ntrack.get(j); 
           
            for(int k=0 ; k<ntrack ; k++)
            {
                if(eta[k]<1 && eta[k]>-1 && pT[k]>0.5 )
                {
                   nsum[i]+=1;
                   if(pid[k]>0)  netcharge[i]+=1;
                   if(pid[k]<0)  netcharge[i]-=1;
                   if(pid[k]==0) netcharge[i]+=0;
                }
                 
                if(eta[k]>1 || eta[k]<-1)
                {
                   mult[i]+=1;
                } 
            }       
        }
        
        meanmult[i]=mult[i]/max_index;
        ncmean[i]=netcharge[i]/max_index;
        var[i]=nsum[i]/max_index-(ncmean[i]*ncmean[i]); // As the charge of particles is either +1 or -1 , the sum of squares of charges of particles is the no of particles.
        std[i]=sqrt(var[i]);
  } 
  
  Double_t array[6]={10,30,50,70,90,110};
  Double_t var_scaledby_Mult[6]={var[0]/meanmult[0], var[1]/meanmult[1], var[2]/meanmult[2], var[3]/meanmult[3], var[4]/meanmult[4], var[5]/meanmult[5]};
  Double_t var_scaledby_Multsqr[6]={var[0]/(meanmult[0]*meanmult[0]), var[1]/(meanmult[1]*meanmult[1]), var[2]/(meanmult[2]*meanmult[2]), var[3]/(meanmult[3]*meanmult[3]), var[4]/(meanmult[4]*meanmult[4]),var[5]/(meanmult[5]*meanmult[5])};
  


  TCanvas *C1= new TCanvas();
  TGraph *gr1type1= new TGraph(6,array,ncmean);
          gr1type1->GetYaxis()->SetTitle("Mean(M)");
          gr1type1->GetXaxis()->SetTitle("Multiplicity bins");
          gr1type1->SetMarkerStyle(22);
          gr1type1->SetMarkerColor(2);
          gr1type1->SetMarkerSize(2);
          gr1type1->Draw("AP");

  TCanvas *C2= new TCanvas();
  TGraph *gr1type2= new TGraph(6,meanmult,ncmean);
          gr1type2->GetYaxis()->SetTitle("Mean(M)");
          gr1type2->GetXaxis()->SetTitle("Mean Multiplicities");
          gr1type2->SetMarkerStyle(22);
          gr1type2->SetMarkerColor(2);
          gr1type2->SetMarkerSize(2);
          gr1type2->Draw("AP");

  TCanvas *C3= new TCanvas();
  TGraph *gr2type1= new TGraph(6,meanmult,var);
          gr2type1->GetYaxis()->SetTitle("#sigma^{ 2}                         "); 
          gr2type1->GetXaxis()->SetTitle("Multiplicity bins");
          gr2type1->SetMarkerStyle(33);
          gr2type1->SetMarkerColor(4);
          gr2type1->SetMarkerSize(2);
          gr2type1->Draw("AP");

  TCanvas *C4= new TCanvas();
  TGraph *gr2type2= new TGraph(6,meanmult,var);
          gr2type2->GetYaxis()->SetTitle("#sigma^{ 2}                         "); 
          gr2type2->GetXaxis()->SetTitle("Mean Multiplicities");
          gr2type2->SetMarkerStyle(33);
          gr2type2->SetMarkerColor(4);
          gr2type2->SetMarkerSize(2);
          gr2type2->Draw("AP");

  TCanvas *C5= new TCanvas();
  TGraph *gr3type1= new TGraph(6,array,var_scaledby_Mult);
          gr3type1->GetYaxis()->SetTitle("#frac{#sigma^{ 2}}{Mean Multiplicity}                    ");
          gr3type1->GetXaxis()->SetTitle("Multiplicity bins");
          gr3type1->SetMarkerStyle(22);
          gr3type1->SetMarkerColor(6);
          gr3type1->SetMarkerSize(2);
          gr3type1->Draw("AP");


  TCanvas *C6= new TCanvas();
  TGraph *gr3type2= new TGraph(6,meanmult,var_scaledby_Mult);
          gr3type2->GetYaxis()->SetTitle("#frac{#sigma^{ 2}}{Mean Multiplicity}                    ");
          gr3type2->GetXaxis()->SetTitle("Mean Multiplicities");
          gr3type2->SetMarkerStyle(22);
          gr3type2->SetMarkerColor(6);
          gr3type2->SetMarkerSize(2);
          gr3type2->Draw("AP");


  TCanvas *C7= new TCanvas();
  TGraph *gr4type1= new TGraph(6,array,var_scaledby_Multsqr);
          gr4type1->GetYaxis()->SetTitle("#frac{#sigma^{ 2}}{square of mean multiplicity}                ");
          gr4type1->GetXaxis()->SetTitle("Multiplicity bins");
          gr4type1->SetMarkerStyle(33);
          gr4type1->SetMarkerColor(4);
          gr4type1->SetMarkerSize(2);
          gr4type1->Draw("AP");

  TCanvas *C8= new TCanvas();
  TGraph *gr4type2= new TGraph(6,meanmult,var_scaledby_Multsqr);
          gr4type2->GetYaxis()->SetTitle("#frac{#sigma^{ 2}}{square of mean multiplicity}                ");
          gr4type2->GetXaxis()->SetTitle("Mean Multiplicities");
          gr4type2->SetMarkerStyle(33);
          gr4type2->SetMarkerColor(4);
          gr4type2->SetMarkerSize(2);
          gr4type2->Draw("AP");

  TCanvas *C9= new TCanvas();
  TGraph *gr5type1= new TGraph(6,array,std);
          gr5type1->GetYaxis()->SetTitle("#sigma                                    ");
          gr5type1->GetXaxis()->SetTitle("Multiplicity bins");
          gr5type1->SetMarkerStyle(22);
          gr5type1->SetMarkerColor(2);
          gr5type1->SetMarkerSize(2);
          gr5type1->Draw("AP");

  TCanvas *C10= new TCanvas();
  TGraph *gr5type2= new TGraph(6,meanmult,std);
          gr5type2->GetYaxis()->SetTitle("#sigma                                    ");
          gr5type2->GetXaxis()->SetTitle("Mean Multiplicities");
          gr5type2->SetMarkerStyle(22);
          gr5type2->SetMarkerColor(2);
          gr5type2->SetMarkerSize(2);
          gr5type2->Draw("AP");


}