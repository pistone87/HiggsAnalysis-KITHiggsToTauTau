
#include "Artus/Consumer/interface/LambdaNtupleConsumer.h"
#include "HiggsAnalysis/KITHiggsToTauTau/interface/HttEnumTypes.h"
#include "HiggsAnalysis/KITHiggsToTauTau/interface/Producers/RecoTauCPProducer.h"
void RecoTauCPProducer::Init(setting_type const& settings)
{
	ProducerBase<HttTypes>::Init(settings);
	
	//adding possible quantities for the lambda ntuples consumers
	LambdaNtupleConsumer<HttTypes>::Quantities["recoPhiStarCP"] = [](event_type const & event, product_type const & product)
	{
		return product.m_recoPhiStarCP;
	};
	LambdaNtupleConsumer<HttTypes>::Quantities["recoChargedHadron1HiggsFrameEnergy"] = [](event_type const & event, product_type const & product)
	{
		return product.m_recoChargedHadronEnergies.first;
	};
	LambdaNtupleConsumer<HttTypes>::Quantities["recoChargedHadron2HiggsFrameEnergy"] = [](event_type const & event, product_type const & product)
	{
		return product.m_recoChargedHadronEnergies.second;
	};
	LambdaNtupleConsumer<HttTypes>::Quantities["ImpactParameter1"] = [](event_type const & event, product_type const & product)
	{
		return product.abs_n1;
	};
	LambdaNtupleConsumer<HttTypes>::Quantities["ImpactParameter2"] = [](event_type const & event, product_type const & product)
	{
		return product.abs_n2;
	};
}
void RecoTauCPProducer::Produce(event_type const& event, product_type& product, setting_type const& settings) const
{
	// Hadronic TauTau channel
	//std::cout << (product.m_validTaus[0])->signalNeutrHadrCands.size() << std::endl;
	if(product.m_decayChannel == HttEnumTypes::DecayChannel::TT)
	{
		KDataVertex primevertex = event.m_vertexSummary->pv;
		KDataPFTau* tau1 = product.m_validTaus[0];
		KDataPFTau* tau2 = product.m_validTaus[1];
		if(tau1->signalChargedHadrCands.size()==1 && tau2->signalChargedHadrCands.size()==1)
		{
			KPFCandidate* chargePart1 = &(tau1->signalChargedHadrCands[0]);
			KPFCandidate* chargePart2 = &(tau2->signalChargedHadrCands[0]);
			product.m_recoPhiStarCP = CPQuantities::CalculatePhiStarCP(primevertex, tau1->track, tau2->track, chargePart1->p4, chargePart2->p4, product.abs_n1, product.abs_n2);
			product.m_recoChargedHadronEnergies.first = CPQuantities::CalculateChargedHadronEnergy(product.m_diTauSystem, chargePart1->p4);
			product.m_recoChargedHadronEnergies.second = CPQuantities::CalculateChargedHadronEnergy(product.m_diTauSystem, chargePart2->p4);
		}
		else 
		{
			product.m_recoPhiStarCP = DefaultValues::UndefinedDouble;
			product.m_recoChargedHadronEnergies.first = DefaultValues::UndefinedDouble;
			product.m_recoChargedHadronEnergies.second = DefaultValues::UndefinedDouble;
		}
	}

	else if (product.m_decayChannel == HttEnumTypes::DecayChannel::EE)
	{
		KDataVertex primevertex = event.m_vertexSummary->pv;
		KDataElectron* electron1 = product.m_validElectrons[0];
		KDataElectron* electron2 = product.m_validElectrons[1];
		product.m_recoPhiStarCP = CPQuantities::CalculatePhiStarCP(primevertex, electron1->track, electron2->track, electron1->p4, electron2->p4,product.abs_n1, product.abs_n2);
		
		product.m_recoChargedHadronEnergies.first = DefaultValues::UndefinedDouble;
		product.m_recoChargedHadronEnergies.second = DefaultValues::UndefinedDouble;
	}
	else if (product.m_decayChannel == HttEnumTypes::DecayChannel::MM)
	{
		KDataVertex primevertex = event.m_vertexSummary->pv;
		KDataMuon* muon1 = product.m_validMuons[0];
		KDataMuon* muon2 = product.m_validMuons[1];
		product.m_recoPhiStarCP = CPQuantities::CalculatePhiStarCP(primevertex, muon1->track, muon2->track, muon1->p4, muon2->p4, product.abs_n1, product.abs_n2);
		
		product.m_recoChargedHadronEnergies.first = DefaultValues::UndefinedDouble;
		product.m_recoChargedHadronEnergies.second = DefaultValues::UndefinedDouble;
	}
	else if (product.m_decayChannel == HttEnumTypes::DecayChannel::ET)
	{
		KDataVertex primevertex = event.m_vertexSummary->pv;
		KDataElectron* electron1 = product.m_validElectrons[0];
		KDataPFTau* tau1 = product.m_validTaus[0];
		if (tau1->signalChargedHadrCands.size()==1)
		{
			KPFCandidate* chargePart1 = &(tau1->signalChargedHadrCands[0]);
			product.m_recoPhiStarCP = CPQuantities::CalculatePhiStarCP(primevertex, electron1->track, tau1->track, electron1->p4, chargePart1->p4, product.abs_n1, product.abs_n2);
			product.m_recoChargedHadronEnergies.first = CPQuantities::CalculateChargedHadronEnergy(product.m_diTauSystem, chargePart1->p4);
			
			product.m_recoChargedHadronEnergies.second = DefaultValues::UndefinedDouble;
		}
		else 
		{
			product.m_recoPhiStarCP = DefaultValues::UndefinedDouble;
			product.m_recoChargedHadronEnergies.first = DefaultValues::UndefinedDouble;
			product.m_recoChargedHadronEnergies.second = DefaultValues::UndefinedDouble;
		}
	}
	else if (product.m_decayChannel == HttEnumTypes::DecayChannel::MT)
	{
		KDataVertex primevertex = event.m_vertexSummary->pv;
		KDataMuon* muon1 = product.m_validMuons[0];
		KDataPFTau* tau1 = product.m_validTaus[0];
		if (tau1->signalChargedHadrCands.size()==1)
		{
			KPFCandidate* chargePart1 = &(tau1->signalChargedHadrCands[0]);
			product.m_recoPhiStarCP = CPQuantities::CalculatePhiStarCP(primevertex, muon1->track, tau1->track, muon1->p4, chargePart1->p4, product.abs_n1, product.abs_n2);
			product.m_recoChargedHadronEnergies.first = CPQuantities::CalculateChargedHadronEnergy(product.m_diTauSystem, chargePart1->p4);
			
			product.m_recoChargedHadronEnergies.second = DefaultValues::UndefinedDouble;
		}
		else 
		{
			product.m_recoPhiStarCP = DefaultValues::UndefinedDouble;
			product.m_recoChargedHadronEnergies.first = DefaultValues::UndefinedDouble;
			product.m_recoChargedHadronEnergies.second = DefaultValues::UndefinedDouble;
		}
	}
	else if (product.m_decayChannel == HttEnumTypes::DecayChannel::EM)
	{
		KDataVertex primevertex = event.m_vertexSummary->pv;
		KDataElectron* electron1 = product.m_validElectrons[0];
		KDataMuon* muon1 = product.m_validMuons[0];
		product.m_recoPhiStarCP = CPQuantities::CalculatePhiStarCP(primevertex, electron1->track, muon1->track, electron1->p4, muon1->p4, product.abs_n1, product.abs_n2);
		
		product.m_recoChargedHadronEnergies.first = DefaultValues::UndefinedDouble;
		product.m_recoChargedHadronEnergies.second = DefaultValues::UndefinedDouble;
	}
	else
	{
		product.m_recoPhiStarCP = DefaultValues::UndefinedDouble;
		product.m_recoChargedHadronEnergies.first = DefaultValues::UndefinedDouble;
		product.m_recoChargedHadronEnergies.second = DefaultValues::UndefinedDouble;
	}
}
