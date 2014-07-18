// =================================================
//  STHitCluster Class                          
//                                                  
//  Description:                                    
//    Container for a cluster of hits
//                                                  
//  Genie Jhang ( geniejhang@majimak.com )          
//  2014. 07. 15                                    
// =================================================

#ifndef STCLUSTER_H
#define STCLUSTER_H

#include "STHit.hh"

#include "TObject.h"
#include "TVector3.h"
#include "TMatrixD.h"

#include <vector>

using std::vector;

class STHitCluster : public TObject 
{
  public:
    STHitCluster();
    STHitCluster(STHitCluster *cluster);
    ~STHitCluster();

    void SetClusterID(Int_t clusterID);
    Int_t GetClusterID();

    TVector3 GetPosition();
    TVector3 GetPosSigma();
    TMatrixD GetCovMatrix();

    Int_t GetNumHits();
    vector<Int_t> *GetHitIDs();

    void AddHit(STHit *hit);

  private:
    Int_t fClusterID;
    TVector3 fPosition;
    TVector3 fPosSigma;
    TMatrixD fCovariant;

    vector<Int_t> fHitIDArray;

  ClassDef(STHitCluster, 1)
};

#endif
