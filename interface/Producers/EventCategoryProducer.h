
#pragma once

#include "HiggsAnalysis/KITHiggsToTauTau/interface/HttTypes.h"
#include "HiggsAnalysis/KITHiggsToTauTau/interface/HttEnumTypes.h"


/** Producer that defines the event categories.
 */
class EventCategoryProducer: public ProducerBase<HttTypes> {
public:

	typedef typename HttTypes::event_type event_type;
	typedef typename HttTypes::product_type product_type;
	typedef typename HttTypes::setting_type setting_type;

	virtual std::string GetProducerId() const ARTUS_CPP11_OVERRIDE {
		return "EventCategoryProducer";
	}
	
	static bool EventInEventCategory(HttEnumTypes::EventCategory const& eventCategory,
	                                 product_type const& product);
	
	virtual void Init(setting_type const& settings) ARTUS_CPP11_OVERRIDE;
	
	virtual void Produce(event_type const& event, product_type& product,
	                     setting_type const& settings) const ARTUS_CPP11_OVERRIDE;
};


/** Producer that defines the event categories for ttH analysis
 */
class TTHEventCategoryProducer: public EventCategoryProducer {
public:

	typedef typename HttTypes::event_type event_type;
	typedef typename HttTypes::product_type product_type;
	typedef typename HttTypes::setting_type setting_type;

	virtual std::string GetProducerId() const ARTUS_CPP11_OVERRIDE {
		return "TTHEventCategoryProducer";
	}
	
	virtual void Produce(event_type const& event, product_type& product,
	                     setting_type const& settings) const ARTUS_CPP11_OVERRIDE;
};

