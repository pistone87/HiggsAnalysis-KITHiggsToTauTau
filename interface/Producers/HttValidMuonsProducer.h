
#pragma once

#include "Artus/KappaAnalysis/interface/Producers/ValidMuonsProducer.h"

#include "HiggsAnalysis/KITHiggsToTauTau/interface/HttTypes.h"


/**
   \brief GlobalProducer, for valid muons.
   
   Required config tags in addtion to the ones of the base class:
   - MuonChargedIsoVetoConeSize (default given)
   - MuonNeutralIsoVetoConeSize (default given)
   - MuonPhotonIsoVetoConeSize (default given)
   - MuonDeltaBetaIsoVetoConeSize (default given)
   - MuonChargedIsoPtThreshold (default given)
   - MuonNeutralIsoPtThreshold (default given)
   - MuonPhotonIsoPtThreshold (default given)
   - MuonDeltaBetaIsoPtThreshold (default given)
   - MuonIsoSignalConeSize
   - MuonDeltaBetaCorrectionFactor
   - MuonIsoPtSumOverPtThresholdEB
   - MuonIsoPtSumOverPtThresholdEE
*/

class HttValidMuonsProducer: public ValidMuonsProducer<HttTypes>
{

public:

	typedef typename HttTypes::event_type event_type;
	typedef typename HttTypes::product_type product_type;
	typedef typename HttTypes::setting_type setting_type;
	
	HttValidMuonsProducer(
			std::vector<KDataMuon*> product_type::*validMuons=&product_type::m_validMuons,
			std::vector<KDataMuon*> product_type::*invalidMuons=&product_type::m_invalidMuons,
			std::string (setting_type::*GetMuonID)(void) const=&setting_type::GetMuonID,
			std::string (setting_type::*GetMuonIsoType)(void) const=&setting_type::GetMuonIsoType,
			std::string (setting_type::*GetMuonIso)(void) const=&setting_type::GetMuonIso,
			std::vector<std::string>& (setting_type::*GetLowerPtCuts)(void) const=&setting_type::GetMuonLowerPtCuts,
			std::vector<std::string>& (setting_type::*GetUpperAbsEtaCuts)(void) const=&setting_type::GetMuonUpperAbsEtaCuts,
			float (setting_type::*GetMuonChargedIsoVetoConeSize)(void) const=&setting_type::GetMuonChargedIsoVetoConeSize,
			float (setting_type::*GetMuonNeutralIsoVetoConeSize)(void) const=&setting_type::GetMuonNeutralIsoVetoConeSize,
			float (setting_type::*GetMuonPhotonIsoVetoConeSize)(void) const=&setting_type::GetMuonPhotonIsoVetoConeSize,
			float (setting_type::*GetMuonDeltaBetaIsoVetoConeSize)(void) const=&setting_type::GetMuonDeltaBetaIsoVetoConeSize,
			float (setting_type::*GetMuonChargedIsoPtThreshold)(void) const=&setting_type::GetMuonChargedIsoPtThreshold,
			float (setting_type::*GetMuonNeutralIsoPtThreshold)(void) const=&setting_type::GetMuonNeutralIsoPtThreshold,
			float (setting_type::*GetMuonPhotonIsoPtThreshold)(void) const=&setting_type::GetMuonPhotonIsoPtThreshold,
			float (setting_type::*GetMuonDeltaBetaIsoPtThreshold)(void) const=&setting_type::GetMuonDeltaBetaIsoPtThreshold,
			float (setting_type::*GetMuonIsoSignalConeSize)(void) const=&setting_type::GetMuonIsoSignalConeSize,
			float (setting_type::*GetMuonDeltaBetaCorrectionFactor)(void) const=&setting_type::GetMuonDeltaBetaCorrectionFactor,
			float (setting_type::*GetMuonIsoPtSumOverPtThresholdEB)(void) const=&setting_type::GetMuonIsoPtSumOverPtThresholdEB,
			float (setting_type::*GetMuonIsoPtSumOverPtThresholdEE)(void) const=&setting_type::GetMuonIsoPtSumOverPtThresholdEE,
			float (setting_type::*GetMuonTrackDxyCut)(void) const=&setting_type::GetMuonTrackDxyCut,
			float (setting_type::*GetMuonTrackDzCut)(void) const=&setting_type::GetMuonTrackDzCut
	);
	
