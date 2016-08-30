/**
 * @file SegPerfApp.cpp
 * @brief Implementation of CSegPerfApp class. The class to structure application and application progress.
 * @author Florent Leray
 * @date 13/04/2016
 * @version 2.0
 */
#include "SegPerfApp.h"

#include <tclap/CmdLine.h>
#include <iostream>
#include <cmath>
#include <limits>


using namespace std;
using namespace TCLAP;

/**
   @brief    It is the constructor to used.
*/
CSegPerfApp::CSegPerfApp(void)
{
   //////////////////////////////////////////////////////////////////////////
   // Output way
   m_bTxt = true;
   m_bXml = false;
   m_bScreen = false;

   //////////////////////////////////////////////////////////////////////////
   // Group of metrics enable
   m_bSegmentationEvaluation = true;
   m_bAdvancedEvaluation = false;
   m_bSurfaceEvaluation = true;
   m_bLesionsDetectionEvaluation = true;

   //////////////////////////////////////////////////////////////////////////
   // Lesions specific metrics
   m_fSensitivity = std::numeric_limits<float>::quiet_NaN();
   m_fSpecificity = std::numeric_limits<float>::quiet_NaN();
   m_fPPV = std::numeric_limits<float>::quiet_NaN();
   m_fNPV = std::numeric_limits<float>::quiet_NaN();
   m_fDice = std::numeric_limits<float>::quiet_NaN();
   m_fJaccard = std::numeric_limits<float>::quiet_NaN();
   m_fRVE = std::numeric_limits<float>::quiet_NaN();

   //////////////////////////////////////////////////////////////////////////
   // Distances metrics
   m_fHausdorffDist = std::numeric_limits<float>::quiet_NaN();
   m_fMeanDist = std::numeric_limits<float>::quiet_NaN();
   m_fAverageDist = std::numeric_limits<float>::quiet_NaN();

   //////////////////////////////////////////////////////////////////////////
   // Detection scores
   m_fPPVL = std::numeric_limits<float>::quiet_NaN();
   m_fSensL = std::numeric_limits<float>::quiet_NaN();
   m_fF1 = std::numeric_limits<float>::quiet_NaN();

   //////////////////////////////////////////////////////////////////////////
   // general informations
   m_iNbThreads = 0;      /*<! number of thread used by processing. */
   m_pchOutBase = NULL;   /*<! base name used for output file. */

   //////////////////////////////////////////////////////////////////////////
   // Detection scores parameters
   m_fDetectionLesionMinVolume = 3.0;
   m_fTPLMinOverlapRatio = 0.1;
   m_fTPLMaxFalsePositiveRatio =0.7;
   m_fTPLMaxFalsePositiveRatioModerator =0.65;
}

/**
   @brief    Destructor.
   @details  It is empty.
*/
CSegPerfApp::~CSegPerfApp(void)
{
}

