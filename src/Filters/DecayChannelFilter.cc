
#include "HiggsAnalysis/KITHiggsToTauTau/interface/Filters/DecayChannelFilter.h"


bool DecayChannelFilter::DoesEventPassLocal(HttEvent const& event,
                                            HttProduct const& product,
                                            HttPipelineSettings const& settings) const
{
	return (product.m_decayChannel == HttProduct::ToDecayChannel(settings.GetChannel()));
}