	virtual void Init(setting_type const& settings) {

		ValidMuonsProducer<HttTypes>::Init(settings);

		// add possible quantities for the lambda ntuples consumers
		LambdaNtupleConsumer<HttTypes>::AddQuantity("leadingMuonIso", [this](event_type const& event, product_type const& product) {
			return product.m_validMuons.size() >=1 ? SafeMap::GetWithDefault(product.m_muonIsolation, product.m_validMuons[0], DefaultValues::UndefinedDouble) : DefaultValues::UndefinedDouble;
		});
		LambdaNtupleConsumer<HttTypes>::AddQuantity("leadingMuonIsoOverPt", [this](event_type const& event, product_type const& product) {
			return product.m_validMuons.size() >=1 ? SafeMap::GetWithDefault(product.m_muonIsolationOverPt, product.m_validMuons[0], DefaultValues::UndefinedDouble) : DefaultValues::UndefinedDouble;
		});
	}


protected:

	// Htautau specific additional definitions
	virtual bool AdditionalCriteria(KDataMuon* muon, event_type const& event,
	                                product_type& product, setting_type const& settings) const  ARTUS_CPP11_OVERRIDE;


private:
	float (setting_type::*GetMuonChargedIsoVetoConeSize)(void) const;
	float (setting_type::*GetMuonNeutralIsoVetoConeSize)(void) const;
	float (setting_type::*GetMuonPhotonIsoVetoConeSize)(void) const;
	float (setting_type::*GetMuonDeltaBetaIsoVetoConeSize)(void) const;
	float (setting_type::*GetMuonChargedIsoPtThreshold)(void) const;
	float (setting_type::*GetMuonNeutralIsoPtThreshold)(void) const;
	float (setting_type::*GetMuonPhotonIsoPtThreshold)(void) const;
	float (setting_type::*GetMuonDeltaBetaIsoPtThreshold)(void) const;
	float (setting_type::*GetMuonIsoSignalConeSize)(void) const;
	float (setting_type::*GetMuonDeltaBetaCorrectionFactor)(void) const;
	float (setting_type::*GetMuonIsoPtSumOverPtThresholdEB)(void) const;
	float (setting_type::*GetMuonIsoPtSumOverPtThresholdEE)(void) const;
	float (setting_type::*GetMuonTrackDxyCut)(void) const;
	float (setting_type::*GetMuonTrackDzCut)(void) const;

};


/**
   \brief 
*/

class HttValidLooseMuonsProducer: public HttValidMuonsProducer
{

public:

	typedef typename HttTypes::event_type event_type;
	typedef typename HttTypes::product_type product_type;
	typedef typename HttTypes::setting_type setting_type;

	virtual std::string GetProducerId() const ARTUS_CPP11_OVERRIDE {
		return "HttValidLooseMuonsProducer";
	}
	
	virtual void Init(setting_type const& settings) ARTUS_CPP11_OVERRIDE {
	
		HttValidMuonsProducer::Init(settings);
	
		// add possible quantities for the lambda ntuples consumers
		LambdaNtupleConsumer<HttTypes>::AddQuantity("nLooseMuons", [this](event_type const& event, product_type const& product) {
			return product.m_validLooseMuons.size();
		});
	}
	
	HttValidLooseMuonsProducer(
			std::vector<KDataMuon*> product_type::*validMuons=&product_type::m_validLooseMuons,
			std::vector<KDataMuon*> product_type::*invalidMuons=&product_type::m_invalidLooseMuons,
			std::string (setting_type::*GetMuonID)(void) const=&setting_type::GetLooseMuonID,
			std::string (setting_type::*GetMuonIsoType)(void) const=&setting_type::GetLooseMuonIsoType,
			std::string (setting_type::*GetMuonIso)(void) const=&setting_type::GetLooseMuonIso,
			std::vector<std::string>& (setting_type::*GetLowerPtCuts)(void) const=&setting_type::GetLooseMuonLowerPtCuts,
			std::vector<std::string>& (setting_type::*GetUpperAbsEtaCuts)(void) const=&setting_type::GetLooseMuonUpperAbsEtaCuts,
			float (setting_type::*GetMuonChargedIsoVetoConeSize)(void) const=&setting_type::GetMuonChargedIsoVetoConeSize,
			float (setting_type::*GetMuonNeutralIsoVetoConeSize)(void) const=&setting_type::GetMuonNeutralIsoVetoConeSize,
			float (setting_type::*GetMuonPhotonIsoVetoConeSize)(void) const=&setting_type::GetMuonPhotonIsoVetoConeSize,
			float (setting_type::*GetMuonDeltaBetaIsoVetoConeSize)(void) const=&setting_type::GetMuonDeltaBetaIsoVetoConeSize,
			float (setting_type::*GetMuonChargedIsoPtThreshold)(void) const=&setting_type::GetMuonChargedIsoPtThreshold,
			float (setting_type::*GetMuonNeutralIsoPtThreshold)(void) const=&setting_type::GetMuonNeutralIsoPtThreshold,
			float (setting_type::*GetMuonPhotonIsoPtThreshold)(void) const=&setting_type::GetMuonPhotonIsoPtThreshold,
			float (setting_type::*GetMuonDeltaBetaIsoPtThreshold)(void) const=&setting_type::GetMuonDeltaBetaIsoPtThreshold,
			float (setting_type::*GetMuonIsoSignalConeSize)(void) const=&setting_type::GetMuonIsoSignalConeSize,
			float (setting_type::*GetMuonDeltaBetaCorrectionFactor)(void) const=&setting_type::GetMuonDeltaBetaCorrectionFactor,
			float (setting_type::*GetMuonIsoPtSumOverPtThresholdEB)(void) const=&setting_type::GetLooseMuonIsoPtSumOverPtThresholdEB,
			float (setting_type::*GetMuonIsoPtSumOverPtThresholdEE)(void) const=&setting_type::GetLooseMuonIsoPtSumOverPtThresholdEE,
			float (setting_type::*GetMuonTrackDxyCut)(void) const=&setting_type::GetLooseMuonTrackDxyCut,
			float (setting_type::*GetMuonTrackDzCut)(void) const=&setting_type::GetLooseMuonTrackDzCut
	);

};


