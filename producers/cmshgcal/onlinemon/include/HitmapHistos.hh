/*
 * HitmapHistos.hh
 *
 *  Created on: Jun 16, 2011
 *      Author: stanitz
 */

#ifndef HITMAPHISTOS_HH_
#define HITMAPHISTOS_HH_

#include <TH2I.h>
#include <TH2Poly.h>
#include <TFile.h>

#include <map>

#include "SimpleStandardEvent.hh"

using namespace std;

class RootMonitor;

class HitmapHistos {
protected:
  string _sensor;
  int _id;
  int _maxX;
  int _maxY;
  bool _wait;

  TH2Poly *_hexagons_occupancy;
  TH2Poly *_hexagons_charge;
  TH2Poly *_hexagons_occ_tot;
  TH2Poly *_hexagons_occ_toa;
  TH2I *_hitmap;
  TH1I *_hitXmap;
  TH1I *_hitYmap;
  TH2I *_clusterMap;
  TH2D *_HotPixelMap;
  TH1I *_lvl1Distr;
  TH1I *_lvl1Width;
  TH1I *_lvl1Cluster;
  TH1I *_totSingle;
  TH1I *_totCluster;
  TH1F *_hitOcc;
  TH1I *_clusterSize;
  TH1I *_nClusters;
  TH1I *_nHits;
  TH1I *_clusterXWidth;
  TH1I *_clusterYWidth;
  TH1I *_nbadHits;
  TH1I *_nHotPixels;
  TH1I *_nPivotPixel;
  TH1I *_hitmapSections;
  TH1I **_nHits_section;
  TH1I **_nClusters_section;
  TH1I **_nClustersize_section;
  TH1I **_nHotPixels_section;

public:
  HitmapHistos(SimpleStandardPlane p, RootMonitor *mon);

  void Fill(const SimpleStandardHit &hit);
  void Fill(const SimpleStandardPlane &plane);
  void Fill(const SimpleStandardCluster &cluster);
  void Reset();

  void Calculate(const int currentEventNum);
  void Write();

  TH2Poly *getHexagonsOccupancyHisto() { return _hexagons_occupancy; }
  TH2Poly *getHexagonsChargeHisto() { return _hexagons_charge; }
  TH2Poly *getHexagonsOccTotHisto() { return _hexagons_occ_tot; }
  TH2Poly *getHexagonsOccToaHisto() { return _hexagons_occ_toa; }
  TH2I *getHitmapHisto() { return _hitmap; }
  TH1I *getHitXmapHisto() { return _hitXmap; }
  TH1I *getHitYmapHisto() { return _hitYmap; }
  TH1I *getHitmapSectionsHisto() { return _hitmapSections; }
  TH2I *getClusterMapHisto() { return _clusterMap; }
  TH2D *getHotPixelMapHisto() { return _HotPixelMap; }
  TH1I *getLVL1Histo() { return _lvl1Distr; }
  TH1I *getLVL1WidthHisto() { return _lvl1Width; }
  TH1I *getLVL1ClusterHisto() { return _lvl1Cluster; }
  TH1I *getTOTSingleHisto() { return _totSingle; }
  TH1I *getTOTClusterHisto() { return _totCluster; }
  TH1F *getHitOccHisto() {
    if (_wait)
      return NULL;
    else
      return _hitOcc;
  }
  TH1I *getClusterSizeHisto() { return _clusterSize; }
  TH1I *getNHitsHisto() { return _nHits; }
  TH1I *getNClustersHisto() { return _nClusters; }
  TH1I *getClusterWidthXHisto() { return _clusterXWidth; }
  TH1I *getClusterWidthYHisto() { return _clusterYWidth; }
  TH1I *getNbadHitsHisto() { return _nbadHits; }
  TH1I *getSectionsNHitsHisto(unsigned int section) {
    return _nHits_section[section];
  }
  TH1I *getSectionsNClusterHisto(unsigned int section) {
    return _nClusters_section[section];
  }
  TH1I *getSectionsNClusterSizeHisto(unsigned int section) {
    return _nClustersize_section[section];
  }
  TH1I *getSectionsNHotPixelsHisto(unsigned int section) {
    return _nHotPixels_section[section];
  }
  TH1I *getNHotPixelsHisto() { return _nHotPixels; }
  TH1I *getNPivotPixelHisto() { return _nPivotPixel; }
  void setRootMonitor(RootMonitor *mon) { _mon = mon; }

private:
  int **plane_map_array; // store an array representing the map
  int zero_plane_array(); // fill array with zeros;
  int SetHistoAxisLabelx(TH1 *histo, string xlabel);
  int SetHistoAxisLabely(TH1 *histo, string ylabel);
  int SetHistoAxisLabels(TH1 *histo, string xlabel, string ylabel);
  int filling_counter; // we don't need occupancy to be refreshed for every
                       // single event