/**
   @brief    This method set the application behavior thanks to command line arguments parsing.
   @param    [in] argc is the number of command line arguments.
   @param    [in] argv is the table  of command line arguments.
   @details  The main function must delegate to it command line arguments. this method will parse and set different option values.
*/
bool CSegPerfApp::init(int argc, char *argv[])
{
   bool bRes = true;
   try
   {
      // Define the command line object.
      CmdLine cmd("Tools to analyze segmentation performances by comparison", ' ', "2.1");

      // Define a value argument and add it to the command line.
      ValueArg<string> oArgInputImg("i", "input", "Input image.", true, "", "string");

      cmd.add( oArgInputImg );

      // Define a value argument and add it to the command line.
      ValueArg<string> oArgRefImg("r", "ref", "Reference image to compare input image.", true, "", "string");

      cmd.add( oArgRefImg );

      // Define a value argument and add it to the command line.
      ValueArg<string> oArgBaseOutputName("o", "outputBase", "Base name for output files", false, "", "string");

      cmd.add( oArgBaseOutputName );

      // Define a value argument and add it to the command line.
      ValueArg<int> oArgNbThreads("t", "threads", "Number of threads", false, 0, "int");

      cmd.add( oArgNbThreads );



      // Define a value argument and add it to the command line.
      SwitchArg oArgAbout("A", "About", "Details on output metrics", false);

      cmd.add( oArgAbout );

      // Define a switch and add it to the command line.
      SwitchArg oArgSwitchText("T", "text", "Stores results into a text file.", false);

      cmd.add( oArgSwitchText );


      // Define a switch and add it to the command line.
      SwitchArg oArgSwitchXml("X", "xml", "Stores results into a xml file.", false);

      cmd.add( oArgSwitchXml );

      // Define a switch and add it to the command line.
      SwitchArg oArgSwitchScreen("S", "screen", "Print results on the screen when program ended.", false);

      cmd.add( oArgSwitchScreen );



      // Define a switch and add it to the command line.
      SwitchArg oArgSegEval("s", "SegmentationEvaluationMetrics", "Compute metrics to evaluate a segmentation.", false);

      cmd.add( oArgSegEval );

      // Define a switch and add it to the command line.
      SwitchArg oArgSwitchAdvancedEvaluation("a", "advancedEvaluation", "Compute results for each cluster (intra-lesion results)", false);

      cmd.add( oArgSwitchAdvancedEvaluation );

      // Define a switch and add it to the command line.
      SwitchArg oArgSwitchDetectionEval("l", "LesionDetectionMetrics", "Compute metrics to evaluate the detection of lesions along to a segmentation.", false);

      cmd.add( oArgSwitchDetectionEval );

      // Define a switch and add it to the command line.
      SwitchArg oArgSwitchSurfaceDist("d", "SurfaceEvaluation", "Surface distances evaluation.", false);

      cmd.add( oArgSwitchSurfaceDist );


      // Define a switch and add it to the command line.
      ValueArg<float> oArgSwitchDetectionLesionMinVolume("v", "MinLesionVolume", "Min volume of lesion for \"Lesions detection metrics\" in mm^3 (default 3mm^3).", false, 3.00, "float");

      cmd.add( oArgSwitchDetectionLesionMinVolume );

      // Define a switch and add it to the command line.
      ValueArg<float> oArgSwitchTPLMinOverlapRatio("x", "MinOverlapRatio", "Minimum overlap ratio to say if a lesion of the GT is detected. (default 0.10)", false, 0.10, "float");

      cmd.add( oArgSwitchTPLMinOverlapRatio );

      // Define a switch and add it to the command line.
      ValueArg<float> oArgSwitchTPLMaxFalsePositiveRatio("y", "MaxFalsePositiveRatio", "Maximum of false positive ratio to limit the detection of a lesion in GT if a lesion in the image is too big. (default 0.7)", false, 0.70, "float");

      cmd.add( oArgSwitchTPLMaxFalsePositiveRatio );

      // Define a switch and add it to the command line.
      ValueArg<float> oArgSwitchTPLMaxFalsePositiveRatioModerator("z", "MaxFalsePositiveRatioModerator", "Percentage of the regions overlapping the tested lesion is not too much outside of this lesion. (default 0.65)", false, 0.65, "float");

      cmd.add( oArgSwitchTPLMaxFalsePositiveRatioModerator );



      // Parse the args.
      cmd.parse( argc, argv );
      // Get the value parsed by each arg.
      if (oArgAbout.getValue())
      {
         //about();
      }
      m_oStrInImage  = oArgInputImg.getValue();
      m_oStrRefImage = oArgRefImg.getValue();
      m_oStrBaseOut  = oArgBaseOutputName.getValue();

      m_bTxt         = oArgSwitchText.getValue();
      m_bXml         = oArgSwitchXml.getValue();
      m_bScreen      = oArgSwitchScreen.getValue();


      m_iNbThreads   = oArgNbThreads.getValue();

      m_bSegmentationEvaluation = oArgSegEval.getValue();
      m_bAdvancedEvaluation = oArgSwitchAdvancedEvaluation.getValue();
      m_bSurfaceEvaluation = oArgSwitchSurfaceDist.getValue();
      m_bLesionsDetectionEvaluation = oArgSwitchDetectionEval.getValue();

      m_fDetectionLesionMinVolume = oArgSwitchDetectionLesionMinVolume.getValue();
      m_fTPLMinOverlapRatio = oArgSwitchTPLMinOverlapRatio.getValue();
      m_fTPLMaxFalsePositiveRatio = oArgSwitchTPLMaxFalsePositiveRatio.getValue();
      m_fTPLMaxFalsePositiveRatioModerator = oArgSwitchTPLMaxFalsePositiveRatioModerator.getValue();
   }

   // catch any exceptions
   catch (ArgException&e)
   {
      cerr << "error: " << e.error() << " for arg " << e.argId() << endl;
      bRes = false;
   }

   return bRes;
}

