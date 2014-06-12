
#include "Artus/Utility/interface/RootFileHelper.h"
#include "Artus/Utility/interface/Utility.h"

#include "HiggsAnalysis/KITHiggsToTauTau/interface/Producers/TriggerWeightProducer.h"


void TriggerWeightProducer::Init(setting_type const& settings)
{
	// parse settings for trigger efficiency files
	// and read the histograms from the files
	// Data
	std::map<std::string, std::vector<std::string> > triggerEfficiencyFilesDataByHltName;
	std::map<size_t, std::vector<std::string> > triggerEfficiencyFilesDataByIndex = Utility::ParseMapTypes<size_t, std::string>(
			Utility::ParseVectorToMap(settings.GetTriggerEfficiencyData()),
			triggerEfficiencyFilesDataByHltName
	);
	std::map<std::string, std::vector<TH2F*> > triggerEfficienciesDataByHltName = RootFileHelper::SafeGetMap<std::string, TH2F>(
			triggerEfficiencyFilesDataByHltName, "triggerEfficiency"
	);
	std::map<size_t, std::vector<TH2F*> > triggerEfficienciesDataByIndex = RootFileHelper::SafeGetMap<size_t, TH2F>(
			triggerEfficiencyFilesDataByIndex, "triggerEfficiency"
	);
	
	// MC
	std::map<std::string, std::vector<std::string> > triggerEfficiencyFilesMcByHltName;
	std::map<size_t, std::vector<std::string> > triggerEfficiencyFilesMcByIndex = Utility::ParseMapTypes<size_t, std::string>(
			Utility::ParseVectorToMap(settings.GetTriggerEfficiencyMc()),
			triggerEfficiencyFilesMcByHltName
	);
	std::map<std::string, std::vector<TH2F*> > triggerEfficienciesMcByHltName = RootFileHelper::SafeGetMap<std::string, TH2F>(
			triggerEfficiencyFilesMcByHltName, "triggerEfficiency"
	);
	std::map<size_t, std::vector<TH2F*> > triggerEfficienciesMcByIndex = RootFileHelper::SafeGetMap<size_t, TH2F>(
			triggerEfficiencyFilesMcByIndex, "triggerEfficiency"
	);
	
	// consistency checks for settings
	assert(triggerEfficienciesDataByHltName.size() == triggerEfficienciesMcByHltName.size());
	assert(triggerEfficienciesDataByIndex.size() == triggerEfficienciesMcByIndex.size());
	
	for (std::map<std::string, std::vector<TH2F*> >::const_iterator triggerEfficiencyDataByHltName = triggerEfficienciesDataByHltName.begin();
	     triggerEfficiencyDataByHltName != triggerEfficienciesDataByHltName.end();
	     ++triggerEfficiencyDataByHltName)
	{
		assert(triggerEfficienciesMcByHltName.count(triggerEfficiencyDataByHltName->first) > 0);
	}
	
	for (std::map<size_t, std::vector<TH2F*> >::const_iterator triggerEfficiencyDataByIndex = triggerEfficienciesDataByIndex.begin();
	     triggerEfficiencyDataByIndex != triggerEfficienciesDataByIndex.end();
	     ++triggerEfficiencyDataByIndex)
	{
		assert(triggerEfficienciesMcByIndex.count(triggerEfficiencyDataByIndex->first) > 0);
	}
	
}

void TriggerWeightProducer::Produce(event_type const& event, product_type& product,
                                    setting_type const& settings) const
{
	// read bin contents from ROOT histograms
	// Data
	std::vector<double> triggerEfficienciesData = GetTriggerEfficiencies(triggerEfficienciesDataByHltName,
	                                                                     triggerEfficienciesDataByIndex,
	                                                                     event, product, settings);
	
	// MC
	std::vector<double> triggerEfficienciesMc = GetTriggerEfficiencies(triggerEfficienciesMcByHltName,
	                                                                   triggerEfficienciesMcByIndex,
	                                                                   event, product, settings);
	
	// calculate the trigger weight
	double triggerWeight = 1.0;
	for (size_t triggerEfficiencyIndex = 0; triggerEfficiencyIndex < triggerEfficienciesData.size();
	     ++triggerEfficiencyIndex)
	{
		triggerWeight *= triggerEfficienciesData[triggerEfficiencyIndex];
		triggerWeight /= triggerEfficienciesMc[triggerEfficiencyIndex];
	}
	
	product.m_weights["triggerWeight"] = triggerWeight;
}


double TriggerWeightProducer::GetTriggerEfficienciesFromHistograms(std::vector<TH2F*> const& histograms,
                                                                   KLepton* lepton) const
{
	double triggerEfficiency = 1.0;
	for (std::vector<TH2F*>::const_iterator histogram = histograms.begin();
	     histogram != histograms.end(); ++histogram)
	{
		triggerEfficiency *= (*histogram)->GetBinContent((*histogram)->FindBin(lepton->p4.Pt(), lepton->p4.Eta()));
	}
	return triggerEfficiency;
}


std::vector<double> TriggerWeightProducer::GetTriggerEfficiencies(
		std::map<std::string, std::vector<TH2F*> > const& triggerEfficienciesByHltName,
		std::map<size_t, std::vector<TH2F*> > const& triggerEfficienciesByIndex,
		event_type const& event, product_type const& product, setting_type const& settings) const
{
	std::vector<double> triggerEfficiencies(triggerEfficienciesByHltName.size() + triggerEfficienciesByIndex.size(), 1.0);
	size_t index = 0;
	
	for (std::map<std::string, std::vector<TH2F*> >::const_iterator triggerEfficiencyByHltName = triggerEfficienciesByHltName.begin();
	     triggerEfficiencyByHltName != triggerEfficienciesByHltName.end();
	     ++triggerEfficiencyByHltName)
	{
		if (triggerEfficiencyByHltName->first == "default")
		{
			for (std::vector<KLepton*>::const_iterator lepton = product.m_ptOrderedLeptons.begin();
			     lepton != product.m_ptOrderedLeptons.end(); ++lepton)
			{
				triggerEfficiencies[index++] = GetTriggerEfficienciesFromHistograms(
						triggerEfficiencyByHltName->second,
						*lepton
				);
			}
		}
		else
		{
			// TODO
		}
	}
	
	for (std::map<size_t, std::vector<TH2F*> >::const_iterator triggerEfficiencyByIndex = triggerEfficienciesByIndex.begin();
	     triggerEfficiencyByIndex != triggerEfficienciesByIndex.end();
	     ++triggerEfficiencyByIndex)
	{
		if (triggerEfficiencyByIndex->first < product.m_ptOrderedLeptons.size())
		{
			triggerEfficiencies[index++] = GetTriggerEfficienciesFromHistograms(
					triggerEfficiencyByIndex->second,
					product.m_ptOrderedLeptons.at(triggerEfficiencyByIndex->first)
			);
		}
	}
	
	return triggerEfficiencies;
}
