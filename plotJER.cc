#include "TApplication.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TF1.h"
#include "TFitResult.h"
#include "TH1F.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TVectorF.h"
#include "TLegend.h"
#include "TStyle.h"

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>

using namespace std;

double GetRMS(std::vector<std::string>& files);
double RMSFitPercentageRange(TH1F* m_histogram);

int main()
{
   // set axis on both sides
   gStyle->SetPadTickX(1);
   gStyle->SetPadTickY(1);

   double cms[] = {91};
   //double cms[] = {91, 200, 350, 500};

   std::vector< std::string > fileDirs;
   std::vector< std::string > graphNames;

   //fileDirs.push_back("./SemiPerfect"); graphNames.push_back("original semi perfect");
   //fileDirs.push_back("./SemiPerfect_curr"); graphNames.push_back("current semi perfect");
   //fileDirs.push_back("./FullPerfect_curr"); graphNames.push_back("perfect");

   //////////////////// ------>
   //fileDirs.push_back("./SemiPerfect_curr"); graphNames.push_back("perfect correction");
   
   // perfect clustering, with real track-cluster association
   //fileDirs.push_back("./TrackClusterAssociation"); graphNames.push_back("DistanceToHelix: 2.5 cm (default)");
   //fileDirs.push_back("./TrackClusterAssociation2"); graphNames.push_back("DistanceToHelix: 2.5 cm (default)");

   // very weird result ???
   //fileDirs.push_back("./RealClustring"); graphNames.push_back("Real clustering");

   ////////////////////////////
   // vary m_maxDistanceToHelix
   // TrackClusterAssociationAlgorithm
   ///////////////////////////
   //fileDirs.push_back("./run_50_false"); graphNames.push_back("DistanceToHelix: 5cm");
   //fileDirs.push_back("./run_100_false"); graphNames.push_back("DistanceToHelix: 10cm");
   //fileDirs.push_back("./run_150_false"); graphNames.push_back("DistanceToHelix: 15 cm");
   //fileDirs.push_back("./PerfectTrackClusterAssociation2"); graphNames.push_back("Perfect clustering and track-cluster association"); 

   // BDT: tree number 800
   //fileDirs.push_back("./mva1"); graphNames.push_back("MVA"); 

   // BDT: tree number 2000
   //fileDirs.push_back("./test_bdt2000"); graphNames.push_back("BDT"); 

   // compatibility < -0.1
   //fileDirs.push_back("./test1"); graphNames.push_back("test -0.1"); 

   // compatibility < -0.2
   // not good 

   // reject the compatibility<0.3 from backgrounds by MC 
   //fileDirs.push_back("./test_rejmc"); graphNames.push_back("Reject some bad combinations (< 0.3)"); 

   // and perfect clustering
   // use MC truth PFO particle to associate cluster and track
   //fileDirs.push_back("./PerfectTrackClusterAssociation"); graphNames.push_back("Perfect clustering and track-cluster association"); 
   
   // a little bit different mathching from the upper one: track-cluster matching first, then
   // matching with MC truth PFO particle
   // And the result is a little bit different, worse
   //fileDirs.push_back("./mcmatching"); graphNames.push_back("Matching by MC truth"); 

   // ------------------------------------------------------------------
   //fileDirs.push_back("./fullPerfect"); graphNames.push_back("MC truth");

   //fileDirs.push_back("./testPfoCreation"); graphNames.push_back("PfoCreation");
   //fileDirs.push_back("./trackCluster"); graphNames.push_back("Track-cluster association");
   //fileDirs.push_back("./testTrackClusterMVAMatching0"); graphNames.push_back("MVA_withcut"); 
   //fileDirs.push_back("./testTrackClusterMVAMatching"); graphNames.push_back("MVA"); 

   //fileDirs.push_back("../../PandoraPFA_clustering/test"); graphNames.push_back("PandoraPFA"); 

   fileDirs.push_back("./"); graphNames.push_back("APRIL"); 
   // issues ...
   //fileDirs.push_back("./testPFODecision"); graphNames.push_back("PFO decision"); 
   //fileDirs.push_back("./testPFODecisionAndTrkClu"); graphNames.push_back("PFO decision & TrkClu"); 


   //fileDirs.push_back("./perfectClustering"); graphNames.push_back("perfectClustering"); 

   ///////////////////////////////////////////////////////

   int nEnergyPoints = (sizeof(cms)/sizeof(*cms));
   int nSettings = fileDirs.size();

   std::cout << "EnergyPoints: " <<  nEnergyPoints << ", file settings: " << nSettings << std::endl;

   ///////////////////////////////////////////////////////

   // color
   std::vector<int> colors = {6, 2, 3, 4, 28, 7, 9, 29, 30, 41};
   std::vector<int> colorsPerfect = {6, 2, 3, 4, 28, 7, 9, 29, 30, 41};

   ///////////////////////////////////////////////////////

   std::vector<float> Ejet;

   for(Int_t i=0; i<nEnergyPoints; ++i)
   {
	   float ejet = cms[i] / 2.;
	   Ejet.push_back(ejet);
   }

   ///////////////////////////////////////////////////////
   //   for each setting of test samples
   ///////////////////////////////////////////////////////

   std::vector< std::vector<float> > JERs;
   std::vector< std::vector<float> > JERexs;
   std::vector< std::vector<float> > JEReys;

   for(Int_t i=0; i<nSettings; ++i)
   {
	   std::vector<float> JER;
	   std::vector<float> JERex;
	   std::vector<float> JERey;

	   std::string fileDir = fileDirs[i];

       for (Int_t i=0; i<nEnergyPoints; ++i) 
       {
           std::vector<std::string> files;

           // use combined root file
           std::string fileName("/rv01-19-05-p01.sv01-19-05.mILD_l5_o1_v02_nobg.Puds");
           fileName = fileDir + fileName + std::to_string((int)cms[i]) + ".root";
           std::cout << fileName << std::endl;

           files.push_back(fileName);
           double jetrms90 = GetRMS(files);

           float jetEnergyReso = jetrms90 / cms[i] * sqrt(2) * 100.;

		   std::cout << "JER: " << jetEnergyReso << std::endl;

		   const int EVENTNUM = 6000;
		   float ResoError = jetEnergyReso / sqrt(2. * EVENTNUM);

		   JER.push_back(jetEnergyReso);
		   JERex.push_back(0.);
		   JERey.push_back(ResoError);
       }

	   JERs.push_back(JER);
	   JERexs.push_back(JERex);
	   JEReys.push_back(JERey);
   }

   ///////////////////////////////////////////////////////

#if 1
   TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,500,400);

   float* grEjet = &Ejet[0];