/**
   @brief    This method check if command line arguments are coherent between us.
   @details  If an incoherency is detected into command line arguments a message will be displayed to user and in better cases consistency will be restored.
*/
bool CSegPerfApp::checkParamsChoerancy()
{
   bool bFault = false;


   if(!(m_bSegmentationEvaluation || m_bAdvancedEvaluation || m_bSurfaceEvaluation || m_bLesionsDetectionEvaluation))
   {
      m_bSegmentationEvaluation = true;
   }

   if (m_bAdvancedEvaluation && !(m_bSegmentationEvaluation || m_bSurfaceEvaluation))
   {
      cout<<"Switch \"advancedEvaluation\" need \"SegmentationEvaluationMetrics\" or/and SurfaceEvaluation switches."<<endl;
      cout<<"!!!\"SegmentationEvaluationMetrics\" has been enabled by default!!!"<<endl;
      m_bSegmentationEvaluation = true;
   }


   if (m_fDetectionLesionMinVolume<0)
   {
      cout<<"!!!!! Error on DetectionLesionMinVolume!!!!!"<<endl;
      bFault = true;
   }

   if (m_fTPLMinOverlapRatio<=0 || m_fTPLMinOverlapRatio>1)
   {
      cout<<"!!!!! Error on TPLMinOverlapRatio!!!!!"<<endl;
      bFault = true;
   }

   if (m_fTPLMaxFalsePositiveRatio<=0 || m_fTPLMaxFalsePositiveRatio>1)
   {
      cout<<"!!!!! Error on TPLMaxFalsePositiveRatio!!!!!"<<endl;
      bFault = true;
   }

   if (m_fTPLMaxFalsePositiveRatioModerator<=0 || m_fTPLMaxFalsePositiveRatioModerator>1)
   {
      cout<<"!!!!! Error on TPLMaxFalsePositiveRatioModerator!!!!!"<<endl;
      bFault = true;
   }

   if(bFault)
   {
      cout<<"*** 0 < DetectionLesionMinVolume               ***"<<endl;
      cout<<"*** 0 < TPLMinOverlapRatio                <= 1 ***"<<endl;
      cout<<"*** 0 < TPLMaxFalsePositiveRatio          <= 1 ***"<<endl;
      cout<<"*** 0 < TPLMaxFalsePositiveRatioModerator <= 1 ***"<<endl;
   }

   return !bFault;
}

/**
   @brief    This method define an output way if none are defined by command line.
   @details  For the moment the default output way is text file.
*/
void CSegPerfApp::checkOutputChoerancy()
{
   if (!(m_bTxt || m_bXml || m_bScreen))
   {
      m_bTxt = true;
   }
}

/**
   @brief    This method define the base file name for output way.
*/
void CSegPerfApp::preparOutput()
{
   if (m_oStrBaseOut.empty())
   {
      int iDotPos = m_oStrInImage.rfind('.');
      int iFolderPos = -1;
      int iSlashPos = m_oStrInImage.rfind('/');
      int iBkSlashPos = m_oStrInImage.rfind('\\');

      if (iSlashPos != string::npos && iBkSlashPos != string::npos)
      {
         iFolderPos = iSlashPos>iBkSlashPos ? iSlashPos : iBkSlashPos;
      }
      else if (iSlashPos != string::npos)
      {
         iFolderPos = iSlashPos;
      }
      else if (iBkSlashPos != string::npos)
      {
         iFolderPos = iBkSlashPos;
      }
      iFolderPos++;



      m_pchOutBase = new char[m_oStrInImage.length()+1];
      strncpy(m_pchOutBase, m_oStrInImage.c_str()+iFolderPos, m_oStrInImage.length()+1);
      if(iDotPos != string::npos)
      {
         m_pchOutBase[iDotPos-iFolderPos] = 0;
      }
   }
   else
   {
      m_pchOutBase = new char[m_oStrBaseOut.length()+1];
      strncpy(m_pchOutBase, m_oStrBaseOut.c_str(), m_oStrBaseOut.length()+1);
   }
}