  void Set_SkiToHexaboard_ChannelMap();
  map < pair < int,int >, int > _ski_to_ch_map;
  
  TH2Poly* get_th2poly(string name, string title);

  RootMonitor *_mon;
  unsigned int mimosa26_max_section;
  // check what kind sensor we're dealing with
  // for the filling this eliminates a string comparison
  bool is_HEXABOARD;
  bool is_MIMOSA26;
  bool is_APIX;
  bool is_USBPIX;
  bool is_USBPIXI4;
  bool is_DEPFET;
};



static const int ch_to_bin_map[133] = {
  104,104,81,92,103,113,121,
  58,69,80,91,102,112,120,126,
  25,46,57,68,79,90,101,111,119,125,127,
  25,35,45,56,67,78,89,100,110,118,124,127,
  24,34,44,55,66,77,88,99,109,117,123,
  14,23,33,43,54,65,76,87,98,108,116,122,
  13,22,32,42,53,64,75,86,97,107,115,
  6,12,21,31,41,52,63,74,85,96,106,114,
  5,11,20,30,40,51,62,73,84,95,105,
  1,4,10,19,29,39,50,61,72,83,94,93,
  1,3,9,18,28,38,49,60,71,82,93,
  2,8,17,27,37,48,59,70,
  7,16,26,36,47,15,15};

static const  char sc_to_ch_map[381] = { //381 = 127*3
  1,0,20,
  2,3,50,
  3,3,48,
  4,3,44,
  5,0,22,
  6,0,12,
  7,3,56,
  8,3,52,
  9,3,36,
  10,3,32,
  11,0,28,
  12,0,24,
  13,0,8,
  14,0,10,
  15,3,46,
  16,3,62,
  17,3,54,
  18,3,38,
  19,3,34,
  20,3,30,
  21,0,26,
  22,0,14,
  23,0,6,
  24,0,2,
  25,0,60,
  26,3,58,
  27,3,60,
  28,3,42,
  29,3,40,
  30,3,28,
  31,0,30,
  32,0,18,
  33,0,16,
  34,0,0,
  35,0,4,
  36,3,6,
  37,3,0,
  38,3,2,
  39,3,20,
  40,3,22,
  41,3,26,
  42,0,32,
  43,0,54,
  44,0,56,
  45,0,58,
  46,0,62,
  47,3,10,
  48,3,12,
  49,3,4,
  50,3,16,
  51,3,18,
  52,3,24,
  53,0,34,
  54,0,38,
  55,0,52,
  56,0,44,
  57,0,50,
  58,0,48,
  59,2,50,
  60,2,48,
  61,3,8,
  62,3,14,
  63,2,44,
  64,1,36,
  65,0,36,
  66,1,28,
  67,0,42,
  68,0,40,
  69,0,46,
  70,2,60,
  71,2,62,
  72,2,46,
  73,2,52,
  74,2,54,
  75,2,36,
  76,1,38,
  77,1,26,
  78,1,22,
  79,1,20,
  80,1,8,
  81,1,16,
  82,2,14,
  83,2,58,
  84,2,0,
  85,2,56,
  86,2,38,
  87,1,40,
  88,1,34,
  89,1,24,
  90,1,2,
  91,1,18,
  92,1,14,
  93,2,16,
  94,2,8,
  95,2,4,
  96,2,2,
  97,2,42,
  98,2,40,
  99,1,32,
  100,1,30,
  101,1,0,
  102,1,4,
  103,1,10,
  104,1,12,
  105,2,18,
  106,2,20,
  107,2,6,
  108,2,10,
  109,1,42,
  110,1,44,
  111,1,58,
  112,1,54,
  113,1,6,
  114,2,22,
  115,2,24,
  116,2,12,
  117,2,30,
  118,1,46,
  119,1,50,
  120,1,56,
  121,1,62,
  122,2,26,
  123,2,28,
  124,2,32,
  125,1,48,
  126,1,52,
  127,2,34
 }; 

#ifdef __CINT__
#pragma link C++ class HitmapHistos - ;
#endif

#endif /* HITMAPHISTOS_HH_ */