/**
   \brief 
*/

class HttValidVetoMuonsProducer: public HttValidMuonsProducer
{

public:

	typedef typename HttTypes::event_type event_type;
	typedef typename HttTypes::product_type product_type;
	typedef typename HttTypes::setting_type setting_type;

	virtual std::string GetProducerId() const ARTUS_CPP11_OVERRIDE {
		return "HttValidVetoMuonsProducer";
	}
	
	HttValidVetoMuonsProducer(
			std::vector<KDataMuon*> product_type::*validMuons=&product_type::m_validVetoMuons,
			std::vector<KDataMuon*> product_type::*invalidMuons=&product_type::m_invalidVetoMuons,
			std::string (setting_type::*GetMuonID)(void) const=&setting_type::GetVetoMuonID,
			std::string (setting_type::*GetMuonIsoType)(void) const=&setting_type::GetVetoMuonIsoType,
			std::string (setting_type::*GetMuonIso)(void) const=&setting_type::GetVetoMuonIso,
			std::vector<std::string>& (setting_type::*GetLowerPtCuts)(void) const=&setting_type::GetVetoMuonLowerPtCuts,
			std::vector<std::string>& (setting_type::*GetUpperAbsEtaCuts)(void) const=&setting_type::GetVetoMuonUpperAbsEtaCuts,
			float (setting_type::*GetMuonChargedIsoVetoConeSize)(void) const=&setting_type::GetMuonChargedIsoVetoConeSize,
			float (setting_type::*GetMuonNeutralIsoVetoConeSize)(void) const=&setting_type::GetMuonNeutralIsoVetoConeSize,
			float (setting_type::*GetMuonPhotonIsoVetoConeSize)(void) const=&setting_type::GetMuonPhotonIsoVetoConeSize,
			float (setting_type::*GetMuonDeltaBetaIsoVetoConeSize)(void) const=&setting_type::GetMuonDeltaBetaIsoVetoConeSize,
			float (setting_type::*GetMuonChargedIsoPtThreshold)(void) const=&setting_type::GetMuonChargedIsoPtThreshold,
			float (setting_type::*GetMuonNeutralIsoPtThreshold)(void) const=&setting_type::GetMuonNeutralIsoPtThreshold,
			float (setting_type::*GetMuonPhotonIsoPtThreshold)(void) const=&setting_type::GetMuonPhotonIsoPtThreshold,
			float (setting_type::*GetMuonDeltaBetaIsoPtThreshold)(void) const=&setting_type::GetMuonDeltaBetaIsoPtThreshold,
			float (setting_type::*GetMuonIsoSignalConeSize)(void) const=&setting_type::GetMuonIsoSignalConeSize,
			float (setting_type::*GetMuonDeltaBetaCorrectionFactor)(void) const=&setting_type::GetMuonDeltaBetaCorrectionFactor,
			float (setting_type::*GetMuonIsoPtSumOverPtThresholdEB)(void) const=&setting_type::GetVetoMuonIsoPtSumOverPtThresholdEB,
			float (setting_type::*GetMuonIsoPtSumOverPtThresholdEE)(void) const=&setting_type::GetVetoMuonIsoPtSumOverPtThresholdEE,
			float (setting_type::*GetMuonTrackDxyCut)(void) const=&setting_type::GetMuonTrackDxyCut,
			float (setting_type::*GetMuonTrackDzCut)(void) const=&setting_type::GetMuonTrackDzCut
	);

};