/*
   for(Int_t i=0; i<nEnergyPoints; ++i)
   {
	   std::cout << " -- " << grEjet[i] << std::endl;
   }
*/

   int usedColorPerfect = 0;

   for(int i=0; i<nSettings; ++i)
   {
	   //float JER* = &[];
	   std::vector<float> iJER = JERs[i];
	   std::vector<float> iJERex = JERexs[i];
	   std::vector<float> iJERey = JEReys[i];

	   float* grJER = &iJER[0];
	   float* grJERex = &iJERex[0];
	   float* grJERey = &iJERey[0];

	   TGraphErrors* gr = new TGraphErrors(nEnergyPoints, grEjet, grJER, grJERex, grJERey);

	   gr->SetName(graphNames[i].c_str());

	   //gr->SetMarkerStyle(20);
	   //gr->SetMarkerStyle(2);
	   gr->SetMarkerStyle(7);

       gr->SetMarkerColor(colors[i]);
       gr->SetLineColor(colors[i]);
	   gr->GetXaxis()->SetRangeUser(25., 265.);
	   gr->GetYaxis()->SetRangeUser(1., 9.);
	   //gr->GetYaxis()->SetRangeUser(1., 10.);
	   //gr->GetYaxis()->SetRangeUser(1., 50);
	   gr->GetXaxis()->SetTitle("E_{jet}[GeV]");
	   gr->GetYaxis()->SetTitle("RMS_{90}(E_{j})/Mean_{90}(E_{j})[%]");
	   gr->SetTitle("");

	   if(int(graphNames[i].find("perfect"))>0)
	   {
		   gr->SetLineStyle(2); //dashed line

		   int color = colorsPerfect[usedColorPerfect++];
		   gr->SetMarkerColor(color);
           gr->SetLineColor(color);
	   }

	   if(i==0)
	   {
		   gr->Draw("ALP");
	   }
	   else
	   {
		   gr->Draw("LP");
	   }

	   //std::cout << "color: " << colors[i] << std::endl;
   }

   //c1->SetGrid();
   //TLegend* legend = new TLegend(0.4, 0.5, 0.87, 0.85);
   const float legendY2 = 0.4 + nSettings * 0.15;
   TLegend* legend = new TLegend(0.4, 0.5, 0.87, legendY2);

   legend->SetBorderSize(0);

   for(int i=0; i<nSettings; ++i)
   {
	   const char* gname = graphNames[i].c_str();
	   legend->AddEntry(gname, gname, "l");
   }

   legend->Draw();

   c1->Print("jers2.pdf");
   
   delete c1;
