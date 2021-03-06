
#include "Artus/Consumer/interface/LambdaNtupleConsumer.h"

#include "HiggsAnalysis/KITHiggsToTauTau/interface/Producers/DiLeptonQuantitiesProducer.h"


void DiLeptonQuantitiesProducer::Init(setting_type const& settings)
{
	ProducerBase<HttTypes>::Init(settings);
	
	// add possible quantities for the lambda ntuples consumers
	LambdaNtupleConsumer<HttTypes>::AddQuantity("diLepPt", [](event_type const& event, product_type const& product) {
		return product.m_diLeptonSystem.Pt();
	});
	LambdaNtupleConsumer<HttTypes>::AddQuantity("diLepEta", [](event_type const& event, product_type const& product) {
		return product.m_diLeptonSystem.Eta();
	});
	LambdaNtupleConsumer<HttTypes>::AddQuantity("diLepPhi", [](event_type const& event, product_type const& product) {
		return product.m_diLeptonSystem.Phi();
	});
	LambdaNtupleConsumer<HttTypes>::AddQuantity("diLepMass", [](event_type const& event, product_type const& product) {
		return product.m_diLeptonSystem.mass();
	});
	LambdaNtupleConsumer<HttTypes>::AddQuantity("diLepMt", [](event_type const& event, product_type const& product) {
		return product.m_diLeptonSystem.Mt();
	});
	
	LambdaNtupleConsumer<HttTypes>::AddQuantity("diLepMetPt", [](event_type const& event, product_type const& product) {
		return product.m_diLeptonPlusMetSystem.Pt();
	});
	LambdaNtupleConsumer<HttTypes>::AddQuantity("diLepMetEta", [](event_type const& event, product_type const& product) {
		return product.m_diLeptonPlusMetSystem.Eta();
	});
	LambdaNtupleConsumer<HttTypes>::AddQuantity("diLepMetPhi", [](event_type const& event, product_type const& product) {
		return product.m_diLeptonPlusMetSystem.Phi();
	});
	LambdaNtupleConsumer<HttTypes>::AddQuantity("diLepMetMass", [](event_type const& event, product_type const& product) {
		return product.m_diLeptonPlusMetSystem.mass();
	});
	LambdaNtupleConsumer<HttTypes>::AddQuantity("diLepMetMt", [](event_type const& event, product_type const& product) {
		return product.m_diLeptonPlusMetSystem.Mt();
	});
}

void DiLeptonQuantitiesProducer::Produce(event_type const& event, product_type& product,
	                                     setting_type const& settings) const
{
	assert(product.m_flavourOrderedLeptons.size() >= 2);
	
	product.m_diLeptonSystem = (product.m_flavourOrderedLeptons[0]->p4 + product.m_flavourOrderedLeptons[1]->p4);
	product.m_diLeptonPlusMetSystem = (product.m_diLeptonSystem + product.m_met->p4);
	
	// collinear approximation
	// reconstruct tau momenta assuming that the neutrinos fly collinear to the taus
	// HiggsAnalysis/KITHiggsToTauTau/doc/collinear_approximation.nb
	double p1x = product.m_flavourOrderedLeptons[0]->p4.Px();
	double p1y = product.m_flavourOrderedLeptons[0]->p4.Py();
	double p2x = product.m_flavourOrderedLeptons[1]->p4.Px();
	double p2y = product.m_flavourOrderedLeptons[1]->p4.Py();
	double pmx = product.m_met->p4.Px();
	double pmy = product.m_met->p4.Py();
	double ratioVisToTau1 = (p1y*p2x - p1x*p2y + p2y*pmx - p2x*pmy) / (p1y*p2x - p1x*p2y);
	double ratioVisToTau2 = (p1y*p2x - p1x*p2y - p1y*pmx + p1x*pmy) / (p1y*p2x - p1x*p2y);
	
	product.m_flavourOrderedTauMomentaCA.clear();
	if (ratioVisToTau1 >= 0.0 && ratioVisToTau2 >= 0.0)
	{
		product.m_flavourOrderedTauMomentaCA.push_back(RMDataLV(product.m_flavourOrderedLeptons[0]->p4 / ratioVisToTau1));
		product.m_flavourOrderedTauMomentaCA.push_back(RMDataLV(product.m_flavourOrderedLeptons[1]->p4 / ratioVisToTau2));
		product.m_diTauSystemCA = product.m_flavourOrderedTauMomentaCA[0] + product.m_flavourOrderedTauMomentaCA[1];
		product.m_validCollinearApproximation = true;
	}
	else
	{
		product.m_validCollinearApproximation = false;
	}
}
