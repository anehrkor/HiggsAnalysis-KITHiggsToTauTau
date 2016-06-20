
#include "DataFormats/TauReco/interface/PFTau.h"

#include "Artus/Consumer/interface/LambdaNtupleConsumer.h"
#include "Artus/Utility/interface/DefaultValues.h"
#include "Artus/Utility/interface/SafeMap.h"

#include "HiggsAnalysis/KITHiggsToTauTau/interface/Producers/PolarisationQuantitiesProducer.h"


void PolarisationQuantitiesProducer::Init(setting_type const& settings)
{
	ProducerBase<HttTypes>::Init(settings);
	
	// add possible quantities for the lambda ntuples consumers
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("rhoNeutralChargedAsymmetry_1", [](event_type const& event, product_type const& product) {
		return static_cast<float>(SafeMap::GetWithDefault(product.m_rhoNeutralChargedAsymmetry, product.m_flavourOrderedLeptons.at(0), DefaultValues::UndefinedDouble));
	});
	LambdaNtupleConsumer<HttTypes>::AddFloatQuantity("rhoNeutralChargedAsymmetry_2", [](event_type const& event, product_type const& product) {
		return static_cast<float>(SafeMap::GetWithDefault(product.m_rhoNeutralChargedAsymmetry, product.m_flavourOrderedLeptons.at(1), DefaultValues::UndefinedDouble));
	});
}

void PolarisationQuantitiesProducer::Produce(event_type const& event, product_type& product,
	                                     setting_type const& settings) const
{
	for (std::vector<KTau*>::iterator tau = product.m_validTaus.begin(); tau != product.m_validTaus.end(); ++tau)
	{
		if (((*tau)->decayMode == reco::PFTau::hadronicDecayMode::kOneProng1PiZero) &&
		    ((*tau)->piZeroCandidates.size() > 0) &&
		    ((*tau)->chargedHadronCandidates.size() > 0))
		{
			double energyNeutralPi = (*tau)->piZeroCandidates.at(0).p4.E();
			double energyChargedPi = (*tau)->chargedHadronCandidates.at(0).p4.E();
			product.m_rhoNeutralChargedAsymmetry[*tau] = (((energyNeutralPi + energyChargedPi) != 0.0) ? (energyNeutralPi - energyChargedPi) / (energyNeutralPi + energyChargedPi) : 0.0);
		}
	}
}