#endif

   return 0;
}


//////////////////////////////////////////////////////////////////////////////////


double GetRMS(std::vector<std::string>& files)
{
    TChain *ch1=new TChain("PfoAnalysisTree");

	for(int i=0; i<files.size(); ++i) 
	{ 
		std::string& fileName = files.at(i);
		cout << fileName << endl;

		ch1->Add(fileName.c_str());
	}

	float pfoEnergyTotal, costQ1;
	ch1->SetBranchAddress("pfoEnergyTotal", &pfoEnergyTotal);
	ch1->SetBranchAddress("costQ1", &costQ1);

	const double MAXCOSQ = 0.7;
	//const double MAXCOSQ = 1.;

	TH1F* hist = new TH1F("", "", 100, 0, 1000);

	for(int i = 0; i < ch1->GetEntries(); ++i)
	{
		ch1->GetEntry(i);

		if(fabs(costQ1)<MAXCOSQ&&pfoEnergyTotal>10.)
		{
			hist->Fill(pfoEnergyTotal);
		}
		//std::cout << pfoEnergyTotal << std::endl;
	}

	hist->Fit("gaus", "QN");
	TF1* fit = (TF1*)gROOT->GetFunction("gaus");

	//cout << fit->GetParameter(0) << endl;
	//cout << fit->GetParameter(1) << endl;
	//cout << fit->GetParameter(2) << endl;
	double mean = fit->GetParameter(1);
	double sigma = fit->GetParameter(2);

	std::cout << "1) mean: " << mean << ", sigma: " << sigma << std::endl;

	////////////////
	std::cout << "--- " << hist->GetEntries() << std::endl;
	hist->Reset();

	/////////////////////
	const double NSIGMA = 5.;
	////////////////////

	double xmin = mean - NSIGMA * sigma;
	double xmax = mean + NSIGMA * sigma;

	hist->SetBins(100, xmin, xmax);

	for(int i = 0; i < ch1->GetEntries(); ++i)
	{
		ch1->GetEntry(i);
		
		if(pfoEnergyTotal>xmin&&pfoEnergyTotal<xmax&&fabs(costQ1)<MAXCOSQ)
		{
			hist->Fill(pfoEnergyTotal);
		}
	}

	std::cout << "--- " << hist->GetEntries() << std::endl;
	hist->Fit("gaus", "QN");

	mean = fit->GetParameter(1);
	sigma = fit->GetParameter(2);

	std::cout << "2) mean: " << mean << ", sigma: " << sigma << std::endl;

	double minrms90 = RMSFitPercentageRange(hist);

	std::cout << "=====> RMS90: " << minrms90 << std::endl;

	delete ch1;
	delete hist;

	return minrms90;
}

//////////////////////////////////////////////////////////////////////////////////

