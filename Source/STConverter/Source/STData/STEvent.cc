// =================================================
//  STEvent Class                          
//                                                  
//  Description:                                    
//    Container for a event data
//                                                  
//  Genie Jhang ( geniejhang@majimak.com )          
//  2014. 05. 19                                    
// =================================================

#include <iostream>
#include <iomanip>

#include "STEvent.hh"

ClassImp(STEvent);

STEvent::STEvent(Bool_t isClustered, Bool_t isTracked)
:TNamed("STEvent", "Event container")
{
  fEventID = -1;

  fIsClustered = isClustered;
  fIsTracked = isTracked;
}

STEvent::STEvent(STEvent *object)
:TNamed("STEvent", "Event container")
{
  fEventID = object -> GetEventID();

  fIsClustered = object -> IsClustered();
  fIsTracked = object -> IsTracked();

  fHitArray = *(object -> GetHitArray());

  if (IsClustered())
    fClusterArray = *(object -> GetClusterArray());
}

STEvent::~STEvent()
{
}

// setters
void STEvent::SetEventID(Int_t evtid)
{
  fEventID = evtid;
}

void STEvent::AddHit(STHit *hit)
{
  fHitArray.push_back(*hit);

  delete hit;
}

void STEvent::AddCluster(STHitCluster *cluster)
{
  fClusterArray.push_back(*cluster);

  delete cluster;
}

// getters
Int_t STEvent::GetEventID() { return fEventID; }

Int_t STEvent::GetNumHits() { return fHitArray.size(); }

STHit *STEvent::GetHit(Int_t hitNo)
{
  return (hitNo < GetNumHits() ? &fHitArray[hitNo] : NULL);
}

STHit *STEvent::RemoveHit(Int_t hitNo)
{
  if (!(hitNo < GetNumHits()))
    return NULL;

  STHit *removedHit = new STHit(fHitArray[hitNo]);
  fHitArray.erase(fHitArray.begin() + hitNo);

  return removedHit;
}

std::vector<STHit> *STEvent::GetHitArray()
{
  return &fHitArray;
}

STHitCluster *STEvent::GetCluster(Int_t clusterNo)
{
  if (!(clusterNo < GetNumClusters()) || !IsClustered())
    return NULL;

  return &fClusterArray[clusterNo];
}

STHitCluster *STEvent::RemoveCluster(Int_t clusterNo)
{
  if (!(clusterNo < GetNumClusters()) || !IsClustered())
    return NULL;

  STHitCluster *removedCluster = new STHitCluster(fClusterArray[clusterNo]);
  fClusterArray.erase(fClusterArray.begin() + clusterNo);

  return removedCluster;
}

std::vector<STHitCluster> *STEvent::GetClusterArray()
{
  return &fClusterArray;
}