/**
   @brief    This method execute images filter to obtain desired measures, marks and scores.
*/
long CSegPerfApp::play()
{
   long lRes = 0;

   CAnalyzer oAnalyzer((char*)m_oStrInImage.c_str(), (char*)m_oStrRefImage.c_str(), m_bAdvancedEvaluation);

   if (m_iNbThreads>0)
   {
      oAnalyzer.setNbThreads(m_iNbThreads);
   }


   int nbLabels = oAnalyzer.getNumberOfClusters();

   string sOutBase = m_pchOutBase;
   string sOut = sOutBase;

   int i = 0;


   //////////////////////////////////////////////////////////////////////////
   // First step of loop is for global, if next steps exist it's for each layer
   do
   {
      sOut = sOutBase;
      std::stringstream outBaseTemp;

      if(i == 0)
         outBaseTemp << sOut<<"_global";
      else
         outBaseTemp << sOut << "_cluster" << i;

      sOut = outBaseTemp.str();
      m_pchOutBase = new char[sOut.length()+1];
      strncpy(m_pchOutBase, sOut.c_str(), sOut.length()+1);

      CResults oRes(m_pchOutBase);


      processAnalyze(oAnalyzer, i);
      storeMetricsAndMarks(oRes);
      lRes = writeStoredMetricsAndMarks(oRes);

      i++;
   } while (i < nbLabels && m_bAdvancedEvaluation);


   MDEL(m_pchOutBase);

   return lRes;
}

/**
   @brief    This method provides computing metrics, marks and scores for desired label.
   @param    [in] pi_roAnalyzer is the reference on the image analyzer class instance.
   @param    [in] pi_iIndex is label index.
   @details  This method is called by play method.
*/
void CSegPerfApp::processAnalyze(CAnalyzer&pi_roAnalyzer, int pi_iIndex)
{
   pi_roAnalyzer.selectCluster(pi_iIndex);

   // Segmentation evaluation
   if(m_bSegmentationEvaluation || m_bSurfaceEvaluation)
   {
      pi_roAnalyzer.computeITKMeasures();
      m_fSensitivity = pi_roAnalyzer.getSensitivity();
      m_fSpecificity = pi_roAnalyzer.getSpecificity();
      m_fPPV = pi_roAnalyzer.getPPV();
      m_fNPV = pi_roAnalyzer.getNPV();
      m_fDice = pi_roAnalyzer.getDiceCoefficient();
      m_fJaccard = pi_roAnalyzer.getJaccardCoefficient();
      m_fRVE = pi_roAnalyzer.getRelativeVolumeError();

      //Surfaces distances computing (Haussdorf, meanDist, Average ...)
      if(m_bSurfaceEvaluation)
      {
         m_fHausdorffDist = pi_roAnalyzer.computeHausdorffDist();
         m_fMeanDist = pi_roAnalyzer.computeMeanDist();
         m_fAverageDist = pi_roAnalyzer.computeAverageSurfaceDistance();
      }
   }

   // Detection lesions
   if(m_bLesionsDetectionEvaluation)
   {
      pi_roAnalyzer.setDetectionThresholdAlpha(m_fTPLMinOverlapRatio);
      pi_roAnalyzer.setDetectionThresholdBeta(m_fTPLMaxFalsePositiveRatio);
      pi_roAnalyzer.setDetectionThresholdGamma(m_fTPLMaxFalsePositiveRatioModerator);
      pi_roAnalyzer.setMinLesionVolumeDetection(m_fDetectionLesionMinVolume);
      pi_roAnalyzer.getDetectionMarks(m_fPPVL, m_fSensL, m_fF1);
   }
}