double RMSFitPercentageRange(TH1F* m_histogram)
{
    float m_fitRangeLow(1.e10);
    float m_fitRangeHigh(1.e10);
    float m_rMSFitRange(1.e10);
    
    static const float FLOAT_MAX(1.e10);

    if (NULL == m_histogram)
        return 0;

    if (5 > m_histogram->GetEntries())
    {
        std::cout << m_histogram->GetName() << " (" << m_histogram->GetEntries() << " entries) - skipped" << std::endl;
        return 0;
    }

    // Calculate raw properties of distribution (ie rms100)
    float sum = 0., total = 0.;
    double sx = 0., sxx = 0.;
    const unsigned int nbins(m_histogram->GetNbinsX());

	//cout << "nbins: " << nbins << endl;

    for (unsigned int i = 0; i <= nbins; ++i)
    {
        const float binx(m_histogram->GetBinLowEdge(i) + (0.5 * m_histogram->GetBinWidth(i)));
        const float yi(m_histogram->GetBinContent(i));
        sx += yi * binx;
        sxx += yi * binx * binx;
        total += yi;
    }

    const float rawMean(sx / total);
    const float rawMeanSquared(sxx / total);
    const float rawRms(std::sqrt(rawMeanSquared - rawMean * rawMean));

	//cout << "rawMean: " << rawMean << ", rawMeanSquared: " << rawMeanSquared << ", rawRms: " << rawRms << endl;

    sum = 0.;
    unsigned int is0 = 0;

	float m_fitPercentage = 90;

    //  The /10 comes from the fact that for rms 90 the start point for the fit must occur in the first 10% of the data.
    float frac = (1 - (m_fitPercentage/100.0));
    for (unsigned int i = 0; (i <= nbins) && (sum < total * frac); ++i)
    {
        sum += m_histogram->GetBinContent(i);
        is0 = i;
    }

    // Calculate truncated properties
    float rmsmin(FLOAT_MAX), mean(FLOAT_MAX), low(FLOAT_MAX);
    float high(0.f);

    for (unsigned int istart = 0; istart <= is0; ++istart)
    {
        double sumn = 0.;
        double csum = 0.;
        double sumx = 0.;
        double sumxx = 0.;
        unsigned int iend = 0;

        for (unsigned int i = istart; (i <= nbins) && (csum < (m_fitPercentage/100) * total); ++i)
        {
            const float binx(m_histogram->GetBinLowEdge(i) + (0.5 * m_histogram->GetBinWidth(i)));
            const float yi(m_histogram->GetBinContent(i));
            //csum is the sum of yi from istart and is used to stop the sum when this exceeds X% of data.
            csum += yi;

            if (sumn < (m_fitPercentage/100) * total)
            {
                // These variables define the final sums required once we have considered X% of data, anything else is 
                // continuously overwritten.
                sumn += yi;
                sumx += yi * binx;
                sumxx+= yi * binx * binx;
                iend = i;
            }
        }

        const float localMean(sumx / sumn);
        const float localMeanSquared(sumxx / sumn);
        // Standard deviation formula
        const float localRms(std::sqrt(localMeanSquared - localMean * localMean));

        if (localRms < rmsmin)
        {
            mean = localMean;
            if (istart==0)
            {
                low = 0;
                m_fitRangeLow = 0;
            }
            else
            {
                low = m_histogram->GetBinLowEdge(istart);
                m_fitRangeLow = m_histogram->GetBinLowEdge(istart) + (0.5 * m_histogram->GetBinWidth(istart));
            }
            
            high = m_histogram->GetBinLowEdge(iend);
            rmsmin = localRms;
            m_fitRangeHigh = m_histogram->GetBinLowEdge(iend) + (0.5 * m_histogram->GetBinWidth(iend));
        }
    }
    
    m_rMSFitRange = rmsmin;
    
#if 0
    std::cout << m_histogram->GetName() << " (" << m_histogram->GetEntries() << " entries), rawrms: " << rawRms << ", rmsx: " << rmsmin
              << " (" << low << "-" << high << "), low_fit and high_fit " << " (" << m_fitRangeLow << "-" << m_fitRangeHigh 
              << "), << mean: " << mean << std::endl;
#endif

	return rmsmin;
}