/**
   @brief    This method store results into CResults class instance.
   @param    [in] pi_roRes is the reference output writer class instance.
   @details  This method is called by play method after processAnalyze method.
*/
void CSegPerfApp::storeMetricsAndMarks(CResults&pi_roRes)
{
   //////////////////////////////////////////////////////////////////////////
   //////////////////////////////////////////////////////////////////////////
   // Following code put out results
   //////////////////////////////////////////////////////////////////////////
   pi_roRes.setTxt(m_bTxt);
   pi_roRes.setXml(m_bXml);
   pi_roRes.setScreen(m_bScreen);

   //Segmentation
   if(m_bSegmentationEvaluation)
   {
      pi_roRes.activeMeasurementOutput(eMesureDice);
      pi_roRes.setDice(m_fDice);
      pi_roRes.activeMeasurementOutput(eMesureJaccard);
      pi_roRes.setJaccard(m_fJaccard);
      pi_roRes.activeMeasurementOutput(eMesureSensibility);
      pi_roRes.setSensibility(m_fSensitivity);
      pi_roRes.activeMeasurementOutput(eMesureSpecificity);
      pi_roRes.setSpecificity(m_fSpecificity);
      pi_roRes.activeMeasurementOutput(eMesureNPV);
      pi_roRes.setNPV(m_fNPV);
      pi_roRes.activeMeasurementOutput(eMesurePPV);
      pi_roRes.setPPV(m_fPPV);
      pi_roRes.activeMeasurementOutput(eMesureRelativeVolumeError);
      pi_roRes.setRVE(m_fRVE*100);
   }

   //Surfaces distances
   if(m_bSurfaceEvaluation)
   {
      pi_roRes.activeMeasurementOutput(eMesureDistHausdorff);
      pi_roRes.setHausdorffDist(m_fHausdorffDist);
      pi_roRes.activeMeasurementOutput(eMesureDistMean);
      pi_roRes.setContourMeanDist(m_fMeanDist);
      pi_roRes.activeMeasurementOutput(eMesureDistAverage);
      pi_roRes.setAverageSurfaceDist(m_fAverageDist);
   }

   //Lesion detection
   if(m_bLesionsDetectionEvaluation)
   {
      pi_roRes.activeMeasurementOutput(eMesurePPVL);
      pi_roRes.setPPVL(m_fPPVL);
      pi_roRes.activeMeasurementOutput(eMesureSensL);
      pi_roRes.setSensL(m_fSensL);
      pi_roRes.activeMeasurementOutput(eMesureF1Test);
      pi_roRes.setF1test(m_fF1);
   }
}

/**
   @brief    This method flush CResults class instance.
   @param    [in] pi_roRes is the reference output writer class instance.
   @details  This method is called by play method after storeMetricsAndMarks method.
*/
long CSegPerfApp::writeStoredMetricsAndMarks(CResults&pi_roRes)
{
   long lRes = (long) !pi_roRes.save();
   return lRes;
}

/**
   @brief    This method display information about SegPerfAnalyzer results.
*/
void CSegPerfApp::about()
{
   cout<<endl;
   cout<<"********************************************************************************"<<endl;
   cout<<"********************************************************************************"<<endl;
   cout<<"SegPerfAnalyser (Segmentation Performance Analyzer) provides different"<<endl;
   cout<<"marks, metrics and scores for segmentation evaluation."<<endl;
   cout<<endl;
   cout<<"3 categories are available:"<<endl;
   cout<<"    - SEGMENTATION EVALUATION:"<<endl;
   cout<<"        Dice, the mean overlap"<<endl;
   cout<<"        Jaccard, the union overlap"<<endl;
   cout<<"        Sensitivity"<<endl;
   cout<<"        Specificity"<<endl;
   cout<<"        NPV (Negative Predictive Value)"<<endl;
   cout<<"        PPV (Positive Predictive Value)"<<endl;
   cout<<"        RVE (Relative Volume Error) in percentage"<<endl;
   cout<<"    - SURFACE DISTANCE EVALUATION:"<<endl;
   cout<<"        Hausdorff distance"<<endl;
   cout<<"        Contour mean distance"<<endl;
   cout<<"        Average surface distance"<<endl;
   cout<<"    - DETECTION LESIONS EVALUATION:"<<endl;
   cout<<"        PPVL (Positive Predictive Value for Lesions)"<<endl;
   cout<<"        SensL, Lesion detection sensitivity"<<endl;
   cout<<"        F1 Score, a F1 Score between PPVL and SensL"<<endl;
   cout<<endl;


   cout<<"Results are provided as follows: "<<endl;
   char const*const*const ppchNameTab = CResults::getMeasureNameTable();
   for (int i=0; i<eMesureLast; ++i)
   {
      cout<<ppchNameTab[i]<<";\t";
   }
   cout<<endl;
   cout<<"********************************************************************************"<<endl;
   cout<<"********************************************************************************"<<endl;
}

